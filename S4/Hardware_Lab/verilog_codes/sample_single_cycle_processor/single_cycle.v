`timescale 1ns / 1ps
module single_cycle(
input clk, reset,
output [31:0] pc,
input [31:0] instr,
output memwrite,
output [31:0] aluout, writedata,
input [31:0] readdata
);
wire branch, memtoreg, pcsrc, zero, spra, alusrc, regwrite, spregwrite, jump, jal, jumpreg, readhilo;
wire [1:0] regdst;
wire [3:0] alucontrol;
controller controller(instr[31:26], instr[5:0], instr[10:6], zero, memwrite, pcsrc, alusrc, regwrite, spregwrite, regdst, memtoreg, jump, jal, jumpreg,alucontrol, spra, readhilo);
datapath datapath(clk, reset, memtoreg, pcsrc, alusrc, regdst, regwrite, spregwrite, jump, jal, jumpreg, instr[10:6], alucontrol, zero, pc, instr, aluout, writedata, readdata, spra, readhilo);
endmodule
