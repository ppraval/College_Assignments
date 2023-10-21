module gated_sr_nor_latch_test;
	reg s, r, en;
	wire q, q_;
	
	gated_sr_nor_latch gsr1 (s, r, en, q, q_);
	initial begin

		en = 1'b1;
		s = 1'b0; r = 1'b0; #10;
		s = 1'b0; r = 1'b1; #10;
		s = 1'b1; r = 1'b0; #10;
		s = 1'b0; r = 1'b0; #10;
		s = 1'b1; r = 1'b1; #10;
		en = 1'b0;
		s = 1'b0; r = 1'b0; #10;
		s = 1'b0; r = 1'b1; #10;
		s = 1'b1; r = 1'b0; #10;
		s = 1'b0; r = 1'b0; #10;
		s = 1'b1; r = 1'b1; 
	end
endmodule
