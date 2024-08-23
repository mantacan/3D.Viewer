#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H
#define GL_SILENCE_DEPRECATION

#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QSettings>

#include "../3DViewer.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "../3DViewer.h"
#ifdef __cplusplus
}
#endif

#if defined(linux) || defined(_WIN32)
#include <GL/glut.h>
#else
#include <GLUT/GLUT.h>
#endif

class MyOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
 private:
  QString m_objFilePath;
  QSettings *settings;

 public:
  explicit MyOpenGLWidget(QWidget *parent = nullptr);
  ~MyOpenGLWidget();
  void setObjFilePath(const QString &objFilePath);
  s21::Model::Data data3d;
  s21::Model::Data data3d_copy;
  GLfloat line_width = 1;
  GLfloat vertex_size = 5;
  int MX_slider;
  int MY_slider;
  int MZ_slider;
  int RX_slider;
  int RY_slider;
  int RZ_slider;
  int m_coeffMatrixLoc = 0;
  int Scale_slider;
  bool central = true;
  bool parallel = false;
  bool solid_edge = true;
  bool vertex_non = false;
  bool vertex_circle = false;
  bool vertex_square = true;
  QColor line_color = Qt::green;
  QColor vertex_color = Qt::green;
  QColor background_color = Qt::black;
  void writeSettings();
  void readSettings();

 protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;
  void setupProjection(int w, int h);
};

#endif  // MYOPENGLWIDGET_H
