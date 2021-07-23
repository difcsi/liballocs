#include "helpers.hpp"

#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <fileno.hpp>
#include <srk31/algorithm.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <dwarfidl/create.hpp>
#include <libgen.h>
#include <cstdio>
#include <memory>

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#include <boost/icl/interval_map.hpp>

extern "C" {
#include <sys/mman.h>
#include "relf.h"
}

// regex usings
using boost::regex;
using boost::regex_match;
using boost::smatch;
using boost::regex_constants::egrep;
using boost::match_default;
using boost::format_all;

using std::cerr;
using std::endl;
using std::ostringstream;
using std::string;
using std::deque;
using namespace dwarf::core;
using dwarf::tool::abstract_c_compiler;

vector<allocsite>
read_allocsites(std::istream& in)
{
	char buf[4096];
	string objname;
	string symname;
	unsigned file_addr;
	string sourcefile; 
	unsigned line;
	unsigned end_line;
	string alloc_typename;
	bool might_be_array;
	
	vector<allocsite> allocsites_to_add;
	
	opt<string> seen_objname;
	
	while (in.getline(buf, sizeof buf - 1)
		&& 0 == read_allocs_line(string(buf), objname, symname, file_addr, sourcefile, line, end_line, alloc_typename, might_be_array))
	{
		string nonconst_typename = alloc_typename;
		string clean_typename = nonconst_typename;
		boost::trim(clean_typename);
		
		allocsites_to_add.push_back((allocsite){
			clean_typename, sourcefile, objname, file_addr,
			/* is_synthetic */ clean_typename.substr(0, sizeof "__uniqtype_" - 1) != "__uniqtype_",
			might_be_array
		});
	} // end while read line
	cerr << "Found " << allocsites_to_add.size() << " allocation sites" << endl;
	return allocsites_to_add;
}
opt<vector<allocsite> > read_allocsites_for_binary(const string& s)
{
	/* Is there an allocsites file for the input object? */
	char *real_path = realpath(s.c_str(), NULL);
	assert(real_path);
	
	string full_path = string(getenv("META_BASE")?:"/usr/lib/meta") + "/" + real_path + ".allocs";
	std::ifstream in(full_path);
	if (in)
	{
		return read_allocsites(in);
	}
	else return opt<vector<allocsite> >();
}

void make_allocsites_relation(
    allocsites_relation_t& allocsites_relation,
    vector<allocsite> const& allocsites_to_add,
    multimap<string, iterator_df<type_die> >& types_by_codeless_name,
	root_die& r
    )
    
{
	for (auto i_alloc = allocsites_to_add.begin(); i_alloc != allocsites_to_add.end(); ++i_alloc)
	{
		string type_symname = i_alloc->clean_typename;
		string sourcefile = i_alloc->sourcefile;
		string objname = i_alloc->objname;
		unsigned file_addr = i_alloc->file_addr;

		/* Find a CU such that 
		 - one of its source files is named sourcefile, taken relative to comp_dir if necessary;
		 - that file defines a type of the name we want
		 */

		// look for a CU embodying this source file 
		auto cus = r.begin().children();
		/* What is the point of this search? It's mostly to resolve the
		 * codeless/ambiguous symnames that come out of dumptypes to the
		 * names that reliably exist in the generated typeinfo. We could
		 * just go with the codeless name and hope that it resolves to
		 * an alias, but there won't be a codeless alias if there are
		 * multiple incompatible definitions using the same name. Since
		 * we have DWARF at per-CU granularity, we use it to precisely
		 * identify the type in that CU. This could instead be done as a
		 * fixup pass over the .i.allocs files, as we do for base types.
		 * However, there are some advantages to doing it here... in particular
		 * the "second chance" mechanism */
		auto add_result = [&allocsites_relation, i_alloc, objname, file_addr]
			(const uniqued_name& name_used, bool is_incomplete) {
			bool declare_as_array0 = !i_alloc->is_synthetic && i_alloc->might_be_array && !is_incomplete;
			// add to the allocsites table too
			// recall: this is the mapping from allocsites to uniqtype addrs
			// the uniqtype addrs are given as idents, so we just have to use the same name
			allocsites_relation.insert(
				make_pair(
					make_pair(objname, file_addr),
					make_pair(name_used, declare_as_array0)
				)
			);
		};
		/* Some symnames are special, in that we can rely on liballocs
		 * to have generated them already, unambiguously and in codeless form.
		 * FIXME: less hacky way to detect these. */
		if (type_symname.size() == 0)
		{
			// we got a blank field from dumpallocs. nothing we can do
			continue;
		}
		if (type_symname == "__uniqtype____uninterpreted_byte"
			|| type_symname == "__uniqtype__void"
			|| type_symname == "__uniqtype____EXISTS1__1"
			|| type_symname == "__uniqtype____EXISTS1___PTR__1")
		{
			unsigned offset = sizeof ("__uniqtype__") - 1;
			add_result(make_pair("", type_symname.substr(offset, type_symname.size() - offset)), false);
			continue;
		}
		std::vector<iterator_df<compile_unit_die> > embodying_cus;
		auto search_one_cu = [=, &embodying_cus](iterator_df<compile_unit_die> i_cu,
			iterator_df<type_die>& out_near_miss) -> iterator_df<type_die> {
			opt<string> found_sourcefile_path;
			iterator_df<type_die> second_chance_type;
			if (i_cu->get_name() && i_cu->get_comp_dir())
			{
				auto cu_die_name = *i_cu->get_name();
				auto cu_comp_dir = *i_cu->get_comp_dir();

				for (unsigned i_srcfile = 1; i_srcfile <= i_cu->source_file_count(); i_srcfile++)
				{
					/* Does this source file have a matching name? */
					string current_sourcepath;
					string cu_srcfile_mayberelative = i_cu->source_file_name(i_srcfile);
					//if (!path(cu_srcfile_mayberelative).has_root_directory())
					if (cu_srcfile_mayberelative.length() > 0 && cu_srcfile_mayberelative.at(0) != '/')
					{ 
						current_sourcepath = cu_comp_dir + '/' + cu_srcfile_mayberelative;
					}
					else current_sourcepath = /*path(*/cu_srcfile_mayberelative/*)*/;

					// we look around a bit, since sizeof isn't enough to keep DIE in the object file
					if (current_sourcepath == sourcefile)
					{ 
						// YES this CU embodies the source file, so we can search for the type
						embodying_cus.push_back(i_cu);

						//auto found_type_entry = named_toplevel_types.find(clean_typename);
						auto found_types = types_by_codeless_name.equal_range(type_symname);

						if (found_types.first == found_types.second)
						{
							cerr << "Found no types for symbol name "
								<< type_symname << "; unique symbol names were: " << endl;
							set<string> uniques;
							for (auto i_el = types_by_codeless_name.begin();
								i_el != types_by_codeless_name.end(); ++i_el)
							{
								uniques.insert(i_el->first);
							}
							for (auto i_el = uniques.begin();
								i_el != uniques.end(); ++i_el)
							{
								if (i_el != uniques.begin()) cerr << ", ";
								cerr << *i_el;
							}
							// try the next source file -- we might get a near-miss there
							continue;
						}
						/* Make sure we get the version that is defined in this CU. */
						for (auto i_found = found_types.first; i_found != found_types.second; ++i_found)
						{
							if (i_found->second.enclosing_cu()
								== i_cu)
							{
								// we can exit the loop now
								cerr << "Success: found a type named " << i_found->first
									<< " in a CU named "
									<< *i_found->second.enclosing_cu().name_here()
									<< " == "
									<< *i_cu.name_here()
									<< endl;
								return i_found->second;
							}
							else 
							{
								assert(i_found->second.enclosing_cu().offset_here()
									!= i_cu.offset_here());

								cerr << "Found a type named " << i_found->first
									<< " but it was defined in a CU named "
									<< *i_found->second.enclosing_cu().name_here()
									<< " whereas we want one named "
									<< *i_cu.name_here()
									<< endl;
								out_near_miss = i_found->second;
								// keep searching
							}
						} // end "for matching types..."
					} // end "if source file matches..."
				} // end "for each source file in this CU..."
			} // end "if CU has a name..."
			return iterator_base::END;
		}; // end search one CU
		iterator_df<compile_unit_die> found_cu = iterator_base::END;
		iterator_df<type_die> found_type = iterator_base::END;
		iterator_df<type_die> found_near_miss = iterator_base::END;
		for (iterator_sibs<compile_unit_die> i_cu = cus.first;
			 i_cu != cus.second; ++i_cu)
		{
			found_type = search_one_cu(i_cu, found_near_miss);
			if (found_type)
			{
				found_cu = i_cu;
				break;
			}
		}
		if (!found_type)
		{
			cerr << "Warning: no type named " << type_symname 
				<< " in CUs embodying source file " << sourcefile
				<< " (found " << embodying_cus.size() << ":";
				for (auto i_cu = embodying_cus.begin(); i_cu != embodying_cus.end(); ++i_cu)
				{
					if (i_cu != embodying_cus.begin()) cerr << ", ";
					cerr << *(*i_cu)->get_name();
				}
				cerr << ") but required by allocsite: " << objname 
				<< "<" << type_symname << "> @" << std::hex << file_addr << std::dec << ">" << endl;

			if (found_near_miss)
			{
				cerr << "Warning: guessing that we can get away with " 
					<< found_near_miss << endl;
				found_type = found_near_miss;
			}
		}
		if (!found_type) continue; // give up! next allocation site
		// now we found the type
		//cerr << "SUCCESS: found type: " << *found_type << endl;
		/* NOTE: we can still get incomplete types used as sizeof, if the 
		 * user did "offsetof" on a field in them. That is how we will get
		 * them here. FIXME: if the user uses offsetof even on a *complete*
		 * type, we should skip the ARR0 here. E.g. if we have the variable-
		 * -length array be [1] not [0], we would ues offsetof to allocate
		 * space for extra training elements. */
		bool incomplete = !found_type->calculate_byte_size();
		add_result(canonical_key_for_type(found_type), incomplete);
	} // end for allocsite
}	

void merge_and_rewrite_synthetic_data_types(root_die& r, vector<allocsite>& as)
{
	for (auto i_a = as.begin(); i_a != as.end(); ++i_a)
	{
		if (i_a->is_synthetic)
		{
			cerr << "Found synthetic typename " << i_a->clean_typename;

			/* Add under the last CU in the file, to avoid (for now) offset woes. */
			auto cus_seq = r.begin().children().subseq_of<compile_unit_die>();

			auto last_cu = cus_seq.first;

			for (auto i_cu = cus_seq.first; 
				i_cu != cus_seq.second; 
				++i_cu, (i_cu != cus_seq.second && ((last_cu = i_cu), true)));

			auto created = dwarfidl::create_dies(last_cu, i_a->clean_typename);
			assert(created);
			assert(created.is_a<type_die>());
			/* We use the codeless name here, which is what dumpallocs would emit. */
			i_a->clean_typename = mangle_typename(make_pair("", 
				canonical_key_for_type(created.as_a<type_die>()).second));
		}
	}
}

std::pair<std::unique_ptr<root_die>, std::unique_ptr<std::ifstream> >
make_root_die_and_merge_synthetics(vector<allocsite>& as)
{
	/* what's the objname of the first entry? */
	string seen_objname = as.begin()->objname;
	auto p_objfile = std::unique_ptr<std::ifstream>(new std::ifstream(seen_objname));
	if (!*p_objfile)
	{
		assert(false);
	}
	/* what's the objname of the first entry? */
	auto p_root = std::unique_ptr<root_die>(new root_die(fileno(*p_objfile)));
	assert(p_root);
	/* rewrite the allocsites we were passed */
	merge_and_rewrite_synthetic_data_types(*p_root, as);
	return std::move(make_pair(std::move(p_root), std::move(p_objfile)));
}

string summary_code_to_string(opt<uint32_t> maybe_code)
{
	if (!maybe_code) return "";
	uint32_t code = *maybe_code;
	ostringstream summary_string_str;
	summary_string_str << std::hex << std::setfill('0') << std::setw(2 * sizeof code) << code 
		<< std::dec;
	return summary_string_str.str();
}
string
name_for_complement_base_type(iterator_df<base_type_die> base_t)
{
	/* For base types, we use our own language-independent naming scheme. */
	ostringstream name;
	unsigned size = *base_t->get_byte_size();
	auto encoding = base_t->get_encoding();
	assert(encoding == DW_ATE_signed || encoding == DW_ATE_unsigned);
	pair<Dwarf_Unsigned, Dwarf_Unsigned> bit_size_and_offset = base_t->bit_size_and_offset();
	bool needs_suffix = !((bit_size_and_offset.second == 0) 
		&& (bit_size_and_offset.first == 8 * size));
	name << ((base_t->get_encoding() == DW_ATE_signed) ? "uint" : "int")
		<< "$" << bit_size_and_offset.first;
	if (needs_suffix) name << "$" << bit_size_and_offset.second;

	return name.str();
}

string canonical_name_for_type(iterator_df<type_die> t)
{
	/* This is now reimplemented in libdwarfpp. But test against the old code. */
	string libdwarfpp_said = abstract_name_for_type(t);
	if (!t) { assert(libdwarfpp_said == "void"); return "void"; }
	// FIXME: not the right semantics probably
	if (t.is_a<unspecified_type_die>()) { assert(libdwarfpp_said == "void"); return "void"; }
	t = t->get_concrete_type();
	if (!t) { assert(libdwarfpp_said == "void"); return "void"; }
	if (!t.is_a<address_holding_type_die>() && !t.is_a<array_type_die>() && !t.is_a<subroutine_type_die>()
		&& !t.is_a<subprogram_die>())
	{
		/* for base types, the canonical key is *always* the summary code *only*, 
		 * i.e. the name component is empty. UNLESS we can place ourselves in a C
		 * equivalence class, in which case.... */
		string name_to_use;
		if (t.is_a<base_type_die>())
		{
			if (t.as_a<base_type_die>()->get_encoding() == 0)
			{
				assert(t.as_a<base_type_die>()->get_byte_size() == 1);
				name_to_use = "__uninterpreted_byte";
			}
			/* For base types, we use our own language-independent naming scheme. */
			else name_to_use = t.as_a<base_type_die>()->get_canonical_name();
		} 
		else
		{
			/* FIXME: deal with nested/qualified names also (nested data types, 
			   local data types, C++ namespaces). */
			/* FIXME: deal with struct/union tags also (but being sensitive to language: 
			   don't do it with C++ CUs). */
			if (t.name_here())
			{
				name_to_use = *name_for_type_die(t);
			}
			else
			{
				string offsetstr = offset_to_string(t.offset_here());
				/* We really want to allow deduplicating anonymous structure types
				 * that originate in the same header file but are included in multiple
				 * compilation units. Since each gets a different offset, using that
				 * for the fake name string is a bad idea. Instead, use the defining
				 * source file path, if we have it. */
				if (t->get_decl_file() && t->get_decl_line())
				{
					ostringstream s;
					opt<string> maybe_fqp = t.enclosing_cu()->source_file_fq_pathname(*t->get_decl_file());
					s << (maybe_fqp ? 
						*maybe_fqp : 
						t.enclosing_cu()->source_file_name(*t->get_decl_file())) 
						<< "_" << *t->get_decl_line();
					name_to_use = s.str();
				}
				else name_to_use = offsetstr;
			}
		}
		assert(libdwarfpp_said == name_to_use || t.is_a<subrange_type_die>());
		return name_to_use;
	}
	else if (t.is_a<subroutine_type_die>() || t.is_a<subprogram_die>())
	{
		// "__FUN_FROM_" ^ (labelledArgTs argTss 0) ^ (if isSpecial then "__VA_" else "") ^ "__FUN_TO_" ^ (stringFromSig returnTs) 		
		ostringstream s;
		s << "__FUN_FROM_";
		auto fps = t.children().subseq_of<formal_parameter_die>();
		unsigned argnum = 0;
		for (auto i_fp = fps.first; i_fp != fps.second; ++i_fp, ++argnum)
		{
			/* args should not be void */
			/* We're making a canonical typename, so use canonical argnames. */
			s << "__ARG" << argnum << "_" << canonical_name_for_type(i_fp->find_type());
		}
		if (IS_VARIADIC(t))
		{
			s << "__VA_";
		}
		s << "__FUN_TO_";
		iterator_df<type_die> return_t = RETURN_TYPE(t);
		
		s << ((!return_t || !return_t->get_concrete_type()) ? string("void") : canonical_name_for_type(return_t));
		string result = s.str();
		assert(libdwarfpp_said == result);
		return result;
	}
	else if (t.is_a<array_type_die>())
	{
		/* What should the type descriptor for "array of n T" look like? 
		 * What should it be called? 
		 * Answers: always has exactly one nmemb, and use __ARRn_. */
		
		/* What should the type descriptor for "array of undeterminedly-many T" look like?
		 * What should it be called? Answer: use __ARR0_*/
		
		/* How do we encode mutual recursion between array and pointer?
		 * Answer: nothing special: just cut off the array first part and emit it specially,
		 * with a reference to the remainder (what it's an array of).
		 * This handles multidimensional arrays too.
		 */
		
		auto array_t = t.as_a<array_type_die>();
		ostringstream array_prefix;
		opt<Dwarf_Unsigned> element_count = array_t->element_count();
		array_prefix << "__ARR" << (element_count ? *element_count : 0) << "_";
		string el_type_name = canonical_name_for_type(array_t->get_type());
		assert(libdwarfpp_said == array_prefix.str() + el_type_name);
		return array_prefix.str() + el_type_name;
	}
	else if (t.is_a<string_type_die>())
	{
		auto string_t = t.as_a<string_type_die>();
		// get the name of whatever the element type is, and prepend a prefix
		const Dwarf_Unsigned element_size = 1; /* FIXME: always 1? */
		opt<Dwarf_Unsigned> opt_byte_size = string_t->fixed_length_in_bytes();
		opt<Dwarf_Unsigned> element_count
		 = opt_byte_size ? opt<Dwarf_Unsigned>(*opt_byte_size / element_size ) : opt<Dwarf_Unsigned>();
		ostringstream string_prefix;
		string_prefix << "__STR" << (element_count ? *element_count : 0) << "_"
			<< element_size;

		assert(libdwarfpp_said == string_prefix.str());
		return string_prefix.str();
	}
	else // DW_TAG_pointer_type and friends
	{
		int levels_of_indirection = 0;
		ostringstream indirection_prefix;
		iterator_df<type_die> working_t = t->get_concrete_type(); // initially
		while (working_t && working_t.is_a<address_holding_type_die>())
		{
			++levels_of_indirection;
			switch (working_t.tag_here())
			{
				case DW_TAG_pointer_type: 
					indirection_prefix << "__PTR_"; break;
				case DW_TAG_reference_type:
					indirection_prefix << "__REF_"; break;
				case DW_TAG_rvalue_reference_type:
					indirection_prefix << "__RR_"; break;
				default:
					assert(false);
			}
			
			// try moving on to the next in the chain
			if (working_t.is_a<address_holding_type_die>()) 
			{
				working_t = working_t.as_a<address_holding_type_die>()->get_type();
				// concretify if we got something
				if (working_t)
				{
					working_t = working_t->get_concrete_type();
				}
			}
		}
		assert(levels_of_indirection >= 1);
		
		ostringstream os;
		os << indirection_prefix.str() << (!working_t ? "void" : canonical_name_for_type(working_t));
		assert(libdwarfpp_said == os.str());
		return os.str();
	}
	assert(false);
	abort();
}

string canonical_codestring_for_type(iterator_df<type_die> t)
{
	if (!t) return "";
	t = t->get_concrete_type();
	if (!t) return "";

	opt<uint32_t> code = type_summary_code(t);
	string summary_string;
	if (code)
	{
		summary_string = summary_code_to_string(*code);
		assert(summary_string.size() == 2 * sizeof *code);
	} else summary_string = "";
	return summary_string;
}

uniqued_name
canonical_key_for_type(iterator_df<type_die> t)
{
	return make_pair(canonical_codestring_for_type(t), canonical_name_for_type(t));
}

// iterator_df<type_die>
// find_type_in_cu(iterator_df<compile_unit_die> cu, const string& name)
// {
// 	/* For the most part, we just do named_child.
// 	 * BUT, for base types, we widen the search, using our equivalence classes. */
// 	for (const char **const *p_equiv = &abstract_c_compiler::base_typename_equivs[0]; *p_equiv != NULL; ++p_equiv)
// 	{
// 		for (const char **p_el = p_equiv[0]; *p_el != NULL; ++p_el)
// 		{
// 			if (name == string(*p_el))
// 			{
// 				/* We try every element in the class */
// 				for (const char **i_attempt = p_equiv[0]; *i_attempt != NULL; ++i_attempt)
// 				{
// 					iterator_df<type_die> found = cu.named_child(string(*i_attempt));
// 					if (found != iterator_base::END) return found;
// 				}
// 			}
// 		}
// 	}
// 
// 	// if we got here, just try named_child
// 	return iterator_df<type_die>(cu.named_child(name)); //shared_ptr<type_die>();
// }

opt<uint32_t> type_summary_code(core::iterator_df<core::type_die> t)
{
	if (!t) return opt<uint32_t>(0);
	else return t->summary_code();
}
opt<uint32_t> signedness_complement_type_summary_code(core::iterator_df<core::base_type_die> base_t)
{
	unsigned encoding = base_t->get_encoding();
	assert(encoding == DW_ATE_signed || encoding == DW_ATE_unsigned);
	summary_code_word<uint32_t> output_word;
	assert(base_t->get_byte_size());
	unsigned byte_size = *base_t->get_byte_size();
	pair<Dwarf_Unsigned, Dwarf_Unsigned> bit_size_and_offset = base_t->bit_size_and_offset();
	unsigned bit_size = bit_size_and_offset.first;
	unsigned bit_offset = bit_size_and_offset.second;
	output_word << DW_TAG_base_type 
		<< (encoding == DW_ATE_unsigned ? DW_ATE_signed : DW_ATE_unsigned) 
		<< byte_size << bit_size << bit_offset;
	return output_word.val;
}	

void get_types_by_codeless_uniqtype_name(
	std::multimap<string, iterator_df<type_die> >& m, 
	iterator_df<> begin, iterator_df<> end)
{	
	/* First we look through the whole file and index its types by their *codeless*
	 * *canonical* uniqtype name, i.e. we blank out the first element of the name pair. */
	bool done_some_output = false;
	for (iterator_df<> i = begin; i != end; ++i)
	{
		if (i.is_a<type_die>())
		{
			if (isatty(fileno(std::cerr)))
			{
				if (done_some_output) std::cerr << "\r";
				std::cerr << "Codeless uniqtypes: adding DIE at 0x" << std::hex << i.offset_here() << std::dec;
				done_some_output = true;
			}
			opt<string> opt_name = i.name_here(); // for debugging
			if (opt_name)
			{
				string name = *opt_name;
				assert(name != "");
			}
			
			auto t = i.as_a<type_die>();
			assert(t.is_real_die_position());
			auto concrete_t = t->get_concrete_type();
			pair<string, string> uniqtype_name_pair;
			
			// handle void case specially
			string canonical_typename = canonical_name_for_type(t);
			
			/* CIL/trumptr will only generate references to aliases in the case of 
			 * base types. We need to handle these here. What should happen? 
			 * 
			 * - we will see references looking like __uniqtype__signed_char
			 * - we want to link in two things:
			 *    1. the nameless __uniqtype_<code>_ definition of this base type
			 *    2. the alias    __uniqtype_<code>_signed_char from the usual alias handling
			 * - we do this by indexing all our types by a *codeless* version of their
			 *   name, then matching our inputs lines against that.
			 * - the input lines will have signed_char instead of ""
			 * - ... so that's what we need to put in our index.
			 * 
			 * IT GETS WORSE: the same is true for any typename *mentioning* a base
			 * type! We will see references in terms of C-canonicalised base type names, 
			 * but we will be trying to match them against language-independent names. 
			 * It seems that we need to do a separate "C fix up" pass first.
			 * This is now done in link-used-types (and will be 
			 * */
			
			
			if (canonical_typename == "")
			{
				assert(concrete_t.is_a<base_type_die>());
				// if the base type has no name, this DWARF type is useless to us
				if (!concrete_t.name_here()) continue;
				canonical_typename = *name_for_type_die(concrete_t);
			}
			string codeless_symname = mangle_typename(make_pair("", canonical_typename));

			m.insert(make_pair(codeless_symname, concrete_t));

			/* Special handling for base types: also add them by the name in which they 
			 * appear in the DWARF, *and* by their C-canonical name. Our CIL frontend
			 * doesn't know the exact bit-widths so must use the latter. */
			if (concrete_t.is_a<base_type_die>() && concrete_t.name_here()
				&& !concrete_t.as_a<base_type_die>()->is_bitfield_type())
			{
				m.insert(
					make_pair(
						mangle_typename(make_pair("", *name_for_type_die(concrete_t))), 
						concrete_t
					)
				);
				const char **equiv = abstract_c_compiler::get_equivalence_class_ptr(name_for_type_die(concrete_t)->c_str());
				if (equiv)
				{
					m.insert(
						make_pair(
							mangle_typename(make_pair("", equiv[0])), 
							concrete_t
						)
					);
				}
			}
		}
	}
}
pair<pair<ElfW(Sym) *, char*>, unsigned> sticky_root_die::get_symtab()
{
	if (!opt_symtab)
	{
		Elf *e = get_elf();
		Elf_Scn *scn = NULL;
		GElf_Shdr shdr;
		size_t shstrndx;
		if (elf_getshdrstrndx(e, &shstrndx) != 0)
		{
			throw lib::No_entry();
		}
		// iterate through sections looking for symtab
		while (NULL != (scn = elf_nextscn(e, scn)))
		{
			if (gelf_getshdr(scn, &shdr) != &shdr)
			{
				cerr << "Unexpected ELF error" << std::endl;
				throw lib::No_entry(); 
			}
			if (shdr.sh_type == SHT_SYMTAB) break;
		}
		if (!scn) throw lib::No_entry();
		Elf_Data *symtab_rawdata = elf_rawdata(scn, NULL);
		assert(symtab_rawdata);
		assert(symtab_rawdata->d_size >= shdr.sh_size);
		ElfW(Sym) *symtab = reinterpret_cast<ElfW(Sym) *>(symtab_rawdata->d_buf);
		opt_symtab = symtab;
		n = shdr.sh_size / shdr.sh_entsize;
		int strtab_ndx = shdr.sh_link;
		if (strtab_ndx == 0) throw lib::No_entry();
		Elf_Scn *strtab_scn = NULL;
		strtab_scn = elf_getscn(e, strtab_ndx);
		GElf_Shdr strtab_shdr;
		if (gelf_getshdr(strtab_scn, &strtab_shdr) != &strtab_shdr) throw lib::No_entry();
		Elf_Data *strtab_rawdata = elf_rawdata(strtab_scn, NULL);
		assert(strtab_rawdata);
		assert(strtab_rawdata->d_size >= strtab_shdr.sh_size);
		strtab = reinterpret_cast<char *>(strtab_rawdata->d_buf);
		assert(strtab);
		assert(symtab);
		// FIXME: cleanup?
	}
	return make_pair(make_pair(*opt_symtab, strtab), n);
}

bool sticky_root_die::is_base_object(int user_fd)
{
	bool retval = true;
	/* This test distinguishes "base" objects, i.e. those
	 * we might reasonably load and run,
	 * from "meta" objects containing only debug info
	 * i.e. those produced by dh_strip or other methods.
	 * It is quite hard to identify these files because
	 * they may have all of the usual sections and headers.
	 * Most of the PROGBITS sections have become NOBITS,
	 * but ELF allows NOBITS where PROGBITS would do.
	 * We use an ad-hoc test: whether it has either an
	 * entry point that points into nobits,
	 * or a DYNAMIC phdr that point into nobits.
	 * This covers static and dynamic executables
	 * and dynamic shared libraries. */
	struct stat s;
	int ret = fstat(user_fd, &s);
	if (ret != 0) throw No_entry();
	long page_size = sysconf(_SC_PAGESIZE);
	void *mapping = mmap(NULL, ROUND_UP(s.st_size, page_size),
		PROT_READ, MAP_PRIVATE, user_fd, 0);
	if (mapping == MAP_FAILED) throw No_entry();
	const char magic[] = { '\177', 'E', 'L', 'F' };
	if (0 != memcmp(magic, mapping, sizeof magic)) throw No_entry();
	ElfW(Ehdr) *ehdr = reinterpret_cast<ElfW(Ehdr) *>(mapping);
	if (ehdr->e_ident[EI_CLASS] != ELFCLASS64
		|| ehdr->e_ident[EI_DATA] != ELFDATA2LSB)
	{
		std::cerr << "ELF file is of unsupported class or endianness" << std::endl;
		throw No_entry();
	}
	if (!ehdr->e_shoff) throw No_entry();
	ElfW(Shdr) *shdr = reinterpret_cast<ElfW(Shdr) *>((char*) mapping + ehdr->e_shoff);
	if (!ehdr->e_phoff) throw No_entry();
	ElfW(Phdr) *phdr = reinterpret_cast<ElfW(Phdr) *>((char*) mapping + ehdr->e_phoff);
	/* Walk the section headers remembering their base addresses. */
	boost::icl::interval_map<ElfW(Addr), std::set<ElfW(Shdr)*> > m;
	auto& right_open = boost::icl::interval<ElfW(Addr)>::right_open;
	for (unsigned i = 1; i < ehdr->e_shnum; ++i)
	{
		if (!(shdr[i].sh_flags & SHF_ALLOC)) continue; // skip non-allocated sections
		if ((shdr[i].sh_flags & SHF_TLS) && (shdr[i].sh_type == SHT_NOBITS))
		{ continue; /* skip thread-local NOBITS sections, a.k.a. .tbss, as they may overlap
		               other stuff in vaddr-space, and indeed the GNU linker arranges that they do!
		  https://stackoverflow.com/questions/25501044/gcc-ld-overlapping-sections-tbss-init-array-in-statically-linked-elf-bin */ }
		std::set<ElfW(Shdr)*> singleton_set;
		singleton_set.insert(&shdr[i]);
		m += make_pair(
				right_open(shdr[i].sh_addr, shdr[i].sh_addr + shdr[i].sh_size),
				singleton_set
			);
	}
	auto is_nobits = [m, &right_open, shdr](ElfW(Addr) addr, unsigned span_len) -> bool {
		/* Find the section spanning this address range. */
		auto found = m.find(right_open(addr, addr + span_len));
		if (found == m.end()) return false;
		if (found->second.size() == 0) return false;
		if (found->second.size() > 1)
		{
			std::cerr << "Address range 0x" << std::hex << addr
				  << "-0x" << std::hex << (addr + span_len)
				  << " spanned by more than one non-empty section; indices: {";
			for (auto i_found = found->second.begin();
				i_found != found->second.end();
				++i_found)
			{
				// FIXME: these indices, calculated by *i_found - &shdr[0], are coming
				// out screwy, e.g. 6 too low, on one problem object I saw
				// (liballocs/tests/section-group/lib1.so in crunchb-bench, which came out with the
				// .tbss overlap noted above (comment the 'continue' to reproduce) )
				if (i_found != found->second.begin()) std::cerr << ", ";
				std::cerr << (((ElfW(Shdr)*)(*i_found)) - &shdr[0]) << " (begins 0x" << std::hex << (*i_found)->sh_addr << ")";
			}
			std::cerr << "}" << std::endl;
			abort();
		}
		auto &it = *found->second.begin();
		if (it->sh_type == SHT_NOBITS) return true;
		return false;
	};
	/* Which section contains the entry point? Is it nobits? */
	ElfW(Addr) dyn_vaddr = 0;
	if (ehdr->e_entry && is_nobits(ehdr->e_entry, 1))
	{
		retval = false; goto out;
	}
	for (unsigned i = 0; i < ehdr->e_phnum; ++i)
	{
		if (phdr[i].p_type == PT_DYNAMIC) { dyn_vaddr = phdr[i].p_vaddr; break; }
	}
	if (dyn_vaddr && is_nobits(dyn_vaddr, 1))
	{
		retval = false; goto out;
	}
out:
	munmap(mapping, ROUND_UP(s.st_size, page_size));
	return retval;
}

bool sticky_root_die::has_dwarf(int user_fd)
{
	dwarf::lib::Dwarf_Debug d;
	dwarf::lib::Dwarf_Error e;
	bool retval;
	int ret = dwarf::lib::dwarf_init(user_fd, DW_DLC_READ, nullptr, nullptr, &d, &e);
	if (ret == 0)
	{
		Dwarf_Unsigned seen_cu_header_length;
		Dwarf_Half seen_version_stamp;
		Dwarf_Unsigned seen_abbrev_offset;
		Dwarf_Half seen_address_size;
		Dwarf_Half seen_offset_size;
		Dwarf_Half seen_extension_size;
		Dwarf_Unsigned seen_next_cu_header;

		ret = dwarf_next_cu_header_b(d,
			&seen_cu_header_length, &seen_version_stamp,
			&seen_abbrev_offset, &seen_address_size,
			&seen_offset_size, &seen_extension_size,
			&seen_next_cu_header, &e);
		if (ret == DW_DLV_OK)
		{
			retval = true;
		} else retval = false;
		ret = dwarf_finish(d, &e);
	} else retval = false;
	return retval;
}

int sticky_root_die::open_debuglink(int user_fd)
{
	/* FIXME: linux-specific big hacks here. */
	char *cmdstr = NULL;
	char *fdstr = NULL;
	int ret = asprintf(&fdstr, "/dev/fd/%d", user_fd);
	if (ret <= 0) throw No_entry();
	/* HACK HACK HACK */
	ret = asprintf(&cmdstr, "bash -c \". ${LIBALLOCS}/tools/debug-funcs.sh && read_debuglink %s | tr -d '\\n'\"", fdstr);
	if (ret <= 0) throw No_entry();
	assert(cmdstr != NULL);
	FILE *p = popen(cmdstr, "r");
	char debuglink_buf[4096];
	size_t nread = fread(debuglink_buf, 1, sizeof debuglink_buf, p);
	int ret_fd;
	if (nread == sizeof debuglink_buf)
	{
		// basically we overflowed
		std::cerr << "Debuglink contained too many characters" << std::endl;
		ret_fd = -1;
	}
	else if (nread > 0)
	{
		/* We've successfully slurped a debuglink */
		std::cerr << "Slurped debuglink: " << debuglink_buf << std::endl;
		/* How to build the path from the debuglink? GDB docs say we
		 * have to try:
		 * the directory of the executable file, then
		 * in a subdirectory of that directory named .debug, and finally
		 * under each one of the global debug directories,
		 *      in a subdirectory whose name is identical to
		 *      the leading directories of the executable’s absolute file name. */
		std::vector<std::string> paths_to_try;
		char *fd_realpath = realpath(fdstr, NULL);
		if (fd_realpath)
		{
			// to save us from strdup'ing, construct a string
			// only do the first one if debuglink it doesn't match basename
			if (string(debuglink_buf) !=
				string(basename((char*) string(fd_realpath).c_str())))
			{
				/* Try the debuglink basename on the fd realpath */
				paths_to_try.push_back(
					string(dirname((char*) string(fd_realpath).c_str()))
					+ "/" + debuglink_buf
				);
			}
			// try .debug/
			paths_to_try.push_back(
				string(dirname((char*) string(fd_realpath).c_str()))
				+ "/.debug/" + debuglink_buf
			);
			// HACK: try /usr/lib/debug + fd dirname + debuglink
			paths_to_try.push_back(
				string("/usr/lib/debug/")
				+ string(dirname((char*) string(fd_realpath).c_str()))
				+ "/"
				+ debuglink_buf
			);

			free(fd_realpath);
		}
		for (auto i_path = paths_to_try.begin(); i_path != paths_to_try.end();
			++i_path)
		{
			ret_fd = open(i_path->c_str(), O_RDONLY);
			if (ret_fd != -1) break;
		}
	}
	free(cmdstr);
	return ret_fd;
}
int sticky_root_die::open_debug_via_build_id(int user_fd)
{
	/* FIXME: linux-specific big hacks here. */
	char *cmdstr = NULL;
	char *fdstr = NULL;
	int ret = asprintf(&fdstr, "/dev/fd/%d", user_fd);
	if (ret <= 0) throw No_entry();
	/* HACK HACK HACK */
	ret = asprintf(&cmdstr, "bash -c \". ${LIBALLOCS}/tools/debug-funcs.sh && read_build_id %s | tr -d '\\n'\"", fdstr);
	if (ret <= 0) throw No_entry();
	assert(cmdstr != NULL);
	FILE *p = popen(cmdstr, "r");
	char build_id_buf[41];
	size_t nread = fread(build_id_buf, 1, sizeof build_id_buf - 1, p);
	build_id_buf[40] = '\0';
	int ret_fd;
	if (nread > 0)
	{
		/* We've successfully slurped a build_id */
		std::cerr << "Slurped build ID: " << build_id_buf << std::endl;
		/* How to build the path from the build ID? GDB docs say we
		 * have to try:
		 * the directory of the executable file, then
		 * in a subdirectory of that directory named .debug, and finally
		 * under each one of the global debug directories,
		 *      in a subdirectory whose name is identical to
		 *      the leading directories of the executable’s absolute file name. */
		std::vector<std::string> paths_to_try;
		paths_to_try.push_back(
				string("/usr/lib/debug/.build-id/")
				+ build_id_buf[0]
				+ build_id_buf[1]
				+ "/"
				+ (build_id_buf + 2)
				+ ".debug"
			);
		std::cerr << "Trying: " << *paths_to_try.begin() << std::endl;
		for (auto i_path = paths_to_try.begin(); i_path != paths_to_try.end();
			++i_path)
		{
			ret_fd = open(i_path->c_str(), O_RDONLY);
			if (ret_fd != -1) break;
		}
	} else ret_fd = -1;
	free(cmdstr);
	return ret_fd;
}

shared_ptr<sticky_root_die> sticky_root_die::create(int user_fd)
{
	/* This is a helper not a constructor, because we have to
	 * inspect user_fd before we know what constructor to call. */
	bool is_base = is_base_object(user_fd);
	/* Easy case: a base object containing DWARF. */
	if (is_base && has_dwarf(user_fd))
	{ return std::make_shared<sticky_root_die>(user_fd, user_fd); }
	int dbg_fd;
	if (is_base)
	{
		dbg_fd = open_debuglink(user_fd);
		if (dbg_fd == -1) dbg_fd = open_debug_via_build_id(user_fd);
	}
	else dbg_fd = user_fd;
	if (dbg_fd != -1) return std::make_shared<sticky_root_die>(dbg_fd, user_fd);
	return std::shared_ptr<sticky_root_die>();
}
