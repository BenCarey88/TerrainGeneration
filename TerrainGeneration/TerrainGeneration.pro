# This specifies the exe name
TARGET=TerrainGenerator
# where to put the .o files
OBJECTS_DIR=obj
# core Qt Libs to use add more here if needed.
QT+=gui opengl core

# as I want to support 4.8 and 5 this will set a flag for some of the mac stuff
# mainly in the types.h file for the setMacVisual which is native in Qt5
isEqual(QT_MAJOR_VERSION, 5) {
        cache()
        DEFINES +=QT5BUILD
}
# where to put moc auto generated files
MOC_DIR=moc
# on a mac we don't create a .app bundle file ( for ease of multiplatform use)
CONFIG-=app_bundle
# Auto include all .cpp files in the project src directory (can specifiy individually if required)
SOURCES+= $$PWD/src/main.cpp \
          $$PWD/src/TerrainData.cpp \
          $$PWD/src/NGLScene.cpp \
          $$PWD/src/Camera.cpp \
          $$PWD/src/TerrainGenerator.cpp \
          $$PWD/src/MainWindow.cpp

# same for the .h files
HEADERS+= $$PWD/include/NGLScene.h \
          $$PWD/include/TerrainData.h \
          $$PWD/include/Camera.h \
          $$PWD/include/WindowParams.h \
          $$PWD/include/TerrainGenerator.h \
          $$PWD/include/MainWindow.h

FORMS += \
    $$PWD/ui/MainWindow.ui

# and add the include dir into the search path for Qt and make
INCLUDEPATH +=./include \

# where our exe is going to live (root of project)
DESTDIR=./
# add the glsl shader files
OTHER_FILES+= README.md \
              shaders/*.glsl
# were are going to default to a console app
CONFIG += console
# note each command you add needs a ; as it will be run as a single line
# first check if we are shadow building or no,t easiest way is to check out against current
#!equals(PWD, $${OUT_PWD}){
#	copydata.commands = echo "creating destination dirs" ;
#	# now make a dir
#	copydata.commands += mkdir -p $$OUT_PWD/shaders ;
#	copydata.commands += echo "copying files" ;
#	# then copy the files
#	copydata.commands += $(COPY_DIR) $$PWD/shaders/* $$OUT_PWD/shaders/ ;
#	# now make sure the first target is built before copy
#	first.depends = $(first) copydata
#	export(first.depends)
#	export(copydata.commands)
#	# now add it as an extra target
#	QMAKE_EXTRA_TARGETS += first copydata
#}
NGLPATH=$$(NGLDIR)
isEmpty(NGLPATH){ # note brace must be here
        message("including $HOME/NGL")
        include($(HOME)/NGL/UseNGL.pri)
}
else{ # note brace must be here
        message("Using custom NGL location")
        include($(NGLDIR)/UseNGL.pri)
}

#Include Libnoise Library and Headers
INCLUDEPATH += $$PWD/noise
unix: LIBS += -L$$PWD/lib -lnoise -lnoiseutils

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../Libnoise/bin/ -llibnoise
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../Libnoise/bin/ -llibnoised

INCLUDEPATH += $$PWD/../../Libnoise/bin
DEPENDPATH += $$PWD/../../Libnoise/bin
