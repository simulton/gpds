#include "container.h"

using namespace gpds;

container& container::set_comment(const gString& comment)
{
    this->comment = comment;

    return *this;
}

container& container::set_comment(gString&& comment)
{
    this->comment = std::move(comment);

    return *this;
}

bool container::is_list() const
{
    // We need at least two elements
    if (values.size() < 2) {
        return false;
    }

    // Ensure that all elements are the same
    gString name;
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
