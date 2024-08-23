/**
 * @file s21_3D_viewer_main.h
 * @brief This file contains the declaration of the Model class and its
 * associated Data, Vertex, Point, and Polygon structures.
 */

#ifndef S21_3D_VIEWER_MAIN_H_
#define S21_3D_VIEWER_MAIN_H_

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <utility>
#include <vector>

#include "lib/tiny_obj_loader.h"
#define OVER_ONE 1.05
#define UNDER_ONE 0.95

namespace s21 {

/**
 * @class Model
 * @brief A class that represents a 3D model.
 */
class Model {
 private:
  std::string _filePath; /**< The file path to the 3D model. */

 public:
  /**
   * @class Data
   * @brief A nested class that contains the data of the model, including
   * vertices and polygons.
   */
  class Data {
   public:
    /**
     * @struct Point
     * @brief A structure that represents a 3D point.
     */
    struct Point {
      double x; /**< The x-coordinate of the point. */
      double y; /**< The y-coordinate of the point. */
      double z; /**< The z-coordinate of the point. */

      /**
       * @brief Constructor for Point.
       * @param x The x-coordinate.
       * @param y The y-coordinate.
       * @param z The z-coordinate.
       */
      explicit Point(double x = 0, double y = 0, double z = 0);
    };

    /**
     * @struct Polygon
     * @brief A structure that represents a polygon made of vertex indices.
     */
    struct Polygon {
      std::vector<int>
          indices; /**< The indices of the vertices that form the polygon. */

      /**
       * @brief Constructor for Polygon.
       * @param indices The indices of the vertices.
       */
      explicit Polygon(const std::vector<int>& indices);
    };

    /**
     * @struct Vertex
     * @brief A structure that represents a vertex with coordinates and bounding
     * box limits.
     */
    struct Vertex {
      int amount_vert; /**< The number of vertices. */
      Point* coords;   /**< The array of points (coordinates). */
      double minMaxX[2] = {
          std::numeric_limits<double>::max(),
          std::numeric_limits<double>::lowest()}; /**< The minimum and maximum
                                                     x-coordinates. */
      double minMaxY[2] = {
          std::numeric_limits<double>::max(),
          std::numeric_limits<double>::lowest()}; /**< The minimum and maximum
                                                     y-coordinates. */
      double minMaxZ[2] = {
          std::numeric_limits<double>::max(),
          std::numeric_limits<double>::lowest()}; /**< The minimum and maximum
                                                     z-coordinates. */

      /**
       * @brief Constructor for Vertex.
       */
      Vertex() : amount_vert(0), coords(nullptr) {}

      /**
       * @brief Destructor for Vertex.
       */
      ~Vertex() { delete[] coords; }

      /**
       * @brief Copy constructor for Vertex.
       * @param other The vertex to copy from.
       */
      Vertex(const Vertex& other)
          : amount_vert(other.amount_vert),
            coords(new Point[other.amount_vert]) {
        std::copy(other.coords, other.coords + amount_vert, coords);
        std::copy(std::begin(other.minMaxX), std::end(other.minMaxX), minMaxX);
        std::copy(std::begin(other.minMaxY), std::end(other.minMaxY), minMaxY);
        std::copy(std::begin(other.minMaxZ), std::end(other.minMaxZ), minMaxZ);
      }

      /**
       * @brief Copy assignment operator for Vertex.
       * @param other The vertex to copy from.
       * @return Reference to the assigned vertex.
       */
      Vertex& operator=(const Vertex& other) {
        if (this != &other) {
          Point* newCoords = new Point[other.amount_vert];
          std::copy(other.coords, other.coords + other.amount_vert, newCoords);
          delete[] coords;
          coords = newCoords;
          amount_vert = other.amount_vert;
          std::copy(std::begin(other.minMaxX), std::end(other.minMaxX),
                    minMaxX);
          std::copy(std::begin(other.minMaxY), std::end(other.minMaxY),
                    minMaxY);
          std::copy(std::begin(other.minMaxZ), std::end(other.minMaxZ),
                    minMaxZ);
        }
        return *this;
      }

      /**
       * @brief Move constructor for Vertex.
       * @param other The vertex to move from.
       */
      Vertex(Vertex&& other) noexcept
          : amount_vert(other.amount_vert), coords(other.coords) {
        other.coords = nullptr;
        other.amount_vert = 0;
      }

      /**
       * @brief Adds a point to the vertex.
       * @param point The point to add.
       */
      void addPoint(const Point& point);
    };

    /**
     * @brief Copy assignment operator for Data.
     * @param other The data to copy from.
     * @return Reference to the assigned data.
     */
    Data& operator=(const Data& other) {
      if (this != &other) {
        vertices = other.vertices;
        polygons = other.polygons;
      }
      return *this;
    }

    std::vector<Vertex> vertices;  /**< The vertices of the model. */
    std::vector<Polygon> polygons; /**< The polygons of the model. */

    /**
     * @brief Adds a vertex to the data.
     * @param vertex The vertex to add.
     */
    void addVertex(const Vertex& vertex);

    /**
     * @brief Adds a polygon to the data.
     * @param polygon The polygon to add.
     */
    void addPolygon(const Polygon& polygon);

    /**
     * @brief Gets the vertices of the model.
     * @return A const reference to the vector of vertices.
     */
    const std::vector<Vertex>& getVertices() const;

    /**
     * @brief Gets the polygons of the model.
     * @return A const reference to the vector of polygons.
     */
    const std::vector<Polygon>& getPolygons() const;

    /**
     * @brief Gets the number of polygons in the model.
     * @return The number of polygons.
     */
    size_t getPolygonCount() const;

    /**
     * @brief Gets the vertices of the model.
     * @return A reference to the vector of vertices.
     */
    std::vector<Model::Data::Vertex>& getVertices();
  };

 public:
  Data data; /**< The data of the model. */

  /**
   * @brief Default constructor for Model.
   */
  Model();

  /**
   * @brief Constructor for Model with file path.
   * @param filePath The path to the model file.
   */
  explicit Model(std::string filePath);

  /**
   * @brief Destructor for Model.
   */
  ~Model();

  /**
   * @brief Sets the file path to the model.
   * @param file The file path.
   */
  void setFilePath(const char* file);

  /**
   * @brief Parses the model file.
   */
  void parser();

  /**
   * @brief Prints the vertices of the model.
   */
  void printVertices() const;

  /**
   * @brief Prints the polygons of the model.
   */
  void printPolygons() const;

  /**
   * @brief Scales the model by a factor.
   * @param scaleFactor The scale factor.
   */
  void scaleObj(double scaleFactor);

  /**
   * @brief Moves the model by offsets.
   * @param offsetX The x offset.
   * @param offsetY The y offset.
   * @param offsetZ The z offset.
   */
  void moveObj(double offsetX, double offsetY, double offsetZ);

  /**
   * @brief Rotates the model around the x-axis.
   * @param angle The angle in degrees.
   */
  void turnX(double angle);

  /**
   * @brief Rotates the model around the y-axis.
   * @param angle The angle in degrees.
   */
  void turnY(double angle);

  /**
   * @brief Rotates the model around the z-axis.
   * @param angle The angle in degrees.
   */
  void turnZ(double angle);

  /**
   * @brief Gets the data of the model.
   * @return A reference to the data.
   */
  Data& getData();
};

}  // namespace s21

#endif  // S21_3D_VIEWER_MAIN_H_
