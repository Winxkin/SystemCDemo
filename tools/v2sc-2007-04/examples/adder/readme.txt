This is a simple adder. This example demostartes the hierarchical conversion of
v2sc. Use the following command to convert this adder.

$v2sc full_adder.v adder.v test_adder.v -m test_adder

This command generates the following files:

	full_adder_full_adder.h
	adder_four_bit_adder.h
	test_adder_test_adder.h
	full_adder_full_adder.cpp
	adder_four_bit_adder.cpp
	test_adder_test_adder.cpp
	test_adder_main.cpp

The latest file contains sc_main() function which is required for linking the files 
and generating SystemC executable simulation. Compile and link the above files and
see the results.