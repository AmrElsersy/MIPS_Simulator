module INS_MEMORY(instruction, clk, pc);
	output reg[31:0] instruction;
	input  wire[31:0] pc;
	input  wire clk;
	reg[31:0] Imem[0:8191]; // 32KB memory ehich is 8192 register each one is 32bit 
	initial 
	begin 
		$readmemh("ins.txt",Imem);
	end
	always @(negedge clk )
	begin 
		
		instruction <= Imem[pc]; 
	end
endmodule
