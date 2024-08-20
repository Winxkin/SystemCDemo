cmake_minimum_required(VERSION 3.15)
project(SoCProject)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find SystemC
find_package(SystemCLanguage REQUIRED)

# Add subdirectories
add_subdirectory(common)
add_subdirectory(models)
add_subdirectory(SocPlatform)
add_subdirectory(Test)
add_subdirectory(main)