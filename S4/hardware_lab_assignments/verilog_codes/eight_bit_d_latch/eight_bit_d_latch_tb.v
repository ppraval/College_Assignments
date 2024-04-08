module eight_bit_d_latch_tb;
	reg [7:0] d, random;
	reg en, reset;
	wire [7:0] q, q_;
	integer i;
	random = $random;
	eight_bit_d_latch instance1 (a, en, reset, q, q_);
	initial begin
		for(i = 0; i < 5000; i = i + 1)
			begin
				
				d = random * i;
			end
	end
endmodule
