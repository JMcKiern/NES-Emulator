cmake_minimum_required(VERSION 2.6)
project(NES-Emulator)

file(GLOB SOURCES "*.cpp" "Display/*.cpp")

add_executable(NES-Emulator ${SOURCES})

include_directories(../NES\ Emulator)
include_directories(../NES\ Emulator/Display)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
#find_package (PkgConfig REQUIRED)
#pkg_search_module(GLFW REQUIRED glfw3)
find_package(GLEW REQUIRED)

include_directories(
	${OPENGL_INCLUDE_DIRS}
	${GLFW_INCLUDE_DIRS}
	${GLEW_INCLUDE_DIRS}
	)
target_link_libraries(
	NES-Emulator 
	${OPENGL_gl_LIBRARY}
	${OPENGL_LIBRARIES}
	${GLFW_LIBRARIES}
	${GLFW_STATIC_LIBRARIES}
	${GLEW_LIBRARIES}
	glfw
	glfw3
	GLEW
	)
