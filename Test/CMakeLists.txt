set(TEST_SRC
    Register.h
    TestList.h
    01_Bus/tp_01_01.h
    02_Dmac/tp_02_01.h
    02_Dmac/tp_02_02.h
    03_Adder/tp_03_01.h
    04_Counter/tp_04_01.h
    05_Uart/tp_05_01.h
    06_core/riscv32_inst_mix.h
    06_core/tp_06_01.h
    06_core/tp_06_02.h
)

add_library(Test ${TEST_SRC})
target_include_directories(Test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Test SocPlatform)