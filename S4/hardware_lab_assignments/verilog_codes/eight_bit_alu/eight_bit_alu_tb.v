module eight_bit_alu_tb;
	reg [7:0] a, b;
	reg cin;
	wire[7:0] s, d, x;
	wire cout, bout;

	eight_bit_alu instance1 (a, b, s, d, x, cout, bout);
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
