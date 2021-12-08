#include <stdexcept>
#include "value.hpp"
#include "container.hpp"

using namespace gpds;

value::value(const value& other) :
    attributes(other.attributes),
    m_value(other.m_value)
{
    if (std::holds_alternative<container*>(m_value))
        allocate_container_memory(*std::get<container*>(m_value));
}

value::value(value&& other) noexcept :
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
