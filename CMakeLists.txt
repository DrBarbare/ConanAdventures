cmake_minimum_required(VERSION 3.12)
project(ConanAdventures CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(${CMAKE_SOURCE_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(ConanAdventures src/main.cpp)
target_link_libraries(ConanAdventures ${CONAN_LIBS})
