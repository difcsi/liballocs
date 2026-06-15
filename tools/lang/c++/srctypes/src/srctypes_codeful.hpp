// ============================================================================
// srctypes_codeful.hpp  --  canonical uniqtype "codeful" naming
//
// Shared by the emitter (uniqtype_emit.hpp) and the discovery program
// (discover.cpp): the two MUST agree byte-for-byte on the name of every type,
// since one declares __uniqtype__<codeful> objects and the other references
// them. gen_meta #embed's this header and writes it beside both before
// compiling them, so it travels with the self-contained meta TU.
// ============================================================================
#ifndef SRCTYPES_CODEFUL_HPP
#define SRCTYPES_CODEFUL_HPP

#include <meta>
#include <type_traits>
#include <string>
#include <string_view>

// Shared compile-time helpers for pointer chains (used by both the emitter's
// ADDRESS handling and discover's walk).
//   ptr_depth_v<T>     : how many '*' deep T is (0 for a non-pointer)
//   remove_all_ptr_t<T>: the ultimate non-pointer type at the bottom
template <typename T> inline constexpr unsigned ptr_depth_v = 0;
template <typename T> inline constexpr unsigned ptr_depth_v<T *> = 1 + ptr_depth_v<T>;
template <typename T> struct remove_all_ptr { using type = T; };
template <typename T> struct remove_all_ptr<T *> { using type = typename remove_all_ptr<T>::type; };
template <typename T> using remove_all_ptr_t = typename remove_all_ptr<T>::type;

// Map a type's display name to a valid C identifier, so it can be a uniqtype
// symbol: anything outside [A-Za-z0-9_] (the spaces in "unsigned int", the angle
// brackets/commas in "Box<int>", the "::" in namespaced names) becomes '_'.
consteval std::string sanitize_symbol(std::string s) {
    for (char &c : s)
        if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
              (c >= '0' && c <= '9') || c == '_'))
            c = '_';
    return s;
}

// std::to_string is not constexpr in this libstdc++; spell out decimal here.
consteval std::string udec(unsigned long n) {
    if (n == 0) return "0";
    std::string s;
    while (n) { s.insert(s.begin(), char('0' + n % 10)); n /= 10; }
    return s;
}

template <typename T> consteval std::string codeful_name();   // (mutually recursive)

// Function-type name builder, matching dwarftypes' scheme:
//   __FUN_FROM___ARG0_<a0>__ARG1_<a1>...[__VA_]__FUN_TO_<ret>
// e.g. int(int,int) -> __FUN_FROM___ARG0_int__ARG1_int__FUN_TO_int.
template <typename F> struct fn_codeful;
template <typename R, typename... A>
struct fn_codeful<R(A...)> {
    static constexpr unsigned narg  = sizeof...(A);
    static constexpr bool     is_va = false;
    static consteval std::string name() {
        std::string s = "__FUN_FROM_";
        unsigned i = 0;
        ((s += "__ARG" + udec(i++) + "_" + codeful_name<A>()), ...);
        return s + "__FUN_TO_" + codeful_name<R>();
    }
};
template <typename R, typename... A>
struct fn_codeful<R(A..., ...)> {                 // variadic
    static constexpr unsigned narg  = sizeof...(A);
    static constexpr bool     is_va = true;
    static consteval std::string name() {
        std::string s = "__FUN_FROM_";
        unsigned i = 0;
        ((s += "__ARG" + udec(i++) + "_" + codeful_name<A>()), ...);
        return s + "__VA___FUN_TO_" + codeful_name<R>();
    }
};

// Canonical liballocs codeful name, recursively, in the dwarftypes scheme:
//   base/record  -> sanitised display name
//   pointer       -> __PTR_<pointee>      (so node** -> __PTR___PTR_node)
//   array[N]      -> __ARR<N>_<element>
//   function      -> __FUN_FROM_..._FUN_TO_<ret>   (see fn_codeful)
template <typename T>
consteval std::string codeful_name() {
    using U = std::remove_cv_t<T>;
    if constexpr (std::is_pointer_v<U>)
        return "__PTR_" + codeful_name<std::remove_pointer_t<U>>();
    else if constexpr (std::is_unbounded_array_v<U>)        // T[] : __ARR_<elem>
        return "__ARR_" + codeful_name<std::remove_extent_t<U>>();
    else if constexpr (std::is_array_v<U>)                  // T[N]: __ARR<N>_<elem>
        return "__ARR" + udec(std::extent_v<U>) + "_"
             + codeful_name<std::remove_extent_t<U>>();
    else if constexpr (std::is_function_v<U>)
        return fn_codeful<U>::name();
    else if constexpr (std::is_void_v<U>)
        return "void";
    else
        // display_string_of names everything uniquely -- fundamentals ("unsigned
        // int"), templates ("Box<int>"), namespaced types ("ns::T") -- where
        // identifier_of would throw or collide.
        return sanitize_symbol(std::string(
            std::meta::display_string_of(std::meta::dealias(^^U))));
}

// ---------------------------------------------------------------------------
// Input filtering and top-level seeding, shared by the emitter and discover so
// the two passes agree on exactly which types to handle.
//
// The driver-generated srctypes_inputs.hpp -- which MUST be included before this
// header -- #includes the library's sources and defines `srctypes_input_dirs`,
// the directories whose types we emit. in_inputs() applies that filter (system
// headers live elsewhere and are excluded).
// ---------------------------------------------------------------------------
consteval bool in_inputs(std::meta::info r) {
    std::string_view f = std::meta::source_location_of(r).file_name();
    for (std::string_view d : srctypes_input_dirs)
        if (f.size() >= d.size() && f.substr(0, d.size()) == d) return true;
    return false;
}

consteval bool is_emittable(std::meta::info r) {
    return std::meta::is_type(r) && std::meta::is_complete_type(r)
        && (std::meta::is_class_type(r) || std::meta::is_enum_type(r))
        && in_inputs(r);
}

// Invoke f.template operator()<T>() for every emittable top-level library type.
// The emitter passes a sink that calls emit<T>(); discover one that calls walk<T>().
template <typename F>
void for_each_emittable_input_type(F f) {
    template for (constexpr auto r : std::define_static_array(
            std::meta::members_of(^^::, std::meta::access_context::current())))
        if constexpr (is_emittable(r))
            f.template operator()<typename [: r :]>();
}

#endif
