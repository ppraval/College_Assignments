`timescale 1ns / 1ps
module aludec(
input [5:0] funct,
input [4:0] shamt,
input [3:0] aluop,
output reg [3:0] alucontrol,
output jumpreg
);

  always @(*)
    case(aluop)
      4'b0000: alucontrol <= 4'b0010;//addition
      4'b0001: alucontrol <= 4'b0110;//subtraction
      4'b0100: alucontrol <= 4'b0000;//and
      4'b0101: alucontrol <= 4'b0001;//or
      4'b0111: alucontrol <= 4'b0111;//slt
      default: case(funct)//RTYPE
          6'b100000: alucontrol <= 4'b0010;//ADD
          6'b100010: alucontrol <= 4'b0110;//SUB
          6'b100100: alucontrol <= 4'b0000;//AND
          6'b100101: alucontrol <= 4'b0001;//OR
          6'b101010: alucontrol <= 4'b0111;//SLT
          default:   alucontrol <= 4'bxxxx;//???
        endcase
    endcase
assign jumpreg = (funct == 6'b001000) ? 1 : 0;
endmodule
