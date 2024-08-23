#define GL_SILENCE_DEPRECATION
#include "mainwindow.h"

#include <QFileDialog>
#include <QVBoxLayout>

#include "./ui_mainwindow.h"
#include "myopenglwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  // Инициализация класса виджета
  ui->setupUi(this);
  MyOpenGLWidget *openGLWidget = new MyOpenGLWidget(ui->openGLWidget);
  openGLWidget->readSettings();
  // Инициализация данных для отрисовки
  // openGLWidget->data3d.amount_polygon = 0;
  // openGLWidget->data3d_copy.amount_polygon = 0;
  // Инициализация слайдеров для перемещения
  openGLWidget->MX_slider = ui->MX_horizontalSlider->value();
  openGLWidget->MY_slider = ui->MY_horizontalSlider->value();
  openGLWidget->MZ_slider = ui->MZ_horizontalSlider->value();
  // Инициализация слайдеров для поворота
  openGLWidget->RX_slider = ui->RX_horizontalSlider->value();
  openGLWidget->RY_slider = ui->RY_horizontalSlider->value();
  openGLWidget->RZ_slider = ui->RZ_horizontalSlider->value();
  // Инициализация слайдера для маштабирования
  openGLWidget->Scale_slider = 1;
  ui->Central_radioButton->setChecked(1);
  // Настройка отображения виджета
  QVBoxLayout *layout = new QVBoxLayout(ui->openGLWidget);
  layout->addWidget(openGLWidget);
  ui->openGLWidget->setLayout(layout);
  // Настройка слайдера для перемещения объекта по оси Х
  connect(ui->MX_horizontalSlider, &QSlider::valueChanged, ui->MX_spinBox,
          &QSpinBox::setValue);
  connect(ui->MX_spinBox, QOverload<int>::of(&QSpinBox::valueChanged),
          ui->MX_horizontalSlider, &QSlider::setValue);
  ui->MX_spinBox->setMinimum(-500);
  ui->MX_spinBox->setMaximum(500);
  ui->MX_horizontalSlider->setMinimum(-500);
  ui->MX_horizontalSlider->setMaximum(500);
  ui->MX_horizontalSlider->setValue(0);
  // Настройка слайдера для перемещения объекта по оси У
  connect(ui->MY_horizontalSlider, &QSlider::valueChanged, ui->MY_spinBox,
          &QSpinBox::setValue);
  connect(ui->MY_spinBox, QOverload<int>::of(&QSpinBox::valueChanged),
          ui->MY_horizontalSlider, &QSlider::setValue);
  ui->MY_spinBox->setMinimum(-500);
  ui->MY_spinBox->setMaximum(500);
  ui->MY_horizontalSlider->setMinimum(-500);
  ui->MY_horizontalSlider->setMaximum(500);
  ui->MY_horizontalSlider->setValue(0);
  // Настройка слайдера для перемещения объекта по оси Z
  connect(ui->MZ_horizontalSlider, &QSlider::valueChanged, ui->MZ_spinBox,
          &QSpinBox::setValue);
  connect(ui->MZ_spinBox, QOverload<int>::of(&QSpinBox::valueChanged),
          ui->MZ_horizontalSlider, &QSlider::setValue);
  ui->MZ_spinBox->setMinimum(-500);
  ui->MZ_spinBox->setMaximum(500);
  ui->MZ_horizontalSlider->setMinimum(-500);
  ui->MZ_horizontalSlider->setMaximum(500);
  ui->MZ_horizontalSlider->setValue(0);

  // Настройка слайдера для поворота объекта по оси Х
  connect(ui->RX_horizontalSlider, &QSlider::valueChanged, ui->RX_spinBox,
          &QSpinBox::setValue);
  connect(ui->RX_spinBox, QOverload<int>::of(&QSpinBox::valueChanged),
          ui->RX_horizontalSlider, &QSlider::setValue);
  ui->RX_spinBox->setMinimum(-3600000);
  ui->RX_spinBox->setMaximum(3600000);
  ui->RX_horizontalSlider->setMinimum(-1000000);
  ui->RX_horizontalSlider->setMaximum(1000000);
  ui->RX_horizontalSlider->setValue(0);

  // Настройка слайдера для перемещения объекта по оси У
  connect(ui->RY_horizontalSlider, &QSlider::valueChanged, ui->RY_spinBox,
          &QSpinBox::setValue);
  connect(ui->RY_spinBox, QOverload<int>::of(&QSpinBox::valueChanged),
          ui->RY_horizontalSlider, &QSlider::setValue);
  ui->RY_spinBox->setMinimum(-3600000);
  ui->RY_spinBox->setMaximum(3600000);
  ui->RY_horizontalSlider->setMinimum(-1000000);
  ui->RY_horizontalSlider->setMaximum(1000000);
  ui->RY_horizontalSlider->setValue(0);

  // Настройка слайдера для перемещения объекта по оси Z
  connect(ui->RZ_horizontalSlider, &QSlider::valueChanged, ui->RZ_spinBox,
          &QSpinBox::setValue);
  connect(ui->RZ_spinBox, QOverload<int>::of(&QSpinBox::valueChanged),
          ui->RZ_horizontalSlider, &QSlider::setValue);
  ui->RZ_spinBox->setMinimum(-3600000);
  ui->RZ_spinBox->setMaximum(3600000);
  ui->RZ_horizontalSlider->setMinimum(-1000000);
  ui->RZ_horizontalSlider->setMaximum(1000000);
  ui->RZ_horizontalSlider->setValue(0);
  // Настройка слайдера для маштабирования объекта
  connect(ui->Scale_horizontalSlider, &QSlider::valueChanged, ui->Scale_spinBox,
          &QSpinBox::setValue);
  connect(ui->Scale_spinBox, QOverload<int>::of(&QSpinBox::valueChanged),
          ui->Scale_horizontalSlider, &QSlider::setValue);
  ui->Scale_spinBox->setMinimum(1);
  ui->Scale_spinBox->setMaximum(100);
  ui->Scale_horizontalSlider->setMinimum(1);
  ui->Scale_horizontalSlider->setMaximum(100);
  ui->Scale_horizontalSlider->setValue(100);
}

void MainWindow::openButtonGuiAction(QString *filename) {
  MyOpenGLWidget *openGLWidget =
      ui->openGLWidget->findChild<MyOpenGLWidget *>();

  *filename = QFileDialog::getOpenFileName(nullptr, "Открыть файл", "",
                                           "Файлы (*.obj *.stl)");
  ui->FileName_2->setText(*filename);
  openGLWidget->setObjFilePath(*filename);
  ui->Vertexes_num->setText(
      QString::number(openGLWidget->data3d.vertices.size()));
  ui->Polygons_num->setText(
      QString::number(openGLWidget->data3d.polygons.size()));
}

MainWindow::~MainWindow() {
  MyOpenGLWidget *openGLWidget =
      ui->openGLWidget->findChild<MyOpenGLWidget *>();
  openGLWidget->writeSettings();
  // Высвобождение памяти
  // freeAllData(&openGLWidget->data3d);
  // freeAllData(&openGLWidget->data3d_copy);
  delete ui;
}

void MainWindow::saveGif() {
  QPixmap pix = QPixmap::fromImage(ui->openGLWidget->grab().toImage());
  QPixmap scaledPix = pix.scaled(QSize(640, 480), Qt::IgnoreAspectRatio,
                                 Qt::SmoothTransformation);
  gif->addFrame(scaledPix.toImage());

  if (frames == 50) {
    timer->stop();
    QString pathGIF = QFileDialog::getSaveFileName(
        this, ("Save as GIF"), "image.gif", "GIF Image Files (*.gif)");

    if (!pathGIF.isEmpty() && !pathGIF.isNull()) {
      gif->save(pathGIF);
      delete timer;
      delete gif;
    } else {
      delete timer;
      delete gif;
    }
  }
  frames++;
}

void MainWindow::renderFrame() {
  MyOpenGLWidget *openGLWidget =
      ui->openGLWidget->findChild<MyOpenGLWidget *>();
  openGLWidget->update();
}
