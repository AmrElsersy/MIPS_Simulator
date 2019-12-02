module ALU_CONTROL(alu_ctrl, JR_Signal, alu_op, inst_5_0);
	
	output reg[3:0] alu_ctrl;	 //signal going to ALU can be modified to more bits when adding more instr 
	output reg JR_Signal;
	input wire[3:0] alu_op; 	//coming from control unit
	input wire[5:0] inst_5_0;  	//funct field in instr

	always @(*)  	//to make sure when any input change;
	begin                         	//operations are executed from begining;
		if(alu_op==4'b0000)	//sw or lw --> add or addi
		begin
			JR_Signal <= 1'b0;
			alu_ctrl <= 4'b0010; 
		end

		else if(alu_op==4'b0001) // sub for beq
		begin
			JR_Signal <= 1'b0;
			alu_ctrl <= 4'b0110;
		end
		
		else if(alu_op==4'b1000) // new sgti
		begin
			
			JR_Signal <= 1'b0;
			alu_ctrl <= 4'b0101;
		end

		else if(alu_op==4'b0011) // andi
		begin
			JR_Signal <= 1'b0;
			alu_ctrl <= 4'b0000;
		end

		else if(alu_op==4'b0100) // ori
		begin
			JR_Signal <= 1'b0;
			alu_ctrl <= 4'b0001;
		end

		else if(alu_op==4'b0101) // slti
		begin
			JR_Signal <= 1'b0;
			alu_ctrl <= 4'b0111;
		end

		else if(alu_op==4'b0110) // xori
		begin
			JR_Signal <= 1'b0;
			alu_ctrl <= 4'b0011;
		end

		else if(alu_op==4'b0111) // lui
		begin
			JR_Signal <= 1'b0;
			alu_ctrl <= 4'b1010;
		end

		else if(alu_op==4'b0010)   //Incase (010) indicate we have R_FORMATE inst; 
		begin                    //so we diffrentiate using funct field in instr;
			if(inst_5_0==6'b001000)
			begin
				alu_ctrl <= 4'b0000;
				JR_Signal <= 1'b1;
			end
			else
			begin
				JR_Signal <= 1'b0;
				case (inst_5_0)
					6'b000000:alu_ctrl <= 4'b0100;  //sll
					6'b000001:alu_ctrl <= 4'b0101;	//sgt
					6'b000010:alu_ctrl <= 4'b1000;  //srl
					6'b000011:alu_ctrl <= 4'b1001;  //sra
					6'b100000:alu_ctrl <= 4'b0010;  //add
					6'b100010:alu_ctrl <= 4'b0110;  //subtract
					6'b100100:alu_ctrl <= 4'b0000;  //AND
					6'b100101:alu_ctrl <= 4'b0001;  //OR
					6'b101010:alu_ctrl <= 4'b0111;  //slt 
					6'b100110:alu_ctrl <= 4'b0011;  //xor
					6'b100111:alu_ctrl <= 4'b1100;  //nor
					default  :alu_ctrl <= 4'b0000; 
				endcase
			end
		end
		else
		begin
			alu_ctrl <= 4'b0000;
			JR_Signal <= 1'b0;
		end
	end
endmodule

