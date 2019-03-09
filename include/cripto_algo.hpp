#pragma once

#include <iterator>
#include <type_traits>

namespace steganography
{
    /*
        make binary mask with turned on N lowest bits
    */
    template <class T>
    [[nodiscard]]
    constexpr inline
    T fill_n_bit (const unsigned int n)
    {
        return ~( ~ T( 0 ) << n );
    }

    /*
        round up division
    */
    template <class T>
    [[nodiscard]]
    constexpr inline
    T div_up (T const& x, T const& y)
    {
        return T( 1 ) + ( x - T( 1 ) ) / y;
    }

    /*
        size of encoded value of type T without skipping bytes
    */
    template <class T>
    [[nodiscard]]
    constexpr inline
    unsigned int sizeof_encoded (const unsigned int n)
    {
        return div_up <unsigned int> ( sizeof( T ) * 8, n );
    }

    /*
        encode value
    */
    template <class Tp, class InIt, class OutIt, class Policy>
    OutIt encode (Tp value, InIt first, OutIt result, Policy const& policy)
    {
        using T = std::make_unsigned_t <Tp>;
        using U = std::make_unsigned_t <typename std::iterator_traits <InIt> :: value_type>;

        const auto n = policy.encoded_bits_per_byte;

        const auto mask     =  fill_n_bit <T> ( n );
        const auto mask_del = ~fill_n_bit <U> ( n );

        auto iter_num = sizeof_encoded <T> ( n );

        *result = static_cast <T> ( *first & mask_del ) | ( value & mask );
        while( --iter_num )
        {
            value >>= n;

            policy.skip( first, result );

            *++result = static_cast <T> ( *++first & mask_del ) | ( value & mask );
        }

        policy.skip( first, result );
        return ++result;
    }

    /*
        encode N values
    */
    template <class InIt1, class InIt2, class OutIt, class Policy>
    OutIt encode_n (InIt1 first1, size_t N, InIt2 first2, OutIt result, Policy const& policy)
    {
        if( N == 0 ) return result;

        result = encode( *first1, first2, result, policy );
        while( --N )
        {
            result = encode( *++first1, ++first2, result, policy );
        }
        return result;
    }

    /*
        encode range of values
    */
    template <class InIt1, class InIt2, class OutIt, class Policy>
    OutIt encode (InIt1 first1, InIt1 last1, InIt2 first2, OutIt result, Policy const& policy)
    {
        if( first1 == last1 ) return result;

        result = encode( *first1, first2, result, policy );
        while( ++first1 != last1 )
        {
            result = encode( *first1, ++first2, result, policy );
        }
        return result;
    }

    /*
        decode value of type T
    */
    template <class Tp, class InIt, class Policy>
    Tp decode (InIt first, Policy const& policy)
    {
        using T = std::make_unsigned_t <Tp>;
        using U = std::make_unsigned_t <typename std::iterator_traits <InIt> :: value_type>;

        const auto n = policy.encoded_bits_per_byte;

        const auto mask = fill_n_bit <U> ( n );

        auto iter_num = sizeof_encoded <T> ( n );

        T value = *first & mask;
        for(auto shift = n; --iter_num; shift += n )
        {
            policy.skip( first );

            value |= static_cast <T> ( *++first & mask ) << shift;
        }
        policy.skip( first );
        return value;
    }

    /*
        decode N value of type T
    */
    template <class T, class InIt, class OutIt, class Policy>
    OutIt decode_n (InIt first, unsigned int N, OutIt result, Policy const& policy)
    {
        if( N == 0 ) return result;

        *result = decode <T> ( first, policy );
        while( --N )
        {
            *++result = decode <T> ( ++first, policy );
        }
        return ++result;
    }

    /*
        decode range of values of type T
    */
    template <class T, class InIt, class OutIt, class Policy>
    OutIt decode (InIt first, InIt last, OutIt result, Policy const& policy)
    {
        if( first == last) return result;

        *result = decode <T> ( first, policy );
        while( ++first != last )
        {
            *++result = decode <T> ( first, policy );
        }
        return ++result;
    }
}
