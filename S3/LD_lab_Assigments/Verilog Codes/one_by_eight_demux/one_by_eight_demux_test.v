module one_by_eight_demux_test;
	reg s0, s1, s2, d;
	wire y0, y1, y2, y3, y4, y5, y6, y7;

	one_by_eight_demux gate1 (d, s0, s1, s2, y0, y1, y2, y3, y4, y5, y6, y7);
	integer i;
	initial begin
		for (i = 0; i < 16; i = i + 1) 
			begin
				{s0, s1, s2, d} = i;
				#20;
			end
		end
endmodule
