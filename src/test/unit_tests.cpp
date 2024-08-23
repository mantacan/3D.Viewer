
// Created by qwe on 18.05.24.
//

#include <gtest/gtest.h>

#include "../3DViewer.h"

TEST(ModelTest, TestParserSuccess) {
  std::string testFileName = "test/pyramid.txt";
  s21::Model model;
  model.setFilePath(testFileName.c_str());
  // Проверка количества вершин
  const auto& vertices = model.getData().getVertices();

  ASSERT_EQ(vertices.size(), 5);

  // Проверка координат вершин
  ASSERT_DOUBLE_EQ(vertices[0].coords[0].x, 0.0);
  ASSERT_DOUBLE_EQ(vertices[0].coords[0].y, 1.0);
  ASSERT_DOUBLE_EQ(vertices[0].coords[0].z, 0.0);

  ASSERT_DOUBLE_EQ(vertices[1].coords[0].x, -1.0);
  ASSERT_DOUBLE_EQ(vertices[1].coords[0].y, 0.0);
  ASSERT_DOUBLE_EQ(vertices[1].coords[0].z, -1.0);

  ASSERT_DOUBLE_EQ(vertices[2].coords[0].x, 1.0);
  ASSERT_DOUBLE_EQ(vertices[2].coords[0].y, 0.0);
  ASSERT_DOUBLE_EQ(vertices[2].coords[0].z, -1.0);

  ASSERT_DOUBLE_EQ(vertices[3].coords[0].x, 1.0);
  ASSERT_DOUBLE_EQ(vertices[3].coords[0].y, 0.0);
  ASSERT_DOUBLE_EQ(vertices[3].coords[0].z, 1.0);

  ASSERT_DOUBLE_EQ(vertices[4].coords[0].x, -1.0);
  ASSERT_DOUBLE_EQ(vertices[4].coords[0].y, 0.0);
  ASSERT_DOUBLE_EQ(vertices[4].coords[0].z, 1.0);

  const auto& polygons = model.getData().getPolygons();

  ASSERT_EQ(polygons.size(), 18);

  ASSERT_EQ(polygons[0].indices[0], 0);
  ASSERT_EQ(polygons[1].indices[0], 3);
  ASSERT_EQ(polygons[2].indices[0], 2);

  ASSERT_EQ(polygons[3].indices[0], 0);
  ASSERT_EQ(polygons[4].indices[0], 2);
  ASSERT_EQ(polygons[5].indices[0], 3);

  ASSERT_EQ(polygons[6].indices[0], 0);
  ASSERT_EQ(polygons[7].indices[0], 3);
  ASSERT_EQ(polygons[8].indices[0], 4);

  ASSERT_EQ(polygons[9].indices[0], 0);
  ASSERT_EQ(polygons[10].indices[0], 4);
  ASSERT_EQ(polygons[11].indices[0], 1);

  ASSERT_EQ(polygons[12].indices[0], 2);
  ASSERT_EQ(polygons[13].indices[0], 1);
  ASSERT_EQ(polygons[14].indices[0], 3);

  ASSERT_EQ(polygons[15].indices[0], 1);
  ASSERT_EQ(polygons[16].indices[0], 4);
  ASSERT_EQ(polygons[17].indices[0], 3);
}

TEST(ModelTest, TestParserFileNotFound) {
  s21::Model model("non_existent.obj");
  testing::internal::CaptureStderr();
  model.parser();
  std::string output = testing::internal::GetCapturedStderr();
  ASSERT_TRUE(output.find("Cannot open file!") != std::string::npos);
}

TEST(ModelTest, TestScaleObj) {
  std::string testFileName = "test/pyramid.txt";
  s21::Model model(testFileName);
  s21::Model default_model(testFileName);

  model.parser();
  default_model.parser();

  const auto& originalVertices = default_model.getData().getVertices();

  double scaleFactor = 2.0;
  model.scaleObj(scaleFactor);

  const auto& scaledVertices = model.getData().getVertices();

  ASSERT_EQ(originalVertices.size(), scaledVertices.size());

  for (size_t i = 0; i < originalVertices.size(); ++i) {
    for (int j = 0; j < originalVertices[i].amount_vert; ++j) {
      ASSERT_DOUBLE_EQ(scaledVertices[i].coords[j].x,
                       originalVertices[i].coords[j].x * scaleFactor);
      ASSERT_DOUBLE_EQ(scaledVertices[i].coords[j].y,
                       originalVertices[i].coords[j].y * scaleFactor);
      ASSERT_DOUBLE_EQ(scaledVertices[i].coords[j].z,
                       originalVertices[i].coords[j].z * scaleFactor);
    }

    ASSERT_DOUBLE_EQ(scaledVertices[i].minMaxX[0],
                     originalVertices[i].minMaxX[0] * scaleFactor);
    ASSERT_DOUBLE_EQ(scaledVertices[i].minMaxX[1],
                     originalVertices[i].minMaxX[1] * scaleFactor);
    ASSERT_DOUBLE_EQ(scaledVertices[i].minMaxY[0],
                     originalVertices[i].minMaxY[0] * scaleFactor);
    ASSERT_DOUBLE_EQ(scaledVertices[i].minMaxY[1],
                     originalVertices[i].minMaxY[1] * scaleFactor);
    ASSERT_DOUBLE_EQ(scaledVertices[i].minMaxZ[0],
                     originalVertices[i].minMaxZ[0] * scaleFactor);
    ASSERT_DOUBLE_EQ(scaledVertices[i].minMaxZ[1],
                     originalVertices[i].minMaxZ[1] * scaleFactor);
  }
}

TEST(ModelTest, TestMoveObj) {
  std::string testFileName = "test/pyramid.txt";
  s21::Model model;
  model.setFilePath(testFileName.c_str());
  s21::Model default_model;
  default_model.setFilePath(testFileName.c_str());

  model.parser();
  default_model.parser();

  const auto& originalVertices = default_model.getData().getVertices();

  double offsetX = 1.0, offsetY = 2.0, offsetZ = -1.0;
  double scaleFactor = (offsetZ > 0) ? OVER_ONE : UNDER_ONE;

  model.moveObj(offsetX, offsetY, offsetZ);

  const auto& movedVertices = model.getData().getVertices();

  ASSERT_EQ(originalVertices.size(), movedVertices.size());

  for (size_t i = 0; i < originalVertices.size(); ++i) {
    for (int j = 0; j < originalVertices[i].amount_vert; ++j) {
      ASSERT_DOUBLE_EQ(
          movedVertices[i].coords[j].x,
          (originalVertices[i].coords[j].x + offsetX) * scaleFactor);
      ASSERT_DOUBLE_EQ(
          movedVertices[i].coords[j].y,
          (originalVertices[i].coords[j].y + offsetY) * scaleFactor);
      ASSERT_DOUBLE_EQ(
          movedVertices[i].coords[j].z,
          (originalVertices[i].coords[j].z + offsetZ) * scaleFactor);
    }

    ASSERT_DOUBLE_EQ(movedVertices[i].minMaxX[0],
                     (originalVertices[i].minMaxX[0] + offsetX) * scaleFactor);
    ASSERT_DOUBLE_EQ(movedVertices[i].minMaxX[1],
                     (originalVertices[i].minMaxX[1] + offsetX) * scaleFactor);
    ASSERT_DOUBLE_EQ(movedVertices[i].minMaxY[0],
                     (originalVertices[i].minMaxY[0] + offsetY) * scaleFactor);
    ASSERT_DOUBLE_EQ(movedVertices[i].minMaxY[1],
                     (originalVertices[i].minMaxY[1] + offsetY) * scaleFactor);
    ASSERT_DOUBLE_EQ(movedVertices[i].minMaxZ[0],
                     (originalVertices[i].minMaxZ[0] + offsetZ) * scaleFactor);
    ASSERT_DOUBLE_EQ(movedVertices[i].minMaxZ[1],
                     (originalVertices[i].minMaxZ[1] + offsetZ) * scaleFactor);
  }
}

TEST(ModelTest, TestTurnX) {
  std::string testFileName = "./test/pyramid.txt";
  s21::Model model;
  model.setFilePath(testFileName.c_str());
  s21::Model default_model;
  default_model.setFilePath(testFileName.c_str());

  // Загружаем исходные данные
  model.parser();
  default_model.parser();

  // Создаём копию исходных вершин для сравнения
  auto originalVertices = default_model.getData().getVertices();

  // Угол поворота в градусах
  double angle = 45.0;
  // Переводим угол в радианы
  double rad = angle * M_PI / 180.0;

  // Поворачиваем объект вокруг оси X
  model.turnX(angle);

  // Получаем вершины после поворота
  const auto& turnedVertices = model.getData().getVertices();

  // Проверяем, что количество вершин не изменилось
  ASSERT_EQ(originalVertices.size(), turnedVertices.size());

  // Проверяем, что координаты вершин повернуты правильно
  for (size_t i = 0; i < originalVertices.size(); ++i) {
    for (int j = 0; j < originalVertices[i].amount_vert; ++j) {
      double originalY = originalVertices[i].coords[j].y;
      double originalZ = originalVertices[i].coords[j].z;

      // Ожидаемые координаты после поворота
      double expectedY = originalY * cos(rad) - originalZ * sin(rad);
      double expectedZ = originalY * sin(rad) + originalZ * cos(rad);

      // Проверяем, что координаты соответствуют ожидаемым
      ASSERT_NEAR(turnedVertices[i].coords[j].y, expectedY,
                  1e-6);  // Устанавливаем допустимую погрешность для сравнения
                          // вещественных чисел
      ASSERT_NEAR(turnedVertices[i].coords[j].z, expectedZ, 1e-6);
    }
  }
}

TEST(ModelTest, TestTurnY) {
  std::string testFileName = "./test/pyramid.txt";
  s21::Model model(testFileName);
  s21::Model default_model(testFileName);

  // Загружаем исходные данные
  model.parser();
  default_model.parser();

  // Создаем копию исходных вершин для сравнения
  auto originalVertices = default_model.getData().getVertices();

  // Угол поворота в градусах
  double angle = 45.0;
  // Переводим угол в радианы
  double rad = angle * M_PI / 180.0;

  // Поворачиваем объект вокруг оси Y
  model.turnY(angle);

  // Получаем вершины после поворота
  const auto& turnedVertices = model.getData().getVertices();

  // Проверяем, что количество вершин не изменилось
  ASSERT_EQ(originalVertices.size(), turnedVertices.size());

  // Проверяем, что координаты вершин повернуты правильно
  for (size_t i = 0; i < originalVertices.size(); ++i) {
    for (int j = 0; j < originalVertices[i].amount_vert; ++j) {
      double originalX = originalVertices[i].coords[j].x;
      double originalZ = originalVertices[i].coords[j].z;

      // Ожидаемые координаты после поворота
      double expectedX = originalX * cos(rad) + originalZ * sin(rad);
      double expectedZ = -originalX * sin(rad) + originalZ * cos(rad);

      // Проверяем, что координаты соответствуют ожидаемым
      ASSERT_NEAR(turnedVertices[i].coords[j].x, expectedX,
                  1e-6);  // Устанавливаем допустимую погрешность для сравнения
                          // вещественных чисел
      ASSERT_NEAR(turnedVertices[i].coords[j].z, expectedZ, 1e-6);
    }
  }
}

TEST(ModelTest, TestTurnZ) {
  std::string testFileName = "./test/pyramid.txt";
  s21::Model model(testFileName);
  s21::Model default_model(testFileName);

  // Загружаем исходные данные
  model.parser();
  default_model.parser();

  // Создаем копию исходных вершин для сравнения
  auto originalVertices = default_model.getData().getVertices();

  // Угол поворота в градусах
  double angle = 45.0;
  // Переводим угол в радианы
  double rad = angle * M_PI / 180.0;

  // Поворачиваем объект вокруг оси Z
  model.turnZ(angle);

  // Получаем вершины после поворота
  const auto& turnedVertices = model.getData().getVertices();

  // Проверяем, что количество вершин не изменилось
  ASSERT_EQ(originalVertices.size(), turnedVertices.size());

  // Проверяем, что координаты вершин повернуты правильно
  for (size_t i = 0; i < originalVertices.size(); ++i) {
    for (int j = 0; j < originalVertices[i].amount_vert; ++j) {
      double originalX = originalVertices[i].coords[j].x;
      double originalY = originalVertices[i].coords[j].y;

      // Ожидаемые координаты после поворота
      double expectedX = originalX * cos(rad) - originalY * sin(rad);
      double expectedY = originalX * sin(rad) + originalY * cos(rad);

      // Проверяем, что координаты соответствуют ожидаемым
      ASSERT_NEAR(turnedVertices[i].coords[j].x, expectedX,
                  1e-6);  // Устанавливаем допустимую погрешность для сравнения
                          // вещественных чисел
      ASSERT_NEAR(turnedVertices[i].coords[j].y, expectedY, 1e-6);
    }
  }
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
