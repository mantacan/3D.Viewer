Data parse_file(const char *filename, Data *data3d) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    fprintf(stderr, "No such file");
  } else {
    fclose(file);
    *data3d = parse_counting(filename, data3d);
    *data3d = parse_values(filename, data3d);
    *data3d = find_minMax_points(data3d);
    *data3d = center_values(data3d);
    *data3d = scale(data3d, 0.7);
  }
  return *data3d;
}
//  Функция очистки памяти
void freeAllData(Data *data3d) {
  free(data3d->v->coord);
  free(data3d->v);
  for (int i = 0; i < data3d->amount_polygon; i++) {
    free(data3d->p[i].p);
  }
  free(data3d->p);
  free(data3d);
}

//посчитать количество вершин, записать координаты, найти Мин| Макс по каждой
//координате

Data parse_counting(const char *filename, Data *data3d) {
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  int count_vertex = 0, count_polygon = 0;
  FILE *file = fopen(filename, "r");
  while ((read = getline(&line, &len, file)) != -1) {
    if (line[0] == 'v' && line[1] == ' ')
      count_vertex++;
    else if (line[0] == 'f' && line[1] == ' ')
      count_polygon++;
  }
  points *coords = malloc(sizeof(points) * count_vertex);
  data3d->v->coord = coords;
  data3d->v->amount_vert = count_vertex;

  polygon *p = malloc(sizeof(polygon) * count_polygon);
  data3d->p = p;
  data3d->amount_polygon = count_polygon;
  free(line);
  fclose(file);
  return *data3d;
}

Data parse_values(const char *filename, Data *data3d) {
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  int count_vertex = 0, count_polygon = 0;
  FILE *file = fopen(filename, "r");
  while ((read = getline(&line, &len, file)) != -1) {
    if (line[0] == 'v' && line[1] == ' ') {
      read_point_value(data3d, line, count_vertex);
      count_vertex++;
    } else if (line[0] == 'f' && line[1] == ' ') {
      read_polygon_value(data3d, line, len, count_polygon, count_vertex);
      count_polygon++;
    }
  }
  free(line);
  fclose(file);
  return *data3d;
}

Data read_point_value(Data *data3d, char *line, int num_of_coord) {
  int counter = 0;
  int sign = 1;

  for (int i = 0; line[i] != '\0'; i++) {
    if (line[i] == '-') {
      sign = -1;
    } else if ((line[i] >= '0' && line[i] <= '9') || line[i] == '.' ||
               line[i] == ',' || line[i] == 'E' || line[i] == 'e') {
      double value = 0;
      bool dec_num = false;
      int cnt = 0;

      while ((line[i] >= '0' && line[i] <= '9') || line[i] == '.' ||
             line[i] == ',' || line[i] == 'E' || line[i] == 'e' ||
             line[i] == '+' || line[i] == '-') {
        if (line[i] == ',' || line[i] == '.') {
          dec_num = true;
        } else if (line[i] == 'E' || line[i] == 'e') {
          // Handling exponential notation
          i++;
          int exp_sign = 1;
          if (line[i] == '+' || line[i] == '-') {
            exp_sign = (line[i] == '-') ? -1 : 1;
            i++;
          }
          int exponent = 0;
          while (line[i] >= '0' && line[i] <= '9') {
            exponent = exponent * 10 + (line[i] - '0');
            i++;
          }
          value *= pow(10, exp_sign * exponent);
          break;
        } else {
          value = value * 10 + (line[i] - '0');
          if (dec_num) {
            cnt++;
          }
        }
        i++;
      }

      if (dec_num) {
        value /= pow(10, cnt);
      }

      i--;

      if (sign == -1) {
        value *= -1;
        sign = 1;
      }

      if (counter == 0) {
        data3d->v->coord[num_of_coord].x = value;
        counter++;
      } else if (counter == 1) {
        data3d->v->coord[num_of_coord].y = value;
        counter++;
      } else {
        data3d->v->coord[num_of_coord].z = value;
      }
    }
  }

  return *data3d;
}

Data read_polygon_value(Data *data3d, char *line, int len, int num_of_polygon,
                        int num_of_vertex) {
  int counter = 0, sign = 1;
  int polygon_values[100];
  for (int i = 0; i < len && line[i] != '\n'; i++) {
    //Условие для обработки таких полигонов f 1/2/2 2/3/3 3/1/1
    if (line[i - 1] != ' ' && line[i - 1] != '-') continue;
    if (line[i] == '-') {
      sign = 21;
    } else if ((line[i] >= '0' && line[i] <= '9')) {
      double value = 0;
      while ((line[i] >= '0' && line[i] <= '9')) {
        value = value * 10 + (line[i] - '0');
        i++;
      }
      i--;
      if (sign == 21) {
        value = num_of_vertex - (int)value + 1;
      }
      if (counter == 0) {
        polygon_values[0] = (int)value;
        counter++;
      } else if (polygon_values[counter - 1] != (int)value) {
        polygon_values[counter] = (int)value;
        counter++;
      }
    }
  }

  data3d->p[num_of_polygon].amount_p = counter;
  int *p = malloc(sizeof(int) * counter);
  data3d->p[num_of_polygon].p = p;
  for (int i = 0; i < counter; i++) {
    data3d->p[num_of_polygon].p[i] = polygon_values[i];
  }
  // free(p);
  return *data3d;
}

Data find_minMax_points(Data *data3d) {
  double minx = data3d->v->coord[0].x, maxx = data3d->v->coord[0].x,
         miny = data3d->v->coord[0].y, maxy = data3d->v->coord[0].y,
         maxz = data3d->v->coord[0].z, minz = data3d->v->coord[0].z;

  for (int i = 0; i < data3d->v->amount_vert; i++) {
    if (data3d->v->coord[i].x < minx) minx = data3d->v->coord[i].x;
    if (data3d->v->coord[i].x > maxx) maxx = data3d->v->coord[i].x;
    if (data3d->v->coord[i].y < miny) miny = data3d->v->coord[i].y;
    if (data3d->v->coord[i].y > maxy) maxy = data3d->v->coord[i].y;
    if (data3d->v->coord[i].z < minz) minz = data3d->v->coord[i].z;
    if (data3d->v->coord[i].z > maxz) maxz = data3d->v->coord[i].z;
  }
  data3d->v->minMaxX[0] = minx;
  data3d->v->minMaxX[1] = maxx;
  data3d->v->minMaxY[0] = miny;
  data3d->v->minMaxY[1] = maxy;
  data3d->v->minMaxZ[0] = minz;
  data3d->v->minMaxZ[1] = maxz;
  return *data3d;
}

Data center_values(Data *data3d) {
  double centerX = data3d->v->minMaxX[0] +
                   (data3d->v->minMaxX[1] - data3d->v->minMaxX[0]) / 2;
  double centerY = data3d->v->minMaxY[0] +
                   (data3d->v->minMaxY[1] - data3d->v->minMaxY[0]) / 2;
  double centerZ = data3d->v->minMaxZ[0] +
                   (data3d->v->minMaxZ[1] - data3d->v->minMaxZ[0]) / 2;

  for (int i = 0; i < data3d->v->amount_vert; i++) {
    data3d->v->coord[i].x -= centerX;
    data3d->v->coord[i].y -= centerY;
    data3d->v->coord[i].z -= centerZ;
  }
  return *data3d;
}

Data scale(Data *data3d, double scale) {
  double dx, dy, dz, dmax;
  Data *data_copy = data3d;
  dx = data3d->v->minMaxX[1] - data3d->v->minMaxX[0];
  dy = data3d->v->minMaxY[1] - data3d->v->minMaxY[0];
  dz = data3d->v->minMaxZ[1] - data3d->v->minMaxZ[0];
  dmax = fmax(dx, fmax(dy, dz));
  scale = (scale - (scale * (-1))) / dmax;
  for (int i = 0; i < data3d->v->amount_vert; i++) {
    data_copy->v->coord[i].x *= scale;
    data_copy->v->coord[i].y *= scale;
    data_copy->v->coord[i].z *= scale;
  }
  return *data_copy;
}

Data data_preparation(const char *filename) {
  Data *data3d = malloc(sizeof(Data));
  data3d->v = malloc(sizeof(vertex));
  *data3d = parse_file(filename, data3d);
  return *data3d;
}

Data turnX(Data *data3d, double angle) {
  double degree = (angle * M_PI) / 180.0;
  double copy_dots[2] = {};

  for (int i = 0; i < data3d->v->amount_vert; i++) {
    copy_dots[0] = data3d->v->coord[i].z;
    copy_dots[1] = data3d->v->coord[i].y;

    data3d->v->coord[i].z =
        cos(degree) * copy_dots[0] + sin(degree) * copy_dots[1];
    data3d->v->coord[i].y =
        -sin(degree) * copy_dots[0] + cos(degree) * copy_dots[1];
  }

  return *data3d;
}

Data turnY(Data *data3d, double angle) {
  double degree = (angle * M_PI) / 180.0;
  double copy_dots[2] = {};

  for (int i = 0; i < data3d->v->amount_vert; i++) {
    copy_dots[0] = data3d->v->coord[i].x;
    copy_dots[1] = data3d->v->coord[i].z;

    data3d->v->coord[i].x =
        cos(degree) * copy_dots[0] + sin(degree) * copy_dots[1];
    data3d->v->coord[i].z =
        -sin(degree) * copy_dots[0] + cos(degree) * copy_dots[1];
  }

  return *data3d;
}

Data turnZ(Data *data3d, double angle) {
  double degree = (angle * M_PI) / 180.0;
  double copy_dots[2] = {};

  for (int i = 0; i < data3d->v->amount_vert; i++) {
    copy_dots[0] = data3d->v->coord[i].x;
    copy_dots[1] = data3d->v->coord[i].y;

    data3d->v->coord[i].x =
        cos(degree) * copy_dots[0] + sin(degree) * copy_dots[1];
    data3d->v->coord[i].y =
        -sin(degree) * copy_dots[0] + cos(degree) * copy_dots[1];
  }

  return *data3d;
}

Data moveObj(Data *data3d, double offsetX, double offsetY, double offsetZ) {
  for (int i = 0; i < data3d->v->amount_vert; i++) {
    data3d->v->coord[i].x += offsetX / 1000;
    data3d->v->coord[i].y += offsetY / 1000;
    data3d->v->coord[i].z += offsetZ / 1000;
    if (offsetX == 0 && offsetY == 0 && offsetZ < 0) {
      data3d->v->coord[i].x *= 0.95;
      data3d->v->coord[i].y *= 0.95;
    } else if (offsetX == 0 && offsetY == 0 && offsetZ > 0) {
      data3d->v->coord[i].x *= 1.05;
      data3d->v->coord[i].y *= 1.05;
    }
  }
  return *data3d;
}

Data scaleObj(Data *data3d, double scale) {
  for (int i = 0; i < data3d->v->amount_vert; i++) {
    data3d->v->coord[i].x *= scale;
    data3d->v->coord[i].y *= scale;
    data3d->v->coord[i].z *= scale;
  }
  return *data3d;
}
