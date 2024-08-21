
module dffa(clock, reset, din, dout);
input clock, reset, din;
output dout;
reg dout;

always @(posedge clock or reset)
begin
	if (reset)
		dout <= 1'b0;
	else
		dout = din;
end

endmodule