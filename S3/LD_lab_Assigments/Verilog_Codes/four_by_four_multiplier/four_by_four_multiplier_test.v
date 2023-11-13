module four_by_four_multiplier_test;
	reg [3:0] a, b;
	wire [7:0] p;
	
	four_by_four_multiplier instance0 (a, b, p);
	integer i;
	initial begin
	for(i = 0; i < 5000; i = i + 1)
		begin
			a = $random();
			b = $random();
			#10;
		end
	end
endmodule
	