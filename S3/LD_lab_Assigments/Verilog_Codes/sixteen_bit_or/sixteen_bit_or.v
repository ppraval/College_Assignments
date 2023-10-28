module orgate(output c, input a, b);
		wire a_, b_;
		nand n3 (a_, a, a);
		nand n4 (b_, b, b);
		nand n5 (c, a_, b_);
endmodule



module sixteen_bit_or(input [15:0] a, b, output [15:0] c);
	orgate g1(c[0], b[0], a[0]);
	orgate g2(c[1], b[1], a[1]);
	orgate g3(c[2], b[2], a[2]);
	orgate g4(c[3], b[3], a[3]);
	
	orgate g5(c[4], b[4], a[4]);
	orgate g6(c[5], b[5], a[5]);
	orgate g7(c[6], b[6], a[6]);
	orgate g8(c[7], b[7], a[7]);
	
	orgate g9(c[8], b[8], a[8]);
	orgate g10(c[9], b[9], a[9]);
	orgate g11(c[10], b[10], a[10]);
	orgate g12(c[11], b[11], a[11]);
	
	orgate g13(c[12], b[12], a[12]);
	orgate g14(c[13], b[13], a[13]);
	orgate g15(c[14], b[14], a[14]);
	orgate g16(c[15], b[15], a[15]);
endmodule
