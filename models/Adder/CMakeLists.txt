set(ADDER_SRC
    src/full_adder_full_adder.cpp
)

set(ADDER_INC
    Adder_wrapper.h
    inc/adder_four_bit_adder.h
    inc/full_adder_full_adder.h
)

add_library(Adder ${ADDER_SRC} ${ADDER_INC})
target_include_directories(Adder PUBLIC inc)
target_link_libraries(Adder common)
set_target_properties(Adder PROPERTIES LINKER_LANGUAGE CXX)