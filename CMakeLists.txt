cmake_minimum_required(VERSION 3.1)
set (CMAKE_CXX_STANDARD 14)
project(NES-Emulator)

file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADERS "${PROJECT_SOURCE_DIR}/src/*.h")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

add_executable(NES-Emulator ${SOURCES} ${HEADERS})
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT NES-Emulator)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
	set(CMAKE_CXX_FLAGS_DEBUG "-g")
	set(CMAKE_CXX_FLAGS_RELEASE "-O3")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-trigraphs")
endif()
if(MSVC)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
endif()

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

find_package(OpenGL REQUIRED)
foreach(file_path ${OPENGL_LIBRARIES})
	message(${file_path})
endforeach()
foreach(file_path ${OPENGL_INCLUDE_DIRS})
	message(${file_path})
endforeach()
find_package(GLEW REQUIRED)
foreach(file_path ${GLEW_LIBRARIES})
	message(${file_path})
endforeach()
foreach(file_path ${GLEW_INCLUDE_DIRS})
	message(${file_path})
endforeach()
find_package(GLFW REQUIRED)
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
