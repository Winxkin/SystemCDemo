
module shift(din, clk, load, LR, dout);
input [0:7] din;
input clk, load, LR;
output [0:7] dout;
wire [0:7] dout;

reg [0:7] shiftval;

assign dout = shiftval;

always @(posedge clk)
begin
	if (load)
		shiftval = din;
	else if (LR)
	begin
		shiftval[0:6] = shiftval[1:7];
		shiftval[7] = 1'b0;
	end
	else
	begin
		shiftval[1:7] = shiftval[0:6];
		shiftval[0] = 1'b0;
	end
end

endmodule