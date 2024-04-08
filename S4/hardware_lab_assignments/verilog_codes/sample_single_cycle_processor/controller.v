`timescale 1ns / 1ps
module controller(
input [5:0] op, funct,
input [4:0] shamt,
input zero,
output memwrite,
output pcsrc, alusrc,
output regwrite, spregwrite,
output [1:0] regdst, 
output memtoreg,
output jump, jal, jumpreg,
output [3:0] alucontrol,
output spra, readhilo
);
wire [3:0] aluop;
wire branch;
maindec main_decoder(op, funct, memwrite, branch, alusrc, regwrite, spregwrite, regdst, memtoreg, jump, jal, aluop, spra, readhilo);
aludec alu_decoder(funct, shamt, aluop, alucontrol, jumpreg);
assign pcsrc = branch & zero;
endmodule
