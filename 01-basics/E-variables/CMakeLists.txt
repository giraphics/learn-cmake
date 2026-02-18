cmake_minimum_required(VERSION 3.16)

project(hello_variables VERSION 1.2.3)

# --- Normal Variables ---
set(MY_VARIABLE "Hello")
message(STATUS "MY_VARIABLE = ${MY_VARIABLE}")

# --- List Variables ---
set(SOURCE_FILES src/main.cpp)
message(STATUS "SOURCE_FILES = ${SOURCE_FILES}")

# --- Cache Variables ---
set(USE_FEATURE ON CACHE BOOL "Enable the special feature")
message(STATUS "USE_FEATURE = ${USE_FEATURE}")

# --- Built-in Variables ---
message(STATUS "CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")
message(STATUS "CMAKE_BINARY_DIR = ${CMAKE_BINARY_DIR}")
message(STATUS "PROJECT_NAME = ${PROJECT_NAME}")
message(STATUS "PROJECT_VERSION = ${PROJECT_VERSION}")
message(STATUS "CMAKE_SYSTEM_NAME = ${CMAKE_SYSTEM_NAME}")

# --- Environment Variables ---
message(STATUS "PATH = $ENV{PATH}")

add_executable(hello_variables ${SOURCE_FILES})
