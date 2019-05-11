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
}
