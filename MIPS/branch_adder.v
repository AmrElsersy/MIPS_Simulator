module BR_ADDER(branchAdded, PC_Adder_output, Sign_Ext_Output);
	output reg [31:0] branchAdded;
	input  wire[31:0] PC_Adder_output;
	input  wire signed [31:0] Sign_Ext_Output; // signed since mover up or down
	always @(PC_Adder_output or Sign_Ext_Output)
	begin
		branchAdded <= PC_Adder_output + Sign_Ext_Output;
	end
endmodule
