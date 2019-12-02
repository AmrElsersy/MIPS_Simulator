`timescale 1ns / 1ns
module CLOCK(Clock);
	output reg Clock;
	initial
	begin
		Clock=0;
	end
	always
	begin
		#31.25
		Clock=~(Clock);
	end
endmodule
