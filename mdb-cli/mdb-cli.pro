TEMPLATE = app
CONFIG += console c++17
# executables will be bundled
CONFIG -= app_bundle
CONFIG -= qt
SOURCES += main.cpp

include(../mdb/Defines.pri)
unix:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../mdb/ -lmdb
else:unix:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../mdb/ -lmdb

message(In $$PWD)
