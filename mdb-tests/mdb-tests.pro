TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

include(../mdb/Defines.pri)

unix:CONFIG(release, debug|release): LIBS+= -L$$OUT_PWD/../mdb/ -llibmdb
else:unix:CONFIG(debug, debug|release): LIBS+= -L$$OUT_PWD/../mdb/ -llibmdb
