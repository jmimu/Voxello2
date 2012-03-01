QT       -= gui core

TARGET = voxello2appli_SDL
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += appli_SDL.cpp \
    voximg.cpp \
    voxspr.cpp \
    vox_scene.cpp \
    vox_particle.cpp \
    particle_manager.cpp \
    mat3x3.cpp \
    pt3d.cpp \
    graph_SDL.cpp \
    draw_vox_ortho.cpp \
    SDL_draw/Draw_Circle.c \
    SDL_draw/Draw_FillCircle.c \
    SDL_draw/Draw_FillRound.c \
    SDL_draw/Draw_Line.c \
    SDL_draw/Draw_Rect.c \
    SDL_draw/Draw_VLine.c \
    SDL_draw/Draw_Ellipse.c \
    SDL_draw/Draw_FillEllipse.c \
    SDL_draw/Draw_HLine.c \
    SDL_draw/Draw_Pixel.c \
    SDL_draw/Draw_Round.c \
    SDL_draw/SDL_draw.c

HEADERS += \
    voximg.h \
    voxspr.h \
    vox_scene.h \
    vox_particle.h \
    particle_manager.h \
    graph.h \
    mat3x3.h \
    pt3d.h \
    graph_SDL.h \
    draw_vox_ortho.h \
    SDL_draw/SDL_draw.h


LIBS += -lSDL -lSDL_gfx

