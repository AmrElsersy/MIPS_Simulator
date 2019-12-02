module PC_ADDER(PC_Adder_output,PC);
	output reg[31:0] PC_Adder_output;
	input [31:0] PC;
	always @(PC)
	begin
		PC_Adder_output=PC+1;
	end
endmodule
