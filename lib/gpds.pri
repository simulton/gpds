INCLUDEPATH += \
    $$PWD \
    $$PWD/3rdparty/rapidxml-1.13

HEADERS += \
    $$PWD/3rdparty/rapidxml-1.13/rapidxml.hpp \
    $$PWD/3rdparty/rapidxml-1.13/rapidxml_iterators.hpp \
    $$PWD/3rdparty/rapidxml-1.13/rapidxml_print.hpp \
    $$PWD/3rdparty/rapidxml-1.13/rapidxml_utils.hpp \
    $$PWD/3rdparty/rapidxml-1.13/rapidxml_ext.hpp \
    $$PWD/include/gpds/attributes.h \
    $$PWD/include/gpds/value.h \
    $$PWD/include/gpds/container.h \
    $$PWD/include/gpds/serialize.h \
    $$PWD/include/gpds/archiver.h \
    $$PWD/include/gpds/archiverxml.h \
    $$PWD/include/gpds/utils.h \
    $$PWD/include/gpds/types.h

SOURCES += \
    $$PWD/src/archiverxml.cpp \
    $$PWD/src/container.cpp \
    $$PWD/src/value.cpp \
