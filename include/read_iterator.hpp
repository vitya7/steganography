#pragma once

#include <iostream>

namespace file_utility
{
    template <class Char>
    class read_iterator
    {
    public :
        using stream_type = std::istream;

        using value_type        = Char;
        using pointer           = value_type const*;
        using reference         = value_type const&;
        using iterator_category = std::input_iterator_tag;
        using difference_type   = size_t;

        read_iterator () = default;

        read_iterator (stream_type & stream)
            : p_stream { &stream }
        {
            update_proxy_object();
        }

        bool operator == (read_iterator const& it) const
        {
            return ( end_of_stream() && it.end_of_stream() ) || ( p_stream == it.p_stream );
        }

        bool operator != (read_iterator const& it) const
        {
            return !( *this == it );
        }

        bool end_of_stream () const
        {
            return p_stream ? p_stream->eof() : true;
        }

        reference operator * () const
        {
            return m_proxy_object;
        }
        pointer operator -> () const
        {
            return &m_proxy_object;
        }

        read_iterator & operator ++ ()
        {
            if( p_stream )
            {
                update_proxy_object();
            }
            return *this;
        }

        read_iterator operator ++ (int)
        {
            if( p_stream )
            {
                auto tmp = *this;
                ++(*this);
                return tmp;
            }
            else
            {
                return *this;
            }
        }

    private :

        void update_proxy_object ()
        {
            if( p_stream )
            {
                p_stream->read( reinterpret_cast <typename stream_type::char_type*> ( &m_proxy_object ), sizeof( m_proxy_object ) );
            }
        }

        stream_type *p_stream = nullptr;

        value_type m_proxy_object;
    };
}
