# config -> features and options the project will be configed with during gen
# a+=b -> append/add to b, a | -= -> remove b from a | *= -> add if not present already
CONFIG -= qt

TEMPLATE = lib

# target is a static library
CONFIG += staticlib

# utileze/enable support for c++17
CONFIG += c++17
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# forming the sources and header files
SOURCES += mdb.cpp

HEADERS += mdb.h

# Default rules for deployment.
# a{statement} -> scope declaratio; similar to if; runs if a is true
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Defines.pri

mdb-tests.depends=mdb
mdb-cli.depends=mdb
