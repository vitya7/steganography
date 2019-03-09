#pragma once

#include "file_utility.hpp"
#include "coding_policy.hpp"

#include <fstream>
#include <iomanip>

using steganography::coding_policy;

namespace program_utility
{
    using file_utility::encode_file;
    using file_utility::encode_file_n;
    using file_utility::decode_file;

    using file_utility::read_iterator;
    using file_utility::write_iterator;


    struct encode_tag;
    struct decode_tag;
    struct text_tag;
    struct file_tag;


    template <class Coding, class Mode>
    struct program {};

    template <>
    struct program <encode_tag, text_tag>
    {
        std::string i_path, o_path, msg;
        coding_policy policy;

        void operator ()() const
        {
            std::ifstream i_file {i_path, std::ifstream::binary};
            std::ofstream o_file {o_path, std::ofstream::binary};

            encode_file( i_file
                       , o_file
                       , msg
                       , policy );
        }
    };

    template <>
    struct program <encode_tag, file_tag>
    {
        std::string i_path, o_path, src;
        coding_policy policy;

        void operator ()() const
        {
            std::ifstream i_file   {i_path, std::ifstream::binary};
            std::ofstream o_file   {o_path, std::ofstream::binary};
            std::ifstream src_file {src   , std::ifstream::binary};

            src_file.seekg( 0, std::istream::end );
            auto src_size = src_file.tellg();
            src_file.seekg( 0, std::istream::beg );

            encode_file_n( i_file
                         , o_file
                         , read_iterator <uint8_t> {src_file}
                         , src_size
                         , policy );
        }
    };

    template <>
    struct program <decode_tag, text_tag>
    {
        std::string i_path;
        coding_policy policy;

        void operator ()() const
        {
            std::ifstream i_file {i_path, std::ifstream::binary};

            auto msg = decode_file( i_file, policy );

            std::cout << std::quoted( msg );
        }
    };

    template <>
    struct program <decode_tag, file_tag>
    {
        std::string i_path, o_path;
        coding_policy policy;

        void operator ()() const
        {
            std::ifstream i_file {i_path, std::ifstream::binary};
            std::ofstream o_file {o_path, std::ofstream::binary};

            decode_file <uint8_t> ( i_file
                                  , write_iterator <uint8_t> {o_file}
                                  , policy );
        }
    };
}

#include <typeinfo>
#include <typeindex>
#include <variant>
#include <map>
#include <functional>

namespace cmd_utility
{
    /*
        parsing and some govnocode here
    */

    using std::type_index;
    using std::string;
    using std::map;
    using std::variant;
    using std::function;
    using std::invalid_argument;

    using std::next;
    using std::stoul;
    using std::get;
    using std::move;

    using namespace program_utility;

    template <class T>
    type_index get_type_id ()
    {
        return typeid( T );
    }

    struct param_data
    {
        type_index type_id;
        int param_id;

        param_data (type_index type_id, int param_id)
            : type_id {type_id}
            , param_id {param_id}
        {}
    };

    struct no_arg {};

    const map <string, param_data> parameters
    {
        { "-encode"  , {get_type_id <no_arg> (),            0} }
    ,   { "-decode"  , {get_type_id <string> (),            0} }

    ,   { "-t"       , {get_type_id <string> (),            1} }
    ,   { "-src"     , {get_type_id <string> (),            1} }

    ,   { "-i"       , {get_type_id <string> (),            2} }
    ,   { "-o"       , {get_type_id <string> (),            3} }

    ,   { "-bits"    , {get_type_id <unsigned long> (),     5} }
    ,   { "-skip"    , {get_type_id <unsigned long> (),     6} }
    };


    class  program_config
    {
    public :

        template <class InIt>
        program_config (size_t argn, InIt args)
        {
            parse( argn, args );
        }

        template <class InIt>
        void parse (size_t argn, InIt args)
        {
            for(auto first = next( args ), last = next( args, argn ) ; first != last; ++first)
            {
                string s = *first;

                if(auto p_data = parameters.find( s ); p_data != parameters.end() )
                {
                    auto const& [str, data] = *p_data;
                    add_param( s );

                    if( data.type_id == get_type_id <no_arg> () )
                    {
                        m_param_list[ s ] = no_arg{};
                    }

                    if( data.type_id == get_type_id <string> () )
                    {
                        if( ++first == last )
                        {
                            throw invalid_argument{ "parameter " + s + " needs argument" };
                        }
                        m_param_list[ s ] = string( *first );
                    }

                    if( data.type_id == get_type_id <unsigned long> () )
                    {
                        if( ++first == last )
                        {
                            throw invalid_argument{ "parameter " + s + " needs argument" };
                        }
                        m_param_list[ s ] = stoul( *first );
                    }
                }
                else
                {
                    throw invalid_argument{ s + " is not a parameter" };
                }
            }

        }

        void add_param (string const& x)
        {
            if( ! m_param_id_list.count( parameters.at( x ).param_id ) )
            {
                m_param_id_list[ parameters.at( x ).param_id ] = x;
            }
            else
            {
                auto y = m_param_id_list[ parameters.at( x ).param_id ];
                if( x != y )
                {
                    throw invalid_argument{ "parameter conflict : " + x + " vs " + y };
                }
                else
                {
                    throw invalid_argument{ "parameter " + x + " duplicates " };
                }
            }

        }

        unsigned long get_bits () const
        {
            if(auto p_bits = m_param_list.find( "-bits" ); p_bits != m_param_list.end() )
                return get <unsigned long> ( p_bits->second );
            return 8;
        }

        unsigned long get_skip () const
        {
            if(auto p_bits = m_param_list.find( "-skip" ); p_bits != m_param_list.end() )
                return get <unsigned long> ( p_bits->second );
            return 0;
        }

        function <void()>
        get_program () const
        {
            auto bits = get_bits();
            auto skip = get_skip();

            auto i_path = get <string> ( m_param_list.at( "-i" ) );

            coding_policy policy {bits, skip};

            if( m_param_list.count( "-encode" ) )
            {
                auto o_path = get <string> ( m_param_list.at( "-o" ) );

                if( m_param_list.count( "-t" ) )
                {
                    auto msg = get <string> ( m_param_list.at( "-t" ) );

                    return program <encode_tag, text_tag>   { move( i_path )
                                                            , move( o_path )
                                                            , move( msg )
                                                            , policy };
                }
                else
                {
                    auto src = get <string> ( m_param_list.at( "-src" ) );

                    return program <encode_tag, file_tag>   { move( i_path )
                                                            , move( o_path )
                                                            , move( src )
                                                            , policy };
                }
            }
            else
            {
                auto decode_mode = get <string> ( m_param_list.at( "-decode" ) );

                if( decode_mode == "t" )
                {
                    return program <decode_tag, text_tag>   { move( i_path )
                                                            , policy };
                }
                if( decode_mode == "src" )
                {
                    auto o_path = get <string> ( m_param_list.at( "-o" ) );

                    return program <decode_tag, file_tag>   { move( i_path )
                                                            , move( o_path )
                                                            , policy };
                }
                return {};
            }
        }

    private :
        map <string, variant <no_arg, string, unsigned long>> m_param_list;
        map <int, string> m_param_id_list;
    };

}

