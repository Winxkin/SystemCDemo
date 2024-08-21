
module counter(clock, load, clear, din, dout);
input clock, load, clear;
input [0:7] din;
output [0:7] dout;
wire [0:7] dout;

reg [0:7] countval;

assign dout = countval;

always @(posedge clock)
begin
	if (clear)
		countval = 0;
	else if (load)
		countval = din;
	else
		countval = countval + 1;
end

endmodule