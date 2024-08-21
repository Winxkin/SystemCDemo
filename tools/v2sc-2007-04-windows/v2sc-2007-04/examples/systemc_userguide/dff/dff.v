
module dff(din, clock, dout);
input din;
input clock;
output dout;
reg dout;

always @(posedge clock)
	dout <= din;

endmodule