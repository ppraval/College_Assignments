module sequence_detector_tb;
	wire [7:0] stream = 8'b10100101;
	wire clk;
	wire bit;
	reg out;
	integer i;
	sequence_detector instance1 (clk, bit, out);
	
	initial
		begin
			clk<=0;
			#1
			for(i = 0; i < 8; i = i + 1) 
				begin
					clk <= ~clk;
					i <= stream[i];
					#5
					clk = ~clk;
					#5;
				end
		end
endmodule;
