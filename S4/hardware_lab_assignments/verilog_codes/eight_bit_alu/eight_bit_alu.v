module eight_bit_subtractor(input [7:0] a, b, input bin, output [7:0] diff, output bout);
	wire [8:0] res;

	assign res = a - b - bin;

	assign diff = res[7:0];

	assign bout = res[8];

endmodule

module eight_bit_adder(input [7:0] a, b, input cin, output [7:0] sum, output cout);
	wire [8:0] res;

	assign res = a + b + cin;

	assign sum = res[7:0];

	assign cout = res[8];

endmodule

module eight_bit_exor(input [7:0] a, b, output [7:0] exor);
	assign exor = a ^ b;
endmodule

module eight_bit_alu(input [7:0] a, b, output [7:0] sum, diff, exor, leftshift, output cout, bout);
	eight_bit_adder instance1 (a, b, 1b'0, sum, cout);
	eight_bit_subtractor instance2 (a, b, 1b'0, diff, bout);
	eight_bit_exor instance3 (a, b, exor);
endmodule

	
	