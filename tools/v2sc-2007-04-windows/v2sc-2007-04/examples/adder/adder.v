
module four_bit_adder(sum, cout, a, b, cin);
	input  [3:0] a, b;
	input  cin;
	output [3:0] sum;
	output cout;

	wire   s1,s2,s3;
	
	full_adder m0(sum[0],s1,  a[0],b[0],cin);
	full_adder m1(sum[1],s2,  a[1],b[1],s1);
	full_adder m2(sum[2],s3,  a[2],b[2],s2);
	full_adder m3(sum[3],cout,a[3],b[3],s3);	
	
endmodule
