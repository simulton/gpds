#include <gpds/value.h>
#include <gpds/container.h>

// These functions are not located in value.h to prevent circular dependency issues
// between the 'Container' and the 'Value' class.
// These three functions are the only thing that keep us from having a nice header-only
// library.
// Maybe someone more experienced could tell me how to fix this.

using namespace Gpds;

void Value::fromString(std::string&& string)
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
            if (not std::isdigit(static_cast<int>( *it ))) {
                isInteger = false;
            }

            // Check for minus sign
            if (it == string.cbegin() and !isInteger and *it == '-') {
                isInteger = true;
            }

            if (not isInteger) {
                break;
            }
        }

        if (isInteger) {
            try {
                int i = std::stoi( string );
                set(i);
                return;
            } catch (const std::invalid_argument &e) {
                (void) e;
                // Nothing to do here. Fall through.
            }
        }
    }

    // Is it a double?
    {

        try {
            double d = std::stod( string );
            set(d);
            return;
        } catch (const std::invalid_argument &e) {
            (void) e;
            // Nothing to do here. Fall through.
        }
    }

    // Lets just assume it's a string :>
    {
        set( std::move( string ) );
        return;
    }
}

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
