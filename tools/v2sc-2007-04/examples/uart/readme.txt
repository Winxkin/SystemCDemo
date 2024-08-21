This folder contains Verilog source codes of UART obtained
from Mentor Graphics - FPGA Advantage. 

Use the following command to convert this example.

$v2sc address_decode.v clock_divider.v control_operation.v cpu_interface.v serial_interface.v status_registers.v tester.v uart_tb.v uart_top.v xmit_rcv_control.v -m uart_tb

The top module is "uart_tb" module and "-m" option is used for it.