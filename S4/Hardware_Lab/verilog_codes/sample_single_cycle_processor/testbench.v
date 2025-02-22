`timescale 1ns / 1ps
module upper_module_tb;
	reg clk;
	reg reset;
	wire [31:0] writedata, dataadr;
	wire memwrite;
	
	initial
		begin
			clk = 0;
			reset = 1;
			#10;
			reset = 0;
		end
	always #5 clk = ~clk;
	
	upper_module instance1 (.clk(clk), .reset(reset), .writedata(writedata), .dataadr(dataadr), .memwrite(memwrite));	
endmodule
	