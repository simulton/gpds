#pragma once

#ifdef GPDS_SUPPORT_QT
    #include <QtGlobal>
    #include <QString>
#endif

namespace Gpds
{
    class Container;

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
    using gContainer = Container*;
}
