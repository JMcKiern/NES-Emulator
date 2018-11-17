cmake_minimum_required(VERSION 2.6)
project(NES-Emulator)

file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")
#foreach(file_path ${SOURCES})
#	message(${file_path})
#endforeach()

add_executable(NES-Emulator ${SOURCES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-trigraphs")

include_directories(src/)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories(
	${OPENGL_INCLUDE_DIRS}
	${GLEW_INCLUDE_DIRS}
	)
target_link_libraries(
	NES-Emulator 
	${OPENGL_LIBRARIES}
	${GLEW_LIBRARIES}
	glfw
	GLEW
	)
