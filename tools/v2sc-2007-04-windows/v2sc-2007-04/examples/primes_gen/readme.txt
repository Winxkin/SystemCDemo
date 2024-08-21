This is a prime generator obtained from Tenison Technology. Use the following command 
to convert.

$v2sc primes.v -m test_primes

This command generates the following files:

	primes_test_primes.h
	primes_PRIMERUN.h
	primes_PRIMEGEN.h
	primes_test_primes.cpp
	primes_PRIMERUN.cpp
	primes_PRIMEGEN.cpp
	primes_main.cpp

The latest file contains sc_main() function which is required for linking the files 
and generating SystemC executable simulation. Compile and link the above files and
see the generated primes.