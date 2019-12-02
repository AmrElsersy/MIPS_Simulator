module Reg_Dst_MUX(Write_Register,in0,in1,RegDst);
	input  wire [4:0] in0,in1;
	input  wire [1:0] RegDst;
	output reg  [4:0] Write_Register;
	reg [4:0] ra;
	initial 
	begin
		ra = 5'd31;
	end
	always@(*)
	begin 
		if (RegDst == 1)
		begin
			Write_Register <= in1;
		end

		else if (RegDst == 2)
		begin
			Write_Register <= ra;
		end

		else 
		begin
			Write_Register <= in0;
		end
	end
endmodule
module MUX_32_1(output_mux,input0,input1,selector);
	output reg  [31:0] output_mux;
	input  wire [31:0] input0;
	input  wire [31:0] input1;
	input  wire selector;
	always@(*)
	begin 
		if (selector == 1)
		begin
			output_mux <= input1;
		end
		else
		begin
			output_mux <= input0;
		end
	
	end
endmodule
module MUX_32_2(output_mux,input0,input1,input2,selector);

	input  wire [31:0] input0;
	input  wire [31:0] input1;
	input  wire [31:0] input2;
	input wire  [1:0] selector;
	output reg  [31:0] output_mux;
	
	always@(*)
	begin 
		if (selector == 1)
		begin
			output_mux <= input1;
		end
	
		else if (selector == 2)
		begin
			output_mux <= input2;
		end

		else 
		begin
			output_mux <= input0;
		end
end

endmodule
