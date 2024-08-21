This folder contains a classic light controller from Samir Palnitkar Verilog book.
This examples depicts v2sc points in event and delay handling.
Use the following command to convert the Verilog file into systemC:

$v2sc sig_ctrl.v -m stimulus

this command grenerates the following files:

	sig_ctrl_sig_control.h
	sig_ctrl_stimulus.h
	sig_ctrl_sig_control.cpp
	sig_ctrl_stimulus.cpp
	sig_ctrl_main.cpp

Compile and link the above files to see the simulation results.