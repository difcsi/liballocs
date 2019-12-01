#ifndef LIBALLOCSTOOL_ALLOCSITES_INFO_HPP_
#define LIBALLOCSTOOL_ALLOCSITES_INFO_HPP_

#include <map>
#include <set>
#include <vector>
#include <string>
#include <utility>
#include <memory>
#include <boost/optional.hpp>

#include "uniqtypes.hpp"

namespace allocs
{
namespace tool
{

using std::vector;
using std::string;
using std::pair;
using boost::optional;
using namespace dwarf;

struct allocsite
{
	string clean_typename;
	string sourcefile;
	string objname;
	unsigned file_addr;
	bool is_synthetic;
	bool might_be_array;
};

vector<allocsite> read_allocsites(std::istream& in);
optional<vector<allocsite> > read_allocsites_for_binary(const string& s);

void merge_and_rewrite_synthetic_data_types(core::root_die& r, vector<allocsite>& as);

pair<std::unique_ptr<core::root_die>, std::unique_ptr<std::ifstream> >
make_root_die_and_merge_synthetics(vector<allocsite>& as);

int read_allocs_line(
	const string& str,
	string& objname,
	string& symname,
	unsigned& file_addr,
	string& cuname,
	unsigned& line,
	unsigned& end_line,
	string& alloc_typename,
	bool& might_be_array
);
// HACK now that this field is not explicit
#define DECLARE_AS_ARRAY0(a) \
(!(a).is_synthetic && (a).might_be_array)

/* The allocsites relation map a pair <objname, offset>
 * to a pair <uniqued_name, declare_as_array0> */
typedef std::map< pair< string, unsigned long >, pair<uniqued_name, allocsite > > allocsites_relation_t;

void make_allocsites_relation(
	allocsites_relation_t& allocsites_relation,
	vector<allocsite>& allocsites_to_add,
	multimap<string, iterator_df<type_die> >& types_by_codeless_name,
	root_die& r
);

iterator_df<type_die>
get_or_create_uninterpreted_byte_type(root_die& r);
iterator_df<type_die>
get_or_create_generic_pointer_type(root_die& r);
iterator_df<type_die>
get_or_create_array_of_uninterpreted_byte_type(root_die& r);


} // end namespace tool
} // end namespace allocs
#endif
