include(gtest_dependency.pri)

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += thread
CONFIG -= qt

INCLUDEPATH += ../TerrainGeneration/Include/

HEADERS += \
        tst_testcase.h \
        TerrainTests.h

SOURCES += \
        main.cpp \
        ../TerrainGeneration/src/terrainData.cpp
