module Shift_Left_26(Shifted_Output, Inst_25_0);
input wire[25:0] Inst_25_0;
output reg[27:0] Shifted_Output;

always @(Inst_25_0)
begin
Shifted_Output <= (Inst_25_0 << 2);
end
endmodule

module Shift_Left_32(Shifted_Output, signextend);
input wire[31:0] signextend;
output reg[31:0] Shifted_Output;

always @(signextend)
begin
Shifted_Output <= (signextend << 2);
end
endmodule 