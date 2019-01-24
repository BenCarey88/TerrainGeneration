//----------------------------------------------------------------------------------------------------------------------
/// @file NGLScene.h
/// @author Jonathan Macey
/// @version 2.0
/// @date 23/01/19
/// Revision History :
/// 10/9/13: This is an initial version used for the new NGL6 / Qt 5 demos
/// 14/12/18: updated by Ben Carey for use in terrainGeneration project
//---------------------------------------------------------------------------------------------------------------------

#ifndef NGLSCENE_H_
#define NGLSCENE_H_

#include <ngl/AbstractVAO.h>
#include <ngl/Mat4.h>
#include <ngl/Vec3.h>
#include "WindowParams.h"
#include <QOpenGLWindow>
#include <memory>
#include "TerrainData.h"
#include "Camera.h"
#include "TerrainGenerator.h"

#include <QEvent>
#include <QResizeEvent>
#include <QOpenGLWidget>

//----------------------------------------------------------------------------------------------------------------------
/// @class NGLScene
/// @brief this class inherits from the Qt OpenGLWidget and allows us to use NGL to draw OpenGL
//----------------------------------------------------------------------------------------------------------------------


class NGLScene : public QOpenGLWidget
{

  Q_OBJECT

public:

  //CONSTRUCTORS
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief ctor for our NGL drawing class
  /// @param [in] parent the parent window to the class
  //----------------------------------------------------------------------------------------------------------------------
  NGLScene(QWidget *_parent);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief dtor must close down ngl and release OpenGL resources
  //----------------------------------------------------------------------------------------------------------------------
  ~NGLScene() override;

public slots:

  //SLOTS
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a slot to toggle wireframe mode
  /// @param[in] mode, the mode passed from the togglebutton
  //----------------------------------------------------------------------------------------------------------------------
  void toggleWireframe( bool _mode );
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a slot to set the number of octaves in m_terrainValues
  /// @param[in] octaves, the int passed from m_octaves in ui
  //----------------------------------------------------------------------------------------------------------------------
  void setOctaves(int _octaves);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a slot to set the frequency in m_terrainValues
  /// @param[in] frequency, the double passed from m_frequency in ui
  //----------------------------------------------------------------------------------------------------------------------
  void setFrequency(double _frequency);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a slot to set the lacunarity in m_terrainValues
  /// @param[in] lacunarity, the double passed from m_lacunarity in ui
  //----------------------------------------------------------------------------------------------------------------------
  void setLacunarity(double _lacunarity);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a slot to set the persistence in m_terrainValues
  /// @param[in] persistence, the double passed from m_persistence in ui
  //----------------------------------------------------------------------------------------------------------------------
  void setPersistence(double _persistence);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a slot to generate a new terrain based on current values
  //----------------------------------------------------------------------------------------------------------------------
  void generate();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a slot to set the tolerance for the LOD algorithm
  /// @param[in] tolerance, the double passed from m_tolerance in ui
  //----------------------------------------------------------------------------------------------------------------------
  void setTolerance(double _tolerance);

protected:

  //PROTECTED MEMBER VARIABLES
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the windows params such as mouse and rotations etc
  //----------------------------------------------------------------------------------------------------------------------
  WinParams m_win;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief matrices to set up camera view
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Mat4 m_view;
  ngl::Mat4 m_project;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the model position for mouse movement
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_modelPos;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief used to store the global mouse transforms
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Mat4 m_mouseGlobalTX;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the VAO
  //----------------------------------------------------------------------------------------------------------------------
  std::unique_ptr<ngl::AbstractVAO> m_vao;

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief TerrainGenerator object used to generate heightmap values
  //----------------------------------------------------------------------------------------------------------------------
  TerrainGenerator m_terrainValues;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief TerrainData object which applies LOD algorithms to the heightmap values from m_terrainValues and creates
  /// a vertex list to be passed into paintGL
  //----------------------------------------------------------------------------------------------------------------------
  TerrainData m_terrain;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief our camera
  //----------------------------------------------------------------------------------------------------------------------
  Camera m_camera;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief toggle to store whether or not we draw in wireframe mode
  //----------------------------------------------------------------------------------------------------------------------
  bool m_wireframe = false;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief float to store the user-specified error tolerance passed into the LOD algorithm in m_terrain
  //----------------------------------------------------------------------------------------------------------------------
  float m_tolerance = 0.02f;


  //PROTECTED MEMBER FUNCTIONS
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the initialize class is called once when the window is created and we have a valid GL context
  /// use this to setup any default GL stuff
  //----------------------------------------------------------------------------------------------------------------------
  void initializeGL() override;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this is called everytime we want to draw the scene
  //----------------------------------------------------------------------------------------------------------------------
  void paintGL() override;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this is called everytime we resize
  //----------------------------------------------------------------------------------------------------------------------
  void resizeGL(int _w, int _h) override;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief build our VAO from m_terrainValues and m_terrain by applying meshRefine method
  //----------------------------------------------------------------------------------------------------------------------
  void buildVAO();

  //EVENTS
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Qt Event called when a key is pressed
  /// @param [in] _event the Qt event to query for size etc
  //----------------------------------------------------------------------------------------------------------------------
  void keyPressEvent(QKeyEvent *_event) override;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this method is called every time a mouse is moved
  /// @param _event the Qt Event structure
  //----------------------------------------------------------------------------------------------------------------------
  void mouseMoveEvent (QMouseEvent * _event ) override;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this method is called everytime the mouse button is pressed
  /// inherited from QObject and overridden here.
  /// @param _event the Qt Event structure
  //----------------------------------------------------------------------------------------------------------------------
  void mousePressEvent ( QMouseEvent *_event) override;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this method is called everytime the mouse button is released
  /// inherited from QObject and overridden here.
  /// @param _event the Qt Event structure
  //----------------------------------------------------------------------------------------------------------------------
  void mouseReleaseEvent ( QMouseEvent *_event ) override;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this method is called everytime the mouse wheel is moved
  /// inherited from QObject and overridden here.
  /// @param _event the Qt Event structure
  //----------------------------------------------------------------------------------------------------------------------
  void wheelEvent( QWheelEvent *_event) override;
};



#endif //NGLSCENE_H_
