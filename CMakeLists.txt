cmake_minimum_required(VERSION 3.17 FATAL_ERROR)

set (CMAKE_AUTOMOC ON)

project(screenshotUtilities)

set(MITK_SUPERBUILD_DIR "/home/noa/MITK-build")
set(QT_DIR "/home/noa")

set(CMAKE_PREFIX_PATH
		"${MITK_SUPERBUILD_DIR}/MITK-build"
		"${MITK_SUPERBUILD_DIR}/ep/src/VTK/CMake/"
		"${QT_DIR}/Qt5.12.8/5.12.8/gcc_64/lib/cmake/Qt5"
		)

find_package(MITK CONFIG REQUIRED)
find_package(OpenMP REQUIRED)

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
												MitkQtWidgetsExt
											)
