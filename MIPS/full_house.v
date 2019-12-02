`include "pc.v"
`include "pc_adder.v"
`include "inst_memory.v"
`include "reg_file.v"
`include "control.v"
`include "alu_control.v"
`include "alu.v"
`include "data_memory.v"
`include "branch_adder.v"
`include "sign_extend.v"
`include "jump_address.v"
`include "mux.v"
`include "clock.v"
`include "tempreg.v"
module MIPS();

/*********WIRES********/
wire Clock, eof, Ori, Branch, Branch_Not_Equal, Mem_Read, Mem_Write, ALU_Src, Reg_Write, Jump, JR_Signal, Zero;
wire beq_and_output, bne_and_output, bne_not_output, branch_or_output, jr_not_output;
wire[1:0] Reg_Dstn, Mem_to_Reg;
wire[3:0] ALU_Op;
wire[3:0] ALUctrl;
wire[4:0] reg_dest_mux_output;
wire [31:0]jal_temp_reg_out ;
wire[31:0] pcIn, pcOut, Read_Data, Read_Data_1, Read_Data_2, instruction, Sign_Ext_Output, Alu_Result,jal_pc4_reg;
wire[31:0] branch_adder_output, pc_branch_mux_output, jump_address_output, jump_mux_output, jr_mux_output, data_memory_mux_output, alu_src_mux_Output;
/**********************/
tempreg jal_pc4(jal_pc4_reg,pcIn,Clock);
/**********CLOCK*******/
CLOCK myClock(Clock);
/**********************/

/*****PC & PC ADDER*****/
PC pc(pcOut,jr_mux_output,Clock,eof);
PC_ADDER pc_adder(pcIn, pcOut);
/***********************/

/******INSTRUCTION MEMORY**********/
INS_MEMORY ins_memory(instruction, Clock, pcOut);
/**********************************/

/**********REGISTER FILE**********/
Reg_Dst_MUX reg_dest_mux(reg_dest_mux_output, instruction[20:16], instruction[15:11], Reg_Dstn);
REG_FILE register_file(Read_Data_1, Read_Data_2, instruction[25:21], instruction[20:16], reg_dest_mux_output, data_memory_mux_output, jr_and_output, Clock,eof);
/**********************************/

/**************CONTROLLER**********/
CONTROL control_unit(Ori,Reg_Dstn, Branch, Branch_Not_Equal, Mem_Read, Mem_to_Reg, ALU_Op, Mem_Write, ALU_Src, Reg_Write, Jump, instruction[31:26]);
/**********************************/

/************SIGN EXTEND***********/
SIGN_EXTEND sign_extend(Sign_Ext_Output, Ori, instruction[15:0]);
/**********************************/

/************ALU CONTROL***********/
ALU_CONTROL alu_control(ALUctrl, JR_Signal, ALU_Op, instruction[5:0]);
/**********************************/

/****************ALU *******************/
ALU alu(Alu_Result, Zero, instruction[10:6], Read_Data_1, alu_src_mux_Output, ALUctrl);
MUX_32_1 alu_src_mux(alu_src_mux_Output, Read_Data_2, Sign_Ext_Output, ALU_Src);
/***************************************/

/**********DATA MEMORY**************/
DATA_MEMORY data_memory(Read_Data, Mem_Write, Mem_Read, Alu_Result[12:0], Read_Data_2, Clock, eof);
MUX_32_2 data_mem_mux(data_memory_mux_output, Alu_Result, Read_Data, jal_pc4_reg, Mem_to_Reg);
/**********************************/

/***BRANCH EQUAL AND BRANCH NOT EQUAL***/
BR_ADDER branch_adder(branch_adder_output, pcIn, Sign_Ext_Output);
and  beq_and(beq_and_output,Branch,Zero);
not  bne_not(bne_not_output,Zero);
and  bne_and(bne_and_output,Branch_Not_Equal,bne_not_output);
or   branch_or(branch_or_output,bne_and_output,beq_and_output);
MUX_32_1 pc_branch_mux(pc_branch_mux_output, pcIn, branch_adder_output, branch_or_output);
/****************************************/

/***********JUMP ADDRESS EXTEND***********/
JUMP_ADDRESS jump_add(jump_address_output,instruction[25:0],pcIn[31:26]);
MUX_32_1 jump_mux(jump_mux_output,pc_branch_mux_output,jump_address_output, Jump);
/*****************************************/

/**************JUMP REGISTER**************/
not jr_not(jr_not_output, JR_Signal);
and jr_and(jr_and_output, Reg_Write, jr_not_output);
MUX_32_1 jr_mux(jr_mux_output, jump_mux_output, Read_Data_1, JR_Signal);
/*****************************************/

integer file;
always @ (posedge Clock)
begin
	file = $fopen("pc.txt");
	$fwrite(file,"%0d\n",pcOut);
end
initial
begin
$monitor("***************** %b *******************\n pcOut=%h, pcIn:%h,reg_dest_mux_output : %h \n, data_memory_mux_output : %h, instruction: %h \n Read_Data_1:%h, Read_Data_2:%h, instruction[25:21]:%h, instruction[20:16]:%h \n beq_and_output:%h, Branch:%h, branch_or_output:%h, pc_branch_mux_output:%h\n Read_Data_1:%h, alu_src_mux_Output:%h, ALU_Src: %h, Zero:%h, Alu_Result: %h\n ***************************************",Clock,pcOut,pcIn,reg_dest_mux_output,data_memory_mux_output,instruction,Read_Data_1, Read_Data_2, instruction[25:21], instruction[20:16], beq_and_output, Branch,branch_or_output,pc_branch_mux_output, Read_Data_1, alu_src_mux_Output, ALU_Src,Zero, Alu_Result);
end
endmodule
