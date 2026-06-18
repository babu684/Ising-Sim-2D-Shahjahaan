cmake_minimum_required(VERSION 3.15)

project(Standard2DIsing)

set(SFML_DIR "C:/Users/Shahjahaan/SFML-3.1.0/lib/cmake/SFML")

find_package(SFML 3.1 REQUIRED COMPONENTS Graphics Window System)

add_executable(IsingApp Ising_2d_Classic.cpp)

target_link_libraries(IsingApp PRIVATE SFML::Graphics SFML::Window SFML::System)