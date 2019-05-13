#pragma once

#include <string>
#ifdef GPDS_SUPPORT_QT
    #include <QtGlobal>
    #include <QString>
#endif

namespace Gpds
{
    /*
     * A wrapper class to process strings within GPDS in a generic way independent of
     * the underlying string object (eg. std::string or QString)
     */
    struct String : std::string
    {
        String( ) = default;
        String( const String& other ) = default;
        String( String&& other ) = default;

        String( const char* str ) :
            std::string( str )
        {
        }

        #ifdef GPDS_SUPPORT_QT
            String( const QString& str ) :
                std::string( str.toStdString() )
            {
            }
        #endif

        #ifdef GPDS_SUPPORT_QT
            QString toNative() const
            {
                return QString::fromStdString( *this );
            }
        #else
            std::string toNative() const
            {
                return this;
            }
        #endif

        String& operator=(const String& other) = default;
        String& operator=(String&& other) = default;
    };

    // Boolean
    using gBool = bool;

    // Integer
    using gInt = int;

    // Real
    using gReal =
        #ifdef GPDS_SUPPORT_QT
            qreal;
        #else
            double;
        #endif

    // String
    using gString =
        #ifdef GPDS_SUPPORT_QT
            QString;
        #else
            std::string;
        #endif

    // Container (nesting)
    class Container;
    using gContainer = Container*;
}
