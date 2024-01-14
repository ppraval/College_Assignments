module eight_bit_adder(input [7:0] a, b, input cin, output [7:0] sum, output cout);
	wire [8:0] res;

	assign res = a + b + cin;

	assign sum = res[7:0];

	assign cout = res[8];

endmodule

