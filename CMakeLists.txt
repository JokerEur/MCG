cmake_minimum_required(VERSION 3.8)

project(nurbs)

set(CMAKE_CXX_STANDARD 20)

if(APPLE)
 add_definitions(-Wno-deprecated-declarations)
endif()

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

if(UNIX AND NOT APPLE)
 find_package(GLEW REQUIRED)
endif()

set(INCLUDE_DIRS ${OPENGL_INCLUDE_DIR} ${GLUT_INCLUDE_DIR})
set(LIBRARIES ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})

include_directories(${INCLUDE_DIRS})

file(GLOB SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB INCLUDE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${INCLUDE_FILES})

target_link_libraries(${PROJECT_NAME} ${LIBRARIES})