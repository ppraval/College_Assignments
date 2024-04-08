module alu(
input [31:0] a, b, 
input [4:0] shamt,
input [3:0] alu_control, 
output reg [31:0] result, wd0, wd1,
output zero
);

wire [31:0] b2, sum, slt, sra_sign, sra_aux;
wire [63:0] product, quotient, remainder;
assign b2 = alu_control[2] ? ~b:b; 
assign sum = a + b2 + alu_control[2];
assign slt = sum[31];
assign sra_sign = 32'b1111_1111_1111_1111 << (32 - shamt);
assign sra_aux = b >> shamt;
assign product = a * b;
assign quotient = a / b;
assign remainder = a % b;
always@(*)
    case(alu_control[3:0])
      4'b0000: result <= a & b;
      4'b0001: result <= a | b;
      4'b0010: result <= sum;
      4'b0011: result <= b << shamt;
      4'b1011: result <= b << a;
      4'b0100: result <= b >> shamt;
      4'b1100: result <= b >> a;
      4'b0101: result <= sra_sign | sra_aux;
      4'b0110: result <= sum;
      4'b0111: result <= slt;
      4'b1010: 
        begin
          result <= product[31:0]; 
          wd0 <= product[31:0];
          wd1 <= product[63:32];
        end
      4'b1110: 
        begin
          result <= quotient; 
          wd0 <= quotient;
          wd1 <= remainder;
        end
      4'b1000: result <= b << 5'd16;
    endcase

  assign zero = (result == 32'd0);
endmodule
