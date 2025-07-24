#pragma once

#include <string>

namespace gpds
{

    struct error
    {
        error() = default;

        error(const error&) = default;

        error(error&&) = default;

        explicit
        error(std::string message) :
            m_message{ std::move(message) }
        {
        }

        virtual
        ~error() = default;

        [[nodiscard]]
        constexpr
        explicit
        operator bool() const
        {
            return is_error();
        }

        [[nodiscard]]
        constexpr
        bool
        is_error() const noexcept
        {
            return !m_message.empty();
        }

        [[nodiscard]]
        constexpr
        const std::string&
        message() const noexcept
        {
            return m_message;
        }

    private:
        std::string m_message;
    };

}
