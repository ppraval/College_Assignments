module notgate(output b, input a);
	nand n1 (b, a, a);
endmodule

module four_bit_negator(input [3:0] a, output [3:0] b);
	notgate g1(b[0],a[0]);
	notgate g2(b[1],a[1]);
	notgate g3(b[2],a[2]);
	notgate g4(b[3],a[3]);
endmodule
