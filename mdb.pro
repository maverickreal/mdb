# The kind of the project the template is
TEMPLATE = subdirs

# Since it's a subdirs proj, the subdirs
SUBDIRS += mdb \
    mdb-cli \
    mdb-tests

mdb-tests.depends=mdb
mdb-cli.depends=mdb

CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

message(In $$PWD)
