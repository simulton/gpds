DEFINES += GPDS_SUPPORT_QT

INCLUDEPATH += \
    $$PWD \
    $$PWD/3rdparty/rapidxml-1.13

HEADERS += \
    $$PWD/3rdparty/rapidxml-1.13/rapidxml.hpp \
    $$PWD/3rdparty/rapidxml-1.13/rapidxml_iterators.hpp \
    $$PWD/3rdparty/rapidxml-1.13/rapidxml_print.hpp \
    $$PWD/3rdparty/rapidxml-1.13/rapidxml_utils.hpp \
    $$PWD/3rdparty/rapidxml-1.13/rapidxml_ext.hpp \
    $$PWD/gpds/attributes.h \
    $$PWD/gpds/value.h \
    $$PWD/gpds/container.h \
    $$PWD/gpds/serialize.h \
    $$PWD/gpds/archiver.h \
    $$PWD/gpds/archiverxml.h \
    $$PWD/gpds/utils.h \
    $$PWD/gpds/types.h

SOURCES += \
    $$PWD/src/archiverxml.cpp \
    $$PWD/src/container.cpp \
    $$PWD/src/value.cpp \
