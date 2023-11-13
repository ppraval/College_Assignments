module mealy_machine_test;
	reg in,rst,clk;
	wire out;
		
	mealy_machine ml(in,rst,clk, out);
	
	initial begin
		clk = 0;
		forever #10 clk = ~clk;
	end
	
	initial begin
		rst = 1'b1;
		in = 1'b0;
		#50
		rst = 1'b0;
		in = 1'b0;
		#50
		in = 1'b1;
		#50
		in = 1'b0;
		$finish;
	end
endmodule
