#include "attributes.hpp"

namespace gpds
{

    attributes::attributes(const attributes& other) :
        map(other.map)
    {
    }

    attributes::attributes(attributes&& other) :
        map(std::move(other.map))
    {
    }

}
