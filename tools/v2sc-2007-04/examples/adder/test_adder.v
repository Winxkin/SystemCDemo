
module test_adder;
	reg  [3:0] a, b;
	reg cin;
	wire [3:0] sum;
	wire cout;
	
	four_bit_adder top(sum, cout, a, b, cin);
	
	initial
	begin
		a = 0;
		b = 0;
		cin = 0;
		#5;
		a = 4;
		b = 8;
		cin = 0;
		#5;
		a = 6;
		b = 3;
		cin = 1;
		#5;
		a = 5;
		b = 12;
		cin = 0;
		#5;
		a = 8;
		b = 8;
		cin = 0;
		#5;
		a = 9;
		b = 1;
		cin = 1;
		#5;
		$stop;
	end
	
endmodule
