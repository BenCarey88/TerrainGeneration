//----------------------------------------------------------------------------------------------------------------------
/// @file MainWindow.h
/// @brief MainWindow class, implementing NGL Scene and UI
//----------------------------------------------------------------------------------------------------------------------

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <QMainWindow>
#include "NGLScene.h"
#include "TerrainData.h"
#include "TerrainGenerator.h"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private:
  /// @brief our user interface
  Ui::MainWindow *m_ui;
  /// @brief our openGL widget
  NGLScene *m_gl;
};

#endif // MAINWINDOW_H
