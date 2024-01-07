module eight_bit_alu_tb;
	reg [7:0] a, b;
	wire[7:0] sum, diff, exor, left_shift_a, left_shift_b;
	wire cout, bout;

	eight_bit_alu instance1 (a, b, sum, cout, diff, bout, exor, left_shift_a, left_shift_b);
	integer i;
	initial begin
		 for(i = 0; i < 256; i = i + 1)
		 begin
			a = i;
			b = a * i;
			#10;
		 end
	end
endmodule
