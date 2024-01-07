module eight_bit_subtractor_tb;
	reg [7:0] a, b;
	reg bin;
	wire[7:0] d;
	wire bout;

	eight_bit_subtractor instance1 (a, b, bin, d, bout);
	integer i;
	initial begin
		bin = 0;
		 for(i = 0; i < 256; i = i + 1)
		 begin
			a = i;
			b = a * i;
			#10;
		 end
	end
endmodule
