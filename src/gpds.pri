DEFINES += GPDS_SUPPORT_QT

INCLUDEPATH += \
    $$PWD/include \
    $$PWD/3rdparty/rapidxml-1.13

HEADERS += \
    $$PWD/3rdparty/rapidxml-1.13/rapidxml.hpp \
    $$PWD/3rdparty/rapidxml-1.13/rapidxml_iterators.hpp \
    $$PWD/3rdparty/rapidxml-1.13/rapidxml_print.hpp \
    $$PWD/3rdparty/rapidxml-1.13/rapidxml_utils.hpp \
    $$PWD/3rdparty/rapidxml-1.13/rapidxml_ext.hpp \
    $$PWD/include/attributes.h \
    $$PWD/include/value.h \
    $$PWD/include/container.h \
    $$PWD/include/serialize.h \
    $$PWD/include/archiver.h \
    $$PWD/include/archiverxml.h \
    $$PWD/include/utils.h \
    $$PWD/include/types.h

SOURCES += \
    $$PWD/src/value.cpp
