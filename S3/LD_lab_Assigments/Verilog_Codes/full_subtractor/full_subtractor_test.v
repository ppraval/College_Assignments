module full_subtractor_test;
reg a, b, borrin;
wire diff, borrout;
	full_subtractor f1(a, b, borrin, diff, borrout);
	initial begin
		borrin = 0;
			 a = 0; b = 0;
		#10 a = 0; b = 1;
		#10 a = 1; b = 0;
		#10 a = 1; b = 1;
		#10;
		borrin = 1;
		    a = 0; b = 0;
		#10 a = 0; b = 1;
		#10 a = 1; b = 0;
		#10 a = 1; b = 1;
	end
endmodule
