module JUMP_ADDRESS(J_address,Inst_25_0,pc_31_26);
	input wire[25:0] Inst_25_0;
	input wire[5:0] pc_31_26;
	output reg[31:0] J_address;

	always @(Inst_25_0)
	begin
		J_address <= {pc_31_26,Inst_25_0};
	end
endmodule
