`timescale 1ns / 1ps
module datapath(
input clk, reset,
input memtoreg, 
input pcsrc,
input alusrc, 
input [1:0] regdst,
input regwrite, spregwrite, jump, jal, jumpreg,
input [4:0] shamt,
input [3:0] alucontrol,
output zero,
output [31:0] pc,
input [31:0] instr,
output [31:0] aluout, writedata,
input [31:0] readdata,
input spra, readhilo
);
wire [4:0]  writereg;
wire [31:0] pcnextjr, pcnext, pcnextbr, pcplus4, pcbranch;
wire [31:0] signimm, signimmsh;
wire [31:0] srca, srcb, wd0, wd1, sprd;
wire [31:0] result, resultjal, resulthilo;
flopr #(32) pcreg(clk, reset, pcnext, pc); //pc next
adder pcadd1(pc, 32'b100, pcplus4);
sl2  immsh(signimm, signimmsh);
adder pcadd2(pcplus4, signimmsh, pcbranch);
mux2 #(32)pcbrmux(pcplus4, pcbranch, pcsrc, pcnextbr);
mux2 #(32)pcmux(pcnextbr, {pcplus4[31:28], instr[25:0], 2'b00}, jump, pcnext);
mux2 #(32)pcmuxjr(pcnext, srca, jumpreg, pcnextjr);
regfile reg_file(clk, regwrite, instr[25:21], instr[20:16], writereg, resulthilo, srca, writedata); //register file
mux3 #(5) wrmux(instr[20:16], instr[15:11], 5'b11111, regdst, writereg);
mux2 #(32)resmux(aluout, readdata, memtoreg, result);
mux2 #(32)wrmuxjal(result, pcplus4, jal, resultjal);
mux2 #(32)wrmuxhilo(resultjal, sprd, readhilo, resulthilo);
signext se(instr[15:0], signimm);
mux2 #(32)srcbmux(writedata, signimm, alusrc, srcb); //ALU
alu alu(srca, srcb, shamt, alucontrol, aluout, wd0, wd1, zero);
spregfile sprf(clk, spregwrite, spra, wd0, wd1, sprd); //Special register file for hi/lo
endmodule
