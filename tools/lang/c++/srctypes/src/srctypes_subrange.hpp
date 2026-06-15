// this is an opt-in marker type that allow's client code to emit a subranges uniqtype
// as C/C++ do not have such types within the language
// this is an example of how source-level tooling can potentially provide more information than dwarf. 
// though indeed, client code needs to be modified to take advantage of this
#ifndef SRCTYPES_SUBRANGE_HPP
#define SRCTYPES_SUBRANGE_HPP

namespace srctypes {

template <typename Host, Host Min, Host Max>
struct subrange { Host value; };

}

// Subrange-Probe
template <typename T> struct subrange_traits { static constexpr bool is = false; }; 

template <typename H, H Min, H Max>
struct subrange_traits<srctypes::subrange<H, Min, Max>> {
    static constexpr bool is  = true;
    using host = H; // We emit the hosted value type as an alias, so we can retrieve it later
    static constexpr long min = (long) Min;
    static constexpr long max = (long) Max;
};

#endif
