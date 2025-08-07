cmake_minimum_required(VERSION 3.10)
project(MagicVault)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(MagicVault src/MagicVault.cpp)
target_include_directories(MagicVault PUBLIC include)
