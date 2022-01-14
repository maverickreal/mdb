# consoles the args
message(Including $$_File_ from $$IN_PWD)

# files in target loc will be searched for inclusion by compiler
# in_pwd -> pwd | out_pwd -> full path to the directory where qmake places the generated Makefile
INCLUDEPATH=$$IN_PWD

# target has following dependencies
PRE_TARGETDEPS=$$OUT_PWD/../libmdb/libmdb.a
