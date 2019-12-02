module tempreg(data,pcIn,clock);
	output reg [31:0] data;
	input wire [31:0] pcIn;	
	input clock;
	always @ (posedge clock)
	begin
		data<=pcIn;
	end
endmodule

