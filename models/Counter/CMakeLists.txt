set(COUNTER_SRC
    src/counter_counter.cpp
)

set(COUNTER_INC
    Counter_wrapper.h
    inc/counter_counter.h
)

add_library(Counter ${COUNTER_SRC} ${COUNTER_INC})
target_include_directories(Counter PUBLIC inc)
target_link_libraries(Counter common)