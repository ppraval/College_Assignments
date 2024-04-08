`timescale 1ns / 1ps
module upper_module(
input clk, reset, 
output [31:0] writedata, dataadr, 
output memwrite
);
wire [31:0] pc, instr, readdata;
single_cycle single_cycle(clk, reset, pc, instr, memwrite, dataadr, writedata, readdata);
instruction_memory instruction_memory(pc, instr);
data_memory data_memory(clk, memwrite, dataadr, writedata, readdata);
endmodule
