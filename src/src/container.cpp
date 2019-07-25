#include <gpds/container.h>

using namespace Gpds;

Container& Container::addAttribute(gString&& key, gString&& value)
{
    attributes.add( std::forward< gString >( key ), std::forward< gString >( value ) );

    return *this;
}

Container& Container::addAttribute(gString&& key, const gString& value)
{
    return addAttribute( std::forward< gString >( key ), gString( value ) );
}

Container& Container::setComment(const gString& comment)
{
    this->comment = comment;

    return *this;
}

Container& Container::setComment(gString&& comment)
{
    this->comment = std::move( comment );

    return *this;
}

bool Container::isList() const
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

        // Gotta be a container type
        if ( not it->second.isType<Container*>() ) {
            return false;
        }

        // All elements need to share the same name
        if (it->first != name) {
            return false;
        }
    }

    return true;
}
