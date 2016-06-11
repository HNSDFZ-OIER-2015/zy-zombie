//
// Copyright 2016 HNSDFZ
//

#ifndef UNICODE_H_
#define UNICODE_H_

#include <cstddef>
#include <codecvt>
#include <memory>

#ifdef __WINDOWS__
typedef wchar_t CharType;
#endif  // IFDEF __WINDOWS__

#ifdef __LINUX__
typedef char16_t CharType;
#endif  // IFDEF __LINUX__

template <typename TChar>
inline auto convert(const char* from, const size_t from_size)
    -> std::unique_ptr<TChar>;

template <>
inline auto convert(const char* from, const size_t from_size)
    -> std::unique_ptr<wchar_t> {
    typedef std::codecvt<wchar_t, char, std::mbstate_t> facet_type;
    const facet_type& myfacet = std::use_facet<facet_type>(std::locale());

    mbstate_t state = std::mbstate_t();
    std::unique_ptr<wchar_t> pwstr(new wchar_t[from_size]);
    const char* pc;
    wchar_t* pwc;

    facet_type::result myresult =
        myfacet.in(state, from, from + from_size, pc, pwstr.get(),
                   pwstr.get() + from_size, pwc);

    if (myresult == facet_type::ok)
        return pwstr;
    else
        return nullptr;
}

template <>
inline auto convert(const char* from, const size_t from_size)
    -> std::unique_ptr<char16_t> {
    typedef std::codecvt_utf8<char16_t> facet_type;
    const facet_type& myfacet = std::use_facet<facet_type>(std::locale());

    mbstate_t state = std::mbstate_t();
    std::unique_ptr<char16_t> pwstr(new char16_t[from_size]);
    const char* pc;
    char16_t* pwc;

    facet_type::result myresult =
        myfacet.in(state, from, from + from_size, pc, pwstr.get(),
                   pwstr.get() + from_size, pwc);

    if (myresult == facet_type::ok)
        return pwstr;
    else
        return nullptr;
}

#endif  // UNICODE_H_
