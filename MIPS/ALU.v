module ALU(alu_result, zero, inst_10_6, read_data_1, alu_mux_output, alu_ctrl);
	
	input  wire [4:0] inst_10_6;
	input  wire [3:0] alu_ctrl;
	input  wire signed [31:0] read_data_1;
	input  wire signed [31:0] alu_mux_output;
	
	output reg  signed [31:0] alu_result;
	output reg  zero;
	
	localparam AND  = 4'b0000;
	localparam OR   = 4'b0001;
	localparam ADD  = 4'b0010;
	localparam XOR  = 4'b0011;
	localparam SLL  = 4'b0100;
	localparam SGT  = 4'b0101; //new
	localparam SUB  = 4'b0110;
	localparam SLT  = 4'b0111;
	localparam SRL  = 4'b1000;
	localparam SRA  = 4'b1001;
	localparam LUI  = 4'b1010;
	localparam NOR  = 4'b1100;
	//localparam LB = 4'b1011;
	//localparam LH = 4'b1101;
	//localparam SW = 4'b1110;
	//localparam SH = 4'b1111;
	//localparam  	= 4'b0101; 

	initial 
	begin 
		zero =  0 ; 
	end	
	
	always @(*)
	begin 
		case(alu_ctrl)
			
			AND: 
			begin
				alu_result <=  read_data_1 &  alu_mux_output ;
				zero = (read_data_1 &  alu_mux_output) ? 0: 1;
			end
			
			OR :
			begin
				alu_result <=  read_data_1 |  alu_mux_output ;
				zero = ( read_data_1 |  alu_mux_output )? 0: 1;
			end
			
			ADD:
			begin
				alu_result <=  read_data_1 +  alu_mux_output ;
				zero <= (read_data_1 +  alu_mux_output )? 1'b0: 1'b1;
			end
			
			SUB: 
			begin
				alu_result <=  (read_data_1 -  alu_mux_output) ;
				zero <= (read_data_1 -  alu_mux_output)? 1'b0: 1'b1;
			end
			
			SLT: 
			begin
				alu_result <=  (read_data_1 < alu_mux_output);
				zero <= (read_data_1 != alu_mux_output)? 1'b0: 1'b1;
			end

			SGT: 
			begin
				alu_result <=  (read_data_1 > alu_mux_output);
				zero <= (read_data_1 != alu_mux_output)? 1'b0: 1'b1;
			end
			
			NOR: 
			begin
				alu_result <= ~(read_data_1 | alu_mux_output);
				zero <= (~(read_data_1 | alu_mux_output))? 1'b0: 1'b1;
			end
			
			XOR:
			begin
				alu_result <=   read_data_1 ^ alu_mux_output ;
				zero <= (read_data_1 ^ alu_mux_output )? 1'b0: 1'b1;
			end
			
			SLL:
			begin
				alu_result <=   alu_mux_output << inst_10_6  ;
				zero <= (alu_mux_output << inst_10_6 )? 1'b0: 1'b1;
			end
			
			SRL:
			begin
				alu_result <=   alu_mux_output >> inst_10_6  ;
				zero <= (alu_mux_output >> inst_10_6)? 1'b0: 1'b1;
			end
			
			SRA:
			begin
				alu_result <=   alu_mux_output >>>inst_10_6  ;
				zero <= (alu_mux_output >>>inst_10_6 )? 1'b0: 1'b1;
			end
			
			LUI: 
			begin
				alu_result <=   alu_mux_output << 16  ;
				zero <= (alu_mux_output << 16)? 1'b0: 1'b1;
			end
			
			default : 
			begin
				alu_result <= 0;
				zero <= 0;
			end
		endcase
	end
endmodule
