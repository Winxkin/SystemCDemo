set(SOCPLATFORM_SRC
    IO.h
    top.h
)

add_library(SocPlatform ${SOCPLATFORM_SRC})
target_link_libraries(SocPlatform common Adder Counter Uart)
target_include_directories(SocPlatform PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(SocPlatform PROPERTIES LINKER_LANGUAGE CXX)