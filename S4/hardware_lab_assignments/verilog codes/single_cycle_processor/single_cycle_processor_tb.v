module single_cycle_processor_tb;
	reg clk;
	initial clk = 0;
	always #5 clk = ~clk;
	single_cycle_processor instance1 (clk);
endmodule
