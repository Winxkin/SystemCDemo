set(MAIN_SRC
    main.cpp
)

add_executable(SoCMain ${MAIN_SRC})
target_link_libraries(SoCMain Test)
target_include_directories(SoCMain PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})