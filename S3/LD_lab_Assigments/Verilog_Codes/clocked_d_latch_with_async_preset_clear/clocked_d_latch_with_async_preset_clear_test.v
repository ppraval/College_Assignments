module clocked_d_latch_with_async_preset_clear_test;
	reg d, pre, clr, clk;
	wire q, q_;
	integer i;
	 
	clocked_d_latch_with_async_preset_clear gate (d, pre, clr, clk, q, q_);
	
	initial
		begin
			for(i = 0; i < 16; i = i + 1)
				begin
					{d, pre, clr, clk} = i;
					#10;
				end
		end
endmodule
