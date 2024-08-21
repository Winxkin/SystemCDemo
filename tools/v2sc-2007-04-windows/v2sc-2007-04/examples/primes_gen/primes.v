 
//
// primes.v - simple primes generator
//
// (C) Tenison Technology, 2002.
//

module test_primes;
	reg clk, reset;
	wire [31:0] count;
	wire stop, pass;
	
	PRIMERUN u(clk, reset, count, stop, pass);
	
	initial
	begin
		reset = 1;
		clk = 0;
		#10;
		reset = 0;
	end
	
	always #5
		clk = ~clk;

	initial
	begin
		#200000;
		$stop;
	end		
	
endmodule

//
// A wrapper/converter that operates with 
// the generic runmain.cpp top-level program.
//
module PRIMERUN(clk, reset, count, stop, pass);

  input clk, reset;
  input [31:0] count;
  output stop, pass;
  
  parameter topbit = 7;

  wire [topbit:0] out; // a prime number
  wire guard;           // prime number provided
  wire hs = guard;      // handshake output value - always immediate

  PRIMEGEN primegen(clk, reset, out, guard, hs);

  always @(posedge clk)
  begin
    if (guard) begin
      $display(out);
      end
  end

  assign stop = guard && out > 100;
  assign pass = 1;

endmodule

//
// The primes generator
//
module PRIMEGEN(clk, reset, out, guard, hs);

  parameter topbit = 7; // `TOPBIT;

  input clk, reset;
  output [topbit:0] out;
  output guard;
  input hs;


  reg [7:0] static2 [32767:0];	// synthesis map_to SSRAM/SSRAM32x8

  reg [topbit:0] p, q;

  reg [1:0] phase;
  reg guard;
  reg [topbit:0] out;
  reg pc;
  reg [7:0] hold;

    always @(posedge clk) begin

	if (reset) begin
		pc <= 0;
		phase = 0;
		p = 0;
	 	q = 0;
		guard <= 0;
		end

	// Clear the array
	else if (phase == 0) begin
		static2 [p[topbit:3]] <= 0;
		p <= p + 8;
		if (p[topbit]) begin
			phase <= phase + 1;
			p <= 4;
			q <= 2;
			end
		end

	// Cross out multiples
	else if (phase == 1) begin
		pc <= ~pc;
		if (~pc) hold <=  static2[p[topbit:3]]; 
		else begin
			static2[p[topbit:3]] <= hold | (1 << p[2:0]);
			if (q[topbit]) begin
				phase <= phase + 1;
				p <= 0;
				end

			else if (p[topbit]) begin
				q <= q + 1;
				p <= 2 * (q+1);
				// $display("Phase %d %d\n", phase, q);
				end
			else p <= p + q;		  
			end	
		end

	// Print out results
	else if (phase == 2) begin
		if (p[topbit]) begin
			$display("All done at %t\n", $time);
			$finish;
			end

 		if (hs) begin
			guard <= 0;
			p <= p + 1;
			end
		else begin
			if ((static2[p[topbit:3]] & (1 << p[2:0])) == 0) begin
				guard <= 1;
				out <= p;
				end
			else p <= p + 1;
		 	end
		end
	end

endmodule
