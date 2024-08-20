set(SOCPLATFORM_SRC
    main.cpp
)

add_executable(SocPlatform ${SOCPLATFORM_SRC})
target_link_libraries(SocPlatform common Adder Counter Uart)
target_include_directories(SocPlatform PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})