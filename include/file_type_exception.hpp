#pragma once

#include <optional>
#include <string>

namespace file_utility
{
    using namespace std::literals::string_literals;

    class file_type_exception : public std::exception
    {
    public :
        file_type_exception (std::string str)
            : m_what {"file_type_exception : expected "s + '"' + move( str ) + '"' + " file type"s}
        {}

        const char* what() const noexcept override
        {
            return m_what.data();
        }

    private :
        std::string m_what;
    };
}
