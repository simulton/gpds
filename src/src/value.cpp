#include <gpds/value.h>
#include <gpds/container.h>

using namespace Gpds;

Value::Value( const Value& other ) :
    attributes( other.attributes ),
    comment( other.comment ),
    _value( other._value )
{
    if ( std::holds_alternative<Container*>( _value ) ) {
        allocateContainerMemory( *std::get<Container*>( _value ) );
    }
}

Value::Value( Value&& other ) :
    attributes( std::move( other.attributes ) ),
    comment( std::move( other.comment ) ),
    _value( std::move( other._value) )
{
    other._value = nullptr;
}

Value::~Value() noexcept 
{
    // Ensure that we won't throw
    GPDS_ASSERT( not _value.valueless_by_exception() );

    freeContainerMemory();
}

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

std::string Value::toString() const
{
    if ( isType<gBool>() ) {
        return valueToString( get<gBool>() );
    }

    else if ( isType<gInt>() ) {
        return valueToString( get<gInt>() );
    }

    else if ( isType<gReal>() ) {
        return valueToString( get<gReal>() );
    }

    else if ( isType<gString>() ) {
        return valueToString( get<gString>() );
    }

    return {};
}

Value& Value::setComment(const gString& comment)
{
    this->comment = comment;

    return *this;
}

Value& Value::setComment(gString&& comment)
{
    this->comment = std::move( comment );

    return *this;
}

void Value::freeContainerMemory()
{
    // Containers need to be cleaned up
    if ( std::holds_alternative<Container*>( _value ) ) {
        delete std::get<Container*>( _value );
        _value = nullptr;
    }
}

void Value::allocateContainerMemory(const Container& container)
{
    _value = new Container( container );
}

void Value::allocateContainerMemory(Container&& container)
{
    _value = new Container( std::move( container ) );
}
