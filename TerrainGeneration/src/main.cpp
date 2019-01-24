#include <QApplication>
#include "MainWindow.h"

int main(int argc, char **argv)
{
  // create an OpenGL format specifier
  QSurfaceFormat format;
  // set the number of samples for multisampling
  // will need to enable glEnable(GL_MULTISAMPLE); once we have a context
  format.setSamples(4);
  #if defined( __APPLE__)
    // at present mac osx Mountain Lion only supports GL3.2
    // the new mavericks will have GL 4.x so can change
    format.setMajorVersion(4);
    format.setMinorVersion(2);
  #else
    // with luck we have the latest GL version so set to this
    format.setMajorVersion(4);
    format.setMinorVersion(3);
  #endif
  // now we are going to set to CoreProfile OpenGL so we can't use and old Immediate mode GL
  format.setProfile(QSurfaceFormat::CoreProfile);
  // now set the depth buffer to 24 bits
  format.setDepthBufferSize(24);

  // this will set the format for all widgets

  QSurfaceFormat::setDefaultFormat(format);
  // make an instance of the QApplication
  QApplication a(argc, argv);
  // Create a new MainWindow
  MainWindow w;
  // show it
  w.show();
  // hand control over to Qt framework
  return a.exec();
}



/****************************************************************************
basic OpenGL demo modified from http://qt-project.org/doc/qt-5.0/qtgui/openglwindow.html
****************************************************************************/

/*#include <QtGui/QGuiApplication>
#include <iostream>
#include "NGLScene.h"
#include "TerrainGenerator.h"

int main(int argc, char **argv)
{
  QGuiApplication app(argc, argv);
  // create an OpenGL format specifier
  QSurfaceFormat format;
  // set the number of samples for multisampling
  // will need to enable glEnable(GL_MULTISAMPLE); once we have a context
  format.setSamples(4);
  #if defined( __APPLE__)
    // at present mac osx Mountain Lion only supports GL3.2
    // the new mavericks will have GL 4.x so can change
    format.setMajorVersion(4);
    format.setMinorVersion(1);
  #else
    // with luck we have the latest GL version so set to this
    format.setMajorVersion(4);
    format.setMinorVersion(3);
  #endif
  // now we are going to set to CoreProfile OpenGL so we can't use and old Immediate mode GL
  format.setProfile(QSurfaceFormat::CoreProfile);
  // now set the depth buffer to 24 bits
  format.setDepthBufferSize(24);
  // now we are going to create our scene window
*/  /*size_t dimension = 9;
  std::vector<float> heightMap = {1,2,1,2,1,2,1,2,1,
                                  2,4,3,2,1,2,3,4,3,
                                  2,1,1,2,3,4,2,1,1,
                                  1,2,3,1,2,3,1,2,3,
                                  2,4,3,2,1,2,3,4,3,
                                  1,2,3,1,2,3,1,2,3,
                                  2,1,1,2,3,4,2,1,1,
                                  2,4,3,2,1,2,3,4,3,
                                  1,2,3,1,2,3,1,2,3};*/

  /*size_t dimension = 5;
  std::vector<float> heightMap = {1,2,1,4,3,
                                  1,2,3,1,2,
                                  3,4,1,3,2,
                                  2,4,3,1,4,
                                  1,0,4,3,2};*/

  /*size_t dimension = 17;
  std::vector<float> heightMap = {1,2,1,2,1,2,1,2,1,1,2,1,2,1,2,1,2,
                                  2,4,3,2,1,2,3,4,3,2,4,3,2,1,2,3,4,
                                  2,1,1,2,3,4,2,1,1,2,1,1,2,3,4,2,1,
                                  1,2,3,1,2,3,1,2,3,1,2,3,1,2,3,1,2,
                                  2,4,3,2,1,2,3,4,3,2,4,3,2,1,2,3,4,
                                  1,2,3,1,2,3,1,2,3,1,2,3,1,2,3,1,2,
                                  2,1,1,2,3,4,2,1,1,2,1,1,2,3,4,2,1,
                                  2,4,3,2,1,2,3,4,3,2,4,3,2,1,2,3,4,
                                  1,2,3,1,2,3,1,2,3,1,2,3,1,2,3,1,2,
                                  1,2,1,2,1,2,1,2,1,1,2,1,2,1,2,1,2,
                                  2,4,3,2,1,2,3,4,3,2,4,3,2,1,2,3,4,
                                  2,1,1,2,3,4,2,1,1,2,1,1,2,3,4,2,1,
                                  1,2,3,1,2,3,1,2,3,1,2,3,1,2,3,1,2,
                                  2,4,3,2,1,2,3,4,3,2,4,3,2,1,2,3,4,
                                  1,2,3,1,2,3,1,2,3,1,2,3,1,2,3,1,2,
                                  2,1,1,2,3,4,2,1,1,2,1,1,2,3,4,2,1,
                                  2,4,3,2,1,2,3,4,3,2,4,3,2,1,2,3,4};*/
  /*size_t dimension = 3;
  std::vector<float> heightMap = {0,1.2,2,
                                  2,1.8,1,
                                  2,1.4,1.6};

  size_t dimension = 17;1
  std::vector<float> heightMap(17*17);*/

/*  size_t dimension = 129;
  TerrainGenerator t(dimension);
  t.generate(0,3,1,0.8,0.5);

  TerrainData terrain(t.m_dimension, t.m_heightMap);
  //TerrainData terrain(dimension,heightMap);*/

  /*for (auto v : terrain.m_vertexStorage)
  {
      std::cout<<"("<<v.x<<", "<<v.y<<"): Delta "<<v.deltaValue<<", AugmentedDelta "<<v.augmentedDeltaValue<<", Radius "<<v.boundingSphereRadius<<'\n';
  }

  terrain.meshRefine(0,1,2,0.01,1);


  for(auto i : terrain.m_vertexList)
      std::cout<<'\n'<<i;

  std::cout<<"\n \n \n";
  for (auto list : terrain.m_verticesArrangedByGraphLevel)
  {
      for (auto i: list)
      {
          std::cout<<"("<<terrain.m_vertexStorage[i].x<<", "<<terrain.m_vertexStorage[i].y<<") \n";
      }
      std::cout<<'\n';
  }*/

/*  NGLScene window(terrain);
  // and set the OpenGL format
  window.setFormat(format);
  // we can now query the version to see if it worked
  std::cout<<"Profile is "<<format.majorVersion()<<" "<<format.minorVersion()<<"\n";
  // set the window size
  window.resize(1024, 720);
  // and finally show
  window.show();

  return app.exec();

}
*/



