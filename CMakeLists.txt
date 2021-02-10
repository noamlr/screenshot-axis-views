cmake_minimum_required(VERSION 3.17 FATAL_ERROR)

set (CMAKE_AUTOMOC ON)

project(screenshot-axis-views)

# set(MITK_SUPERBUILD_DIR "/home/noa/MITK-build")
set(MITK_SUPERBUILD_DIR "$ENV{MITK_BUILD_PATH}")
# set(QT_DIR "/home/noa")
set(QT_DIR "$ENV{QT_ROOT_PATH}")

set(CMAKE_PREFIX_PATH
		"${MITK_SUPERBUILD_DIR}/MITK-build"
		"${MITK_SUPERBUILD_DIR}/ep/src/VTK/CMake/"
		"${QT_DIR}/gcc_64/lib/cmake/Qt5"
		)

find_package(MITK CONFIG REQUIRED)
find_package(OpenMP REQUIRED)

#BEGIN: Add for time delay
#find_package(OPENGL REQUIRED)
#find_package(OpenGL REQUIRED COMPONENTS OpenGL EGL GLX)
#include_directories(${OPENGL_INCLUDE_DIRS})

#message("OpenGL include dirs" )
#message("${OPENGL_INCLUDE_DIR}")
#message("EGL include dirs" )
#message("${OPENGL_EGL_INCLUDE_DIRS}")
#END: Add for time delay

set(cpp_files
  ScreenshotUtilities.cpp
  ScreenshotUtilitiesMain.cpp
	)

set(header_files
  ScreenshotUtilities.h
	)

add_executable(${PROJECT_NAME} ${cpp_files})
target_link_libraries(  ${PROJECT_NAME}
                        MitkCore 
												MitkSceneSerializationBase
												MitkQtWidgetsExt
												EGL
											)
