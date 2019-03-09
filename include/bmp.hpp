#pragma once

#include <array>

namespace file_utility::bmp
{
    using id_type = std::array <char, 2>;

    constexpr id_type BMP_ID {'B','M'};

    const auto bmp_file_exception = file_type_exception {"BMP"};


    struct bmp_header
    {
        id_type id;
        uint32_t file_size;
        uint32_t pad;
        uint32_t offset;

        operator bool () const
        {
            return id == BMP_ID;
        }
    }
    __attribute__(( packed ));


    bmp_header read_bmp_header (std::istream & file)
    {
        bmp_header header = *read_iterator <bmp_header> {file};

        file.seekg( std::istream::beg );

        if( ! header )
        {
            throw bmp::bmp_file_exception;
        }
        return header;
    }
}
