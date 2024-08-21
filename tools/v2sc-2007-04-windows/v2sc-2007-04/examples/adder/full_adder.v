
module full_adder(sum,cout,a,b,cin);
	
	input  a,b,cin;
	output sum,cout;

	assign {cout, sum} = a + b + cin;
	
endmodule
