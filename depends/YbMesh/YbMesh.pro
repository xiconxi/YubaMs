TEMPLATE = lib
TARGET = YbMesh

DEFINES += YBMESH_LIBRARY

INCLUDEPATH += \
    ../../depends/easyloggingpp/src \
    ../../depends/GLM \
    ../../depends/EIGEN

HEADERS += \
    inc/YbMesh/YbMesh.hpp \
    src/meshcodecs/format_obj.h \
    src/geometry.h \
    src/types/HalfEdge.h \
    src/types/type_mesh.h \
    src/subdivision/sqrt3.h \
    src/visualization.h \
    src/slice.h \
    ybmesh_global.h

SOURCES += \
    ../easyloggingpp/src/easylogging++.cc \
    src/meshcodecs/format_obj.cpp \
    src/geometry.cpp \
    src/types/HalfEdge.cpp \
    src/subdivision/sqrt3.cpp \
    src/visualization.cpp \
    src/slice.cpp


DESTDIR = ../FrameWorks/

QMAKE_LFLAGS_SONAME = '-Wl,-install_name,@rpath/'
