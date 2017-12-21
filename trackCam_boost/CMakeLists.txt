cmake_minimum_required(VERSION 3.7)

set(CMAKE_MODULE_PATH "/usr/local/lib/cmake/${CMAKE_MODULE_PATH}")

project(trackCam)

find_package(OpenCV REQUIRED)
find_package(raspicam REQUIRED)
find_package(Threads REQUIRED)

#IF (OpenCV_FOUND AND raspicam_CV_FOUND)
#MESSAGE(STATUS "COMPILING OPENCV")

# Build a shared library named after the project from the files in `src/`"
file(GLOB SOURCE_FILES "src/*.cpp" "src/*.h")
#add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
SET(SRC_LIST src/main_boost.cpp
	src/trackCam_boost.cpp)
ADD_EXECUTABLE(${PROJECT_NAME} ${SRC_LIST})
#add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
target_link_libraries(${PROJECT_NAME} ${raspicam_LIBS})
target_link_libraries(${PROJECT_NAME} ${raspicam_CV_LIBS})
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
