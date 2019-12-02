module DATA_MEMORY(Read_Data,MemWrite,MemRead,Address,Write_data,clock,eof);
	output reg signed[31:0] Read_Data;// reg wait for change value
	input wire clock, eof;
	input wire MemWrite,MemRead;
	input wire [12:0] Address ;
	input wire signed [31:0] Write_data;
	reg[31:0]write_data_storage[0:8191];
	integer file , size , i , _;
	integer mem_file ;
	
	initial
	begin
		file = $fopen ("dataMemory.txt","w");
		$fwrite(file,""); 
		$fclose(file); 
	end
	
	always @(posedge eof)
	begin
		$finish();
	end
	
	always @ (negedge clock)
	begin 
		if(MemWrite == 1)
    		begin
			write_data_storage[Address] <= Write_data;    
		end
		file = $fopen ("dataMemory.txt","a");
		for ( i = 0; i < 8192 ; i = i+1)
		begin
			if ( write_data_storage[i] !== 'hxxxx ) // don't store the garbage values in memory 
			begin $fwrite(file,"%0d,%0d ",i,write_data_storage[i]); 	end
		end
		$fwrite(file,"\n");
		$fclose(file); 
	end

	always @ (*)
	begin
		if(MemRead)
		 Read_Data <= write_data_storage[Address];
	end
endmodule
