#include "attributes.h"

namespace Gpds
{

    Attributes::Attributes(const Attributes& other) :
        map( other.map )
    {
    }

    Attributes::Attributes(Attributes&& other) :
        map( std::move( other.map ) )
    {
    }

}
