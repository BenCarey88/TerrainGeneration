//----------------------------------------------------------------------------------------------------------------------
/// @file NGLScene.cpp
/// @brief implementation file for NGLScene class
//----------------------------------------------------------------------------------------------------------------------

#include <QMouseEvent>
#include <QGuiApplication>

#include <ngl/Transformation.h>
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include <ngl/VAOFactory.h>
#include <ngl/SimpleIndexVAO.h>
#include <array>
#include <ngl/Random.h>
#include <ngl/Quaternion.h>

#include "NGLScene.h"
#include "Camera.h"
#include "TerrainGenerator.h"


//------------------------------------------------------------------------------------------------------------------------
///CONSTRUCTORS AND DESTRUCTORS
//------------------------------------------------------------------------------------------------------------------------

NGLScene::NGLScene(QWidget *_parent) : QOpenGLWidget( _parent )
{
  // set this widget to have the initial keyboard focus
  setFocus();
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  this->resize(_parent->size());
}

NGLScene::~NGLScene()
{
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
  m_vao->removeVAO();
}


//------------------------------------------------------------------------------------------------------------------------
///OTHER PUBLIC METHODS
//------------------------------------------------------------------------------------------------------------------------

void NGLScene::resizeGL( int _w, int _h )
{
  m_project=ngl::perspective(fieldOfView, static_cast<float>( _w ) / _h, nearFrame, farFrame );
  m_win.width  = static_cast<int>( _w * devicePixelRatio() );
  m_win.height = static_cast<int>( _h * devicePixelRatio() );
}


void NGLScene::initializeGL()
{
  //generate initial terrain
  int dimension = 129;
  m_terrainValues = TerrainGenerator(dimension);
  m_terrainValues.generate();
  m_terrain = TerrainData(m_terrainValues.m_dimension, m_terrainValues.m_heightMap);

  // we need to initialise the NGL lib which will load all of the OpenGL functions, this must
  // be done once we have a valid GL context but before we call any GL commands. If we dont do
  // this everything will crash
  ngl::NGLInit::instance();

  glClearColor(0.4f, 0.4f, 0.4f, 1.0f);			   // Grey Background

  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_MULTISAMPLE);
  // Now we will create a basic Camera from the graphics library using our camera class
  m_view=ngl::lookAt(m_camera.m_from, m_camera.m_to, m_camera.m_up);
  // set the shape using FOV 45 Aspect Ratio based on Width and Height
  // The final two are near and far clipping planes
  m_project=ngl::perspective(fieldOfView ,720.0f/576.0f,nearFrame,farFrame);

  // now to load the shader and set the values
  // grab an instance of shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  // load a frag and vert shaders
  constexpr auto ColourShader="ColourShader";
  constexpr auto ColourVertex="ColourVertex";
  constexpr auto ColourFragment="ColourFragment";
  shader->createShaderProgram(ColourShader);

  shader->attachShader(ColourVertex,ngl::ShaderType::VERTEX);
  shader->attachShader(ColourFragment,ngl::ShaderType::FRAGMENT);
  shader->loadShaderSource(ColourVertex,"shaders/ColourVertex.glsl");
  shader->loadShaderSource(ColourFragment,"shaders/ColourFragment.glsl");

  shader->compileShader(ColourVertex);
  shader->compileShader(ColourFragment);
  shader->attachShaderToProgram(ColourShader,ColourVertex);
  shader->attachShaderToProgram(ColourShader,ColourFragment);

  shader->linkProgramObject(ColourShader);
  (*shader)[ColourShader]->use();

  buildVAO();
  ngl::VAOFactory::listCreators();
}


void NGLScene::buildVAO()
{
  //run LOD algorithm on m_terrain so m_terrain.m_indices is set up with a list of vertices to be drawn
  m_terrain.meshRefine(m_camera.m_from, m_tolerance, 100.0);

  //create list of vertex positions and colours
  std::vector<ngl::Vec3> vertAndColour;
  float size = m_terrain.m_indices.size();
  for(size_t i = 0; i<size; ++i)
  {
    //set position
    TerrainData::Vertex vertex = m_terrain.m_vertices[m_terrain.m_indices[i]];
    ngl::Vec3 pos (vertex.sceneX,vertex.sceneY,vertex.sceneZ);

    //set colour
    ngl::Vec3 col;
    float height = vertex.sceneZ/(m_terrain.m_scale*m_terrainValues.m_amplitude);
    if (height > 0.8f)
    {
      col  = ngl::Vec3(1,1,1);
      col *= std::abs(height);
    }
    else if (height > 0.4f)
    {
      col  = ngl::Vec3(0.5,0.5,0.5);
      col *= std::abs(height);
    }
    else if (height > -0.4f)
    {
      col  = ngl::Vec3(0.4f,0.8f,0);
      col *= std::abs(height+0.4f)*0.5f;
    }
    else if (height > -0.8f)
    {
      col  = ngl::Vec3(0.25f,0.2f,0);
      col *= (1-std::abs(height));
    }
    else
    {
      col  = ngl::Vec3(0.2f,0.2f,0);
      col *= (1-std::abs(height));
    }
    vertAndColour.push_back(pos);
    vertAndColour.push_back(col);
  }

  // create list of indices
  std::vector<GLshort> indices;
  for(size_t i = 0; i< m_terrain.m_indices.size(); ++i)
  {
    indices.push_back(GLshort(i));
  }

  // create a vao using GL_TRIANGLE_STRIP
  m_vao=ngl::VAOFactory::createVAO(ngl::simpleIndexVAO,GL_TRIANGLE_STRIP);
  m_vao->bind();

  // set our data as the vertices above
  m_vao->setData(ngl::SimpleIndexVAO::VertexData(
                                                  sizeof(ngl::Vec3)*vertAndColour.size(),
                                                  vertAndColour[0].m_x,
                                                  uint(indices.size()),
                                                  &indices[0],
                                                  GL_UNSIGNED_SHORT));
  // data is 24 bytes apart ( two Vec3's) first index
  // is 0 second is 3 floats into the data set (i.e. vec3 offset)
  m_vao->setVertexAttributePointer(0,3,GL_FLOAT,24,0);
  m_vao->setVertexAttributePointer(1,3,GL_FLOAT,24,3);
  m_vao->setNumIndices(m_terrain.m_indices.size());

  // now unbind
  m_vao->unbind();
}



void NGLScene::paintGL()
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0,0,m_win.width,m_win.height);
  if (m_wireframe == true)
  {
      glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  }
  else
  {
      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  }

  m_view = ngl::lookAt(m_camera.m_from, m_camera.m_to, m_camera.m_up);

  buildVAO();

  ngl::Mat4 translate1;
  ngl::Mat4 translate2;

  ngl::Quaternion quatRotX;
  ngl::Quaternion quatRotY;
  quatRotX.fromAxisAngle(m_camera.m_right,-m_win.spinXFace);
  quatRotY.fromAxisAngle({0,0,1},-m_win.spinYFace);
  ngl::Mat4 rotX = quatRotX.toMat4();
  ngl::Mat4 rotY = quatRotY.toMat4();
  m_camera.update();
  m_camera.m_up = m_camera.m_trueUp;
  m_camera.m_from -= m_camera.m_to;
  m_camera.m_from = rotX*rotY*m_camera.m_from;
  m_camera.m_up = rotX*rotY*m_camera.m_up;
  m_camera.m_from += m_camera.m_to;
  m_camera.update();

  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["ColourShader"]->use();


  ngl::Mat4 MVP= m_project*m_view*translate2*m_mouseGlobalTX*translate1;
  shader->setUniform("MVP",MVP);

  m_vao->bind();
  m_vao->draw();
  m_vao->unbind();
}


//------------------------------------------------------------------------------------------------------------------------
///EVENTS
//------------------------------------------------------------------------------------------------------------------------

void NGLScene::mouseMoveEvent( QMouseEvent* _event )
{
  // note the method buttons() is the button state when event was called
  // that is different from button() which is used to check which button was
  // pressed when the mousePress/Release event is generated
  if ( m_win.rotate && _event->buttons() == Qt::LeftButton )
  {
    int diffx = _event->x() - m_win.origX;
    int diffy = _event->y() - m_win.origY;
    m_win.spinXFace = static_cast<int>( 0.2f * diffy );
    m_win.spinYFace = static_cast<int>( 0.2f * diffx );
    m_win.origX = _event->x();
    m_win.origY = _event->y();

    update();

  }
  // right mouse translate code
  else if ( m_win.translate && _event->buttons() == Qt::MidButton )
  {
    int diffX      = static_cast<int>( _event->x() - m_win.origXPos );
    int diffY      = static_cast<int>( _event->y() - m_win.origYPos );
    m_win.origXPos = _event->x();
    m_win.origYPos = _event->y();

    m_camera.update();

    m_camera.m_from -= INCREMENT * diffX * m_camera.m_right;
    m_camera.m_to -= INCREMENT * diffX * m_camera.m_right;
    m_camera.m_from += INCREMENT * diffY * m_camera.m_trueUp;
    m_camera.m_to += INCREMENT * diffY * m_camera.m_trueUp;

    update();
  }
}


//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mousePressEvent( QMouseEvent* _event )
{
  // that method is called when the mouse button is pressed in this case we
  // store the value where the maouse was clicked (x,y) and set the Rotate flag to true
  if ( _event->button() == Qt::LeftButton )
  {
    m_win.origX  = _event->x();
    m_win.origY  = _event->y();
    m_win.rotate = true;
  }
  // right mouse translate mode
  else if ( _event->button() == Qt::MidButton )
  {
    m_win.origXPos  = _event->x();
    m_win.origYPos  = _event->y();
    m_win.translate = true;
  }
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mouseReleaseEvent( QMouseEvent* _event )
{
  // this event is called when the mouse button is released
  // we then set Rotate to false
  if ( _event->button() == Qt::LeftButton )
  {
    m_win.rotate = false;
    m_win.spinXFace = 0;
    m_win.spinYFace = 0;
  }
  // middle mouse translate mode
  if ( _event->button() == Qt::MidButton )
  {
    m_win.translate = false;
  }
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::wheelEvent( QWheelEvent* _event )
{

  // check the diff of the wheel position (0 means no change)
  if ( _event->delta() > 0 )
  {
    m_camera.m_from += ZOOM*(m_camera.m_to-m_camera.m_from);
  }
  else if ( _event->delta() < 0 )
  {
    m_camera.m_from -= ZOOM*(m_camera.m_to-m_camera.m_from);
  }
  update();
}

//------------------------------------------------------------------------------------------------------------------------
///PUBLIC SLOTS
//------------------------------------------------------------------------------------------------------------------------

void NGLScene::setOctaves(int _octaves)
{
    m_terrainValues.m_octaves = _octaves;
}

void NGLScene::setFrequency(double _frequency)
{
    m_terrainValues.m_frequency = _frequency;
}

void NGLScene::setLacunarity(double _lacunarity)
{
    m_terrainValues.m_lacunarity = _lacunarity;
}

void NGLScene::setPersistence(double _persistence)
{
    m_terrainValues.m_persistence = _persistence;
}

void NGLScene::setAmplitude(double _amplitude)
{
    m_terrainValues.m_amplitude = float(_amplitude);
}

void NGLScene::setSeed(double _seed)
{
    m_terrainValues.m_seed = _seed;
}

void NGLScene::generate()
{
    m_terrainValues.generate();
    m_terrain = TerrainData(m_terrainValues.m_dimension, m_terrainValues.m_heightMap);
    m_camera.initialise();
    update();
}

void NGLScene::setTolerance(double _tolerance)
{
    m_tolerance = float(_tolerance);
    update();
}

void NGLScene::toggleWireframe(bool _mode	 )
{
    m_wireframe=_mode;
    update();
}
