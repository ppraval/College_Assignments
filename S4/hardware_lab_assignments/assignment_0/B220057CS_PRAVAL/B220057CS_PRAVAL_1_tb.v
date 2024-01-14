module eight_bit_adder_tb;
	reg [7:0] a, b;
	reg cin;
	wire[7:0] s;
	wire cout;

	eight_bit_adder instance1 (a, b, cin, s, cout);
	integer i;
	initial begin
		cin = 0;
		 for(i = 0; i < 256; i = i + 1)
		 begin
			a = i;
			b = a * i;
			#10;
		 end
	end
endmodule
