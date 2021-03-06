cmake_minimum_required(VERSION 3.8)

project(papiwrapper LANGUAGES CXX)


# Compile options
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fopenmp -march=native")

# configure optimization
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(OPTIMIZATION_FLAGS "-g -O0 -DDEBUG")
    message("-- Configuring debug build")
else()
    set(OPTIMIZATION_FLAGS "-O3 -DNDEBUG")
    message("-- Configuring release build")
endif()

# Collect outputs in a single bin directory.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Enable release mode by default (if nothing else is specified).
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Release' as none was specified.")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
endif()

#Modules
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")
find_package(PAPI)

if(PAPI_FOUND)
    message (STATUS "Including Papi Directories")
    include_directories(${PAPI_INCLUDE_DIRS})
else()
    message (STATUS "Defining NOPAPIW in order to disable PAPIW functions" )
    add_compile_definitions(NOPAPIW)
endif(PAPI_FOUND)

# Define Variables
SET(EXECUTABLE_NAME "example/example.cpp")

# Build
ADD_EXECUTABLE(papiw_example ${EXECUTABLE_NAME})

# Target libraries
target_include_directories(papiw_example INTERFACE include/) 
if(PAPI_FOUND)
    message (STATUS "Linking to Papi Libraries")
    target_link_libraries(papiw_example ${PAPI_LIBRARIES})
endif(PAPI_FOUND)



