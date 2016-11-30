#ifndef __CONSTEXPR_ITOA_H__
#define __CONSTEXPR_ITOA_H__

static constexpr char digits[] = "0123456789ABCDEF";

// calculate number of digits
constexpr int num_digits (unsigned int x, unsigned int base) {
    return x < base ? 1 : 1 + num_digits (x / base, base);
}

// metaprogramming string type: each different string is a unique type
template<const char... args>
struct metastring {
    const char data[sizeof... (args)] = {args...};
};

//template <char c, char...args>
//struct metastring <c, metastring<args...> >  {
//    const char data[sizeof... (args) + 1] = {c, args...};
//};

template<char...LeftChars, char...RightChars>
constexpr auto operator+(struct metastring<LeftChars...> a, struct metastring<RightChars...> b) -> metastring<LeftChars..., RightChars...> {
    return metastring<LeftChars..., RightChars...>();
}

//template<char...Chars>
//constexpr auto operator+(const char c, struct metastring<Chars...> str) -> metastring<c, Chars...> {
//    return metastring<c, Chars...>();
//}
//
//template<char...Chars>
//constexpr auto operator+(struct metastring<Chars...> str, const char c) -> metastring<Chars..., c> {
//    return metastring<Chars..., c>();
//}



// recursive number-printing template, general case (for three or more digits)
template<int size, unsigned int x, unsigned int base, char... args>
struct numeric_builder {
    typedef typename numeric_builder<size - 1, x / base, base, digits[x % base], args...>::type type;
};

template<std::size_t N, std::size_t iter, const char... args>
constexpr auto toMetastring_impl(const char str[N]) {
    metastring<args...> a;
    return iter == N ? a : toMetastring_impl<N, str[iter], args...>(str);
}

//template<char... args>
//constexpr auto toMetastring_impl<0, args...>(const char (&str)[0]) -> metastring<args...> {
//    return metastring<args...>;
//}


template<std::size_t N>
constexpr auto toMetastring(const char str[N]) {
    return toMetastring_impl<N, 1, str[0]>(str);
}

// special case for two digits
template<unsigned int x, unsigned int base, char... args>
struct numeric_builder<2, x, base, args...> {
    typedef metastring<digits[x / base], digits[x % base], args...> type;
};

// special case for one digit
template<unsigned int x, unsigned int base, char... args>
struct numeric_builder<1, x, base, args...> {
    typedef metastring<digits[x], args...> type;
};

// convenience wrapper for numeric_builder
template<unsigned int x, unsigned int base>
class numeric_string {
    static_assert (base <= 16, "");
 public:
    // returns a pointer to the instantiated string
    static constexpr const char * c_str() { return value.data; }

 private:
    // generate a unique string type representing this number
    typedef typename numeric_builder<num_digits(x, base), x, base, '\0'>::type type;

    // declare a static string of that type (instantiated later at file scope)
    static constexpr type value {};
};

// convenience wrapper for numeric_builder


// instantiate numeric_string::value as needed for different numbers
template<unsigned int x, unsigned int base>
constexpr typename numeric_string<x, base>::type numeric_string<x, base>::value;

#endif
