#pragma once

#include "cripto_algo.hpp"

#include <algorithm>

namespace file_utility
{
    using steganography::encode;
    using steganography::encode_n;
    using steganography::decode;
    using steganography::decode_n;
    using std::copy;
    using std::copy_n;


    /*
        encode message to file
    */
    template <class InIt, class Policy>
    void
    encode_file_n ( std::istream & i_file
                  , std::ostream & o_file
                  , InIt first
                  , typename Policy::size_type n
                  , Policy const& policy )
    {
        using value_type = typename std::iterator_traits <InIt> :: value_type;

        auto header = bmp::read_bmp_header( i_file );

        copy_n( read_iterator <value_type> {i_file}
              , header.offset
              , write_iterator <value_type> {o_file} );

        encode( n
              , read_iterator <value_type> {i_file}
              , write_iterator <value_type> {o_file}
              , policy );

        encode_n( first
                , n
                , read_iterator <value_type> {i_file}
                , write_iterator <value_type> {o_file}
                , policy );

        copy( read_iterator <value_type> {i_file}
            , {}
            , write_iterator <value_type> {o_file} );
    }

    /*
        encode string message to file
    */
    template <class Char, class CharTraits, class Policy>
    void
    encode_file ( std::istream & i_file
                , std::ostream & o_file
                , std::basic_string <Char, CharTraits> const& msg
                , Policy const& policy )
    {
        encode_file_n( i_file, o_file, msg.begin(), msg.size(), policy );
    }

    /*
        decode message from file
    */
    template <class T, class OutIt, class Policy>
    void
    decode_file ( std::istream & file
                , OutIt result
                , Policy const& policy )
    {
        using value_type = T;
        using size_type = typename Policy :: size_type;

        auto header = bmp::read_bmp_header( file );

        file.seekg( header.offset, std::istream::beg );

        size_type n = decode <size_type> ( read_iterator <value_type> {file}, policy );

        decode_n <value_type> ( read_iterator <value_type> {file}
                              , n
                              , result
                              , policy );
    }

    /*
        decode message from file
    */
    template <class Char = char, class CharTraits = std::char_traits <Char>, class Policy>
    std::basic_string <Char, CharTraits>
    decode_file ( std::istream & file
                , Policy const& policy )
    {
        using value_type = Char;
        std::basic_string <Char, CharTraits> msg;

        decode_file <value_type> ( file
                                 , std::back_insert_iterator {msg}
                                 , policy );
        return msg;
    }
}
