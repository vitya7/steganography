#pragma once

#include <iostream>

namespace file_utility
{
    template <class Char>
    class write_iterator
    {
    public :
        using stream_type = std::ostream;

        using value_type        = Char;
        using pointer           = void;
        using reference         = void;
        using iterator_category = std::output_iterator_tag;
        using difference_type   = void;

        write_iterator () = default;

        write_iterator (stream_type & stream)
            : p_stream { &stream }
        {}

        write_iterator & operator = (value_type value)
        {
            p_stream->write( reinterpret_cast <typename stream_type::char_type*> ( &value ), sizeof( value_type ) );
            return *this;
        }

        write_iterator & operator * ()
        {
            return *this;
        }

        write_iterator & operator ++ ()
        {
            return *this;
        }

        write_iterator operator ++ (int)
        {
            return *this;
        }

    private :

        stream_type *p_stream = nullptr;
    };
}
