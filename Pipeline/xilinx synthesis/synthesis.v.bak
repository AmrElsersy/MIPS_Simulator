module PC( PC_plus , PC , clock , hold_pc, reset);
	input  wire clock , hold_pc, reset; 
	input  wire [12:0] PC;
	output reg[12:0] PC_plus;
	always @(posedge clock)
begin
	if(reset)
		PC_plus <= 13'd0;
	else
	begin
		if(hold_pc == 0)//if no stall pass the pc+4;	
		begin
			PC_plus <= PC;  //if hold =1 do nothing;
		end
	end
end
endmodule

module PC_ADDER(PC_Adder_output,PC);
	output reg[12:0] PC_Adder_output;
	input [12:0] PC;
	always @(*)
	begin
			PC_Adder_output=PC+1'b1;
	end
endmodule

module INS_MEMORY(instruction, clk, pc, reset, mips);
	output reg[31:0] instruction;
	input  wire[12:0] pc;
	input wire[31:0]mips;
	input  wire clk, reset;
	reg[31:0] Imem[0:8191]; // 32KB memory ehich is 8192 register each one is 32bit 
	always @(negedge clk )
	begin 
		if(reset)
			Imem[pc] <= mips;
		else
			instruction <= Imem[pc];
	end
endmodule

module IF_ID_Reg(instrOut , PCplus4Out , instrIn , PCplus4 , clk , hold , IF_Flush );

	input wire [31:0] instrIn;
	input wire [12:0] PCplus4 ;
	input wire        clk , hold , IF_Flush ;
	output reg [31:0] instrOut;
	output reg [12:0] PCplus4Out ;
	always @(posedge clk)
	begin
		if ( hold ==1'b0 && IF_Flush==1'b1) 
        	begin
          		PCplus4Out <= 13'b0 ;
					instrOut   <= 32'b0 ;
      		end
		else if (hold==1'b0 && IF_Flush==1'b0) 
        	begin
          
      			PCplus4Out <= PCplus4 ;   // da5l el instr ely wa2fa mestnya tetketb 3and el pc out 
      			instrOut   <= instrIn ;   //w eb3t el instr ely kanet feha ely mawgoda fel lines ely metwsla bel ID
      	end
    end
endmodule

module REG_FILE(Read_Data_1, Read_Data_2, Read_Reg_1, Read_Reg_2, Write_Reg, Write_Data, Reg_Write, Clock);

	input wire[4:0] Read_Reg_1, Read_Reg_2, Write_Reg;
	input wire[31:0] Write_Data;
	input Reg_Write, Clock;
	output reg[31:0] Read_Data_1, Read_Data_2;
	reg signed[31:0]  Reg_File[0:31];


	always @(*)
	begin
		Read_Data_1 <= Reg_File[Read_Reg_1];
		Read_Data_2 <= Reg_File[Read_Reg_2];
	end
	
	always @(negedge Clock)
	begin
		if(Reg_Write)
		begin
			Reg_File[Write_Reg] <= Write_Data;
		end		
	end

endmodule 

module Comparator( Zero , read_data1 , read_data2 );

input [31:0]  read_data1 ;
input [31:0]  read_data2 ;
output reg Zero ;

always @(*)
begin
Zero <= ( read_data1 == read_data2 ) ? 1'b1 : 1'b0;
end
endmodule

module SIGN_EXTEND(Sign_Ext_Output, Ori, Inst_15_0);
	input wire[15:0] Inst_15_0;
	input wire Ori;
	output reg signed[31:0] Sign_Ext_Output;

	always @(*)
	begin
		if (Ori == 1)
			Sign_Ext_Output <= Inst_15_0 | 32'h00000000;
		else if(Inst_15_0[15] == 1)
			Sign_Ext_Output <= Inst_15_0 | 32'hffff0000;
		else if (Inst_15_0[15] == 0)
			Sign_Ext_Output <= Inst_15_0 | 32'h00000000;
		else
			Sign_Ext_Output <=32'd0;
	end
endmodule

module HazardDetectionUnit( ID_ExMemRead ,  ID_Ex_RegDst , IF_ID_Instr , holdPC , holdIF_ID , IF_ID_Flush , Branch_And_Output ,muxSelector , Jump );
	output reg holdPC , holdIF_ID , muxSelector , IF_ID_Flush ;
	input wire[4:0] ID_Ex_RegDst ;
 	input wire[15:0] IF_ID_Instr ;
   input wire ID_ExMemRead , Branch_And_Output ,Jump;
	
	reg flag;
	initial
	begin
	flag <= 0;
	end
	always@(*)
	begin
		holdPC <= 0;
		holdIF_ID <= 0;
		muxSelector <= 0;
		IF_ID_Flush <=0;
	if(flag == 1)
	begin
			holdPC<=1;
         holdIF_ID<=1;
         muxSelector<=1;
			flag<=0;
	end
	else
	begin
		if ( ( IF_ID_Instr [15:11] != 6'b000100 || IF_ID_Instr [15:11] != 6'b000101  )&& ID_ExMemRead && ( ID_Ex_RegDst == IF_ID_Instr[10:5] || ID_Ex_RegDst == IF_ID_Instr[4:0]) )  // lw + ay 7aga 8ir beq -> stall
		begin
						holdPC<=1;
             		holdIF_ID<=1;
              		muxSelector<=1;
						flag<=0;
        	end
      		else if( ( IF_ID_Instr [15:11] == 6'b000100 || IF_ID_Instr [15:11] ==  6'b000101 ) && ( ID_Ex_RegDst == IF_ID_Instr[10:5] || ID_Ex_RegDst == IF_ID_Instr[4:0]) ) // ay 7aga 8ir lw + beq -> 1 stall
        	begin
						holdPC<=1;
             		holdIF_ID<=1;
              		muxSelector<=1;
						flag<=0;
        	end
		else if(( IF_ID_Instr [15:11] == 6'b000100 || IF_ID_Instr [15:11] ==  6'b000101 ) && ID_ExMemRead && ( ID_Ex_RegDst == IF_ID_Instr[10:5] || ID_Ex_RegDst == IF_ID_Instr[4:0]) )//stalling0 twice //first stall
        	begin                                                                                                                    		 //lw +beq
          		
						holdPC<=1;
             		holdIF_ID<=1;
              		muxSelector<=1;
						flag<=1;
        	end
			else
			begin
			holdPC <= 0;
			holdIF_ID <= 0;
		muxSelector <= 0;
		IF_ID_Flush <=0;
		flag <= 0;
end			
        end                 
		/*========================= Set Flush When Found Branch or Jump ================*/
		if( ( ( IF_ID_Instr [15:11] == 6'b000100 || IF_ID_Instr [15:11] ==  6'b000101 ) &&  Branch_And_Output ) || Jump  ) 
		begin
			IF_ID_Flush <= 1'b1; //ha2ool lel instruction ely fel IF_ID ely 7aslha fetch w da5lt mesh fakrak yaaad //w nerza3ha ka7ka	
		end  
		else 
		begin
			IF_ID_Flush <= 1'b0; 
		end      
		/*===============================================================================*/
    	end
endmodule
module CONTROL( JR_Signal , Ori , Reg_Dst , Branch , Branch_Not_Equal, Mem_Read ,Mem_to_Reg , ALU_Op , Mem_Write , ALU_Src , Reg_Write , Jump, Inst_31_26, Inst_5_0 );
	output reg  Ori,Branch,Branch_Not_Equal,Mem_Read,Mem_Write,ALU_Src,Reg_Write,Jump , Reg_Dst , Mem_to_Reg ,JR_Signal ;
	output reg  [3:0] ALU_Op;
	// Opcode Field and Function Field
	input  wire [5:0] Inst_31_26 , Inst_5_0;
	
	always@(*) // if any of instruction opcode changes 
	begin
		
		case(Inst_31_26)
			6'd0:     //R-Formate
			begin
				if( Inst_5_0 == 6'd8 )
				begin	//Found to be jr instruction
					JR_Signal <= 1'b1 ;
					Reg_Dst<=1'b0;
					Branch<=1'b0;
					Branch_Not_Equal<=1'b0;
					Jump<=1'b1; //Set Jump so that it sends to hazard unit to flush next instruction
					Mem_Read<=1'b0;
					Mem_to_Reg<=1'b0;
					Mem_Write<=1'b0;
					ALU_Src<=1'b0;
					Reg_Write<=1'b0;
					ALU_Op<=4'b0000;
					Ori <= 1'b0;
				end
				else
				begin
					JR_Signal <=1'b0;
					Reg_Dst<=1'b1;
					Branch<=1'b0;
					Branch_Not_Equal<=1'b0;
					Jump<=1'b0;
					Mem_Read<=1'b0;
					Mem_to_Reg<=1'b0;
					Mem_Write<=1'b0;
					ALU_Src<=1'b0;
					Reg_Write<=1'b1;
					ALU_Op<=4'b0010;
					Ori <= 1'b0;
				end
			end
			6'd2:	//jump
			begin
				Reg_Dst<=1'bx;
				Branch<=1'b0;
				Branch_Not_Equal<=1'b0;
				JR_Signal <=1'b0;
				Jump<=1'b1;
				Mem_Read<=1'b0;
				Mem_to_Reg<=1'bx;
				Mem_Write<=1'b0;
				ALU_Src<=1'bx;
				Reg_Write<=1'b0;
				ALU_Op<=4'b0xxx;
				Ori <= 1'b0;
			end	
			/*6'd3:	//jal
			begin
				Reg_Dst<='b10;
				Branch<=1'b0;
				Branch_Not_Equal<=1'b0;
				Jump<=1'b1;
				Mem_Read<=1'b0;
				Mem_to_Reg<=2'b10;
				Mem_Write<=1'b0;
				ALU_Src<=1'bx;
				Reg_Write<=1'b1;
				ALU_Op<=4'b0xxx;
				Ori <= 1'b0;
			end*/
			6'd4:	//beq
			begin
				JR_Signal <=1'b0;
				Reg_Dst<=1'b0;
				Branch<=1'b1;
				Branch_Not_Equal<=1'b0;
				Jump<=1'b0;
				Mem_Read<=1'b0;
				Mem_to_Reg<=1'b0;
				Mem_Write<=1'b0;
				ALU_Src<=1'b0;
				Reg_Write<=1'b0;
				ALU_Op<=4'b0001;
				Ori <= 1'b0;
			end
			6'd5:	//bne
			begin
				JR_Signal <=1'b0;
				Reg_Dst<=1'b0;
				Branch<=1'b0;
				Branch_Not_Equal<=1'b1;
				Jump<=1'b0;
				Mem_Read<=1'b0;
				Mem_to_Reg<=1'b0;
				Mem_Write<=1'b0;
				ALU_Src<=1'b0;
				Reg_Write<=1'b0;
				ALU_Op<=4'b0001;
				Ori <= 1'b0;
			end
			6'd8: 	//addi
			begin
				JR_Signal <=1'b0;
				Reg_Dst<=1'b0;
				Branch<=1'b0;
				Branch_Not_Equal<=1'b0;
				Jump<=1'b0;
				Mem_Read<=1'b0;
				Mem_to_Reg<=1'b0;
				Mem_Write<=1'b0;
				ALU_Src<=1'b1;
				Reg_Write<=1'b1;
				ALU_Op<=4'b0000;
				Ori <= 1'b0;
			end
			6'd10:	//slti
			begin
				JR_Signal <=1'b0;
				Reg_Dst<=1'b0;
				Branch<=1'b0;
				Branch_Not_Equal<=1'b0;
				Jump<=1'b0;
				Mem_Read<=1'b0;
				Mem_to_Reg<=1'b0;
				Mem_Write<=1'b0;
				ALU_Src<=1'b1;
				Reg_Write<=1'b1;
				ALU_Op<=4'b0101;
				Ori <= 1'b0;
			end
			6'd11:	//sgti
			begin
				JR_Signal <=1'b0;
				Reg_Dst<=1'b0;
				Branch<=1'b0;
				Branch_Not_Equal<=1'b0;
				Jump<=1'b0;
				Mem_Read<=1'b0;
				Mem_to_Reg<=1'b0;
				Mem_Write<=1'b0;
				ALU_Src<=1'b1;
				Reg_Write<=1'b1;
				ALU_Op<=4'b1000;
				Ori <= 1'b0;
			end
			6'd12:	//andi
			begin
				JR_Signal <=1'b0;
				Reg_Dst<=1'b0;
				Branch<=1'b0;
				Branch_Not_Equal<=1'b0;
				Jump<=1'b0;
				Mem_Read<=1'b0;
				Mem_to_Reg<=1'b0;
				Mem_Write<=1'b0;
				ALU_Src<=1'b1;
				Reg_Write<=1'b1;
				ALU_Op<=4'b0011;
				Ori <= 1'b0;
			end
			6'd13:	//ori
			begin
				JR_Signal <=1'b0;
				Reg_Dst<=1'b0;
				Branch<=1'b0;
				Branch_Not_Equal<=1'b0;
				Jump<=1'b0;
				Mem_Read<=1'b0;
				Mem_to_Reg<=1'b0;
				Mem_Write<=1'b0;
				ALU_Src<=1'b1;
				Reg_Write<=1'b1;
				ALU_Op<=4'b0100;
				Ori <= 1'b1;
			end		
			6'd14:	//xori
			begin
				JR_Signal <=1'b0;
				Reg_Dst<=1'b0;
				Branch<=1'b0;
				Branch_Not_Equal<=1'b0;
				Jump<=1'b0;
				Mem_Read<=1'b0;
				Mem_to_Reg<=1'b0;
				Mem_Write<=1'b0;
				ALU_Src<=1'b1;
				Reg_Write<=1'b1;
				ALU_Op<=4'b0110;
				Ori <= 1'b0;
			end
			6'd15:	//lui
			begin
				JR_Signal <=1'b0;
				Reg_Dst<=1'b0;
				Branch<=1'b0;
				Branch_Not_Equal<=1'b0;
				Jump<=1'b0;
				Mem_Read<=1'b0;
				Mem_to_Reg<=1'b0;
				Mem_Write<=1'b0;
				ALU_Src<=1'b1;
				Reg_Write<=1'b1;
				ALU_Op<=4'b0111;
				Ori <= 1'b0;
			end
			6'd32:	//lb
			begin
				JR_Signal <=1'b0;
				Reg_Dst<=1'b0;
				Branch<=1'b0;
				Branch_Not_Equal<=1'b0;
				Jump<=1'b0;
				Mem_Read<=1'b0;
				Mem_to_Reg<=1'b0;
				Mem_Write<=1'b0;
				ALU_Src<=1'b1;
				Reg_Write<=1'b1;
				ALU_Op<=4'b0111;
				Ori <= 1'b0;
			end
			6'd33:	//lh
			begin
				JR_Signal <=1'b0;
				Reg_Dst<=1'b0;
				Branch<=1'b0;
				Branch_Not_Equal<=1'b0;
				Jump<=1'b0;
				Mem_Read<=1'b0;
				Mem_to_Reg<=1'b0;
				Mem_Write<=1'b0;
				ALU_Src<=1'b1;
				Reg_Write<=1'b1;
				ALU_Op<=4'b0111;
				Ori <= 1'b0;
			end
			6'd35:     //lw
			begin
				JR_Signal <=1'b0;
				Reg_Dst<=1'b0;
				Branch<=1'b0;
				Branch_Not_Equal<=1'b0;
				Jump<=1'b0;
				Mem_Read<=1'b1;
				Mem_to_Reg<=1'b1;
				Mem_Write<=1'b0;
				ALU_Src<=1'b1;
				Reg_Write<=1'b1;
				ALU_Op<=4'b0000;
				Ori <= 1'b0;
			end
			/*6'd40:	//sb
			begin
				Reg_Dst<=2'b00;
				Branch<=1'b0;
				Branch_Not_Equal<=1'b0;
				Jump<=1'b0;
				Mem_Read<=1'b0;
				Mem_to_Reg<=2'b00;
				Mem_Write<=1'b0;
				ALU_Src<=1'b1;
				Reg_Write<=1'b1;
				ALU_Op<=4'b0111;
				Ori <= 1'b0;
			end
			6'd41:	//sh
			begin
				Reg_Dst<=2'b00;
				Branch<=1'b0;
				Branch_Not_Equal<=1'b0;
				Jump<=1'b0;
				Mem_Read<=1'b0;
				Mem_to_Reg<=2'b00;
				Mem_Write<=1'b0;
				ALU_Src<=1'b1;
				Reg_Write<=1'b1;
				ALU_Op<=4'b0111;
				Ori <= 1'b0;
			end*/
			6'd43:  //sw
			begin
				JR_Signal <=1'b0;
				Reg_Dst<=1'bx;
				Branch<=1'b0;
				Branch_Not_Equal<=1'b0;
				Jump<=1'b0;
				Mem_Read<=1'b0;
				Mem_to_Reg<=1'bx;
				Mem_Write<=1'b1;
				ALU_Src<=1'b1;
				Reg_Write<=1'b0;
				ALU_Op<=4'b0000;
				Ori <= 1'b0;
			end	
			default : 
			begin
				JR_Signal <=1'b0;
				Branch <= 0;
				Jump <= 0 ;
				Branch_Not_Equal <= 0;
				Mem_Read <= 0 ;
				Mem_Write <=0;
				Reg_Write <= 0;
				ALU_Src <= 0;
				Mem_to_Reg <= 0;
				Reg_Dst <=0;
				ALU_Op<=4'b0000;
				Ori <= 1'b0;
			end		
		endcase
	end
endmodule
module BR_ADDER(branchAdded, PC_Adder_output, Sign_Ext_Output);
	output reg [12:0] branchAdded;
	input  wire[12:0] PC_Adder_output;
	input  wire signed [12:0] Sign_Ext_Output; // signed since mover up or down
	always @(PC_Adder_output or Sign_Ext_Output)
	begin
		branchAdded <= PC_Adder_output + Sign_Ext_Output;
	end
endmodule
module ID_Forwarding_Unit( Comp_Mux_1 , Comp_Mux_2 , Branch , Branch_Not_Equal, IF_ID_Rs , IF_ID_Rt , Ex_Mem_DestReg , Mem_WB_DestReg , Ex_Mem_Write , Mem_WB_Write );
	//done
	output reg  [1:0] Comp_Mux_1 ,Comp_Mux_2 ;
	input  wire [4:0] IF_ID_Rs , IF_ID_Rt , Ex_Mem_DestReg , Mem_WB_DestReg ;
	input  wire       Ex_Mem_Write , Mem_WB_Write , Branch, Branch_Not_Equal ;  
	
	always @ (*)
	begin
		Comp_Mux_1 <= 2'b00;
		Comp_Mux_2 <= 2'b00;
		if((Ex_Mem_Write && Ex_Mem_DestReg!=0 && Branch) || (Ex_Mem_Write && Ex_Mem_DestReg!=0 &&Branch_Not_Equal))
		begin
			if ( Ex_Mem_DestReg ==  IF_ID_Rs && Ex_Mem_DestReg ==  IF_ID_Rt) //beq s1,s1,L1
			begin
				Comp_Mux_1 <= 2'b01;
				Comp_Mux_2 <= 2'b01;
			end
			else if (( Ex_Mem_DestReg ==  IF_ID_Rt) && ( Mem_WB_DestReg ==  IF_ID_Rs ))
			begin
				Comp_Mux_1 <= 2'b10;
				Comp_Mux_2 <= 2'b01;
			end
			else if (( Ex_Mem_DestReg ==  IF_ID_Rs) && ( Mem_WB_DestReg ==  IF_ID_Rt ))
			begin
				Comp_Mux_1 <= 2'b01;
				Comp_Mux_2 <= 2'b10;
			end
			else if( Ex_Mem_DestReg ==  IF_ID_Rs )
				Comp_Mux_1 <= 2'b01;
			else if( Ex_Mem_DestReg ==  IF_ID_Rt)
				Comp_Mux_2 <= 2'b01;
			else if ( (Mem_WB_Write && Mem_WB_DestReg!=0 && Branch) || (Mem_WB_Write && Mem_WB_DestReg!=0 &&Branch_Not_Equal)) 
			begin
			if ( (Mem_WB_DestReg ==  IF_ID_Rs) && (Mem_WB_DestReg ==  IF_ID_Rt) ) //beq s1,s1,L1
			begin
				Comp_Mux_1 <= 2'b10;
				Comp_Mux_2 <= 2'b10;
			end
			else if( Mem_WB_DestReg ==  IF_ID_Rs )
				Comp_Mux_1 <= 2'b10;
			else if( Mem_WB_DestReg ==  IF_ID_Rt)
				Comp_Mux_2 <= 2'b10;
			end
			end
		else if( (Mem_WB_Write && Mem_WB_DestReg!=0 && Branch) || (Mem_WB_Write && Mem_WB_DestReg!=0 &&Branch_Not_Equal))
		begin
			if ( (Mem_WB_DestReg ==  IF_ID_Rs) && (Mem_WB_DestReg ==  IF_ID_Rt) ) //beq s1,s1,L1
			begin
				Comp_Mux_1 <= 2'b10;
				Comp_Mux_2 <= 2'b10;
			end
			else if (( Ex_Mem_DestReg ==  IF_ID_Rs) && ( Mem_WB_DestReg ==  IF_ID_Rt ))
			begin
				Comp_Mux_1 <= 2'b01;
				Comp_Mux_2 <= 2'b10;
			end
			else if (( Ex_Mem_DestReg ==  IF_ID_Rt) && ( Mem_WB_DestReg ==  IF_ID_Rs ))
			begin
				Comp_Mux_1 <= 2'b10;
				Comp_Mux_2 <= 2'b01;
			end
			else if( Mem_WB_DestReg ==  IF_ID_Rs )
				Comp_Mux_1 <= 2'b10;
			else if( Mem_WB_DestReg ==  IF_ID_Rt)
				Comp_Mux_2 <= 2'b10;
			else if((Ex_Mem_Write && Ex_Mem_DestReg!=0 && Branch) || (Ex_Mem_Write && Ex_Mem_DestReg!=0 &&Branch_Not_Equal))
			begin
				if ( (Ex_Mem_DestReg ==  IF_ID_Rs) && (Ex_Mem_DestReg ==  IF_ID_Rt)) //beq s1,s1,L1
				begin
					Comp_Mux_1 <= 2'b01;
					Comp_Mux_2 <= 2'b01;
				end
				else if( Ex_Mem_DestReg ==  IF_ID_Rs )
					Comp_Mux_1 <= 2'b01;
				else if( Ex_Mem_DestReg ==  IF_ID_Rt)
					Comp_Mux_2 <= 2'b01;
			end		
		end
end
endmodule

module ID_EX_reg ( Reg_Write , Mem_to_Reg , Mem_Write , Mem_Read , ALU_Src , Reg_Dst , ALU_Op , Read_Data1_Out ,  Read_Data2_Out , shift_amount_out , Sign_Ext_out ,rs , rt , rd , func_field ,control_signal,Read_Data1,Read_Data2,Sign_Ext,IF_Inst,clk); //control unit input 
  
  input wire [9:0] control_signal ;
  input wire [31:0] Read_Data1,Read_Data2 ,Sign_Ext;
	input wire [25:0] IF_Inst;
  input wire clk; 
  output reg Reg_Write , Mem_to_Reg , Mem_Write , Mem_Read , ALU_Src , Reg_Dst ;
  output reg [3:0]  ALU_Op;
  output reg [31:0] Read_Data1_Out ,  Read_Data2_Out ,  Sign_Ext_out;
  output reg [4:0] rs , rt , rd  ,  shift_amount_out ;
  output reg [5:0]   func_field ;
  
  always @(posedge clk)
    begin
    Reg_Write <= control_signal[9] ;
    Mem_to_Reg <= control_signal[8];
    Mem_Write <= control_signal[7];
    Mem_Read <=  control_signal[6];
    ALU_Src <=  control_signal[5];
    Reg_Dst <=   control_signal[4];
    ALU_Op  <= control_signal[3:0];
    Read_Data1_Out <= Read_Data1 ;
    Read_Data2_Out <= Read_Data2 ;
    Sign_Ext_out <= Sign_Ext;
    shift_amount_out <= IF_Inst[10:6];
    rs <= IF_Inst[25:21];
    rt <= IF_Inst[20:16];
    rd <= IF_Inst[15:11];
    func_field <=  IF_Inst[5:0];
    end
endmodule
module ALU(alu_result, inst_10_6, read_data_1, alu_mux_output, alu_ctrl);
	
	input  wire [4:0] inst_10_6;
	input  wire [3:0] alu_ctrl;
	input  wire signed [31:0] read_data_1;
	input  wire signed [31:0] alu_mux_output;
	
	output reg  signed [31:0] alu_result;
	
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

	
	always @(*)
	begin 
		case(alu_ctrl)
			
			AND: 
			begin
				alu_result <=  read_data_1 &  alu_mux_output ;
				
			end
			
			OR :
			begin
				alu_result <=  read_data_1 |  alu_mux_output ;
				
			end
			
			ADD:
			begin
				alu_result <=  read_data_1 +  alu_mux_output ;
				
			end
			
			SUB: 
			begin
				alu_result <=  (read_data_1 -  alu_mux_output) ;
				
			end
			
			SLT: 
			begin
				alu_result <=  (read_data_1 < alu_mux_output);
			end

			SGT: 
			begin
				alu_result <=  (read_data_1 > alu_mux_output);
				
			end
			
			NOR: 
			begin
				alu_result <= ~(read_data_1 | alu_mux_output);
				
			end
			
			XOR:
			begin
				alu_result <=   read_data_1 ^ alu_mux_output ;
				
			end
			
			SLL:
			begin
				alu_result <=   alu_mux_output << inst_10_6  ;
				
			end
			
			SRL:
			begin
				alu_result <=   alu_mux_output >> inst_10_6  ;
				
			end
			
			SRA:
			begin
				alu_result <=   alu_mux_output >>>inst_10_6  ;
				
			end
			
			LUI: 
			begin
				alu_result <=   alu_mux_output << 16  ;
				
			end
			
			default : 
			begin
				alu_result <= 0;
			end
		endcase
	end
endmodule
module ALU_CONTROL(alu_ctrl, alu_op, inst_5_0);
	
	output reg[3:0] alu_ctrl;	 //signal going to ALU can be modified to more bits when adding more instr 
	input wire[3:0] alu_op; 	//coming from control unit
	input wire[5:0] inst_5_0;  	//funct field in instr

	always @(*)  	//to make sure when any input change;
	begin                         	//operations are executed from begining;
		if(alu_op==4'b0000)	//sw or lw --> add or addi
		begin
			
			alu_ctrl <= 4'b0010; 
		end

		else if(alu_op==4'b0001) // sub for beq
		begin
			
			alu_ctrl <= 4'b0110;
		end
		
		else if(alu_op==4'b1000) // new sgti
		begin
			
			
			alu_ctrl <= 4'b0101;
		end

		else if(alu_op==4'b0011) // andi
		begin
			
			alu_ctrl <= 4'b0000;
		end

		else if(alu_op==4'b0100) // ori
		begin
			
			alu_ctrl <= 4'b0001;
		end

		else if(alu_op==4'b0101) // slti
		begin
			
			alu_ctrl <= 4'b0111;
		end

		else if(alu_op==4'b0110) // xori
		begin
			
			alu_ctrl <= 4'b0011;
		end

		else if(alu_op==4'b0111) // lui
		begin
			
			alu_ctrl <= 4'b1010;
		end

		else if(alu_op==4'b0010)   //Incase (010) indicate we have R_FORMATE inst; 
		begin                    //so we diffrentiate using funct field in instr;

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
		else
		begin
			alu_ctrl <= 4'b0000;
		end
	end
endmodule
module Forwarding_Unit_EX(ID_EX_rs,ID_EX_rt,EX_MEM_register_destination,MEM_WB_register_destination,EX_MEM_regwrite,MEM_WB_regwrite,forwardA,forwardB);
	input [4:0] EX_MEM_register_destination; //the register to be written at of instruction in EX/mem reg pipeline;
	input [4:0] MEM_WB_register_destination;//_______________________________________________MEM/WB reg pipeline
	input [4:0] ID_EX_rs;
	input [4:0] ID_EX_rt;
	input    EX_MEM_regwrite;
	input    MEM_WB_regwrite;
	
	output reg [1:0] forwardA;  //to control ALU input with output from EXE or MEM; 
	output reg [1:0] forwardB;  //10 choose EXE out ,01 choose MEM;
	always@(*)
	begin
		forwardA<=2'b00;
		forwardB<=2'b00;
		if((EX_MEM_regwrite)&&(EX_MEM_register_destination!=0))
		begin
			if((EX_MEM_register_destination==ID_EX_rs) && (EX_MEM_register_destination==ID_EX_rt))
			begin
				forwardA <= 2'b10;
				forwardB <= 2'b10;
			end
			else if((EX_MEM_register_destination==ID_EX_rs) && (MEM_WB_register_destination==ID_EX_rt))
			begin
				forwardA <= 2'b10;
				forwardB <= 2'b01;
			end
			else if((EX_MEM_register_destination==ID_EX_rt) &&(MEM_WB_register_destination==ID_EX_rs))
			begin
				forwardA <= 2'b01;
				forwardB <= 2'b10;
			end
			else if(EX_MEM_register_destination==ID_EX_rs)
				forwardA <= 2'b10;//3amltha el awal bas 3shan 3ayezha priority 3lshan lw feh inst 2 wra ba3d beyktbo f nafs register ya5od agdad wa7da
			else if(EX_MEM_register_destination==ID_EX_rt)
				forwardB <= 2'b10;
			else if((MEM_WB_regwrite)&&(MEM_WB_register_destination!=0) && (!(EX_MEM_register_destination==ID_EX_rs)))
			begin
				if((MEM_WB_register_destination==ID_EX_rs) && (MEM_WB_register_destination==ID_EX_rt))
				begin
					forwardA <= 2'b01;
					forwardB <= 2'b01;
				end	
				else if(MEM_WB_register_destination==ID_EX_rs)
					forwardA <= 2'b01;
				else if(MEM_WB_register_destination==ID_EX_rt)  // 5aletha if 3shan momken el etnin yeb2a beytketb fehom 3ady
					forwardB <= 2'b01;
				
			end
		end
		else if((MEM_WB_regwrite)&&(MEM_WB_register_destination!=0))
		begin
			if((MEM_WB_register_destination==ID_EX_rs) && (MEM_WB_register_destination==ID_EX_rt))
			begin
				forwardA <= 2'b01;
				forwardB <= 2'b01;
			end
			else if((EX_MEM_register_destination==ID_EX_rs) && (MEM_WB_register_destination==ID_EX_rt))
			begin
				forwardA <= 2'b10;
				forwardB <= 2'b01;
			end
			else if((EX_MEM_register_destination==ID_EX_rt) &&(MEM_WB_register_destination==ID_EX_rs))
			begin
				forwardA <= 2'b01;
				forwardB <= 2'b10;
			end
			else if(MEM_WB_register_destination==ID_EX_rs)
				forwardA <= 2'b01;
			else if(MEM_WB_register_destination==ID_EX_rt)  // 5aletha if 3shan momken el etnin yeb2a beytketb fehom 3ady
				forwardB <= 2'b01;
			else if((EX_MEM_regwrite)&&(EX_MEM_register_destination!=0)&& (!(MEM_WB_register_destination==ID_EX_rs)))
			begin
				if((EX_MEM_register_destination==ID_EX_rs) && (EX_MEM_register_destination==ID_EX_rt))
				begin
					forwardA <= 2'b10;
					forwardB <= 2'b10;
				end
				else if(EX_MEM_register_destination==ID_EX_rs)
					forwardA <= 2'b10;//3amltha el awal bas 3shan 3ayezha priority 3lshan lw feh inst 2 wra ba3d beyktbo f nafs register ya5od agdad wa7da
				else if(EX_MEM_register_destination==ID_EX_rt)
					forwardB <= 2'b10;
			end
		end
end
endmodule
module EX_MemReg (clk,RegWrite, MemtoReg,MemWrite, MemRead,ALUresult,writedata,writeReg,RegWriteOut, MemtoRegOut,MemWriteOut
			,MemReadOut,ALUresultOut,writedataOut,writeRegOut);
  
  input clk;
  input RegWrite, MemtoReg; //WB
  input MemWrite, MemRead; //MEM
  input [31:0] ALUresult,writedata; //writedata is the data in rt in case of storing in memory
  input [4:0] writeReg; //rt
  output reg RegWriteOut, MemtoRegOut ,MemWriteOut, MemReadOut;
  output reg [31:0] ALUresultOut,writedataOut;
  output reg [4:0] writeRegOut;
  
  always@(posedge clk)
    begin
      RegWriteOut  <= RegWrite;
      MemtoRegOut  <= MemtoReg;
      MemWriteOut  <= MemWrite;
      MemReadOut   <= MemRead;
      ALUresultOut <= ALUresult;
      writedataOut <= writedata;
      writeRegOut  <= writeReg;
      
    end
endmodule
module DATA_MEMORY(Read_Data,MemWrite,MemRead,Address,Write_data,clock);
	
	output reg signed[31:0] Read_Data;// reg wait for change value
	input wire clock;
	input wire MemWrite,MemRead;
	input wire [12:0] Address ;
	input wire signed [31:0] Write_data;
	
	reg[31:0] write_data_storage[0:8191];
	
	always @ (posedge clock)
	if(MemWrite)
	begin 
				write_data_storage[Address] <= Write_data;
	end

	always @ (*)
	begin
		  if(MemRead)
	      Read_Data <= write_data_storage[Address];
			else
			Read_Data <= 32'd0;
	end
endmodule
module Mem_WbReg(RegWrite, MemtoReg,ALUresult,clk,readData,writeReg,RegWriteOut,MemtoRegOut,readDataOut,ALUresultOut,writeRegOut);
  
  input clk;
  input RegWrite, MemtoReg; //WB
  input [4:0] writeReg; //rt
  input [31:0] ALUresult, readData;   
  output reg RegWriteOut, MemtoRegOut;
  output reg [31:0] readDataOut,ALUresultOut;
  output reg [4:0] writeRegOut;
  
  always@(posedge clk)
    begin
      RegWriteOut  <= RegWrite;
      MemtoRegOut  <= MemtoReg;
      readDataOut  <= readData;
      ALUresultOut <= ALUresult;
      writeRegOut  <= writeReg;
      
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
module MUX_13_1(output_mux,input0,input1,selector);
	output reg  [12:0] output_mux;
	input  wire [12:0] input0;
	input  wire [12:0] input1;
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
module MUX_32_2(output_mux,input_0,input1,input2,selector);

	input  wire [31:0] input_0;
	input  wire [31:0] input1;
	input  wire [31:0] input2;
	input wire  [1:0] selector;
	output reg  [31:0] output_mux;
	
	always@(input_0,input1,input2,selector)
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
			output_mux <= input_0;
		end
	end
endmodule
module MUX_13_2(output_mux,input_0,input1,input2,selector);

	input  wire [12:0] input_0;
	input  wire [12:0] input1;
	input  wire [12:0] input2;
	input wire  [1:0] selector;
	output reg  [12:0] output_mux;
	
	always@(input_0,input1,selector)
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
			output_mux <= input_0;
		end
	end
endmodule

module MUX_5_1(output_mux,input0,input1,selector);
	output reg  [4:0] output_mux;
	input  wire [4:0] input0;
	input  wire [4:0] input1;
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

module Hazard_MUX_10_1(output_mux,Reg_Write,Mem_to_Reg,Mem_Write,Mem_Read,ALU_Src,Reg_Dst,ALU_Op,selector);
	
	input wire  Reg_Write,Mem_to_Reg,Mem_Write,Mem_Read,ALU_Src,Reg_Dst;
	input wire  [3:0] ALU_Op;
	output reg  [9:0] output_mux;
	input  wire selector;
	always@(*)
	begin 
		if (selector == 1)
		begin
			output_mux <= 10'b0000010000;

		end
		else
		begin
			output_mux <= {Reg_Write,Mem_to_Reg,Mem_Write,Mem_Read,ALU_Src,Reg_Dst,ALU_Op};
		end
	
	end
endmodule

module Reg_Dst_MUX(Write_Register,in0,in1,RegDst);
	input  wire [4:0] in0,in1;
	input  wire [1:0] RegDst;
	output reg  [4:0] Write_Register;
	wire[4:0] ra;
	assign	ra = 5'd31;
	always@(in0,in1,RegDst)
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

module Pipeline_MIPS(Clock, reset, mips, data, en);

/*=============================================================== WIRES =================================================================*/
input wire Clock, reset, en;
input wire[31:0] mips;
wire Zero, JR_Signal, Ori, Reg_Dstn, Branch, Branch_Not_Equal, Mem_Read, Mem_to_Reg, Mem_Write, ALU_Src, Reg_Write, Jump, ex_mem_reg_write, ex_mem_mem_write, ex_mem_mem_read, mem_wb_reg_write, mem_wb_memtoreg;
wire beq_and_output, bne_not_output, bne_and_output, branch_or_output, hazard_mux_selector , hold_pc , hold_if_id_reg ,IF_Flush, id_ex_reg_write, id_ex_memtoreg, id_ex_mem_write, id_ex_mem_read,id_ex_alu_src, id_ex_regdst;
wire [1:0]  Comp_Mux_1, Comp_Mux_2, upper_mux_forward, lower_mux_forward;
wire [3:0]  id_ex_aluop, alu_ctrl, ALU_Op;
wire [4:0]  id_ex_rs,id_ex_rt,id_ex_rd,shift_amount, regdst_mux_out,ex_mem_dstreg, mem_wb_dstreg;
wire [5:0]  id_ex_func_field;
wire [9:0]  hazard_mux_output;
wire[12:0] pcIn, pcOut, pc_branch_mux_output, PCplus4_IF_Out, jump_mux_output, jr_mux_output, branch_adder_output;
wire [31:0] instruction, inst_IF_Out, Read_Data_1, Read_Data_2, Sign_Ext_Output;
wire [31:0] comp_upper_mux_out, comp_lower_mux_out, id_ex_Sign_Ext_Output, id_ex_read_data1, id_ex_read_data2, Alu_Result, alu_lower_mux_out, alu_upper_mux_out, alu_src_mux_Output, Read_Data, mem_wb_mux_output, mem_wb_read_data, mem_wb_alu_result, ex_mem_alu_result, ex_mem_lower_mux_out;
output reg[31:0] data;
/*=======================================================================================================================================*/


/*======================================================== IF STAGE + IF/ID REGISTER ======================================================*/

/***** PC & PC ADDER *****/ 
PC pc( pcOut , jr_mux_output , Clock , hold_pc, reset );
PC_ADDER pc_adder( pcIn , pcOut);
/***********************/

/******INSTRUCTION MEMORY**********/
INS_MEMORY ins_memory( instruction , Clock, pcOut, reset, mips);
/***********************************/

/*********BEQ AND BNE MUX***********/
MUX_13_1 pc_branch_mux( pc_branch_mux_output , pcIn , branch_adder_output, branch_or_output );
/***********************************/

/*********** JUMP MUX **************/
MUX_13_1 jump_mux(jump_mux_output,pc_branch_mux_output,inst_IF_Out[12:0], Jump);
MUX_13_1 jr_mux(jr_mux_output , jump_mux_output , Read_Data_1[12:0] , JR_Signal ); //WARNING YA SERsssssSY Jump and JR_Signal both are set to 1 if only jr inst comes since I want to flush the next inst by jump signal that is input to hazard unit
/***********************************/

/******IF/ID REGISTER**************/
IF_ID_Reg IF_ID_reg( inst_IF_Out , PCplus4_IF_Out , instruction , pcIn , Clock , hold_if_id_reg , IF_Flush);
/**********************************/
 

/*=======================================================================================================================================*/

/*=====================================================ID STAGE + ID/EX REGISTER=========================================================*/

/**********REGISTER FILE**********/
REG_FILE register_file( Read_Data_1 , Read_Data_2 , inst_IF_Out[25:21], inst_IF_Out[20:16], mem_wb_dstreg , mem_wb_mux_output, mem_wb_reg_write , Clock);
/**********************************/

/**************JUMP****************/
/**********************************/

/**********COMPARATOR **********/
Comparator branch_compare( Zero , comp_upper_mux_out , comp_lower_mux_out );
MUX_32_2 comp_upper_mux(comp_upper_mux_out ,Read_Data_1,ex_mem_alu_result,mem_wb_mux_output,Comp_Mux_1);
MUX_32_2 comp_lower_mux(comp_lower_mux_out ,Read_Data_2,ex_mem_alu_result,mem_wb_mux_output,Comp_Mux_2);
/**********************************/

/**************CONTROLLER**********/
CONTROL control_unit( JR_Signal , Ori , Reg_Dstn, Branch, Branch_Not_Equal, Mem_Read, Mem_to_Reg, ALU_Op , Mem_Write , ALU_Src , Reg_Write , Jump , inst_IF_Out[31:26] ,inst_IF_Out[5:0]); 
/**********************************/

/***************SIGN EXTEND *******/
SIGN_EXTEND sign_extend(Sign_Ext_Output, Ori , inst_IF_Out[15:0]) ;
/**********************************/

/**********BRANCH EQUAL AND BRANCH NOT EQUAL ADDER***/
BR_ADDER branch_adder( branch_adder_output , PCplus4_IF_Out , Sign_Ext_Output[12:0] );
and  beq_and(beq_and_output, Branch , Zero );
not  bne_not( bne_not_output , Zero);
and  bne_and( bne_and_output , Branch_Not_Equal , bne_not_output );
or   branch_or(branch_or_output,bne_and_output , beq_and_output);
/****************************************************/

/*******************ID FORWARD UNIT*****************/
ID_Forwarding_Unit id_forwarding_unit( Comp_Mux_1 , Comp_Mux_2 , Branch, Branch_Not_Equal , inst_IF_Out[25:21] , inst_IF_Out[20:16] , ex_mem_dstreg , mem_wb_dstreg , ex_mem_reg_write , mem_wb_reg_write );
/****************************************************/

/********HAZARD DETECTION UNIT***********/
HazardDetectionUnit hazard_unit ( id_ex_mem_read ,  regdst_mux_out , inst_IF_Out[31:16] , hold_pc , hold_if_id_reg , IF_Flush , branch_or_output , hazard_mux_selector,Jump);
Hazard_MUX_10_1 hazard_mux( hazard_mux_output , Reg_Write , Mem_to_Reg , Mem_Write , Mem_Read , ALU_Src , Reg_Dstn , ALU_Op , hazard_mux_selector);
/****************************************/

/**************ID_EX REGISTER**********/
ID_EX_reg id_ex_reg( id_ex_reg_write , id_ex_memtoreg , id_ex_mem_write , id_ex_mem_read , id_ex_alu_src , id_ex_regdst , id_ex_aluop , id_ex_read_data1 ,  id_ex_read_data2 , shift_amount , id_ex_Sign_Ext_Output ,id_ex_rs , id_ex_rt , id_ex_rd ,id_ex_func_field,
		hazard_mux_output , Read_Data_1 , Read_Data_2 , Sign_Ext_Output ,inst_IF_Out[25:0] ,Clock ); //control unit input 
/*************************************/
/*=======================================================================================================================================*/

/*=====================================EXECUTION STAGE + EX_MEM REGISIER=================================================================*/

MUX_32_2 alu_upper_mux(alu_upper_mux_out,id_ex_read_data1,mem_wb_mux_output,ex_mem_alu_result,upper_mux_forward);
MUX_32_2 alu_lower_mux(alu_lower_mux_out,id_ex_read_data2,mem_wb_mux_output,ex_mem_alu_result,lower_mux_forward);
MUX_32_1 alu_src_mux(alu_src_mux_Output, alu_lower_mux_out, id_ex_Sign_Ext_Output, id_ex_alu_src);

ALU alu(Alu_Result,shift_amount, alu_upper_mux_out, alu_src_mux_Output, alu_ctrl );	

ALU_CONTROL alu_control ( alu_ctrl, id_ex_aluop, id_ex_func_field);

Forwarding_Unit_EX ex_forward_unit(id_ex_rs,id_ex_rt,ex_mem_dstreg,mem_wb_dstreg,ex_mem_reg_write,mem_wb_reg_write,upper_mux_forward,lower_mux_forward);

MUX_5_1 reg_dst_mux(regdst_mux_out,id_ex_rt,id_ex_rd,id_ex_regdst);

EX_MemReg ex_mem_reg (Clock,id_ex_reg_write, id_ex_memtoreg,id_ex_mem_write, id_ex_mem_read,Alu_Result,alu_lower_mux_out,regdst_mux_out,ex_mem_reg_write, ex_mem_memtoreg,ex_mem_mem_write
			,ex_mem_mem_read,ex_mem_alu_result,ex_mem_lower_mux_out,ex_mem_dstreg);

/*=========================================== DATA MEMORY STAGE + MEM/WB REGISTER========================================================*/

DATA_MEMORY data_memory( Read_Data , ex_mem_mem_write , ex_mem_mem_read , ex_mem_alu_result[12:0] , ex_mem_lower_mux_out , Clock);

Mem_WbReg mem_wb_reg(ex_mem_reg_write , ex_mem_memtoreg , ex_mem_alu_result ,  Clock , Read_Data  , ex_mem_dstreg ,mem_wb_reg_write , mem_wb_memtoreg , mem_wb_read_data , mem_wb_alu_result , mem_wb_dstreg );
MUX_32_1 mem_wb_mux(mem_wb_mux_output , mem_wb_alu_result , mem_wb_read_data , mem_wb_memtoreg );

/*******************************WIRES*******************************/

/*=======================================================================================================================================*/
always @(posedge Clock)
begin
	if(en)
		data <= mem_wb_mux_output;
	else
		data <= 32'd0;
end
endmodule

