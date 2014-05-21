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
unix: QMAKE_CXXFLAGS += -std=c++11 -fno-exceptions -fno-rtti

unix: LIBS += -lGLU -lGLEW


SOURCES += main.cpp\
        MainWindow.cpp \
    GLViewport.cpp \
    Simple-OpenGL-Image-Library/src/image_DXT.c \
    Simple-OpenGL-Image-Library/src/image_helper.c \
    Simple-OpenGL-Image-Library/src/SOIL.c \
    Simple-OpenGL-Image-Library/src/stb_image_aug.c \
    GLTexture.cpp \
    MeshObjLoader.cpp \
    GLCamera.cpp \
    GLRenderer.cpp \
    GLProgram.cpp \
    Model.cpp \
    Terrain.cpp \
    GLStaticMesh.cpp \
    TerrainBlendMap.cpp \
    TerrainTextureArray.cpp

HEADERS  += MainWindow.h \
    GLViewport.h \
    Simple-OpenGL-Image-Library/src/image_DXT.h \
    Simple-OpenGL-Image-Library/src/image_helper.h \
    Simple-OpenGL-Image-Library/src/stbi_DDS_aug_c.h \
    Simple-OpenGL-Image-Library/src/SOIL.h \
    Simple-OpenGL-Image-Library/src/stb_image_aug.h \
    Simple-OpenGL-Image-Library/src/stbi_DDS_aug.h \
    GLObject.h \
    GLTexture.h \
    MeshObjLoader.h \
    GLCamera.h \
    GLRenderer.h \
    GLProgram.h \
    Model.h \
    Terrain.h \
    GLStaticMesh.h \
    TerrainBlendMap.h \
    TerrainTextureArray.h

FORMS    += MainWindow.ui

OTHER_FILES += \
    vertex.glsl \
    fragment.glsl


######################
# BTRFdom
######################

INCLUDEPATH += $$PWD/../BTRFdom/shared-lib/common $$PWD/../BTRFdom/shared-lib/interfaces
DEPENDPATH += $$PWD/../BTRFdom/shared-lib/common $$PWD/../BTRFdom/shared-lib/interfaces

unix:!macx: LIBS += -L$$PWD/../BTRFdom/build-linux-gnu-amd64-bin/ -lBTRFdom
unix:!macx: PRE_TARGETDEPS += $$PWD/../BTRFdom/build-linux-gnu-amd64-bin/libBTRFdom.a

win32: LIBS += -L$$PWD/../BTRFdom/build-msvc2010-bin/ -lBTRFdom
win32: PRE_TARGETDEPS += $$PWD/../BTRFdom/build-msvc2010-bin/BTRFdom.lib


######################
# libuv
######################

#INCLUDEPATH += $$PWD/../RappelzUnified/libuv/include
#DEPENDPATH += $$PWD/../RappelzUnified/libuv/include

#unix:!macx: LIBS += -L$$PWD/../RappelzUnified/build-linux-amd64-bin/ -luv
#unix:!macx: PRE_TARGETDEPS += $$PWD/../RappelzUnified/build-linux-amd64-bin/libuv.a

#win32: LIBS += -L$$PWD/../RappelzUnified/build-windows8-msvc2010-x86-bin/ -luv
#win32: PRE_TARGETDEPS += $$PWD/../RappelzUnified/build-windows8-msvc2010-x86-bin/uv.lib


######################
# RappelzLib
######################

#DEFINES += USING_RAPPELZLIB

#INCLUDEPATH += $$PWD/../RappelzUnified/RappelzLib/src
#DEPENDPATH += $$PWD/../RappelzUnified/RappelzLib/src

#unix:!macx: LIBS += -L$$PWD/../RappelzUnified/build-linux-amd64-bin/ -lRappelzLib
#unix:!macx: PRE_TARGETDEPS += $$PWD/../RappelzUnified/build-linux-amd64-bin/libRappelzLib.a

#win32: LIBS += -L$$PWD/../RappelzUnified/build-windows8-msvc2010-x86-bin/ -lRappelzLib
#win32: PRE_TARGETDEPS += $$PWD/../RappelzUnified/build-windows8-msvc2010-x86-bin/RappelzLib.lib


######################
# glew (windows only)
######################

win32: DEFINES += GLEW_STATIC

win32: INCLUDEPATH += D:/Programmes/Programmation/libraries/glew-1.10.0/include
win32: DEPENDPATH += D:/Programmes/Programmation/libraries/glew-1.10.0/include

win32: LIBS += -LD:/Programmes/Programmation/libraries/glew-1.10.0/lib/Release/Win32/ -lglew32s
win32: PRE_TARGETDEPS += D:/Programmes/Programmation/libraries/glew-1.10.0/lib/Release/Win32/glew32s.lib


######################
# glm
######################

INCLUDEPATH += glm
DEPENDPATH += glm
