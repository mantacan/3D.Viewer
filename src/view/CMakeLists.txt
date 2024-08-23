cmake_minimum_required(VERSION 3.5)

project(3DViewer VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Настройка поиска пакетов Qt5 и OpenGL
if(APPLE)
    find_package(OpenGL REQUIRED)
    find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets OpenGLWidgets Gui)
else()
    find_package(OpenGL REQUIRED)
    find_package(Qt5 COMPONENTS Widgets OpenGL REQUIRED)
endif()

set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_SOURCE_DIR})

set(PROJECT_SOURCES
    ../controller/controller.h
    ../controller/controller.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    ../model/cordinate_processing.cpp
    ../model/data.cpp
    myopenglwidget.h
    myopenglwidget.cpp
    ../3DViewer.h
    ../lib/tiny_obj_loader.cc
    ../lib/tiny_obj_loader.h
    QtGifImage/giflib/dgif_lib.c
    QtGifImage/giflib/egif_lib.c
    QtGifImage/giflib/gif_err.c
    QtGifImage/giflib/gif_font.c
    QtGifImage/giflib/gif_hash.c
    QtGifImage/giflib/gif_hash.h
    QtGifImage/giflib/gif_lib_private.h
    QtGifImage/giflib/gif_lib.h  
    QtGifImage/giflib/gifalloc.c
    QtGifImage/giflib/quantize.c
    QtGifImage/gifimage/qgifglobal.h
    QtGifImage/gifimage/qgifimage_p.h
    QtGifImage/gifimage/qgifimage.h
    QtGifImage/gifimage/qgifimage.cpp
)

set(SANITIZER_FLAGS "-fsanitize=address -g")
add_executable(3DViewer ${PROJECT_SOURCES})
target_compile_options(3DViewer PRIVATE -fsanitize=address -g)
target_link_libraries(3DViewer PRIVATE -fsanitize=address)

if(APPLE)
    target_link_libraries(3DViewer PRIVATE Qt${QT_VERSION_MAJOR}::OpenGL Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::OpenGLWidgets OpenGL::GL)
else()
    target_link_libraries(3DViewer PRIVATE ${OPENGL_LIBRARIES} Qt5::Widgets Qt5::OpenGL)
endif()

# Установка свойств для мак приложения
set_target_properties(3DViewer PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Это необходимо только для macOS
if(APPLE)
    set_target_properties(3DViewer PROPERTIES
        RESOURCE QtGifImage/gifimage/
    )
endif()

