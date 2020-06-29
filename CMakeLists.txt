cmake_minimum_required(VERSION 3.1)

project(foxlogicgates)

set(CMAKE_CXX_STANDARD 11)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

# Compiler Options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -lpugixml")

include_directories( /usr/include/fox-1.6 )

add_executable(foxlogicgates
	MainWindow.h
	MainWindow.cpp
	main.cpp
	Gate.cpp
	Gate.h
	icons.h
	Object.h
)
target_link_libraries(foxlogicgates FOX-1.6)
