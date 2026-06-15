#include <meta>
#include <type_traits>
#include <cstdio>
#include <string>
#include <set>

#include "srctypes_inputs.hpp"   // #includes the library + defines in_inputs()

namespace sm = std::meta;

#include "srctypes_codeful.hpp"
#include "srctypes_subrange.hpp"

static std::set<std::string> g_seen;

// Walk the type graph exactly as emit() does, printing one line per distinct uniqtype.
template <typename T> void walk();

// Print T's line if not seen before
template <typename T>
static bool visit(std::size_t nrelated) {
    static constexpr const char *n = std::define_static_string(codeful_name<T>());
    if (!g_seen.insert(n).second) return false;
    std::printf("%s\t%zu\n", n, nrelated);
    return true;
}

template <typename F> struct fn_walk;
template <typename R, typename... A>
struct fn_walk<R(A...)> {
    static constexpr std::size_t nrel = 1 + sizeof...(A);
    static void go() { walk<R>(); (walk<A>(), ...); }
};
template <typename R, typename... A>
struct fn_walk<R(A..., ...)> {
    static constexpr std::size_t nrel = 1 + sizeof...(A);
    static void go() { walk<R>(); (walk<A>(), ...); }
};

template <typename T>
void walk() {
    using U = std::remove_cv_t<T>;
    if constexpr (std::is_pointer_v<U>) {
        // related[0]=immediate pointee; related[1]=ultimate pointee if multi-level
        constexpr unsigned indir = 1 + ptr_depth_v<std::remove_pointer_t<U>>;
        if (visit<U>(indir > 1 ? 2 : 1))
            walk<std::remove_pointer_t<U>>();
    } else if constexpr (std::is_array_v<U>) {
        if (visit<U>(1))                      // related[0] = element
            walk<std::remove_extent_t<U>>();
    } else if constexpr (std::is_function_v<U>) {
        if (visit<U>(fn_walk<U>::nrel))       // related[0]=return, then args
            fn_walk<U>::go();
    } else if constexpr (subrange_traits<U>::is) {
        if (visit<U>(1))                      // related[0] = host type
            walk<typename subrange_traits<U>::host>();
    } else if constexpr (std::is_class_v<U> || std::is_union_v<U>) {
        static constexpr auto members = std::define_static_array(
            sm::nonstatic_data_members_of(^^U, sm::access_context::current()));
        if (visit<U>(members.size() + 1))     // +1 for the member-names vector
            template for (constexpr auto m : members)
                // bit-field members get a member-local, non-canonical base type
                if constexpr (!sm::is_bit_field(m))
                    walk<typename [: sm::type_of(m) :]>();
    } else if constexpr (std::is_arithmetic_v<U>) {
        visit<U>(0);
    } else if constexpr (std::is_enum_v<U>) {
        constexpr std::size_t nenum = sm::enumerators_of(^^U).size();
        if (visit<U>(1 + nenum))              // related[0]=base, related[1..]=enumerators
            walk<std::underlying_type_t<U>>();
    }
    // void is a liballocs global, unhandled here
}

int main() {
    for_each_emittable_input_type([]<typename T>{ walk<T>(); });
    return 0;
}
