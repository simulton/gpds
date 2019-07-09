#include "value.h"
#include "container.h"

// These functions are not located in value.h to prevent circular dependency issues
// between the 'Container' and the 'Value' class.
// These three functions are the only thing that keep us from having a nice header-only
// library.
// Maybe someone more experienced could tell me how to fix this.

using namespace Gpds;

void Value::freeContainerMemory()
{
    // Containers need to be cleaned up
    if ( std::holds_alternative<Container*>( _value ) ) {
        delete std::get<Container*>( _value );
        _value = nullptr;
    }
}

// Not located in value.h to prevent circular dependency
void Value::allocateContainerMemory(const Container& container)
{
    _value = new Container( container );
}

// Not located in value.h to prevent circular dependency
void Value::allocateContainerMemory(Container&& container)
{
    _value = new Container( std::move( container ) );
}
