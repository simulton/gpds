#include "container.hpp"

using namespace gpds;

bool container::is_list() const
{
    // We need at least two elements
    if (values.size() < 2) {
        return false;
    }

    // Ensure that all elements are the same
    std::string name;
    for (auto it = values.cbegin(); it != values.cend(); ++it) {
        // Store the name so we can compare them
        if (it == values.begin()) {
            name = it->first;
        }

        // Must not be a container
        if (it->second.is_type<container*>()) {
            return false;
        }

        // All elements need to share the same name
        if (it->first != name) {
            return false;
        }
    }

    return true;
}
