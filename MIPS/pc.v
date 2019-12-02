module PC(PC_plus, PC, clock, eof);
	integer file ,size, _ ;
	input wire clock;
	input wire [31:0] PC;
	output reg[31:0] PC_plus;
	output reg eof;
	
	initial 
	begin
		PC_plus = 32'd0;
		eof = 0 ;
	// ========================= for Size Calculation ============================
	size = 0;
	file = $fopen("ins.txt","r");
	while (! $feof (file) )
	begin
	_ = $fscanf (file,"%h",_);
	size = size +1;
	end
	$display ("SIZE === %d ",size);
	// ============================================================================
	end

	always @(posedge clock)
	begin
		PC_plus = PC;
		if( PC > size )
		begin
		$display ("end_program will save the memory data and exit the verilog program ");
		eof = 1 ;
		end
	end
endmodule


