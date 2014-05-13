#-------------------------------------------------
#
# Project created by QtCreator 2014-05-12T22:03:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT += opengl

TARGET = RappelzMapEditorGL
TEMPLATE = app
CONFIG += debug_and_release debug_and_release_target

LIBS += -lGLU -lGLEW


SOURCES += main.cpp\
        MainWindow.cpp \
    GLViewport.cpp \
    Simple-OpenGL-Image-Library/src/image_DXT.c \
    Simple-OpenGL-Image-Library/src/image_helper.c \
    Simple-OpenGL-Image-Library/src/SOIL.c \
    Simple-OpenGL-Image-Library/src/stb_image_aug.c \
    GLTexture.cpp

HEADERS  += MainWindow.h \
    GLViewport.h \
    Simple-OpenGL-Image-Library/src/image_DXT.h \
    Simple-OpenGL-Image-Library/src/image_helper.h \
    Simple-OpenGL-Image-Library/src/stbi_DDS_aug_c.h \
    Simple-OpenGL-Image-Library/src/SOIL.h \
    Simple-OpenGL-Image-Library/src/stb_image_aug.h \
    Simple-OpenGL-Image-Library/src/stbi_DDS_aug.h \
    GLObject.h \
    GLTexture.h

FORMS    += MainWindow.ui

OTHER_FILES += \
    vertex.glsl \
    fragment.glsl
