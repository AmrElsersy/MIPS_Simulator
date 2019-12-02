module REG_FILE(Read_Data_1, Read_Data_2, Read_Reg_1, Read_Reg_2, Write_Reg, Write_Data, Reg_Write, Clock ,eof);

	input wire[4:0] Read_Reg_1, Read_Reg_2, Write_Reg;
	input wire[31:0] Write_Data;
	input Reg_Write, Clock;
	output reg[31:0] Read_Data_1, Read_Data_2;
	integer j;
	reg signed[31:0]  Reg_File[0:31];

	input wire eof;
	integer file,i;
	initial
	begin
		for(j=0; j <32; j = j+ 1)
		begin
		if(j == 29)
		Reg_File[j] <= 32'h00001FFF; // stack pointer
		else
		Reg_File[j]  <= 32'h00000000;
		end
		file = $fopen ("regFile.txt","w");
		$fwrite(file,""); 
		$fclose(file); 
	end
	
	always @(*)
	begin
		Read_Data_1 = Reg_File[Read_Reg_1];
		Read_Data_2 = Reg_File[Read_Reg_2];
	end
	
	always @(negedge Clock)
	begin
		if(Reg_Write)
		begin
			Reg_File[Write_Reg] <= Write_Data;
		end
		file = $fopen ("regFile.txt","a");
		for ( i = 0; i < 32 ; i = i+1)
		begin
			if ( Reg_File[i] !== 'hxxxx )
			$fwrite(file,"%0d,%0d ",i,Reg_File[i]);
		end
		$fwrite(file,"\n"); 
		$fclose(file);
	end

endmodule 

