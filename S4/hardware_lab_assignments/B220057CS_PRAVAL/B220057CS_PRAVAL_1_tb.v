module single_cycle_processor_tb;
	reg clk;
	wire [15:0] mem_0, mem_1,  mem_2, mem_3, mem_4, mem_5, mem_6, mem_7, curr_instr, reg_1, reg_2, reg_w, alu_calc;
	initial clk = 0;
	always #5 clk = ~clk;
	single_cycle_processor instance1 (clk, mem_0, mem_1,  mem_2, mem_3, mem_4, mem_5, mem_6, mem_7, curr_instr, reg_1, reg_2, reg_w, alu_calc);
endmodule
