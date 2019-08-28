cmake_minimum_required(VERSION 3.1)
set (CMAKE_CXX_STANDARD 14)
project(NES-Emulator-Test)

file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/../src/*.cpp" "${PROJECT_SOURCE_DIR}/src/*.cpp")
list(FILTER SOURCES EXCLUDE REGEX "${PROJECT_SOURCE_DIR}/\.\./src/main\.cpp")
file(GLOB_RECURSE HEADERS "${PROJECT_SOURCE_DIR}/../src/*.h" "${PROJECT_SOURCE_DIR}/src/*.h")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

add_executable(NES-Emulator-Test ${SOURCES} ${HEADERS})
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT NES-Emulator-Test)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
	set(CMAKE_CXX_FLAGS_DEBUG "-g")
	set(CMAKE_CXX_FLAGS_RELEASE "-O3")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-trigraphs")
endif()

target_compile_definitions(NES-Emulator-Test PRIVATE NES_DISABLE_AUDIO)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/../cmake")

# If using vcpkg
if(CMAKE_TOOLCHAIN_FILE)

	find_package(OpenGL REQUIRED)
	find_package(GLEW CONFIG REQUIRED)
	find_package(glfw3 CONFIG REQUIRED)
	find_package(SDL2 CONFIG REQUIRED)
	target_link_libraries(
		NES-Emulator-Test
		PRIVATE
		GLEW::GLEW
		glfw
		SDL2::SDL2
		SDL2::SDL2main
		)

else(CMAKE_TOOLCHAIN_FILE)

	find_package(OpenGL REQUIRED)
	foreach(file_path ${OPENGL_LIBRARIES})
		message(STATUS ${file_path})
	endforeach()
	foreach(file_path ${OPENGL_INCLUDE_DIRS})
		message(STATUS ${file_path})
	endforeach()
	find_package(GLEW REQUIRED)
	foreach(file_path ${GLEW_LIBRARIES})
		message(STATUS ${file_path})
	endforeach()
	foreach(file_path ${GLEW_INCLUDE_DIRS})
		message(STATUS ${file_path})
	endforeach()
	find_package(GLFW REQUIRED)
	foreach(file_path ${GLFW_LIBRARY})
		message(STATUS ${file_path})
	endforeach()
	foreach(file_path ${GLFW_INCLUDE_DIR})
		message(STATUS ${file_path})
	endforeach()
	find_package(SDL2 REQUIRED)
	foreach(file_path ${SDL2_LIBRARY})
		message(STATUS ${file_path})
	endforeach()
	foreach(file_path ${SDL2_INCLUDE_DIR})
		message(STATUS ${file_path})
	endforeach()

	include_directories(
		${OPENGL_INCLUDE_DIRS}
		${GLEW_INCLUDE_DIRS}
		${GLFW_INCLUDE_DIR}
		${SDL2_INCLUDE_DIR}
		)
	target_link_libraries(
		NES-Emulator-Test
		${OPENGL_LIBRARIES}
		${OPENGL_gl_LIBRARY}
		${GLEW_LIBRARIES}
		${GLFW_LIBRARY}
		${SDL2_LIBRARY}
		)

endif(CMAKE_TOOLCHAIN_FILE)
