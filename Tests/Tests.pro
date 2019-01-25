win32: include(gtest_dependency.pri)

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += thread
CONFIG -= qt

INCLUDEPATH += ../TerrainGeneration/include/

SOURCES += \
        main.cpp \
        ../TerrainGeneration/src/terrainData.cpp

unix: LIBS+=-L/public/devel/lib -L/usr/local/lib -lgtest

NGLPATH=$$(NGLDIR)
isEmpty(NGLPATH){ # note brace must be here
        message("including $HOME/NGL")
        include($(HOME)/NGL/UseNGL.pri)
}
else{ # note brace must be here
        message("Using custom NGL location")
        include($(NGLDIR)/UseNGL.pri)
}
