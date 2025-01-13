/* This is a simple dwarfpp program which generates a C file
 * recording data on a uniqued set of data types  allocated in a given executable.
 */
 
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <cctype>
#include <cstdlib>
#include <memory>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/icl/interval_map.hpp>
#include <srk31/algorithm.hpp>
#include <srk31/ordinal.hpp>
#include <cxxgen/tokens.hpp>
#include <dwarfpp/lib.hpp>
#include <dwarfpp/frame.hpp>
#include <dwarfpp/regs.hpp>
#include <fileno.hpp>

#include "stickyroot.hpp"
#include "uniqtypes.hpp"
#include "frame-element.hpp" /* we mostly don't use this, except find_equal_range_last */
#include "relf.h"

using std::cin;
using std::cout;
using std::cerr;
using std::map;
using std::make_shared;
using std::ios;
using std::ifstream;
using std::dynamic_pointer_cast;
using boost::optional;
using std::ostringstream;
using std::set;
using namespace dwarf;
//using boost::filesystem::path;
using dwarf::core::iterator_base;
using dwarf::core::iterator_df;
using dwarf::core::iterator_sibs;
using dwarf::core::type_die;
using dwarf::core::subprogram_die;
using dwarf::core::compile_unit_die;
using dwarf::core::member_die;
using dwarf::core::with_data_members_die;
using dwarf::core::variable_die;
using dwarf::core::program_element_die;
using dwarf::core::with_dynamic_location_die;
using dwarf::core::address_holding_type_die;
using dwarf::core::array_type_die;
using dwarf::core::type_chain_die;

using namespace dwarf::lib;

// regex usings
using boost::regex;
using boost::regex_match;
using boost::smatch;
using boost::regex_constants::egrep;
using boost::match_default;
using boost::format_all;

using namespace allocs::tool;

static string typename_for_vaddr_interval(iterator_df<subprogram_die> i_subp, 
	const boost::icl::discrete_interval<Dwarf_Off> interval)
{
	std::ostringstream s_typename;
	if (i_subp.name_here()) s_typename << *i_subp.name_here();
	else s_typename << "0x" << std::hex << i_subp.offset_here() << std::dec;
	s_typename << "_vaddrs_0x" << std::hex << interval.lower() << "_0x" 
		<< interval.upper() << std::dec;

	return s_typename.str();
}

static int debug_out = 1;

using dwarf::lib::Dwarf_Off;
using dwarf::lib::Dwarf_Addr;
using dwarf::lib::Dwarf_Signed;
using dwarf::lib::Dwarf_Unsigned;

struct compare_first_signed_second_offset
{
	bool operator()(const pair< Dwarf_Signed, iterator_df<with_dynamic_location_die> >& x,
		            const pair< Dwarf_Signed, iterator_df<with_dynamic_location_die> >& y)
		const
	{
		return (x.first < y.first)
			|| ((x.first == y.first) && x.second.offset_here() < y.second.offset_here());
	}
};

using std::unordered_set;

template <class Target>
struct iter_hash
{
	typedef iterator_df<Target> T;
	
	static size_t hash_fn(const T& v) { return v.offset_here(); }
	static bool eq_fn(const T& v1, const T& v2)
	{ return v1.offset_here() == v2.offset_here(); }
	
	struct set : unordered_set<
		T,
		std::function<__typeof__(hash_fn)>,
		std::function<__typeof__(eq_fn)>
	>
	{
		set() : unordered_set<T, std::function<__typeof__(hash_fn)>, std::function<__typeof__(eq_fn)> >({}, 0, hash_fn, eq_fn) {}
	};
};

template <class Target, class Second>
struct iterfirst_pair_hash
{
	typedef pair<iterator_df<Target>, Second> T;
	
	static size_t hash_fn(const T& v)
	{ return v.first.offset_here() ^ std::hash<Second>()(v.second); }
	static bool eq_fn(const T& v1,  const T& v2)
	{ return v1.first.offset_here() == v2.first.offset_here(); }

	struct set : unordered_set< 
		T,
		std::function<__typeof__(hash_fn)>,
		std::function<__typeof__(eq_fn)>
	>
	{
		set() : unordered_set<T, std::function<__typeof__(hash_fn)>, std::function<__typeof__(eq_fn)> >
			({}, 0, hash_fn, eq_fn) 
			{}
	};
};

template <class First, class Target>
struct itersecond_pair_hash
{
	typedef pair<First, iterator_df<Target> > T;
	
	static size_t hash_fn(const T& v) 
	{ return v.second.offset_here() ^ std::hash<First>()(v.first); }
	static bool eq_fn(const T& v1, 	const T& v2)
	{ return v1.first == v2.first &&
	    v1.second.offset_here() == v2.second.offset_here(); }

	struct set : unordered_set< 
		T,
		std::function<__typeof__(hash_fn)>,
		std::function<__typeof__(eq_fn)>
	>
	{
		set() : unordered_set<T, std::function<__typeof__(hash_fn)>, std::function<__typeof__(eq_fn)> >
			({}, 0, hash_fn, eq_fn) 
			{}
	};
};


namespace std {
template <>
struct hash<const dwarf::lib::Dwarf_Loc>
{
	size_t operator()(const dwarf::lib::Dwarf_Loc& v)
	{
		size_t working = 0;
		working ^= v.lr_atom;
		working ^= v.lr_number;
		working ^= v.lr_number2;
		working ^= v.lr_offset;
		return working;
	}
};
template <>
struct hash<dwarf::encap::loc_expr>
{
	size_t operator()(const dwarf::encap::loc_expr& v)
	{
		size_t working = 0;
		for (auto i = v.begin(); i != v.end(); ++i)
		{
			working ^= std::hash<__typeof__(*i)>()(*i);
		}
		return working;
	}
};
}

int main(int argc, char **argv)
{
	/* We open the file named by argv[1] and dump its DWARF types. */ 
	
	if (argc <= 1) 
	{
		cerr << "Please name an input file." << endl;
		exit(1);
	}
	std::ifstream infstream(argv[1]);
	if (!infstream) 
	{
		cerr << "Could not open file " << argv[1] << endl;
		exit(1);
	}
	
	if (getenv("FRAMETYPES_DEBUG"))
	{
		debug_out = atoi(getenv("FRAMETYPES_DEBUG"));
	}
	
	using core::root_die;
	int fd = fileno(infstream);
	shared_ptr<sticky_root_die> p_root = sticky_root_die::create(fd);
	if (!p_root) { std::cerr << "Error opening file" << std::endl; return 1; }
	sticky_root_die& root = *p_root;
	assert(&root.get_frame_section());

	struct subprogram_key : public pair< pair<string, string>, string > // ordering for free
	{
		subprogram_key(const string& subprogram_name, const string& sourcefile_name, 
			const string& comp_dir) : pair(make_pair(subprogram_name, sourcefile_name), comp_dir) {}
		string subprogram_name() const { return first.first; }
		string sourcefile_name() const { return first.second; }
		string comp_dir() const { return second; }
	};

	map<subprogram_key, iterator_df<subprogram_die> > subprograms_list;

	for (iterator_df<> i = root.begin(); i != root.end(); ++i)
	{
		if (i.is_a<subprogram_die>())
		{
			auto i_cu = i.enclosing_cu();
			
			iterator_df<subprogram_die> i_subp = i;
			// only add real, defined subprograms to the list
			if ( 
					( !i_subp->get_declaration() || !*i_subp->get_declaration() )
			   )
			{
				string sourcefile_name = i_subp->get_decl_file() ? 
					i_cu->source_file_name(*i_subp->get_decl_file())
					: "(unknown source file)";
				string comp_dir = i_cu->get_comp_dir() ? *i_cu->get_comp_dir() : "";

				string subp_name;
				if (i_subp.name_here()) subp_name = *i_subp.name_here();
				else 
				{
					std::ostringstream s;
					s << "0x" << std::hex << i_subp.offset_here();
					subp_name = s.str();
				}

				auto ret = subprograms_list.insert(
					make_pair(
						subprogram_key(subp_name, sourcefile_name, comp_dir), 
						i_subp
					)
				);
				if (!ret.second)
				{
					/* This means that "the same value already existed". */
					cerr << "Warning: subprogram " << *i_subp
						<< " already in subprograms_list as " 
						<< ret.first->first.subprogram_name() 
						<< " (in " 
						<< ret.first->first.sourcefile_name()
						<< ", compiled in " << ret.first->first.comp_dir()
						<< ")"
						<< endl;
				}
			}
		}
	}
	cerr << "Found " << subprograms_list.size() << " subprograms." << endl;
	
	/* For each subprogram, for each vaddr range for which its
	 * stack frame is laid out differently, output a uniqtype record.
	 * We do this by
	 * - collecting all local variables and formal parameters on a depthfirst walk;
	 * - collecting their vaddr ranges into a partition, splitting any overlapping ranges
	     and building a mapping from each range to the variables/parameters valid in it;
	 * - when we're finished, outputting a distinct uniqtype for each range;
	 * - also, output a table of IPs-to-uniqtypes. 
	 *
	 * We also output an allocsites record for each one, wit the allocsite as the
	 *  */

	
	typedef iterfirst_pair_hash< 
		with_dynamic_location_die, encap::loc_expr/* ,
		compare_first_iter_offset<encap::loc_expr> */
	>::set live_set_t;
	typedef boost::icl::interval_map< Dwarf_Off, live_set_t > intervals_t;
	typedef boost::icl::interval_map< 
			Dwarf_Off, 
			set<pair< 
					Dwarf_Signed, // frame offset
					iterator_df< with_dynamic_location_die >
				>,
				compare_first_signed_second_offset 
			>
		> frame_intervals_t;
#ifdef DEBUG
	typedef boost::icl::interval_map< 
			Dwarf_Off, 
			iterfirst_pair_hash< 
				with_dynamic_location_die,
				string
			>::set/* ,
				compare_first_iter_offset<string> */
		> discarded_intervals_t;
#endif
		
	map< iterator_df<subprogram_die>, frame_intervals_t > intervals_by_subprogram;
	map< iterator_df<subprogram_die>, unsigned > frame_offsets_by_subprogram;
	
	using dwarf::core::with_static_location_die;
	cout << "#include \"allocmeta-defs.h\"\n";
	cout << "#include \"uniqtype-defs.h\"\n\n";
	set<string> names_emitted;

	for (auto i_i_subp = subprograms_list.begin(); i_i_subp != subprograms_list.end(); ++i_i_subp)
	{
		auto i_subp = i_i_subp->second;
		
		intervals_t subp_vaddr_intervals; // CU- or file-relative?

		/* Put this subp's vaddr ranges into the map */
		auto subp_intervals = i_subp->file_relative_intervals(
			root,
			[&i_subp, &root](const std::string&, void *) -> with_static_location_die::sym_binding_t {
				/* We need this symbol resolver because sometimes the DWARF info
				 * won't include a with-address-range entry for a function. I have
				 * seen this for external-definition-emmited C99 inline functions
				 * in gcc 7.2.x, but other cases are possible. */
				Dwarf_Off file_relative_start_addr; 
				Dwarf_Unsigned size;
				
				if (!i_subp.name_here()) throw No_entry();
				string s = *i_subp.name_here();
				
				auto symtab_etc = root.get_symtab();
				auto &symtab = symtab_etc.first.first;
				auto &strtab = symtab_etc.first.second;
				unsigned &n = symtab_etc.second.second;
				
				for (auto p = symtab; p < symtab + n; ++p)
				{
					if (p->st_name != 0 && string(strtab + p->st_name) == s)
					{
						return (with_static_location_die::sym_binding_t)
						{ p->st_value, p->st_size };
					}
				}
				
				throw No_entry();
				
			}, nullptr /* FIXME: write a symbol resolver -- do we need this? can just pass 0? */
		);

		struct iterator_bf_skipping_types : public core::iterator_bf<>
		{
			typedef core::iterator_bf<> super;
			void increment(unsigned min_depth)
			{
				/* The idea here is not that we skip types per se.
				 * It's that we skip the children of types, e.g.
				 * local vars or formals that actually belong to
				 * methods. Remember that subprograms are types.
				 * Also remember that we're allowed to start above
				 * the minimum depth. */
				if (*this != END && depth() < min_depth)
				{
					this->increment_skipping_siblings();
				}
				else if (tag_here() != DW_TAG_subprogram &&
					spec_here().tag_is_type(tag_here()))
				{
					this->increment_skipping_subtree();
				} else this->super::increment();
				if (*this != END && depth() < min_depth) *this = END;
			}
			void increment() { this->increment(0); }
			// forward constructors
			using core::iterator_bf<>::iterator_bf;
		} start_bf(i_subp);
		unsigned start_depth = i_subp.depth();
		for (iterator_bf_skipping_types i_bf = start_bf;
			i_bf != core::iterator_base::END;
			/* After the first inc, we should always be at *at least* 1 + start_depth. */
			i_bf.increment(start_depth + 1))
		{
			// skip if not a with_dynamic_location_die
			if (!i_bf.is_a<with_dynamic_location_die>()) continue;

			/* Exploit "clever" (hopefully) aggregation semantics of 
			 * interval maps.
			 * http://www.boost.org/doc/libs/1_51_0/libs/icl/doc/html/index.html
			 */
			
			// enumerate the vaddr ranges of this DIE
			// -- note that some DIEs will be "for all vaddrs"
			// -- noting also that static variables need handling!
			//    ... i.e. they need to be handled in the *static* handler!
			
			// skip static variables
			if (i_bf.is_a<variable_die>() && i_bf.as_a<variable_die>()->has_static_storage())
			{
				/* FIXME: does sranges already deal with these? */
				continue;
			}
			auto i_dyn = i_bf.as_a<with_dynamic_location_die>();
			
			// skip member/inheritance DIEs
			if (i_dyn->location_requires_object_base()) continue;
			
			/* enumerate the vaddr ranges of this DIE
			 * -- note that some DIEs will be "for all vaddrs" */
			auto var_loclist = i_dyn->get_dynamic_location();
			// rewrite the loclist to use the CFA/frame_base maximally
			// cerr << "Saw loclist " << var_loclist << endl;
			var_loclist = encap::rewrite_loclist_in_terms_of_cfa(
				var_loclist, 
				root.get_frame_section(), 
				dwarf::spec::opt<const encap::loclist&>() /* opt_fbreg */
			);
			// cerr << "Rewrote to loclist " << var_loclist << endl;

			
			// for each of this variable's intervals, add it to the map
			int interval_index = 0;
			for (auto i_locexpr = var_loclist.begin(); 
				i_locexpr != var_loclist.end(); ++i_locexpr)
			{
				iterfirst_pair_hash< with_dynamic_location_die, encap::loc_expr >::set /*,
					compare_first_iter_offset<encap::loc_expr> */ singleton_set;
				/* PROBLEM: we need to remember not only that each i_dyn is valid 
				 * in a given range, but with what loc_expr. So we pair the i_dyn with
				 * the relevant loc_expr. */
				singleton_set.insert(make_pair(i_dyn, *i_locexpr));
				
				// FIXME: disgusting hack
				if (i_locexpr->lopc == 0xffffffffffffffffULL
				|| i_locexpr->lopc == 0xffffffffUL)
				{
					// we got a base address selection entry -- not handled yet
					assert(false);
				}
				
				if (i_locexpr->lopc == i_locexpr->hipc && i_locexpr->hipc != 0) continue; // skip empties
				if (i_locexpr->hipc <  i_locexpr->lopc)
				{
					cerr << "Warning: lopc (0x" << std::hex << i_locexpr->lopc << std::dec
						<< ") > hipc (0x" << std::hex << i_locexpr->hipc << std::dec << ")"
						<< " in " << *i_dyn << endl;
					continue;
				}
				
				/* vaddrs in this CU are relative to what addr? 
				 * If we're an executable, they're absolute. 
				 * If we're a shared library, they should be relative to its load address. */
				auto opt_cu_base = i_subp.enclosing_cu()->get_low_pc();
				if (!opt_cu_base)
				{
					cerr << "Warning: skipping subprogram " << *i_dyn 
						<< " -- in CU with no base address (CU: "
						<< *i_subp.enclosing_cu()
						<< ")" << endl;
					continue;
				}
				Dwarf_Unsigned cu_base = opt_cu_base->addr;
				
				// handle "for all vaddrs" entries
				boost::icl::discrete_interval<Dwarf_Off> our_interval;
				auto print_sp_expr = [&our_interval, &root]() {
					/* Last question. What's the stack pointer in terms of the 
					 * CFA? We can answer this question by faking up a location
					 * list referring to the stack pointer, and asking libdwarfpp
					 * to rewrite that.*/
					cerr << "Calculating rewritten-SP loclist..." << endl;
					auto sp_loclist = encap::rewrite_loclist_in_terms_of_cfa(
						encap::loclist(dwarf_stack_pointer_expr_for_elf_machine(
							root.get_frame_section().get_elf_machine(),
							our_interval.lower(), 
							our_interval.upper()
						)),
						root.get_frame_section(), 
						dwarf::spec::opt<const encap::loclist&>() /* opt_fbreg */
					);
					cerr << "Got SP loclist " << sp_loclist << endl;
					
					/* NOTE: I abandoned the above approach because it doesn't yield
					 * a fixed offset to the SP in general. One reason why not is
					 * alloca(). Other frames might also do weird dynamic sp adjustments
					 * not captured in the unwind information. The Right Fix is to store
					 * one offset per frame, recording the biggest negative offset such 
					 * that all frame elements start at a nonnegative offset from that. */
				};
				auto print_intervals_stats = [&i_subp, &subp_vaddr_intervals, &root]() {
					cerr << "subp_vaddr_intervals for " << i_subp->summary() 
						<< " in compilation unit " << i_subp.enclosing_cu().summary()
						<< " now contains "
						<< subp_vaddr_intervals.size()
						<< " intervals, with total set size ";
					unsigned count = 0;
					for (auto i_int = subp_vaddr_intervals.begin(); i_int != subp_vaddr_intervals.end(); ++i_int)
					{
						count += i_int->second.size();
					}
					cerr << count << std::endl;
				};
				
				if (i_locexpr->lopc == 0 && 0 == i_locexpr->hipc
					|| i_locexpr->lopc == 0 && i_locexpr->hipc == std::numeric_limits<Dwarf_Off>::max())
				{
					// if we have a "for all vaddrs" entry, we should be the only index
					assert(interval_index == 0);
					assert(i_locexpr + 1 == var_loclist.end());
					
					/* we will just add the intervals of the containing subprogram */
					auto subp_intervals = i_subp->file_relative_intervals(root, nullptr, nullptr);
					for (auto i_subp_int = subp_intervals.begin();
						i_subp_int != subp_intervals.end(); 
						++i_subp_int)
					{
						/* NOTE: we do *not* adjust these by cu_base. This has already 
						 * been done, by file_relative_intervals! */
						our_interval = boost::icl::interval<Dwarf_Off>::right_open(
							i_subp_int->first.lower()/* + cu_base*/,
							i_subp_int->first.upper()/* + cu_base*/
						);
						
						cerr << "Borrowing vaddr ranges of " << *i_subp
							<< " for dynamic-location " << *i_dyn << endl;
						
						/* assert sane interval */
						assert(our_interval.lower() < our_interval.upper());
						/* assert sane size -- no bigger than biggest sane function */
						assert(our_interval.upper() - our_interval.lower() < 1024*1024);
						subp_vaddr_intervals += make_pair(
							our_interval,
							singleton_set
						);
						
						// print_sp_expr();
						// print_intervals_stats();
					}
					/* There should be only one entry in the location list if so. */
					assert(i_locexpr == var_loclist.begin());
					assert(i_locexpr + 1 == var_loclist.end());
				}
				else /* we have nonzero lopc and/or hipc */
				{
					/* We *do* have to adjust these by cu_base, because 
					 * we're getting them straight from the location expression. */
					our_interval = boost::icl::interval<Dwarf_Off>::right_open(
						i_locexpr->lopc + cu_base, i_locexpr->hipc + cu_base
					); 
					
					// cerr << "Considering location of " << i_dyn << endl;
					
					/* assert sane interval */
					assert(our_interval.lower() < our_interval.upper());
					/* assert sane size -- no bigger than biggest sane function */
					assert(our_interval.upper() - our_interval.lower() < 1024*1024);
					subp_vaddr_intervals += make_pair(
						our_interval,
						singleton_set
					);
					
					// print_sp_expr();
					// print_intervals_stats();
				}
			}
			
			/* We can get unreasonably big. */
#if 0
			static const unsigned MAX_INTERVALS = 10000;
			if (subp_vaddr_intervals.size() > MAX_INTERVALS)
			{
				cerr << "Warning: abandoning gathering frame intervals for " << i_subp->summary() 
						<< " in compilation unit " << i_subp.enclosing_cu().summary()
						<< " after reaching " << MAX_INTERVALS << std::endl;
				subp_vaddr_intervals.clear();
				break;
			}
#endif
			/* We note that the map is supposed to map file-relative addrs
			 * (FIXME: vaddr is CU- or file-relative? or "applicable base address" blah?) 
			 * to the set of variable/fp DIEs that are 
			 * in the current (top) stack frame when the program counter is at that vaddr. */

		} /* end bfs */

		/* Now we write a *series* of object layouts for this subprogram, 
		 * discriminated by a set of (disjoint) vaddr ranges. */
		
		/* Our naive earlier algorithm had the problem that, once register-based 
		 * locals are discarded, the frame layout is often unchanged from one vaddr range
		 * to the next. But we were outputting a new uniqtype anyway, creating 
		 * huge unnecessary bloat. So instead, we do a pre-pass where we remember
		 * only the stack-located elements, and store them in a new interval map, 
		 * by offset from frame base. 
		 *
		 * Also, we want to report discarded fps/locals once per subprogram, as 
		 * completely discarded or partially discarded. How to do this? 
		 * Keep an interval map of discarded items.
		 * When finished, walk it and build another map keyed by 
		  */
		frame_intervals_t frame_intervals;
#ifdef DEBUG
		discarded_intervals_t discarded_intervals;
#endif
		 
		for (auto i_int = subp_vaddr_intervals.begin(); 
			i_int != subp_vaddr_intervals.end(); ++i_int)
		{
			/* Get the set of <p_dyn, locexpr>s for this vaddr range. */
			auto& frame_elements = i_int->second;
			
			/* Calculate their offset from the frame base, and sort. */
			//std::map<Dwarf_Signed, shared_ptr<with_dynamic_location_die > > by_frame_off;
			//std::vector<pair<shared_ptr<with_dynamic_location_die >, string> > discarded;

			/* We used to check that we don't see the same DIE twice within the same interval.
			 * But WHY? We could have two pairs in frame_elements, with different loc_exprs.
			 * In fact this does happen. So I've deleted the check. */
			for (auto i_el_pair = frame_elements.begin(); i_el_pair != frame_elements.end(); ++i_el_pair)
			{
				/* Note that thanks to the aggregation semantics of subp_vaddr_intervals, 
				 * i_int is already the intersection of the loc_expr interval *and* all
				 * other loc_expr intervals in use within this subprogram. W*/

				/* NOTE: our offset can easily be negative! For parameters, it 
				 * usually is. So we calculate the offset from the middle of the 
				 * (imaginary) address space, a.k.a. 1U<<((sizeof(Dwarf_Addr)*8)-1). 
				 * In a signed two's complement representation, 
				 * this number is -MAX. 
				 * NO -- just reinterpret_cast to a signed? */
				
				auto i_el = &i_el_pair->first;
				
				Dwarf_Addr addr_from_zero;
				/* Check for vars that are part static, part on-stack. 
				 * How does this happen? One example is 
				 * the 'git_packed' that is local within rearrange_packed_git
				 * which gets inlined into prepare_packed_git in sha1_file.c.
				 * 
				 * The answer is: they're static vars that are being manipulated
				 * locally within the function. Because they're "variables" that are
				 * "in scope" (I think this is an interaction with inlining), 
				 * they get their own DW_TAG_variable DIEs within the inlined 
				 * instance's DWARF. While they're being manipulated, these have 
				 * register locations. It would be pointless to spill them to the 
				 * stack, however, so I don't think we need to worry about them. */
				if (i_el_pair->second.size() > 0 && i_el_pair->second.at(0).lr_atom == DW_OP_addr
				 && i_el_pair->second.at(i_el_pair->second.size() - 1).lr_atom != DW_OP_stack_value)
				{
					cerr << "Skipping static var masquerading as local: "
						<< *i_el 
						<< "in the vaddr range " 
						<< std::hex << i_int->first << std::dec << std::endl;
					iterfirst_pair_hash< with_dynamic_location_die, string>::set /*,
						compare_first_iter_offset<string>*/ singleton_set;
					singleton_set.insert(make_pair(*i_el, string("static-masquerading-as-local")));
#ifdef DEBUG
					discarded_intervals += make_pair(i_int->first, singleton_set);
#endif
					continue;
				}
				
				bool saw_register = false;
				auto& spec = i_el_pair->first.spec_here();
				for (auto i_instr = i_el_pair->second.begin(); i_instr != i_el_pair->second.end();
					++i_instr)
				{
					if (spec.op_reads_register(i_instr->lr_atom))
					{ saw_register = true; break; }
				}
				
				/* FIXME: DW_OP_piece complicates this. If we have part in a register, 
				 * part on the stack, we'd like to record this somehow. Perhaps supply
				 * a getter and setter in the make_precise()-generated uniqtype? */
				
				if (saw_register)
				{
					/* This means our variable/fp is in a register and not 
					 * in a stack location. That's fine. Warn and continue. */
					if (debug_out > 1)
					{
						cerr << "Warning: we think this is a register-located local/fp or pass-by-reference fp "
							<< "in the vaddr range " 
							<< std::hex << i_int->first << std::dec
							<< ": "
					 		<< *i_el;
					}
					//discarded.push_back(make_pair(*i_el, "register-located"));
					iterfirst_pair_hash< with_dynamic_location_die, string>::set/*,
						compare_first_iter_offset<string> */ singleton_set;
					singleton_set.insert(make_pair(*i_el, string("register-located")));
#ifdef DEBUG
					discarded_intervals += make_pair(i_int->first, singleton_set);
#endif
					continue;
				}
				else try
				{
					dwarf::expr::evaluator e(i_el_pair->second,
						i_el_pair->first.spec_here(),
						/* fb */ 0, 
						{ 0 } /* push zero (a.k.a. the frame base) onto the initial stack */);
					// FIXME: really want to push the offset of the stack pointer from the frame base
					switch (e.tos_state())
					{
						case dwarf::expr::evaluator::ADDRESS: // the good one
							break;
						default:
							if (debug_out > 1)
							{
								cerr << "Top-of-stack indicates non-address result" << std::endl;
							}
					}
					addr_from_zero = e.tos(dwarf::expr::evaluator::ADDRESS); // may *not* be value; must be loc
				}
				catch (dwarf::lib::No_entry)
				{
					/* Not much can cause this, since we scanned for registers.
					 * One thing would be a local whose location gives DW_OP_stack_value,
					 * i.e. it has only a debug-time-computable value but no location in memory,
					 * or DW_OP_implicit_pointer, i.e. it points within some such value. */
					if (debug_out > 1)
					{
						cerr << "Warning: failed to locate non-register-located local/fp "
							<< "in the vaddr range " 
							<< std::hex << i_int->first << std::dec
							<< ": "
					 		<< *i_el << endl;
					}
					//discarded.push_back(make_pair(*i_el, "register-located"));
					iterfirst_pair_hash< with_dynamic_location_die, string>::set/*,
						compare_first_iter_offset<string> */ singleton_set;
					singleton_set.insert(make_pair(*i_el, string("unknown")));
#ifdef DEBUG
					discarded_intervals += make_pair(i_int->first, singleton_set);
#endif
					continue;
				}
				catch (dwarf::expr::Not_supported)
				{
					cerr << "Warning: unsupported DWARF opcode when computing location for fp: "
						<< *i_el << endl;
					//discarded.push_back(make_pair(*i_el, "register-located"));
					iterfirst_pair_hash< with_dynamic_location_die, string>::set /*,
						compare_first_iter_offset<string> */ singleton_set;
					singleton_set.insert(make_pair(*i_el, string("unsupported-DWARF")));
#ifdef DEBUG
					discarded_intervals += make_pair(i_int->first, singleton_set);
#endif
					continue;
				}
				catch (...)
				{
					cerr << "Warning: something strange happened when computing location for fp: " 
					 	<< *i_el << endl;
					//discarded.push_back(make_pair(*i_el, "register-located"));
					iterfirst_pair_hash< with_dynamic_location_die, string>::set /*,
						compare_first_iter_offset<string> */ singleton_set;
					singleton_set.insert(make_pair(*i_el, string("something-strange")));
#ifdef DEBUG
					discarded_intervals += make_pair(i_int->first, singleton_set);
#endif
					continue;
				}
				if (debug_out > 1) cerr << "Over interval " << std::hex << i_int->first
					<< " succeeded at getting on-stack frame location for fp: "
					<< *i_el << " (expr was " << i_el_pair->second << ")" << endl;
				Dwarf_Signed frame_offset = static_cast<Dwarf_Signed>(addr_from_zero);
				// cerr << "Found on-stack location (fb + " << frame_offset << ") for fp/var " << *i_el 
				// 		<< "in the vaddr range " 
				// 		<< std::hex << i_int->first << std::dec << endl;

				/* We only add to by_frame_off if we have complete type => nonzero length. */
				if ((*i_el)->find_type() && (*i_el)->find_type()->get_concrete_type())
				{
					//by_frame_off[frame_offset] = *i_el;
					set< pair< Dwarf_Signed, iterator_df< with_dynamic_location_die > >,
						compare_first_signed_second_offset > singleton_set;
					singleton_set.insert(make_pair(frame_offset, *i_el));
					frame_intervals += make_pair(i_int->first, singleton_set);
				}
				else
				{
					iterfirst_pair_hash< with_dynamic_location_die, string>::set/*,
						compare_first_iter_offset<string> */ singleton_set;
					singleton_set.insert(make_pair(*i_el, string("no_concrete_type")));
#ifdef DEBUG
					discarded_intervals += make_pair(i_int->first, singleton_set);
#endif
				}
			}
		} /* end for i_int */
		
		intervals_by_subprogram[i_subp] = frame_intervals;
		if (frame_intervals.size() == 0)
		{
			cerr << "Warning: no frame element intervals for subprogram " << i_subp << endl;
		}
		
		/* Now figure out the positive and negative extents of the frame. */
		typedef decltype(frame_intervals) is_t;
		std::map< is_t::key_type, unsigned> interval_maxoffs;
		std::map< is_t::key_type, signed>   interval_minoffs;
		signed overall_frame_minoff = 0;
		for (auto i_frame_int = frame_intervals.begin(); i_frame_int != frame_intervals.end();
			++i_frame_int)
		{
			unsigned interval_maxoff;
			signed interval_minoff;
			//if (by_frame_off.begin() == by_frame_off.end()) frame_size = 0;
			if (i_frame_int->second.size() == 0) { interval_maxoff = 0; interval_minoff = 0; }
			else
			{
				{
					frame_intervals_t::codomain_type::iterator i_maxoff_el = i_frame_int->second.end(); --i_maxoff_el;
// 					Dwarf_Signed seen_maxoff = std::numeric_limits<Dwarf_Signed>::min();
// 					for (auto i_el = i_frame_int->second.begin(); i_el != i_frame_int->second.end(); ++i_el)
// 					{
// 						if (i_el->first > seen_maxoff)
// 						{
// 							seen_maxoff = i_el->first;
// 							i_maxoff_el = i_el;
// 						}
// 					}
					auto p_maxoff_type = i_maxoff_el->second->find_type();
					unsigned calculated_maxel_size;
					if (!p_maxoff_type || !p_maxoff_type->get_concrete_type()) 
					{
						cerr << "Warning: found local/fp with no type  (assuming zero length): " 
							<< *i_maxoff_el->second;
						calculated_maxel_size = 0;
					}
					else 
					{
						opt<Dwarf_Unsigned> opt_size = p_maxoff_type->calculate_byte_size();
						if (!opt_size)
						{
							cerr << "Warning: found local/fp with no size (assuming zero length): " 
								<< *i_maxoff_el->second;
							calculated_maxel_size = 0;						
						} else calculated_maxel_size = *opt_size;
					}
					signed interval_max_offset = i_maxoff_el->first + calculated_maxel_size;
					interval_maxoff = (interval_max_offset < 0) ? 0 : interval_max_offset;
				}
				{
					auto i_minoff_el = i_frame_int->second.begin();
					signed interval_min_offset = i_minoff_el->first;
					interval_minoff = (interval_min_offset > 0) ? 0 : interval_min_offset;
				}
			}
			
			interval_maxoffs.insert(make_pair(i_frame_int->first, interval_maxoff));
			interval_minoffs.insert(make_pair(i_frame_int->first, interval_minoff));
			if (interval_minoff < overall_frame_minoff) overall_frame_minoff = interval_minoff;
		}
		unsigned offset_to_all = 0;
		if (overall_frame_minoff < 0)
		{
			/* The offset we want to apply to everything is the negation of 
			 * overall_frame_minoff (likely a negative number), rounded *up* to a word
			 * (i.e. decreasing its magnitude, if negative;
			 * shifting upwards the range of offsets being used in the struct).
			 * What this looks like is:
			 *
			 *  higher        .............
			 *  addrs       |_______________| stack-passed parameters   (positive CFA offset)
			 *           ^  |               | <-- CFA
			 *           |  |               |
			 *       size|  |               |   |  stack growth
			 *           |  |               |   v
			 *  lower    |  |               | locals                    (negative CFA offset)
			 *  addrs    v  |_______________| <-- top of frame (TOF)
			 * THEN:         ..............
			 *
			 * -- each offset in the struct will get offset_to_all added to the CFA offset
			 *      (always turning a negative offset to a nonnegative one)
			 *      (turning CFA-relative offsets into TOF-relative)
			 * -- the size of the struct will be emitted as interval_maxoff + offset_to_all
			 *      (recalling that interval_maxoff already reflects the size of the element there)
			 *        ** IS THIS CORRECT? Seems wrong. But interval_maxoff is not a size,
			 *        because it might be small or negative
			 * -- offset_to_all will be remembered as
			 *      frame_offsets_by_subprogram[i_subp] = offset_to_all;
			 *
			 * -- the frame alloc record will include offset_to_all
			 * -- offsets will be interpreted relative to XXX FIXME fill in.
			 */
			// FIXME: don't assume host word size
			unsigned remainder = (-overall_frame_minoff) % (sizeof (void*));
			unsigned quotient  = (-overall_frame_minoff) / (sizeof (void*));
			offset_to_all =
				remainder == 0 ? quotient * (sizeof (void*))
					: (quotient + 1) * (sizeof (void*));
		}
		frame_offsets_by_subprogram[i_subp] = offset_to_all;
		
		/* Now for each distinct interval in the frame_intervals map... */
		for (auto i_frame_int = frame_intervals.begin(); i_frame_int != frame_intervals.end();
			++i_frame_int)
		{
			auto found_maxoff = interval_maxoffs.find(i_frame_int->first);
			assert(found_maxoff != interval_maxoffs.end());
			unsigned interval_maxoff = found_maxoff->second;
			auto found_minoff = interval_minoffs.find(i_frame_int->first);
			assert(found_minoff != interval_minoffs.end());
			signed interval_minoff = found_minoff->second;
			auto& by_off = i_frame_int->second;
			
			/* Before we output anything, extern-declare any that we need and haven't
			 * declared yet. */
			for (auto i_by_off = by_off.begin(); i_by_off != by_off.end(); ++i_by_off)
			{
				auto el_type = i_by_off->second->find_type();
				auto name_pair = codeful_name(el_type);
				string mangled_name = mangle_typename(name_pair);
				if (names_emitted.find(mangled_name) == names_emitted.end())
				{
					emit_extern_declaration(std::cout, name_pair, /* force_weak */ false);
					names_emitted.insert(mangled_name);
				}
			}

			/* Output in offset order, CHECKing that there is no overlap (sanity). */
			cout << "\n/* uniqtype for stack frame ";
			string unmangled_typename = typename_for_vaddr_interval(i_subp, i_frame_int->first);
			
			string cu_name = *i_subp.enclosing_cu().name_here();
			
			cout << unmangled_typename
				 << " defined in " << cu_name << ", "
				 << "vaddr range " << std::hex << i_frame_int->first << std::dec << " */\n";
			ostringstream min_s; min_s << "actual min is " << interval_minoff + offset_to_all;
			string mangled_name = mangle_typename(make_pair(string(""), cu_name + unmangled_typename));

			/* Is this the same as a layout we've seen earlier for the same frame? */
			bool emitted_as_alias = false;
			for (auto i_earlier_frame_int = frame_intervals.begin();
				i_earlier_frame_int != i_frame_int;
				++i_earlier_frame_int)
			{
				if (by_off == i_earlier_frame_int->second)
				{
					// just output as an alias
					string unmangled_earlier_typename
					 = typename_for_vaddr_interval(i_subp, i_earlier_frame_int->first);
					string mangled_earlier_name = mangle_typename(
						make_pair("", cu_name + unmangled_earlier_typename));
					cout << "\n/* an alias will do */\n";
					/* This might emit nothing, if */
					emit_weak_alias_idem(cout, mangled_name, mangled_earlier_name); // FIXME: not weak
					emitted_as_alias = true;
					break; // just one alias is enough, even if other duplicates exist
						// (they themselves having been emitted as aliases)
				}
			}
			if (emitted_as_alias) continue;

			write_uniqtype_section_decl(cout, mangled_name);
			write_uniqtype_open_composite(cout,
				mangled_name,
				unmangled_typename,
				interval_maxoff + offset_to_all,
				i_frame_int->second.size(),
				false,
				min_s.str()
			);
			opt<unsigned> prev_offset_plus_size;
			opt<unsigned> highest_unused_offset = opt<unsigned>(0u);
			// FIXME: prev_offset_plus_size needn't be the right thing.
			// We want the highest offset yet seen.
			for (auto i_by_off = by_off.begin(); i_by_off != by_off.end(); ++i_by_off)
			{
				ostringstream comment_s;
				auto el_type = i_by_off->second->find_type();
				unsigned offset_after_fixup = i_by_off->first + offset_to_all;
				opt<Dwarf_Unsigned> el_type_size = el_type ? el_type->calculate_byte_size() :
					opt<Dwarf_Unsigned>();
				if (i_by_off->second.name_here())
				{
					comment_s << *i_by_off->second.name_here();
				}
				else comment_s << "(anonymous)"; 
				comment_s << " -- " << i_by_off->second.spec_here().tag_lookup(
						i_by_off->second.tag_here())
					<< " @" << std::hex << i_by_off->second.offset_here() << std::dec
					<< "(size ";
				if (el_type_size) comment_s << *el_type_size;
				else comment_s << "(no size)";
				comment_s << ")";
				if (highest_unused_offset)
				{
					if (offset_after_fixup > *highest_unused_offset)
					{
						unsigned hole_size = offset_after_fixup - *highest_unused_offset;
						unsigned align = el_type.enclosing_cu()->alignment_of_type(el_type);
						unsigned highest_unused_offset_rounded_to_align
						 = ROUND_UP(highest_unused_offset, align);
						comment_s << " (preceded by ";
						if (hole_size ==
							highest_unused_offset_rounded_to_align - highest_unused_offset)
						{
							comment_s << "an alignment-consistent hole";
						}
						else
						{
							comment_s << " (preceded by an alignment-unexpected HOLE";
						}
						comment_s << " of " << hole_size << " bytes)";
					}
					else if (offset_after_fixup < *highest_unused_offset)
					{
						comment_s << " (constituting an OVERLAP in the first " << (*highest_unused_offset - offset_after_fixup)
							<< " bytes)";
					}
				}
				// FIXME: also want to report holes at the start or end of the frame

				string mangled_name = mangle_typename(codeful_name(el_type));
				write_uniqtype_related_contained_member_type(cout,
					/* is_first */ i_by_off == i_frame_int->second.begin(),
					offset_after_fixup,
					mangled_name,
					comment_s.str()
				);
				if (el_type_size)
				{
					prev_offset_plus_size = offset_after_fixup + *el_type_size;
					highest_unused_offset = std::max<unsigned>(
						offset_after_fixup + *el_type_size, highest_unused_offset);
				}
				else
				{
					prev_offset_plus_size = opt<unsigned>();
					highest_unused_offset = opt<unsigned>();
				}
			}
			write_uniqtype_close(cout, mangled_name);
		}
		/* Now print a summary of what was discarded. */
// 		for (auto i_discarded = discarded.begin(); i_discarded != discarded.end(); 
// 			++i_discarded)
// 		{
// 			cout << "\n\t/* discarded: ";
// 			if (i_discarded->first.name_here())
// 			{
// 				cout << *i_discarded->first.name_here();
// 			}
// 			else cout << "(anonymous)"; 
// 			cout << " -- " << i_discarded->first->get_spec().tag_lookup(
// 					i_discarded->first->get_tag())
// 				<< " @" << std::hex << i_discarded->first->get_offset() << std::dec;
// 			cout << "; reason: " << i_discarded->second;
// 			cout << " */ ";
// 		}
	} // end for subprogram
	
	unsigned total_emitted = 0;
	
	/* NOTE: our allocsite chaining trick in liballocs requires/d that our allocsites 
	 * are sorted in vaddr order, so that adjacent allocsites in the memtable buckets
	 * are adjacent in the table. So we sort them here. */
	set< pair< boost::icl::discrete_interval<Dwarf_Addr>, iterator_df<subprogram_die> > > sorted_intervals;
	for (map< iterator_df<subprogram_die>, frame_intervals_t >::iterator i_subp_intervals 
	  = intervals_by_subprogram.begin(); i_subp_intervals != intervals_by_subprogram.end();
	  ++ i_subp_intervals)
	{
		// now output an allocsites-style table for these 
		for (auto i_int = i_subp_intervals->second.begin(); i_int != i_subp_intervals->second.end(); 
			++i_int)
		{
			sorted_intervals.insert(make_pair(i_int->first, i_subp_intervals->first));
		}
	}
	cout << "struct frame_allocsite_entry frame_vaddrs[] = {" << endl;
	for (auto i_pair = sorted_intervals.begin(); i_pair != sorted_intervals.end(); ++i_pair)
	{
		unsigned offset_from_frame_base = frame_offsets_by_subprogram[i_pair->second];
	
		if (i_pair != sorted_intervals.begin()) cout << ",";
		cout << "\n\t/* frame alloc record for vaddr 0x" << std::hex << i_pair->first.lower() 
			<< "+" << i_pair->first.upper() << std::dec << " */";
		cout << "\n\t{\t" << offset_from_frame_base << ","
			<< "\n\t\t{ 0x" << std::hex << i_pair->first.lower() << "UL, " << std::dec
			<< "&" << mangle_typename(make_pair("", *i_pair->second.enclosing_cu().name_here() +
				typename_for_vaddr_interval(i_pair->second, i_pair->first)))
			<< " }"
			<< "\n\t}";
		++total_emitted;
	}
	// close the list
	cout << "\n};\n";

	// success! 
	return 0;
}
