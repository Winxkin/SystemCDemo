set(COMMON_SRC
    DMAC.h
    DummyMaster.h
    DummySlave.h
    Inverter.h
    Registerif.h
    bus.h
    memory.h
    target.h
    core/RISCV32/decode_inst.h
    core/RISCV32/instruction.h
    core/RISCV32/memory.h
    core/RISCV32/registerbank.h
    core/RISCV32/riscv32.h
)

add_library(common ${COMMON_SRC})
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})