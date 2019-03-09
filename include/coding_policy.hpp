#pragma once

namespace steganography
{
    /*
        coding_policy sets encoding/decoding parameters
    */
    struct coding_policy
    {
        using size_type = uint32_t;

        /* number of message bits one encoded byte */
        unsigned int encoded_bits_per_byte;

        /* distance between encoded bytes with message information */
        unsigned int bytes_to_skip;

        coding_policy (unsigned int encoded_bits_per_byte = 8, unsigned int bytes_to_skip = 0)
            : encoded_bits_per_byte {encoded_bits_per_byte}
            , bytes_to_skip         {bytes_to_skip}
        {}

        /* skip while encoding */
        template <class InIt, class OutIt>
        void skip (InIt first, OutIt result) const
        {
            for(auto n = bytes_to_skip ; n-- ;)
            {
                *++result = *++first;
            }
        }

        /* skip while decoding */
        template <class InIt>
        void skip (InIt first) const
        {
            for(auto n = bytes_to_skip ; n-- ;)
            {
                ++first;
            }
        }

        template <class String = std::string>
        [[nodiscard]]
        size_type sizeof_encoded_string (size_t msg_size) const
        {
            using value_type = typename String :: value_type;

            size_type whitout_skipping = steganography :: sizeof_encoded <size_type> (encoded_bits_per_byte)
                                       + steganography :: sizeof_encoded <value_type> (encoded_bits_per_byte) * msg_size;

            return  whitout_skipping * (bytes_to_skip + 1) - bytes_to_skip;
        }

    };
}
