module d_flip_flop_With_async_preset_clear_test;
	reg d, pre, clr;
	wire q, q_;
	integer i;
	 
	d_flip_flop_With_async_preset_clear gate (d, pre, clr, q, q_);
	
	initial
		begin
			for(i = 0; i < 8; i = i + 1)
				begin
					{d, pre, clr} = i;
					#10;
				end
		end
endmodule
