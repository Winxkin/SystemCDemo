cmake_minimum_required(VERSION 3.15)
project(SoCProject)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include the FindSystemC.cmake file
include(FindSystemC.cmake)

if (NOT SYSTEMC_FOUND)
    message(FATAL_ERROR "SystemC not found. Please check the source directory.")
endif()

set(MAIN_SRC
    main/main.cpp
)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/models)
# Find SystemC
find_package(SystemCLanguage REQUIRED)

# Add subdirectories
add_subdirectory(common)
add_subdirectory(models)
add_subdirectory(SocPlatform)
add_subdirectory(Test)


# Add the executable target
add_executable(SoCProject ${MAIN_SRC})

# Link the SystemC library to your executable
target_link_libraries(SoCProject ${SYSTEMC_LIBRARIES} common SocPlatform Test)