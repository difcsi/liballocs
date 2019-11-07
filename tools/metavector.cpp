#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <string>
#include <cctype>
#include <cstdlib>
#include <memory>
#include <cstdbool>
#include <srk31/algorithm.hpp>
#include <cxxgen/tokens.hpp>
#include <dwarfpp/lib.hpp>
#include <dwarfpp/regs.hpp>
#include <fileno.hpp>

#include "stickyroot.hpp"
#include "uniqtypes.hpp"
#include "allocsites-info.hpp"
#include "metavec.h"
#include "relf.h" /* for ROUND_* */

using std::cin;
using std::cout;
using std::cerr;
using std::map;
using std::make_shared;
using std::ifstream;
using std::ostringstream;
using std::set;
using std::make_pair;
using std::setfill;
using std::setw;
using namespace dwarf;
//using boost::filesystem::path;
using dwarf::core::iterator_base;
using dwarf::core::iterator_df;
using dwarf::core::iterator_sibs;
using dwarf::core::type_die;
using dwarf::core::subprogram_die;
using dwarf::core::variable_die;
using dwarf::core::compile_unit_die;

using namespace dwarf::lib;

static int debug_out = 1;

using dwarf::lib::Dwarf_Off;
using dwarf::lib::Dwarf_Addr;
using dwarf::lib::Dwarf_Signed;
using dwarf::lib::Dwarf_Unsigned;

using namespace allocs::tool;

/* The metavector is a per-segment address-sorted array of records
 * of a fixed format. These records are supplements to the metadata
 * recorded in (some) symbol table entries and (some) relocation
 * records -- those defining the collection of /static allocations/.
 * Roughly, that means that they must have nonzero length and must
 * not overlap with other static allocations; there are some ad-hoc
 * rules for inferring the length of reloc targets that do not fall
 * within a symbol.
 *
 * The vector's main purpose is to support an efficient by-
 * address lookup of this metadata, and to ad type information. */

struct sym_or_reloc_rec_to_generate
{
	sym_or_reloc_kind k;
	sticky_root_die::static_descr::kind priority_k;
	unsigned idx_in_per_kind_table;
	opt<uniqued_name> maybe_uniqtype;
	std::string extra_comment;
};
std::map< Dwarf_Addr, sym_or_reloc_rec_to_generate > generate_recs(sticky_root_die& root);
void output_one_segment_metavec(int idx, ElfW(Phdr) *ph,
	ElfW(Ehdr) *ehdr, sticky_root_die& root,
	const std::map< Dwarf_Addr, sym_or_reloc_rec_to_generate >& recs);

int main(int argc, char **argv)
{
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
	if (getenv("METAVECTOR_DEBUG"))
	{
		debug_out = atoi(getenv("METAVECTOR_DEBUG"));
	}
	using core::root_die;
	int fd = fileno(infstream);
	shared_ptr<sticky_root_die> p_root = sticky_root_die::create(fd);
	if (!p_root) { std::cerr << "Error opening file" << std::endl; return 1; }
	sticky_root_die& root = *p_root;
	boost::icl::interval_map< Dwarf_Addr, sticky_root_die::static_descr_set > statics
	= root.get_statics();
	
	/* For each uniqtype, extern-declare it as weak. */
	set< pair<string, string> > seen_codeful_type_names;
	for (auto i_int = statics.begin(); i_int != statics.end(); ++i_int)
	{
		for (auto i_descr = i_int->second.begin(); i_descr != i_int->second.end(); ++i_descr)
		{
			if (i_descr->k == sticky_root_die::static_descr::DWARF)
			{
				seen_codeful_type_names.insert(
					canonical_key_for_type(i_descr->get_d().is_a<type_die>() ?
						i_descr->get_d().as_a<type_die>() :
						i_descr->get_d().as_a<variable_die>()->find_type()
					)
				);
			}
		}
	}
	for (auto i_name_pair = seen_codeful_type_names.begin();
			i_name_pair != seen_codeful_type_names.end();
			++i_name_pair)
	{
		emit_extern_declaration(std::cout, *i_name_pair, /* force_weak */ true);
	}
	/* How do we get the phdrs? Be careful which ELF file. It's always
	 * the base one. */
	Elf *base_elf = root.get_base_elf();
	GElf_Ehdr ehdr;
	if (!gelf_getehdr(base_elf, &ehdr))
	{
		std::cerr << "Cannot get ehdr" << std::endl;
		abort();
	}

	/* Generate metavector entries.
	 * These come in a few flavours.
	 * REC_DYNSYM,   // in the dynsym of the base object
	 * REC_EXTRASYM, // extra symbols in the meta-object, generated by tools/extrasyms
	 * REC_SYMTAB,   // in the (static) symtab of the base object
	 * REC_RELOC_DYN,// in the dynamic relocs of the base object
	 * REC_RELOC     // in the (static) relocs re-emitted in the base object (if linked -Wl,-q)
	 *
	 * In all cases, we have a record (symbol or reloc) existing
	 * either in the base object or in the meta-object we're generating.
	 * We may optionally have type information to augment this... although
	 * only in the first three cases (symbols)... reloc-only objects have
	 * no type information.
	 *
	 * How do we build the meta-vector from all these different kinds of thing?
	 * We have to assume that the extrasyms have already been generated.
	 * We can just re-walk them and add them from the vector.
	 * Then we can walk the sanely described statics that *don't* need extrasyms,
	 * which already come with their symidxs.
	 * Then do the relocs.
	 * We do this for the whole DSO, then generate per-segment vectors/bitmaps.
	 */
	// this is our generation-time equivalent of sym_or_reloc_rec
	std::map< Dwarf_Addr, sym_or_reloc_rec_to_generate > recs = generate_recs(root);

	std::cout << "#include \"allocmeta-defs.h\"" << std::endl;
	std::cout << "#include \"bitmap.h\"" << std::endl;
	std::cout << "#define STRx(s) #s" << std::endl;
	std::cout << "#define STR(s) STRx(s)" << std::endl;
	// for each LOAD phdr
	for (unsigned i = 0; i < ehdr.e_phnum; ++i)
	{
		GElf_Phdr ph;
		if (!gelf_getphdr(base_elf, i, &ph))
		{
			std::cerr << "Cannot get phdr" << std::endl;
			abort();
		}
		if (ph.p_type == PT_LOAD)
		{
			output_one_segment_metavec(i, &ph, &ehdr, root, recs);
		}
	}

	return 0;
}

static ElfW(Sxword) reloc_synthetic_addend(
						void *rel_or_rela,
						ElfW(Half) relscntype,
						ElfW(Sym) *symtab,
						ElfW(Half) e_machine
					)
{
	ElfW(Xword) r_type
	= ELFW_R_TYPE( ((relscntype==SHT_RELA)?((ElfW(Rela)*)rel_or_rela)->r_info : ((ElfW(Rel)*)rel_or_rela)->r_info) );
#define PAIR(mach, type)   ((((ElfW(Sxword))(mach))<<32)|((ElfW(Sxword))(type)))
	switch (PAIR(e_machine, r_type))
	{
		case (PAIR(EM_386,    R_386_PC32)):
		case (PAIR(EM_386,    R_386_PLT32)):
		case (PAIR(EM_386,    R_386_GOTPC)):
		case (PAIR(EM_X86_64, R_X86_64_PC32)):
		case (PAIR(EM_X86_64, R_X86_64_PLT32)):
		case (PAIR(EM_X86_64, R_X86_64_GOTPCREL)): // FIXME: add others
			return 4; // the actual address is 4 greater than we have already calculated
		default:
			return 0;
	}
#undef PAIR
}

//typedef pair<ElfW(Half), unsigned> reloc_rec_coord; // section idx, idx within section
typedef pair<unsigned, string> reloc_rec_coord; // just the linearised ID + a comment
/* Here we generate all pairs of <referenced-loc, referenced-section-end> for
 * relocation records we fid in the file. Later we will add only the "sane"
 * ones, i.e. the ones that don't overlap other stuff. Note that the
 * section end is used as an upper bound on the target length; we will prune
 * the actual length to the maximum non-overlapping length it could have. */
set<pair<reloc_rec_coord, pair<Dwarf_Addr, Dwarf_Addr> > >
scan_reloc_target_addr_end_pairs(Elf *e,
	ElfW(Half) relscnidx, unsigned relscn_linear_base_idx, ElfW(Half) relscntype)
{
	set<pair<reloc_rec_coord, pair<Dwarf_Addr, Dwarf_Addr>>> target_addrs;
	_Bool is_rela = (relscntype == SHT_RELA);
	/* Scan the relocs and find whether their target section
	 * is within this section. */
	GElf_Ehdr ehdr;
	GElf_Ehdr *found = gelf_getehdr(e, &ehdr);
	if (!found) abort();
	Elf_Data *shstrtab_data = raw_data_by_shndx(e, ehdr.e_shstrndx);
	auto relscn_name = get_shdr(e, relscnidx).sh_name;
	string relscn_namestr = reinterpret_cast<char*>(shstrtab_data->d_buf) + relscn_name;
	unsigned symtab_shndx = get_shdr(e, relscnidx).sh_link;
	assert(symtab_shndx != 0);
	unsigned relocated_shndx = get_shdr(e, relscnidx).sh_info;
	if (relocated_shndx != 0)
	{
		/* We only count relocs whose origins are in allocatable
		 * sections. This stops us from counting the targets of debug content,
		 * which could reference random places like the middle of stuff
		 * (HMM, why?) */
		auto flags = get_shdr(e, relocated_shndx).sh_flags;
		if (!(flags & SHF_ALLOC)) return target_addrs;
	}
	/* We need to get hold of the symtab content. */
	Elf_Data *symtab_data = raw_data_by_shndx(e, symtab_shndx);
	ElfW(Sym) *symtab = reinterpret_cast<ElfW(Sym)*>(symtab_data->d_buf);
	unsigned nrel = get_shdr(e, relscnidx).sh_size / 
		(is_rela ? sizeof (ElfW(Rela)) : sizeof (ElfW(Rel))); // FIXME: use entsize
	Elf_Data *tbl_data = raw_data_by_shndx(e, relscnidx);
	ElfW(Rela) *rela_base = is_rela ? (ElfW(Rela) *) tbl_data->d_buf : NULL;
	ElfW(Rel) *rel_base = is_rela ? NULL : (ElfW(Rel) *) tbl_data->d_buf;
	for (unsigned i = 0; i < nrel; ++i)
	{
		/* Is this relocation referencing a section symbol? */
		Elf64_Xword info = is_rela ? rela_base[i].r_info : rel_base[i].r_info;
		unsigned symind = ELFW_R_SYM(info);
		if (symind &&   (     ELFW_ST_TYPE(symtab[symind].st_info) == STT_SECTION
		                  || (ELFW_ST_TYPE(symtab[symind].st_info) == STT_NOTYPE
		                      && ELFW_ST_BIND(symtab[symind].st_info) == STB_LOCAL)
		                )
		)
		{
			/* NOTE that the *referenced vaddr* is *not*
			 * the r_offset i.e. the relocation site.
			 * It's the vaddr of the referenced section symbol,
			 * i.e. of the referenced section,
			 * plus the addend if any.
			 * In fact it's not even that! There is a reloc-specific
			 * adjustment that's needed, in order to get the actual
			 * logically referenced symbol, at least in the case of PC-relative
			 * relocs on x86{,_x64}. That's because the referenced address
			 * is computed relative to the *next* instruction's base, but the
			 * linker is oblivious to this and hacks around it with a -4 addend
			 * so that the fixed-up bytes have a displacement that is 4 bytes
			 * lower than the displacement from the reloc site. We need to apply
			 * knowledge of the specific relocation's effective base address on
			 * the architecture defining the PC-relative addressing mode, so
			 * that we can find the actual effective referenced address. */
			unsigned shndx = symtab[symind].st_shndx;
			Elf64_Sword referenced_vaddr
				= symtab[symind].st_value +
					(is_rela ? rela_base[i].r_addend : 0) +
					reloc_synthetic_addend(
						is_rela ? (void*)&rela_base[i] : (void*)&rel_base[i],
						is_rela ? SHT_RELA : SHT_REL,
						symtab,
						ehdr.e_machine
					);
			GElf_Shdr shdr = get_shdr(e, shndx);
			Elf64_Sword referenced_section_end = shdr.sh_addr + shdr.sh_size;
			std::cerr << "Saw a reloc target at 0x" << std::hex << referenced_vaddr
				<< " (limit: 0x" << referenced_section_end << ")"<< std::dec << std::endl;
			ostringstream comment;
			comment << relscn_namestr << "[" << i << "]";
			target_addrs.insert(make_pair(
				make_pair(relscn_linear_base_idx + i, comment.str()),
				make_pair(referenced_vaddr, referenced_section_end)
			));
		}
	}
	return target_addrs;
}

void
add_sane_reloc_intervals(
	const boost::icl::interval_map< Dwarf_Addr, sticky_root_die::static_descr_set > statics,
	const set<pair<reloc_rec_coord, pair<Dwarf_Addr, Dwarf_Addr>>>& reloc_target_addr_end_pairs,
	std::map< Dwarf_Addr, sym_or_reloc_rec_to_generate >& recs)
{
	unsigned idx = 0;
	for (auto i_tuple = reloc_target_addr_end_pairs.begin();
		i_tuple != reloc_target_addr_end_pairs.end(); ++i_tuple, ++idx)
	{
		auto target = i_tuple->second;
		// given this reloc target, what would be a sane length?
		// the distance to an overlap or to the next reloc target, whichever is lower
		Dwarf_Addr target_addr = target.first;
		Dwarf_Addr section_end = target.second;
		/* c.find(y) "returns an iterator to the first interval in c that overlaps with y" */
		auto interval_overlapping = boost::icl::interval<Dwarf_Addr>::right_open(target_addr,
			target_addr+1);
		auto found_overlapping = statics.find(interval_overlapping);
		if (found_overlapping == statics.end())
		{
			// OK, nothing currently overlaps it. What's the next-higher?
			opt<unsigned> sane_length_to_static;
			opt<unsigned> sane_length_to_next_reloc_target;
			auto lb = statics.lower_bound(/*target_addr*/ interval_overlapping);
			assert(lb == statics.end() || lb->first.lower() > target_addr);
			if (lb != statics.end())
			{
				sane_length_to_static = lb->first.lower() - target_addr;
			}
			auto i_next_tuple = i_tuple; ++i_next_tuple;
			if (i_next_tuple != reloc_target_addr_end_pairs.end())
			{
				sane_length_to_next_reloc_target = i_next_tuple->second.first - target_addr;
			}
			// add an interval with the lower of the two sane lengths, assuming it's not 0
			unsigned lowest_len;
			if (sane_length_to_static && sane_length_to_next_reloc_target)
			{
				lowest_len = (*sane_length_to_static < *sane_length_to_next_reloc_target) ?
					*sane_length_to_static : *sane_length_to_next_reloc_target;
			}
			else if (sane_length_to_static) lowest_len = *sane_length_to_static;
			else if (sane_length_to_next_reloc_target) lowest_len = *sane_length_to_next_reloc_target;
			else
			{
				// if we got neither, maybe we're the very highest...
				// we should use the section end address
				lowest_len = section_end - target_addr;
			}
			struct sym_or_reloc_rec_to_generate rec = {
				/* kind */ REC_RELOC,
				/* static_descr kind */ sticky_root_die::static_descr::REL,
				/* idx_in_per_kind_table */ i_tuple->first.first /* the index in the linearised collection of reloc-record-containing sections */,
				/* maybe_uniqtype */ opt<uniqued_name>(),
				/* extra comment */ i_tuple->first.second
			};
			recs.insert(make_pair(i_tuple->second.first, /* the addr/end pair is the itself second thing in a pair */
				rec));
		}
		else std::cerr << "Discarding a reloc target at 0x" << std::hex << target.first
				<< " for overlap with static "
				<< found_overlapping->second.get_summary(true).descr_priority_k << std::dec << std::endl;

	}
}

std::map< Dwarf_Addr, sym_or_reloc_rec_to_generate >
generate_recs(sticky_root_die& root)
{
	std::map< Dwarf_Addr, sym_or_reloc_rec_to_generate > recs;
	auto statics = root.get_sanely_described_statics();
	unsigned extrasym_idx = 1;
	for (auto i_static = statics.begin(); i_static != statics.end(); ++i_static)
	{
		auto summary = i_static->second.get_summary(root.symtab_is_external(),
			i_static->first.upper() - i_static->first.lower());
		if (summary.k == REC_EXTRASYM)
		{
			++extrasym_idx; // HACK: we hope this stays in sync with the actual extrasyms table
			sym_or_reloc_rec_to_generate rec = {
				/* kind */ REC_EXTRASYM,
				/* priority kind */ summary.descr_priority_k,
				/* idx_in_per_kind_table */ extrasym_idx,
				/* maybe_uniqtype */ i_static->second.get_type() ?
					opt<uniqued_name>(canonical_key_for_type(summary.t))
					: opt<uniqued_name>()
			};
			recs.insert(make_pair(/* vaddr */ i_static->first.lower(), rec));
		}
	}
	/* Now add the other symbol-y ones. */
	for (auto i_static = statics.begin(); i_static != statics.end(); ++i_static)
	{
		auto summary = i_static->second.get_summary(root.symtab_is_external(),
			i_static->first.upper() - i_static->first.lower());
		if (summary.k != REC_EXTRASYM)
		{
			struct sym_or_reloc_rec_to_generate rec = {
				/* kind */ summary.k,
				/* priority kind */ summary.descr_priority_k,
				/* idx_in_per_kind_table */ *summary.maybe_idx,
				/* maybe_uniqtype */  i_static->second.get_type() ?
					opt<uniqued_name>(canonical_key_for_type(i_static->second.get_type()))
					: opt<uniqued_name>()
			};
			recs.insert(make_pair(/* vaddr */ i_static->first.lower(), rec));
		}
	}
	/* Now add the relocs. With these, we don't have size information.
	 * So we infer the sizes as "up to the start of the next object".
	 * That's the next interval or the next reloc, whichever is lower.
	 * We can compute this at the point where we add them, no biggie.
	 */
	// for each rel/rela section in the binary -- these were added with -q -- we
	// scan for section-relative targets that don't fall within any symbol.
	// We'll do a later pass to figure out the extents, again using our "sane"
	// meaning "no-overlap" condition.
	Elf_Scn *scn = NULL;
	GElf_Shdr shdr;
	unsigned shndx = 0;
	/* We compute a linearised index of all relocation-containing sections. */
	unsigned relscn_global_base = 0;
	// iterate through sections looking for symtab
	while (NULL != (scn = elf_nextscn(root.get_elf(), scn)))
	{
		++shndx;
		if (gelf_getshdr(scn, &shdr) != &shdr)
		{
			cerr << "Unexpected ELF error" << std::endl;
			throw lib::No_entry(); 
		}
		if (shdr.sh_type == SHT_RELA || shdr.sh_type == SHT_REL)
		{
			ElfW(Half) relscnidx = shndx;
			ElfW(Half) relscntype = shdr.sh_type;
			set<pair<reloc_rec_coord, pair<Dwarf_Addr, Dwarf_Addr>>> reloc_target_addr_end_pairs
			 = scan_reloc_target_addr_end_pairs(
			 	root.get_elf(),
				relscnidx,
				relscn_global_base,
				relscntype
			);
			add_sane_reloc_intervals(statics, reloc_target_addr_end_pairs, recs);
			relscn_global_base += shdr.sh_size / shdr.sh_entsize;
		}
	}
	return recs;
}

void output_one_segment_metavec(int idx, ElfW(Phdr) *ph,
	ElfW(Ehdr) *ehdr, sticky_root_die& root,
	const std::map< Dwarf_Addr, sym_or_reloc_rec_to_generate >& recs
)
{
	// what are the segment limits?
	uintptr_t base_addr = ph->p_vaddr;
	uintptr_t limit_addr = base_addr + ph->p_memsz;
	cout << "// metavector for segment " << idx << " spanning 0x" << std::hex << base_addr
		<< " to 0x" << limit_addr << std::dec << std::endl;
	// cout << "unsigned long metavec_0x" << std::hex << base_addr << std::dec << "[] = {" << std::endl;
	cout << "__asm__(\".pushsection .rodata \\n\\" << std::endl;
	cout << ".globl metavec_0x" << std::hex << base_addr << std::dec << " \\n\\" << std::endl;
	cout << "metavec_0x" << std::hex << base_addr << std::dec << ":\\n\\" << std::endl;
	unsigned nrec = 0;
	for (auto i_rec = recs.begin(); i_rec != recs.end(); ++i_rec)
	{
		// FIXME: don't iterate n times for n segments; be smarter
		if (i_rec->first >= base_addr && i_rec->first < limit_addr)
		{
			++nrec;
			cout << ".8byte \"STR(SYM_OR_RELOC_REC_WORD(" << (int) i_rec->second.k << ", "
				<< i_rec->second.idx_in_per_kind_table << ", "
				<< ((i_rec->second.maybe_uniqtype) ? mangle_typename(*i_rec->second.maybe_uniqtype) : "0")
				<< "))\" ; // alloc at 0x" << std::hex << i_rec->first << std::dec
				<< " of kind " << i_rec->second.k
				<< " (based on priority kind " << i_rec->second.priority_k << ")"
				<< " (" << i_rec->second.extra_comment << ")"
				<< "\\n \\" << std::endl;
		};
	}
	// std::cout << "};" << std::endl;
	std::cout << "\");" << std::endl;
	// also output the metavector size
	std::cout << "__asm__(\".size " << "metavec_0x" << std::hex << base_addr << std::dec
		<< ", " << (nrec * sizeof (struct sym_or_reloc_rec)) << "\");" << std::endl;

	/* Also output the bitmap. The bitmap's start address is the segment
	 * start address, rounded down to a 64-byte boundary. */
	uintptr_t bitmap_base_addr = ROUND_DOWN(ph->p_vaddr, BITMAP_WORD_NBITS);
	uintptr_t bitmap_limit_addr = ROUND_UP(limit_addr, BITMAP_WORD_NBITS);
	uintptr_t bitmap_nwords = ROUND_UP((bitmap_limit_addr - bitmap_base_addr), BITMAP_WORD_NBITS) /
		BITMAP_WORD_NBITS;
	/* We simply build the bitmap here, then output it. */
	bitmap_word_t bitmap[bitmap_nwords];
	bzero(bitmap, sizeof (bitmap_word_t) * bitmap_nwords);
	unsigned nbits_set = 0;
	for (auto i_rec = recs.begin(); i_rec != recs.end(); ++i_rec)
	{
		// FIXME: don't iterate n times for n segments; be smarter
		if (i_rec->first >= base_addr && i_rec->first < limit_addr)
		{
			uintptr_t bit_idx = i_rec->first - bitmap_base_addr;
			assert(bit_idx < (bitmap_limit_addr - bitmap_base_addr));
			bitmap_set_b(bitmap, bit_idx);
			++nbits_set;
		}
	}
	cout << "// Now a bitmap spanning " << (bitmap_limit_addr - bitmap_base_addr)
		 << " bytes, with " << nbits_set << " bits set" << std::endl;
	cout << "bitmap_word_t bitmap_0x" << std::hex << base_addr << std::dec << "[] = {" << std::endl;
	cout << "/*               |-00      |-08      |-10      |-18      |-20      |-28      |-30      |-38      |-40      |-48      |-50      |-58      |-60      |-68      |-70      |-78      |-80      |-88      |-90      |-98      |-a0      |-a8      |-b0      |-b8      |-c0      |-c8      |-d0      |-d8      |-e0      |-e8      |-f0      |-f8 ff-|  */" << std::endl;
//  cout << "/* 0b0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 
	/* To make this readable, we format this as follows.
	 * (Remember that our bitmap is big-endian. 
	 * - 256 binary digits per line
	 * - prefix with an address 
	 * - hex digits along the top
	 * - indent the first line if it doesn't start at a 256-byte boundary*/
	uintptr_t base_addr_of_current_word = bitmap_base_addr;
	unsigned printed_words_up_to_idx = 0;
	const int line_nbytes = 256;
	
	auto flush_content_up_to = [&bitmap, bitmap_nwords, bitmap_base_addr, line_nbytes,
		&base_addr_of_current_word, &printed_words_up_to_idx](unsigned idx, bool start_new_line) {
		// either we've hit the end (so won't start a new line), of we are at a line boundary
		assert(start_new_line || (idx == bitmap_nwords));
		assert(!start_new_line ||
			(base_addr_of_current_word % line_nbytes == 0));
		cout << " */ ";
		for (unsigned j = printed_words_up_to_idx; j < idx; ++j)
		{
			// actually print the bitmap words as words
			cout << "0x" << setw(BITMAP_WORD_NBITS / 4) << setfill('0')
				<< std::hex << bitmap[j] << std::dec;
			if (idx != bitmap_nwords || j+1 != idx) cout << ", ";
		}
		if (start_new_line)
		{
			cout << "\n/* 0x" << std::hex << setw(8) << setfill('0')
				<< ROUND_DOWN(base_addr_of_current_word, line_nbytes) << std::dec
				<< ": 0b";
		}
		printed_words_up_to_idx = idx; 
	};

	// we may need to add a starting indent
	unsigned nbytes_to_boundary = ROUND_UP(base_addr_of_current_word, line_nbytes) - base_addr_of_current_word;
	assert(nbytes_to_boundary % (BITMAP_WORD_NBITS/8) == 0);
	// we write 5 bytes per 4 addresses
	cout << "/* 0x" << std::hex << setw(8) << setfill('0') 
		<< ROUND_DOWN(base_addr_of_current_word, line_nbytes) << std::dec << ": ";
	for (unsigned j = 0; j < nbytes_to_boundary / 4; ++j)
	{
		cout << "     ";
	}
	cout << "0b";
	unsigned digits_printed = 0;
	unsigned i = 0;
	for (; i < bitmap_nwords; base_addr_of_current_word += BITMAP_WORD_NBITS, ++i)
	{
		// do we need to start a new line? if we're at an addr that's a multiple of BITMAP_WORD_NBITS
		if (i != 0 && base_addr_of_current_word % line_nbytes == 0)
		{
			flush_content_up_to(i, true);
		}
		
		// actually print the binary digits of the current word, from bit 63 down to 0
		for (unsigned j = 0; j < BITMAP_WORD_NBITS; ++j)
		{
			if (j != 0 && j % 4 == 0) cout << " ";
			cout << ((bitmap[i] & (1ul<<BITMAP_WORD_NBITS-1-j)) ? '1' : '0');
			++digits_printed;
			assert(digits_printed <= (bitmap_limit_addr - bitmap_base_addr));
		}
		cout << " ";
	}
	// now we've flushed all the line-sized boundaries we've crossed,
	// but we may have a final line to flush
	assert(digits_printed == (bitmap_limit_addr - bitmap_base_addr));
	flush_content_up_to(i, false);
	cout << std:: endl << "};" << std::endl;
}



/* Filter intervals by sanity and segment.
 * Build the metavec, using the macros. Skip insane intervals, i.e. where not all
 * the descriptive info has a range that matches the interval itself.
 * Emit the bitmap word-by-word. (Want a sparseness hack for large empty stretches?)
 * Emit the vector record-by-record.
 * For a given static entry, if we have DWARF, also emit the type. (Use the macro.) */

/* What goes into the per-segment metavectors?
 * All statics -- reference their syms + add type info.
 * Non-sym reloc targets.
 *
 * The metadata vector looks something like this.

 	struct sym_or_reloc_rec
	{
		unsigned kind:2; // an instance of sym_or_reloc_kind
		unsigned idx:18; // i.e. at most 256K symbols of each kind, per file
		unsigned long uniqtype_ptr_bits:44; // i.e. the low-order 3 bits are 0
	} *sorted_meta_vec; // addr-sorted list of relevant dynsym/symtab/extrasym/reloc entries

 * and we emit it in assembly code. */
