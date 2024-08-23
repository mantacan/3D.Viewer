#include "controller.h"

#include "../view/ui_mainwindow.h"
s21::CPPViewer::CPPViewer(QWidget *parent)
    : MainWindow(parent)  // Initialize the base class
{
  connect(ui->OpenButton, &QPushButton::clicked, this,
          &s21::CPPViewer::on_OpenButton_clicked);
  connect(ui->Apply_button, &QPushButton::clicked, this,
          &s21::CPPViewer::on_Apply_button_clicked);
  connect(ui->MX_horizontalSlider, &QSlider::sliderMoved, this,
          &s21::CPPViewer::on_MX_horizontalSlider_sliderMoved);
  connect(ui->MY_horizontalSlider, &QSlider::sliderMoved, this,
          &s21::CPPViewer::on_MY_horizontalSlider_sliderMoved);
  connect(ui->MZ_horizontalSlider, &QSlider::sliderMoved, this,
          &s21::CPPViewer::on_MZ_horizontalSlider_sliderMoved);
  connect(ui->RX_horizontalSlider, &QSlider::sliderMoved, this,
          &s21::CPPViewer::on_RX_horizontalSlider_sliderMoved);
  connect(ui->RY_horizontalSlider, &QSlider::sliderMoved, this,
          &s21::CPPViewer::on_RY_horizontalSlider_sliderMoved);
  connect(ui->RZ_horizontalSlider, &QSlider::sliderMoved, this,
          &s21::CPPViewer::on_RZ_horizontalSlider_sliderMoved);
  connect(ui->Scale_horizontalSlider, &QSlider::sliderMoved, this,
          &s21::CPPViewer::on_Scale_horizontalSlider_sliderMoved);
  connect(ui->Parallel_radioButton, &QRadioButton::clicked, this,
          &s21::CPPViewer::on_Parallel_radioButton_clicked);
  connect(ui->Central_radioButton, &QRadioButton::clicked, this,
          &s21::CPPViewer::on_Central_radioButton_clicked);
  connect(ui->Solid_radioButton, &QRadioButton::clicked, this,
          &s21::CPPViewer::on_Solid_radioButton_clicked);
  connect(ui->Dashed_radioButton, &QRadioButton::clicked, this,
          &s21::CPPViewer::on_Dashed_radioButton_clicked);
  connect(ui->line_width, QOverload<int>::of(&QSpinBox::valueChanged), this,
          &s21::CPPViewer::on_line_width_valueChanged);
  connect(ui->choose_color_line, &QPushButton::clicked, this,
          &s21::CPPViewer::on_choose_color_line_clicked);
  connect(ui->choose_color_vertex, &QPushButton::clicked, this,
          &s21::CPPViewer::on_choose_color_vertex_clicked);
  connect(ui->choose_color_background, &QPushButton::clicked, this,
          &s21::CPPViewer::on_choose_color_background_clicked);
  connect(ui->spinBox_vertex_size, QOverload<int>::of(&QSpinBox::valueChanged),
          this, &s21::CPPViewer::on_spinBox_vertex_size_valueChanged);
  connect(ui->Non_radioButton, &QRadioButton::clicked, this,
          &s21::CPPViewer::on_Non_radioButton_clicked);
  connect(ui->Circle_radioButton, &QRadioButton::clicked, this,
          &s21::CPPViewer::on_Circle_radioButton_clicked);
  connect(ui->Square_radioButton, &QRadioButton::clicked, this,
          &s21::CPPViewer::on_Square_radioButton_clicked);
  connect(ui->ScreenshotButton, &QPushButton::clicked, this,
          &s21::CPPViewer::on_ScreenshotButton_clicked);
  connect(ui->RecordingButton, &QPushButton::clicked, this,
          &s21::CPPViewer::on_RecordingButton_clicked);

  //    ui->setupUi(this); // Use the UI setup function
  //    qDebug() << "Controller initialized";
}

void s21::CPPViewer::on_OpenButton_clicked() {
  MyOpenGLWidget *openGLWidget =
      ui->openGLWidget->findChild<MyOpenGLWidget *>();

  QString filename = QFileDialog::getOpenFileName(nullptr, "Открыть файл", "",
                                                  "Файлы (*.obj *.stl)");
  ui->FileName_2->setText(filename);
  model.setFilePath(filename.toStdString().c_str());
  openGLWidget->data3d = model.getData();
  openGLWidget->data3d_copy = model.getData();
  if (openGLWidget) {
    openGLWidget->setObjFilePath(filename);
    MainWindow::renderFrame();  // вызов метода paintGL() для обно
    ui->Vertexes_num->setText(
        QString::number(openGLWidget->data3d.vertices.size()));
    ui->Polygons_num->setText(
        QString::number(openGLWidget->data3d.polygons.size()));
    // отображения}
  }
}

void s21::CPPViewer::on_Apply_button_clicked() {
  MyOpenGLWidget *openGLWidget =
      ui->openGLWidget->findChild<MyOpenGLWidget *>();
  model.turnX(ui->RX_spinBox->value());
  model.turnY(ui->RY_spinBox->value());
  model.turnZ(ui->RZ_spinBox->value());
  model.moveObj(ui->MX_spinBox->value(), ui->MY_spinBox->value(),
                ui->MZ_spinBox->value());
  openGLWidget->data3d = model.data;
  MainWindow::renderFrame();  // вызов метода paintGL()
}

void s21::CPPViewer::on_MX_horizontalSlider_sliderMoved(int position) {
  MyOpenGLWidget *openGLWidget =
      ui->openGLWidget->findChild<MyOpenGLWidget *>();
  model.moveObj(position / 10 - openGLWidget->MX_slider, 0, 0);
  openGLWidget->data3d = model.data;
  MainWindow::renderFrame();  // вызов метода paintGL()
  openGLWidget->MX_slider = position / 10;
}
// Обработка движения слайдера для перемещения по Y
void s21::CPPViewer::on_MY_horizontalSlider_sliderMoved(int position) {
  MyOpenGLWidget *openGLWidget =
      ui->openGLWidget->findChild<MyOpenGLWidget *>();
  model.moveObj(0, position / 10 - openGLWidget->MY_slider, 0);
  openGLWidget->data3d = model.data;
  MainWindow::renderFrame();
  openGLWidget->MY_slider = position / 10;
}
// Обработка движения слайдера для перемещения по Z
void s21::CPPViewer::on_MZ_horizontalSlider_sliderMoved(int position) {
  MyOpenGLWidget *openGLWidget =
      ui->openGLWidget->findChild<MyOpenGLWidget *>();
  if (openGLWidget->central) {
    model.moveObj(0, 0, position / 10 - openGLWidget->MZ_slider);
    openGLWidget->data3d = model.data;
    MainWindow::renderFrame();  // вызов метода paintGL()
    openGLWidget->MZ_slider = position / 10;
  }
}

// Обработка движения слайдера для кручения по Х
void s21::CPPViewer::on_RX_horizontalSlider_sliderMoved(int position) {
  MyOpenGLWidget *openGLWidget =
      ui->openGLWidget->findChild<MyOpenGLWidget *>();
  model.turnX((position - openGLWidget->RX_slider) / 1000);
  openGLWidget->data3d = model.data;
  MainWindow::renderFrame();  // вызов метода paintGL()
  openGLWidget->RX_slider = position;
}

// Обработка движения слайдера для кручения по Y
void s21::CPPViewer::on_RY_horizontalSlider_sliderMoved(int position) {
  MyOpenGLWidget *openGLWidget =
      ui->openGLWidget->findChild<MyOpenGLWidget *>();
  model.turnY((position - openGLWidget->RY_slider) / 1000);
  openGLWidget->data3d = model.data;
  MainWindow::renderFrame();  // вызов метода paintGL()
  openGLWidget->RY_slider = position;
}

// Обработка движения слайдера для кручения по Z
void s21::CPPViewer::on_RZ_horizontalSlider_sliderMoved(int position) {
  MyOpenGLWidget *openGLWidget =
      ui->openGLWidget->findChild<MyOpenGLWidget *>();
  model.turnZ((position - openGLWidget->RZ_slider) / 1000);
  openGLWidget->data3d = model.data;
  MainWindow::renderFrame();  // вызов метода paintGL()
  openGLWidget->RZ_slider = position;
}

// Обработка движения слайдера для маштабирования
void s21::CPPViewer::on_Scale_horizontalSlider_sliderMoved(int position) {
  MyOpenGLWidget *openGLWidget =
      ui->openGLWidget->findChild<MyOpenGLWidget *>();
  if (position < openGLWidget->Scale_slider)
    model.scaleObj((double)(position) / 100);
  else
    model.scaleObj((double)(position) / 100 + 1);
  openGLWidget->data3d = model.data;
  MainWindow::renderFrame();  // вызов метода paintGL()
  openGLWidget->Scale_slider = position;
}

void s21::CPPViewer::on_Parallel_radioButton_clicked(bool checked) {
  MyOpenGLWidget *openGLWidget =
      ui->openGLWidget->findChild<MyOpenGLWidget *>();
  if (checked) {
    openGLWidget->parallel = true;
    openGLWidget->central = false;
  }
  MainWindow::renderFrame();  // вызов метода paintGL()
}

void s21::CPPViewer::on_Central_radioButton_clicked(bool checked) {
  MyOpenGLWidget *openGLWidget =
      ui->openGLWidget->findChild<MyOpenGLWidget *>();
  if (checked) {
    openGLWidget->parallel = false;
    openGLWidget->central = true;
  }
  MainWindow::renderFrame();  // вызов метода paintGL()
}

// Устанвока непрерывной линии
void s21::CPPViewer::on_Solid_radioButton_clicked() {
  MyOpenGLWidget *openGLWidget =
      ui->openGLWidget->findChild<MyOpenGLWidget *>();
  openGLWidget->solid_edge = true;
  MainWindow::renderFrame();  // вызов метода paintGL()
}

// Устанвока прерывистой линии
void s21::CPPViewer::on_Dashed_radioButton_clicked() {
  MyOpenGLWidget *openGLWidget =
      ui->openGLWidget->findChild<MyOpenGLWidget *>();
  openGLWidget->solid_edge = false;
  MainWindow::renderFrame();  // вызов метода paintGL()
}

// Смена ширины линий
void s21::CPPViewer::on_line_width_valueChanged(int arg1) {
  MyOpenGLWidget *openGLWidget =
      ui->openGLWidget->findChild<MyOpenGLWidget *>();
  openGLWidget->line_width = ui->line_width->value();
  MainWindow::renderFrame();  // вызов метода paintGL()
}
// Смена цвета ребер
void s21::CPPViewer::on_choose_color_line_clicked() {
  MyOpenGLWidget *openGLWidget =
      ui->openGLWidget->findChild<MyOpenGLWidget *>();
  openGLWidget->line_color =
      QColorDialog::getColor(Qt::white, this, tr("Select color"));
  MainWindow::renderFrame();  // вызов метода paintGL()
}

// Смена цвета вершин
void s21::CPPViewer::on_choose_color_vertex_clicked() {
  MyOpenGLWidget *openGLWidget =
      ui->openGLWidget->findChild<MyOpenGLWidget *>();
  openGLWidget->vertex_color =
      QColorDialog::getColor(Qt::white, this, tr("Select color"));
  MainWindow::renderFrame();  // вызов метода paintGL()
}

// Смена заднего фона
void s21::CPPViewer::on_choose_color_background_clicked() {
  MyOpenGLWidget *openGLWidget =
      ui->openGLWidget->findChild<MyOpenGLWidget *>();
  openGLWidget->background_color =
      QColorDialog::getColor(Qt::white, this, tr("Select color"));
  MainWindow::renderFrame();  // вызов метода paintGL()
}

// Смена размера вершин
void s21::CPPViewer::on_spinBox_vertex_size_valueChanged(int arg1) {
  MyOpenGLWidget *openGLWidget =
      ui->openGLWidget->findChild<MyOpenGLWidget *>();
  openGLWidget->vertex_size = ui->spinBox_vertex_size->value();
  MainWindow::renderFrame();  // вызов метода paintGL()
}

void s21::CPPViewer::on_Non_radioButton_clicked() {
  MyOpenGLWidget *openGLWidget =
      ui->openGLWidget->findChild<MyOpenGLWidget *>();
  openGLWidget->vertex_non = true;
  MainWindow::renderFrame();  // вызов метода paintGL()
}

void s21::CPPViewer::on_Circle_radioButton_clicked() {
  MyOpenGLWidget *openGLWidget =
      ui->openGLWidget->findChild<MyOpenGLWidget *>();
  openGLWidget->vertex_circle = true;
  openGLWidget->vertex_non = false;
  openGLWidget->vertex_square = false;
  MainWindow::renderFrame();  // вызов метода paintGL()
}

void s21::CPPViewer::on_Square_radioButton_clicked() {
  MyOpenGLWidget *openGLWidget =
      ui->openGLWidget->findChild<MyOpenGLWidget *>();
  openGLWidget->vertex_circle = false;
  openGLWidget->vertex_non = false;
  openGLWidget->vertex_square = true;
  MainWindow::renderFrame();  // вызов метода paintGL()
}

void s21::CPPViewer::on_ScreenshotButton_clicked() {
  QString str = QFileDialog::getSaveFileName(this, nullptr, nullptr,
                                             "JPEG (*.jpeg) ;; BMP (*.bmp");
  if (!str.isEmpty()) {
    ui->openGLWidget->grab().save(str);
  }
}

void s21::CPPViewer::on_RecordingButton_clicked() {
  frames = 0;
  gif = new QGifImage;
  timer = new QTimer(this);
  gif->setDefaultDelay(100);
  connect(timer, &QTimer::timeout, this, &MainWindow::saveGif);
  timer->start(100);
}

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  s21::CPPViewer w;
  w.show();
  return a.exec();
}
