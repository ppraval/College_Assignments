module eight_by_one_mux_test;
reg i0, i1, i2, i3, i4, i5, i6, i7, a, b, c;
wire y;
eight_by_one_mux gate1 (i0, i1, i2, i3, i4, i5, i6, i7, a, b, c, y);
integer i;
initial begin
	for (i = 0; i < 2048; i = i + 1) 
		begin
			{i0, i1, i2, i3, i4, i5, i6, i7, a, b, c} = i;
			#20;
		end
	end
endmodule
