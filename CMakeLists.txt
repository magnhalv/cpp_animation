cmake_minimum_required(VERSION 3.10)

# set the project name
project(cpp_animation)

find_package(OpenGL REQUIRED)

FILE(GLOB SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/*)


# add the executable
add_executable(cpp_animation ${CMAKE_SOURCE_DIR}/src/WinMain.cpp ${SOURCE_FILES})

target_include_directories(cpp_animation PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        )

target_link_libraries(cpp_animation
        ${OPENGL_LIBRARY}
        )

# TODO: Find a way to detect windows 
set_property(TARGET cpp_animation PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/")

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
