#define GL_SILENCE_DEPRECATION

#include "myopenglwidget.h"

#include <pthread.h>

#include <QFileDialog>
#include <QVBoxLayout>

#include "mainwindow.h"

MyOpenGLWidget::MyOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent) {
  setWindowTitle("3D Viewer");
}

MyOpenGLWidget::~MyOpenGLWidget() {}

void MyOpenGLWidget::setObjFilePath(const QString &objFilePath) {
  m_objFilePath = objFilePath;
  printf("our fileName - %s", m_objFilePath.toStdString().c_str());
}

void MyOpenGLWidget::initializeGL() { initializeOpenGLFunctions(); }

void MyOpenGLWidget::resizeGL(int w, int h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
}
void MyOpenGLWidget::paintGL() {
  if (data3d.polygons.size() != 0) {
    if (central) {
      glFrustum(-1, 1, -1, 1, 1, -10);
    } else if (parallel) {
      glOrtho(-1, 1, -1, 1, 1, -10);
    }
    glEnable(GL_DEPTH_TEST);
    glClearColor(background_color.redF(), background_color.greenF(),
                 background_color.blueF(), background_color.alphaF());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Отрисовка вершин
    if (!vertex_non) {
      glPointSize(vertex_size);
      if (vertex_circle)
        glEnable(GL_POINT_SMOOTH);
      else if (vertex_square)
        glDisable(GL_POINT_SMOOTH);

      glBegin(GL_POINTS);
      for (int i = 0; i < data3d.vertices.size(); i++) {
        glColor3f(vertex_color.redF(), vertex_color.greenF(),
                  vertex_color.blueF());
        glVertex3d(data3d.vertices[i].coords->x, data3d.vertices[i].coords->y,
                   data3d.vertices[i].coords->z);
      }
      glEnd();
    }
    // Отрисовка многоугольников
    if (!solid_edge) {
      glEnable(GL_LINE_STIPPLE);
      glLineStipple(3, 0x00FF);
    }

    glLineWidth(line_width);  // установка ширины линии
    for (int i = 0; i < data3d.polygons.size(); i = i + 3) {
      glBegin(GL_LINE_LOOP);
      for (int j = 0; j < 3; j++) {
        int vertexIndex = data3d.polygons[i + j].indices[0];
        glColor3f(line_color.redF(), line_color.greenF(), line_color.blueF());
        glVertex3f(data3d.vertices[vertexIndex].coords->x,
                   data3d.vertices[vertexIndex].coords->y,
                   data3d.vertices[vertexIndex].coords->z);
      }
      glEnd();
    }
    if (!solid_edge) {
      glDisable(GL_LINE_STIPPLE);
    }
  }
}

void MyOpenGLWidget::writeSettings() {
  QSettings settings("SOFTYRU", "3DViewer");
  settings.setValue("line_color", line_color);
  settings.setValue("vertex_color", vertex_color);
  settings.setValue("background_color", background_color);
  settings.setValue("solid_edge", solid_edge);
  settings.setValue("vertex_non", vertex_non);
  settings.setValue("vertex_circle", vertex_circle);
  settings.setValue("vertex_square", vertex_square);
  settings.setValue("line_width", line_width);
  settings.setValue("vertex_size", vertex_size);
}

void MyOpenGLWidget::readSettings() {
  QSettings settings("SOFTYRU", "3DViewer");
  line_color = settings.value("line_color").value<QColor>();
  vertex_color = settings.value("vertex_color").value<QColor>();
  background_color = settings.value("background_color").value<QColor>();
  solid_edge = settings.value("solid_edge").value<bool>();
  vertex_non = settings.value("vertex_non").value<bool>();
  vertex_circle = settings.value("vertex_circle").value<bool>();
  vertex_square = settings.value("vertex_square").value<bool>();
  line_width = settings.value("line_width").value<GLfloat>();
  vertex_size = settings.value("vertex_size").value<GLfloat>();
}
