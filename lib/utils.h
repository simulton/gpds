#pragma once

namespace Gpds
{
    #define GPDS_ASSERT( x ) \
        if ( !(x) ) {  \
            printf("GPDS Assertion!\n"); \
            printf("  Function : %s\n", __PRETTY_FUNCTION__); \
            printf("  File     : %s\n", __FILE__); \
            printf("  Line     : %d\n", __LINE__); \
            abort();  \
        }

    // This only works where the RapidXML document instance named doc is in scope as not all underlying
    // string classes return a C string that stays in scope (eg. QString)
    #ifdef GPDS_SUPPORT_QT
        #define GPDS_STR_TO_CSTR( x )   ( doc.allocate_string( x.toUtf8().constData() ) )
    #else
        #define GPDS_STR_TO_CSTR( x )   ( x.c_str() )
    #endif

    #ifdef GPDS_SUPPORT_QT
        #define GPDS_STR_IS_EMPTY( x )  ( x.isEmpty() )
    #else
        #define GPDS_STR_IS_EMPTY( x )  ( x.empty() )
    #endif
}
