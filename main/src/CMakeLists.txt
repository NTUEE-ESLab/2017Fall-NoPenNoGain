cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH "/usr/local/lib/cmake/${CMAKE_MODULE_PATH}")

project(penGo)

find_package(OpenCV REQUIRED)
find_package(raspicam REQUIRED)
find_package(Threads REQUIRED)

#add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
SET(SRC_LIST main.cpp
	trackCam_boost.cpp)

ADD_EXECUTABLE(${PROJECT_NAME} ${SRC_LIST})

#add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
target_link_libraries(${PROJECT_NAME} ${raspicam_LIBS})
target_link_libraries(${PROJECT_NAME} ${raspicam_CV_LIBS})
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
