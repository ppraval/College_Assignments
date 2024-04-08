`timescale 1ns / 1ps
/*module data_memory(
input clk, we,
input [31:0] a, wd,
output [31:0] rd
 );
reg [7:0]ram_data_mem[1023:0]; //BYTE ADDRESSABLE 4-GiB DATA MEMORY
assign rd = ram_data_mem[a[31:2]];
always @(posedge clk)
if(we)
    ram_data_mem[a[31:2]] <= wd;
endmodule*/


module data_memory(
input clk, memwrite,
input [31:0] address,
input [31:0] write_data,
output reg [31:0] read_data
);
    reg [7:0] data_mem [1023:0];
    initial $readmemh("D:\College_Assignments\S4\hardware_lab_assignments\verilog_codes\sample_single_cycle_processor\data_file.dat", data_mem);
    always@(*) read_data = {data_mem[address], data_mem[address+1], data_mem[address+2], data_mem[address+3]};
    always@(posedge clk)
    begin
    if(memwrite)
    begin
        data_mem[address+3] <= write_data[7:0];
        data_mem[address+2] <= write_data[15:8];
        data_mem[address+1] <= write_data[23:16];
        data_mem[address] <= write_data[31:24];
    end
    end
endmodule
