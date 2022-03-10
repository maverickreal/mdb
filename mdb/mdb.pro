# config -> features and options the project will be configed with during gen
# a+=b -> append/add to b, a | -= -> remove b from a | *= -> add if not present already
CONFIG -= qt

TEMPLATE = lib

# target is a static library
CONFIG += staticlib

CONFIG += c++17
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# forming the sources and header files
SOURCES += src/database.cpp \
           highwayhash/arch_specific.cc \
           highwayhash/benchmark.cc \
           highwayhash/c_bindings.cc \
           highwayhash/data_parallel_benchmark.cc \
           highwayhash/data_parallel_test.cc \
           highwayhash/example.cc \
           highwayhash/hh_avx2.cc \
           highwayhash/hh_neon.cc \
           highwayhash/hh_portable.cc \
           highwayhash/hh_sse41.cc \
           highwayhash/hh_vsx.cc \
           highwayhash/highwayhash_fuzzer.cc \
           highwayhash/highwayhash_target.cc \
           highwayhash/highwayhash_test.cc \
           highwayhash/highwayhash_test_avx2.cc \
           highwayhash/highwayhash_test_neon.cc \
           highwayhash/highwayhash_test_portable.cc \
           highwayhash/highwayhash_test_sse41.cc \
           highwayhash/highwayhash_test_target.cc \
           highwayhash/highwayhash_test_vsx.cc \
           highwayhash/instruction_sets.cc \
           highwayhash/nanobenchmark.cc \
           highwayhash/nanobenchmark_example.cc \
           highwayhash/os_mac.cc \
           highwayhash/os_specific.cc \
           highwayhash/profiler_example.cc \
           highwayhash/scalar_sip_tree_hash.cc \
           highwayhash/sip_hash.cc \
           highwayhash/sip_hash_fuzzer.cc \
           highwayhash/sip_hash_test.cc \
           highwayhash/sip_tree_hash.cc \
           highwayhash/vector_test.cc \
           highwayhash/vector_test_avx2.cc \
           highwayhash/vector_test_neon.cc \
           highwayhash/vector_test_portable.cc \
           highwayhash/vector_test_sse41.cc \
           highwayhash/vector_test_target.cc \
           src/fileKeyValueStore.cpp \
           src/hashes.cpp \
           src/highwayhash.cpp \
           src/mdb.cpp \
           src/fileKeyValueStore.cpp \
           src/memoryKeyValueStore.cpp \
           src/memoryKeyValueStore.cpp \
           src/types.cpp

HEADERS += include/database.h \
           highwayhash/arch_specific.h \
           highwayhash/c_bindings.h \
           highwayhash/compiler_specific.h \
           highwayhash/data_parallel.h \
           highwayhash/endianess.h \
           highwayhash/hh_avx2.h \
           highwayhash/hh_buffer.h \
           highwayhash/hh_neon.h \
           highwayhash/hh_portable.h \
           highwayhash/hh_sse41.h \
           highwayhash/hh_types.h \
           highwayhash/hh_vsx.h \
           highwayhash/highwayhash.h \
           highwayhash/highwayhash_target.h \
           highwayhash/highwayhash_test_target.h \
           highwayhash/iaca.h \
           highwayhash/instruction_sets.h \
           highwayhash/load3.h \
           highwayhash/nanobenchmark.h \
           highwayhash/os_mac.h \
           highwayhash/os_specific.h \
           highwayhash/profiler.h \
           highwayhash/robust_statistics.h \
           highwayhash/scalar.h \
           highwayhash/scalar_sip_tree_hash.h \
           highwayhash/sip_hash.h \
           highwayhash/sip_tree_hash.h \
           highwayhash/state_helpers.h \
           highwayhash/tsc_timer.h \
           highwayhash/vector128.h \
           highwayhash/vector256.h \
           highwayhash/vector_neon.h \
           highwayhash/vector_test_target.h \
           include/mdb.h \
           include/extensions/highwayhash.h \
           include/extensions/extension.h \
           include/types.h \
           include/container.h \
           include/hashes.h \
           mdb.h

INCLUDEPATH += include
# Default rules for deployment.
# a{statement} -> scope declaratio; similar to if; runs if a is true
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target

DISTFILES += Defines.pri \
#mdb-tests.depends=mdb
#mdb-cli.depends=mdb
message(In $$PWD)
