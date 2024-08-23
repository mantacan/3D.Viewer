cmake_minimum_required(VERSION 3.27)
project(CPP4_3DViewer_v2_0_1)

set(CMAKE_CXX_STANDARD 14)

include_directories(src/_example/3Dviewer/src/backend)
include_directories(src/_example/3Dviewer/src/backend/tests)
include_directories(src/_example/3Dviewer/src/frontend/3DViewer)
include_directories(src/_example/3Dviewer/src/frontend/3DViewer/QtGifImage/gifimage)
include_directories(src/_example/3Dviewer/src/frontend/3DViewer/QtGifImage/giflib)
include_directories(src/lib)
include_directories(src/model)

add_executable(CPP4_3DViewer_v2_0_1
        src/3DViewer.h
        src/model/data.cpp
        src/model/main.cpp
        src/model/cordinate_processing.cpp)
