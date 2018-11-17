cmake_minimum_required(VERSION 2.6)
project(NES-Emulator)

file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADERS "${CMAKE_SOURCE_DIR}/src/*.h")

add_executable(NES-Emulator ${SOURCES} ${HEADERS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-trigraphs")

include_directories(src/)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
foreach(file_path ${GLEW_LIBRARIES})
	message(${file_path})
endforeach()
foreach(file_path ${GLEW_INCLUDE_DIRS})
	message(${file_path})
endforeach()

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

configure_file(
	${CMAKE_SOURCE_DIR}/src/ntscpalette.pal
	${CMAKE_SOURCE_DIR}/build/ntscpalette.pal
	COPYONLY
	)
configure_file(
	${CMAKE_SOURCE_DIR}/src/GLScene2D_fs.glsl
	${CMAKE_SOURCE_DIR}/build/GLScene2D_fs.glsl
	COPYONLY
	)
configure_file(
	${CMAKE_SOURCE_DIR}/src/GLScene2D_vs.glsl
	${CMAKE_SOURCE_DIR}/build/GLScene2D_vs.glsl
	COPYONLY
	)
