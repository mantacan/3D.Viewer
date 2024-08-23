#include <QApplication>
#include <QFileDialog>

#include "../3DViewer.h"
#include "../view/mainwindow.h"
#include "../view/myopenglwidget.h"

// namespace Ui {
// class MainWindow;
// }

namespace s21 {
class CPPViewer : public MainWindow {
 public:
  explicit CPPViewer(QWidget *parent = nullptr);
  ~CPPViewer() = default;

 private:
  s21::Model model;
 private slots:
  void on_OpenButton_clicked();
  void on_Apply_button_clicked();

  void on_MX_horizontalSlider_sliderMoved(int position);

  void on_MY_horizontalSlider_sliderMoved(int position);

  void on_MZ_horizontalSlider_sliderMoved(int position);

  void on_RX_horizontalSlider_sliderMoved(int position);

  void on_RY_horizontalSlider_sliderMoved(int position);

  void on_RZ_horizontalSlider_sliderMoved(int position);

  void on_Scale_horizontalSlider_sliderMoved(int position);

  void on_Parallel_radioButton_clicked(bool checked);

  void on_Central_radioButton_clicked(bool checked);

  void on_Solid_radioButton_clicked();

  void on_Dashed_radioButton_clicked();

  void on_line_width_valueChanged(int arg1);

  void on_choose_color_line_clicked();

  void on_choose_color_vertex_clicked();

  void on_choose_color_background_clicked();

  void on_spinBox_vertex_size_valueChanged(int arg1);

  void on_Non_radioButton_clicked();

  void on_Circle_radioButton_clicked();

  void on_Square_radioButton_clicked();

  void on_ScreenshotButton_clicked();

  void on_RecordingButton_clicked();

  void saveGif();

  void renderFrame();
};
}  // namespace s21
