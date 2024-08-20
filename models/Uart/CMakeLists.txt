set(UART_SRC
    src/address_decode_address_decode.cpp
    src/clock_divider_clock_divider.cpp
    src/control_operation_control_operation.cpp
    src/cpu_interface_cpu_interface.cpp
    src/serial_interface_serial_interface.cpp
    src/status_registers_status_registers.cpp
    src/uart_top_uart_top.cpp
    src/xmit_rcv_control_xmit_rcv_control.cpp
)

set(UART_INC
    Uart_wrapper.h
    inc/address_decode_address_decode.h
    inc/clock_divider_clock_divider.h
    inc/control_operation_control_operation.h
    inc/cpu_interface_cpu_interface.h
    inc/serial_interface_serial_interface.h
    inc/status_registers_status_registers.h
    inc/uart_top_uart_top.h
    inc/xmit_rcv_control_xmit_rcv_control.h
)

add_library(Uart ${UART_SRC} ${UART_INC})
target_include_directories(Uart PUBLIC inc)
target_link_libraries(Uart common)