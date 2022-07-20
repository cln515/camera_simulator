cmake_minimum_required (VERSION 3.10)

project(camera_simulator)

add_library(${PROJECT_NAME}  STATIC
	src/camera_simulator.cpp
)

add_executable(run_test
	src/run_test.cpp
)

include_directories(
	include
)

find_package(Eigen3 REQUIRED)
target_link_libraries(${PROJECT_NAME} Eigen3::Eigen)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}  ${OPENGL_LIBRARIES})


find_package(GLEW REQUIRED)
if (GLEW_FOUND)
	message(STATUS "GLEW ${GLEW_INCLUDE_DIR}")
    target_link_libraries(${PROJECT_NAME}  GLEW::GLEW)
endif()

find_package(OpenCV)

if(OpenCV_FOUND)
  include_directories(
      ${OpenCV_INCLUDE_DIRS}
    )
  target_link_libraries(run_test
    ${OpenCV_LIBS}
    )
endif(OpenCV_FOUND)
target_link_libraries(run_test ${PROJECT_NAME})

file(COPY opengl DESTINATION ${CMAKE_BINARY_DIR})

install(TARGETS ${PROJECT_NAME} 
	EXPORT camera-simulator_export
	LIBRARY ARCHIVE DESTINATION lib
	INCLUDES DESTINATION include 
)

install(FILES 
${CMAKE_CURRENT_SOURCE_DIR}/include/camera_simulator.h
DESTINATION include)

INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/opengl/ DESTINATION lib/opengl
          FILES_MATCHING PATTERN "*.frag"  PATTERN "*.vert" PATTERN "*.geom")

install(EXPORT camera-simulator_export
	FILE camera_simulator-config.cmake
	DESTINATION cmake
	EXPORT_LINK_INTERFACE_LIBRARIES
)
