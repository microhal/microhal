/*
 * literal_string.h
 *
 *  Created on: 8 gru 2015
 *      Author: Pawel
 */

#ifndef _MICROHAL_LITERAL_STRING_H_
#define _MICROHAL_LITERAL_STRING_H_

#include <cstdint>
#include <utility>

namespace microhal {

template < std::size_t Size >
struct string;

template < std::size_t Size >
constexpr string<Size-1> make_string(char const (&) [Size]);

template < std::size_t Size >
struct string
{
    using data_type = char const (&) [Size+1];

    constexpr string() : arr{} {}

    constexpr string(char const (&str) [Size+1]) noexcept
        : string(str, std::make_index_sequence<Size>())  {}

    template<std::size_t begin>
    constexpr string<Size - begin> substr() const noexcept {
        static_assert(Size > begin, "");
        return substring_imp(std::make_index_sequence<Size - begin>());
    }

    constexpr std::size_t size() const noexcept {
        return Size;
    }

    constexpr data_type c_str() const noexcept {
        return arr;
    }

    constexpr string<Size+1> push_back(char c) const noexcept {
        return push_back(c, std::make_index_sequence<Size>() );
    }

    constexpr string<Size+1> push_front(char c) const noexcept {
        return push_front(c, std::make_index_sequence<Size>() );
    }

    template < std::size_t N >
    constexpr string<Size+N-1> append(char const (&cstr)[N]) const noexcept {
        return append( std::make_index_sequence<Size>(), std::make_index_sequence<N-1>(), cstr );
    }

    template < std::size_t N >
    constexpr string<Size+N> append(string<N> const& s) const noexcept {
        return append( std::make_index_sequence<Size>(), std::make_index_sequence<N>(), s.c_str() );
    }

    constexpr std::size_t find_first_of(char c, std::size_t npos = Size) const noexcept {
        for (std::size_t iter = 0; iter < npos; iter++) {
            if (arr[iter] == c) return iter;
        }

        return npos;
    }

    constexpr std::size_t find_last_of(char c, std::size_t npos = Size) const noexcept {
        std::size_t iter = npos;

        while(iter) {
            if (arr[iter] == c) return iter;
            iter--;
        }

        return npos;
    }

private:
    const char arr[Size+1];

    template < std::size_t I, std::size_t... Indices >
    constexpr string(char const (&str) [I], std::index_sequence<Indices...> ) noexcept
        : arr{str[Indices]..., '\0'}
    {}

    template < std::size_t... Indices >
    constexpr string<sizeof... (Indices)> substring_imp(const std::index_sequence<Indices...> &s) const noexcept {
        char const newArr[] = {arr[(Size - s.size()) + Indices]..., '\0'};
        return string<s.size()>(newArr);
    }

    template < std::size_t... Indices >
    constexpr string<Size+1> push_back(char c, std::index_sequence<Indices...> ) const noexcept {
        char const newArr[] = {arr[Indices]..., c, '\0'};
        return string<Size+1>(newArr);
    }

    template < std::size_t... Indices >
    constexpr string<Size+1> push_front(char c, std::index_sequence<Indices...> ) const noexcept {
        char const newArr[] = {c, arr[Indices]..., '\0'};
        return string<Size+1>(newArr);
    }

    template < std::size_t... ThisIndices, std::size_t... ThatIndices, std::size_t N >
    constexpr string<Size+N-1> append( const std::index_sequence<ThisIndices...> &s1, const std::index_sequence<ThatIndices...> &s2,
                                       const char (&cstr) [N] ) const noexcept {
        char const newArr[] = {arr[ThisIndices]..., cstr[ThatIndices]..., '\0'};
        return string<Size+N-1>(newArr);
    }
};

template < std::size_t Size >
constexpr string<Size-1> make_string(char const (&cstr) [Size]) {
    return string<Size-1>(cstr);
}

// calculate number of digits
constexpr int digits (unsigned int x, unsigned int base) {
    return x < base ? 1 : 1 + num_digits (x / base, base);
}

template < unsigned int Value, unsigned int Base >
constexpr auto to_string() {
    constexpr const char digit[] = "0123456789ABCDEF";
    constexpr std::size_t stringSize = digits(Value, Base);

    char array[stringSize+1] = {0};

    unsigned int value = Value;

    for (std::size_t i = 0; i < stringSize; i++) {
        array[i] = digit[value % Base];
        value /= Base;
    }

    //reverse
    const char tmp = array[0];
    for (std::size_t i = 0; i < stringSize - 1; i++) {
        array[i] = array[stringSize - 1 - i];
    }
    array[stringSize - 1] = tmp;

    array[stringSize] = '\0';

    return string<stringSize>(array);
}


template < std::size_t Size >
constexpr string<Size + 1> operator + (string<Size> const& s, char c) {
    return s.push_back(c);
}

template < std::size_t Size >
constexpr string<Size + 1> operator + (char c, string<Size> const& s) {
    return s.push_front(c);
}

template < std::size_t S0_size, std::size_t S1_size>
constexpr string<S0_size+S1_size> operator + (string<S0_size> const& s0, string<S1_size> const& s1) {
    return s0.append(s1);
}

} // namespace microhal

#endif // _MICROHAL_LITERAL_STRING_H_
