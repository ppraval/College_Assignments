module eight_bit_d_latch_tb;
	reg [7:0] d, rand;
	reg en, reset;
	reg [7:0] q, q_;
	reg zero_flag;
	integer i;

	eight_bit_d_latch instance1 (d, en, reset, q, q_, zero_flag);

	initial begin
		rand = $random;
		for(i = 0; i < 5000; i = i + 1)
			begin
				en = 1;
				reset = 0;
				d = rand * i;
				en = 0;
				en = 1;
				reset = 1;
				d = rand * i;
			end
	end
endmodule
