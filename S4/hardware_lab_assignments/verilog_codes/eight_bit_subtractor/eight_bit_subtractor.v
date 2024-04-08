module eight_bit_subtractor(input [7:0] a, b, input bin, output [7:0] diff, output bout);
	wire [8:0] res;

	assign res = a - b - bin;

	assign diff = res[7:0];

	assign bout = res[8];

endmodule