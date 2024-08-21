
module ram(addr, enable, readwr, data);
input [0:7] addr;
input enable, readwr;
inout [0:15] data;

reg [0:15] ramdata [0:255];

assign data = (enable & !readwr) ? ramdata[addr] : 16'bz;

always @(addr or enable or readwr or data)
begin
	if (enable & readwr)
		ramdata[addr] = data;
end

endmodule