cmake_minimum_required(VERSION 3.1)
set (CMAKE_CXX_STANDARD 14)
project(NES-Emulator)

file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_executable(NES-Emulator ${SOURCES})

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-trigraphs")
endif()

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLFW REQUIRED)
foreach(file_path ${GLEW_LIBRARIES})
	message(${file_path})
endforeach()
foreach(file_path ${GLEW_INCLUDE_DIRS})
	message(${file_path})
endforeach()
foreach(file_path ${GLFW_LIBRARY})
	message(${file_path})
endforeach()
foreach(file_path ${GLFW_INCLUDE_DIR})
	message(${file_path})
endforeach()

include_directories(
	${OPENGL_INCLUDE_DIRS}
	${GLEW_INCLUDE_DIRS}
	${GLFW_INCLUDE_DIR}
	)
target_link_libraries(
	NES-Emulator 
	${OPENGL_LIBRARIES}
	${OPENGL_gl_LIBRARY}
	${GLEW_LIBRARIES}
	${GLFW_LIBRARY}
	)

configure_file(
	${PROJECT_SOURCE_DIR}/src/ntscpalette.pal
	${PROJECT_BINARY_DIR}/ntscpalette.pal
	COPYONLY
	)
configure_file(
	${PROJECT_SOURCE_DIR}/src/GLScene2D_fs.glsl
	${PROJECT_BINARY_DIR}/GLScene2D_fs.glsl
	COPYONLY
	)
configure_file(
	${PROJECT_SOURCE_DIR}/src/GLScene2D_vs.glsl
	${PROJECT_BINARY_DIR}/GLScene2D_vs.glsl
	COPYONLY
	)
