# Guard against multiple inclusion
if (NOT DEFINED SYSTEMC_FOUND)
    set(SYSTEMC_FOUND FALSE)
endif()

# Set the path to the SystemC source directory
set(SYSTEMC_SOURCE_DIR "${CMAKE_SOURCE_DIR}/systemc")

# Check if the SystemC source directory exists
if (EXISTS "${SYSTEMC_SOURCE_DIR}")
    
    # Add the SystemC source directory as a subdirectory
    add_subdirectory(${SYSTEMC_SOURCE_DIR})
    
    # Include the SystemC include directory
    include_directories(${SYSTEMC_SOURCE_DIR}/src)
    
    # Set the SystemC library to the built target
    set(SYSTEMC_LIBRARIES systemc)
    
    # Mark SystemC as found
    set(SYSTEMC_FOUND TRUE)
    
    message(STATUS "SystemC found and included.")
else()
    message(FATAL_ERROR "SystemC source directory not found.")
endif()

# Export the SystemC variables to the parent scope
set(SYSTEMC_FOUND ${SYSTEMC_FOUND} PARENT_SCOPE)
set(SYSTEMC_LIBRARIES ${SYSTEMC_LIBRARIES} PARENT_SCOPE)