//----------------------------------------------------------------------------------------------------------------------
/// @file MainWindow.cpp
/// @brief Implementation of MainWindow signals and slots
//----------------------------------------------------------------------------------------------------------------------

#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  m_ui(new Ui::MainWindow)
{
  m_ui->setupUi(this);
  m_gl=new  NGLScene(this);
  m_ui->s_mainWindowGridLayout->addWidget(m_gl,0,0,2,1);

  connect(m_ui->m_octaves,SIGNAL(valueChanged(int)),m_gl,SLOT(setOctaves(int)));
  connect(m_ui->m_frequency,SIGNAL(valueChanged(double)),m_gl,SLOT(setFrequency(double)));
  connect(m_ui->m_lacunarity,SIGNAL(valueChanged(double)),m_gl,SLOT(setLacunarity(double)));
  connect(m_ui->m_persistence,SIGNAL(valueChanged(double)),m_gl,SLOT(setPersistence(double)));
  connect(m_ui->m_amplitude,SIGNAL(valueChanged(double)),m_gl,SLOT(setAmplitude(double)));
  connect(m_ui->m_seed,SIGNAL(valueChanged(double)),m_gl,SLOT(setSeed(double)));

  connect(m_ui->m_generate,SIGNAL(clicked()),m_gl,SLOT(generate()));
  connect(m_ui->m_tolerance,SIGNAL(valueChanged(double)),m_gl,SLOT(setTolerance(double)));
  connect(m_ui->m_wireframe,SIGNAL(toggled(bool)),m_gl,SLOT(toggleWireframe(bool)));
}

MainWindow::~MainWindow()
{
  delete m_ui;
}
