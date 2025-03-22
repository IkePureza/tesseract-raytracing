cmake_minimum_required(VERSION 3.10)

project(RayTracer)

# Set C++ standard (C++11 or higher recommended for ray tracing)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add precompiled header support
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-include rtweekend.h)
endif()

# Add the executable
add_executable(raytracer main.cpp)

# If you eventually add implementation files (like vec3.cpp), add them here
# add_executable(raytracer main.cpp vec3.cpp color.cpp)

# Include directories (not strictly necessary in this case since 
# your header files are in the same directory, but good practice)
target_include_directories(raytracer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
