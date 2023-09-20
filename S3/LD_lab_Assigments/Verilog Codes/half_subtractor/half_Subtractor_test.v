module half_subtractor_test;
reg a, b;
wire diff, borr;
	half_subtractor h1(a, b, diff, borr);
	initial begin
			 a = 0; b = 0;
		#10 a = 0; b = 1;
		#10 a = 1; b = 0;
		#10 a = 1; b = 1;
	end
endmodule
