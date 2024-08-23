#include "../3DViewer.h"

namespace s21 {

Model::Data::Point::Point(double x, double y, double z) : x(x), y(y), z(z) {}
Model::Data::Polygon::Polygon(const std::vector<int>& indices)
    : indices(indices) {}

void Model::Data::Vertex::addPoint(const Point& point) {
  Point* newCoords = new Point[amount_vert + 1];
  if (coords) {
    std::copy(coords, coords + amount_vert, newCoords);
    delete[] coords;
  }
  newCoords[amount_vert] = point;
  coords = newCoords;
  amount_vert++;

  minMaxX[0] = std::min(minMaxX[0], point.x);
  minMaxX[1] = std::max(minMaxX[1], point.x);
  minMaxY[0] = std::min(minMaxY[0], point.y);
  minMaxY[1] = std::max(minMaxY[1], point.y);
  minMaxZ[0] = std::min(minMaxZ[0], point.z);
  minMaxZ[1] = std::max(minMaxZ[1], point.z);
}

size_t Model::Data::getPolygonCount() const { return polygons.size(); }

void Model::Data::addVertex(const Vertex& vertex) {
  vertices.push_back(vertex);
}

void Model::Data::addPolygon(const Polygon& polygon) {
  polygons.push_back(polygon);
}

const std::vector<Model::Data::Vertex>& Model::Data::getVertices() const {
  return vertices;
}

const std::vector<Model::Data::Polygon>& Model::Data::getPolygons() const {
  return polygons;
}

std::vector<Model::Data::Vertex>& Model::Data::getVertices() {
  return vertices;
}

}  // namespace s21
