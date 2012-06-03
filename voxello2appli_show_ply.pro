QT       -= gui core

TARGET = voxello2appli_show_ply
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += appli_show_ply.cpp \
    voximg.cpp \
    voxspr.cpp \
    snowball.cpp \
    vox_scene.cpp \
    vox_particle.cpp \
    particle_manager.cpp \
    mat3x3.cpp \
    pt3d.cpp \
    graph_SFML.cpp \
    draw_vox_ortho.cpp\
    ply_io/ply_io.c

HEADERS += \
    voximg.h \
    voxspr.h \
    snowball.h \
    vox_scene.h \
    vox_particle.h \
    particle_manager.h \
    graph.h \
    mat3x3.h \
    pt3d.h \
    graph_SFML.h \
    draw_vox_ortho.h\
    ply_io/ply_io.h


LIBS += -lsfml-graphics -lsfml-window -lsfml-system

