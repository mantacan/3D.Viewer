#include "../3DViewer.h"

/**
 * 1) Придумать структуру хранения
 * 2) Парсер
 * 3) Трансформация:
 *  turnX(), turnY(), turnZ(): Вращение объекта вокруг осей X, Y и Z на заданный
 * угол. moveObj(): Перемещение объекта по трем осям и масштабирование при
 * приближении/отдалении. scaleObj(): Изменение масштаба объекта на заданный
 * коэффициент.
 */

namespace s21 {

Model::Model() { _filePath = "n"; }

Model::Model(std::string filePath) : _filePath(std::move(filePath)) {
  parser();
}

Model::Data& Model::getData() { return data; }

Model::~Model() = default;

void Model::setFilePath(const char* file) {
  _filePath = std::string(file);
  parser();
}

void Model::parser() {
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::string warn, err;

  std::cerr << "FILE: " << _filePath << std::endl;
  std::ifstream file_stream(_filePath);
  if (!file_stream) {
    std::cerr << "Cannot open file!" << std::endl;
    return;
  }

  tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, &file_stream);
  file_stream.close();

  if (!warn.empty()) {
    std::cout << "WARN: " << warn << std::endl;
  }
  if (!err.empty()) {
    std::cerr << "ERR: " << err << std::endl;
    return;
  }

  for (size_t v = 0; v < attrib.vertices.size(); v += 3) {
    double x = attrib.vertices[v];
    double y = attrib.vertices[v + 1];
    double z = attrib.vertices[v + 2];
    data.addVertex(Data::Vertex());
    data.vertices.back().addPoint(Data::Point(x, y, z));
  }

  for (const auto& shape : shapes) {
    for (const auto& index : shape.mesh.indices) {
      data.addPolygon(Data::Polygon(std::vector<int>{index.vertex_index}));
    }
  }

  std::cout << "Total vertices: " << data.getVertices().size() << std::endl;
  std::cout << "Total polygons: " << data.getPolygonCount() << std::endl;
}

void Model::printVertices() const {
  std::cout << "Vertex Coordinates and Bounds:" << std::endl;
  for (const auto& vertex : data.getVertices()) {
    for (int i = 0; i < vertex.amount_vert; i++) {
      std::cout << "Point: (" << vertex.coords[i].x << ", "
                << vertex.coords[i].y << ", " << vertex.coords[i].z << ")"
                << std::endl;
    }
    std::cout << "Min/Max X: [" << vertex.minMaxX[0] << ", "
              << vertex.minMaxX[1] << "]" << std::endl;
    std::cout << "Min/Max Y: [" << vertex.minMaxY[0] << ", "
              << vertex.minMaxY[1] << "]" << std::endl;
    std::cout << "Min/Max Z: [" << vertex.minMaxZ[0] << ", "
              << vertex.minMaxZ[1] << "]" << std::endl;
  }
}

void Model::printPolygons() const {
  std::cout << "Polygons (indices of vertices):" << std::endl;
  for (const auto& polygon : data.getPolygons()) {
    std::cout << "Indices: ";
    for (int index : polygon.indices) {
      std::cout << index + 1 << " ";
    }
    std::cout << std::endl;
  }
}
void Model::scaleObj(double scaleFactor) {
  for (auto& vertex : data.getVertices()) {
    for (int i = 0; i < vertex.amount_vert; i++) {
      vertex.coords[i].x *= scaleFactor;
      vertex.coords[i].y *= scaleFactor;
      vertex.coords[i].z *= scaleFactor;
    }
    vertex.minMaxX[0] *= scaleFactor;
    vertex.minMaxX[1] *= scaleFactor;
    vertex.minMaxY[0] *= scaleFactor;
    vertex.minMaxY[1] *= scaleFactor;
    vertex.minMaxZ[0] *= scaleFactor;
    vertex.minMaxZ[1] *= scaleFactor;
  }
  std::cout << "Изменение масштаба на зад. кофф: " << scaleFactor << std::endl;
}

void Model::moveObj(double offsetX, double offsetY, double offsetZ) {
  double scaleFactor = 1.0;
  if (offsetZ != 0) {
    scaleFactor = (offsetZ > 0) ? OVER_ONE : UNDER_ONE;
  }
  for (auto& vertex : data.getVertices()) {
    for (int i = 0; i < vertex.amount_vert; i++) {
      vertex.coords[i].x = (vertex.coords[i].x + offsetX) * scaleFactor;
      vertex.coords[i].y = (vertex.coords[i].y + offsetY) * scaleFactor;
      vertex.coords[i].z = (vertex.coords[i].z + offsetZ) * scaleFactor;
    }
    vertex.minMaxX[0] = (vertex.minMaxX[0] + offsetX) * scaleFactor;
    vertex.minMaxX[1] = (vertex.minMaxX[1] + offsetX) * scaleFactor;
    vertex.minMaxY[0] = (vertex.minMaxY[0] + offsetY) * scaleFactor;
    vertex.minMaxY[1] = (vertex.minMaxY[1] + offsetY) * scaleFactor;
    vertex.minMaxZ[0] = (vertex.minMaxZ[0] + offsetZ) * scaleFactor;
    vertex.minMaxZ[1] = (vertex.minMaxZ[1] + offsetZ) * scaleFactor;
  }
  std::cout << "Модель перемещается: " << offsetX << ", " << offsetY << ", "
            << offsetZ;
  std::cout << "Маштаб кофф: " << scaleFactor << std::endl;
}

void Model::turnX(double angle) {
  double rad = angle * M_PI / 180.0;
  for (auto& vertex : data.getVertices()) {
    for (int i = 0; i < vertex.amount_vert; i++) {
      double y = vertex.coords[i].y;
      double z = vertex.coords[i].z;
      vertex.coords[i].y = y * cos(rad) - z * sin(rad);
      vertex.coords[i].z = y * sin(rad) + z * cos(rad);
    }
  }
  std::cout << "Модель повернута вокруг оси X на " << angle << " градусов."
            << std::endl;
}

void Model::turnY(double angle) {
  double rad = angle * M_PI / 180.0;
  for (auto& vertex : data.getVertices()) {
    for (int i = 0; i < vertex.amount_vert; i++) {
      double x = vertex.coords[i].x;
      double z = vertex.coords[i].z;
      vertex.coords[i].x = x * cos(rad) + z * sin(rad);
      vertex.coords[i].z = -x * sin(rad) + z * cos(rad);
    }
  }
  std::cout << "Модель повернута вокруг оси Y на " << angle << " градусов."
            << std::endl;
}

void Model::turnZ(double angle) {
  double rad = angle * M_PI / 180.0;
  for (auto& vertex : data.getVertices()) {
    for (int i = 0; i < vertex.amount_vert; i++) {
      double x = vertex.coords[i].x;
      double y = vertex.coords[i].y;
      vertex.coords[i].x = x * cos(rad) - y * sin(rad);
      vertex.coords[i].y = x * sin(rad) + y * cos(rad);
    }
  }
  std::cout << "Модель повернута вокруг оси Z на " << angle << " градусов."
            << std::endl;
}
}  // namespace s21
