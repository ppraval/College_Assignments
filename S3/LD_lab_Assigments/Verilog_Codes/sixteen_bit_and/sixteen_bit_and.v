module andgate(output c, input a, b);
	wire c_;
	nand n1 (c_, a ,b);
	nand n2 (c, c_, c_);
endmodule


module sixteen_bit_and(input [15:0] a, b, output [15:0] c);
	andgate g1(c[0], b[0], a[0]);
	andgate g2(c[1], b[1], a[1]);
	andgate g3(c[2], b[2], a[2]);
	andgate g4(c[3], b[3], a[3]);
	
	andgate g5(c[4], b[4], a[4]);
	andgate g6(c[5], b[5], a[5]);
	andgate g7(c[6], b[6], a[6]);
	andgate g8(c[7], b[7], a[7]);
	
	andgate g9(c[8], b[8], a[8]);
	andgate g10(c[9], b[9], a[9]);
	andgate g11(c[10], b[10], a[10]);
	andgate g12(c[11], b[11], a[11]);
	
	andgate g13(c[12], b[12], a[12]);
	andgate g14(c[13], b[13], a[13]);
	andgate g15(c[14], b[14], a[14]);
	andgate g16(c[15], b[15], a[15]);
endmodule
