#-------------------------------------------------
#
# Project created by QtCreator 2012-04-30T14:05:38
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = FluidSolver
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    openglwidget.cpp \
    fluidsolver.cpp \
    macgrid.cpp \
    vec2.cpp \
    shallowwater.cpp \
    advect.cpp \
    bmploader.cpp \
    exception.cpp \
    heightmap.cpp \
    renderheightmap.cpp \
    render.cpp \
    rendershallowwater.cpp \
    testvec2.cpp \
    onewaycoupling.cpp \
    vec3.cpp \
    quaternion.cpp

HEADERS  += mainwindow.h \
    openglwidget.h \
    fluidsolver.h \
    macgrid.h \
    vec2.h \
    shallowwater.h \
    advect.h \
    bmploader.h \
    exception.h \
    heightmap.h \
    renderheightmap.h \
    FastInvSqrt.h \
    render.h \
    rendershallowwater.h \
    testvec2.h \
    onewaycoupling.h \
    vec3.h \
    quaternion.h

FORMS    += mainwindow.ui
