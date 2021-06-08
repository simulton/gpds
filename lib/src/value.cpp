#include <stdexcept>
#include "value.hpp"
#include "container.hpp"

using namespace gpds;

value::value(const value& other) :
        attributes(other.attributes),
        m_value(other.m_value)
{
    if (std::holds_alternative<container*>(m_value)) {
        allocate_container_memory(*std::get<container*>(m_value));
    }
}

value::value(value&& other) :
    attributes(std::move(other.attributes)),
    m_value(std::move(other.m_value))
{
    other.m_value = nullptr;
}

value::~value() noexcept
{
    // Ensure that we won't throw
    GPDS_ASSERT(!m_value.valueless_by_exception());

    free_container_memory();
}

void value::from_string(std::string&& string)
{
    // Is it a boolean 'true' value?
    if (string == "true") {
        set(true);
        return;
    }

    // Is it a boolean 'false' value?
    if (string == "false") {
        set(false);
        return;
    }

    // Is it an integer?
    {
        // Ensure that this is an integer
        bool isInteger = true;
        for (std::string::const_iterator it = string.cbegin(); it != string.cend(); ++it) {
            // Make sure that this is a digit
            if (!std::isdigit(static_cast<int>( *it ))) {
                isInteger = false;
            }

            // Check for minus sign
            if (it == string.cbegin() && !isInteger && *it == '-') {
                isInteger = true;
            }

            if (!isInteger) {
                break;
            }
        }

        if (isInteger) {
            try {
                int i = std::stoi(string);
                set(i);
                return;
            } catch (const std::invalid_argument& e) {
                (void) e;
                // Nothing to do here. Fall through.
            }
        }
    }

    // Is it a double?
    {
        // Ensure that this is a double
        bool isDouble = true;
        bool foundPoint = false;
        for (std::string::const_iterator it = string.cbegin(); it != string.cend(); ++it) {
            // Make sure that this is a digit
            if (!std::isdigit(static_cast<int>( *it ))) {
                // Check if its a decimal point
                if (!foundPoint && *it == '.') {
                    isDouble = true;
                    foundPoint = true;
                } else {
                    isDouble = false;
                }
            }

            // Check for minus sign
            if (it == string.cbegin() && !isDouble && *it == '-') {
                isDouble = true;
            }

            if (!isDouble) {
                break;
            }
        }

        if (isDouble) {
            try {
                double d = std::stod(string);
                set(d);
                return;
            } catch (const std::invalid_argument& e) {
                (void) e;
                // Nothing to do here. Fall through.
            }
        }
    }

    // Lets just assume it's a string :>
    {
        set(std::move(string));
        return;
    }
}

std::string value::to_string() const
{
    if (is_type<gBool>()) {
        return value_to_string(get<gBool>());
    }

    else if (is_type<gInt>()) {
        return value_to_string(get<gInt>());
    }

    else if (is_type<gReal>()) {
        return value_to_string(get<gReal>());
    }

    else if (is_type<gString>()) {
        return value_to_string(get<gString>());
    }

    return {};
}

void value::free_container_memory()
{
    // Containers need to be cleaned up
    if (std::holds_alternative<container*>(m_value)) {
        delete std::get<container*>(m_value);
        m_value = nullptr;
    }
}

void value::allocate_container_memory(const container& container)
{
    m_value = new gpds::container(container);
}

void value::allocate_container_memory(container&& container)
{
    m_value = new gpds::container(std::move(container));
}
