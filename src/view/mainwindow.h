#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define GL_SILENCE_DEPRECATION

#include <QColor>
#include <QColorDialog>
#include <QDebug>
#include <QMainWindow>
#include <QOpenGLWidget>
#include <QPalette>
#include <QSettings>
#include <QThread>
#include <QTimer>

#include "QtGifImage/gifimage/qgifimage.h"

extern "C" {
#include "QtGifImage/gifimage/qgifimage.h"
}

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  void saveGif();
  void renderFrame();

 protected:
  void openButtonGuiAction(QString *filename);
  Ui::MainWindow *ui;
  QGifImage *gif;
  QTimer *timer;
  int frames;
};
#endif  // MAINWINDOW_H
