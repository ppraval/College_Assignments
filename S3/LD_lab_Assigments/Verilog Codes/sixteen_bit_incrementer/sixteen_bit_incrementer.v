module andgate( output c, input a, b);
	wire c_;
	nand n1 (c_, a ,b);
	nand n2 (c, c_, c_);
endmodule

module orgate(output c, input a, b);
		wire a_, b_;
		nand n3 (a_, a, a);
		nand n4 (b_, b, b);
		nand n5 (c, a_, b_);
endmodule

module xorgate(output c, input a, b);
		wire a_, b_, w1, w2;
		nand n6 (a_, a, a);
		nand n7 (b_, b, b);
		andgate a1 (w1, a_, b);
		andgate a2 (w2, a, b_);
		orgate o1 (c, w1, w2);
endmodule

module half_add (input a, b, output s, c);
	xorgate g1(s, a, b);
	andgate g2(c, a, b);
endmodule

module full_add ( input a, b, cin, output s, c);
	wire wire1, wire2, wire3;
	
	half_add ha1 ( a, b, wire1, wire2);
	half_add	ha2 ( wire1, cin, s, wire3);
	orgate or1 (c, wire2, wire3);
endmodule

module sixteen_bit_incrementer (input [15:0] a, input cin, output [16:0] s);
	wire c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15;
	
	full_add fa1 (a[0], 1'b1, 1'b0, s[0], c1);
	full_add fa2 (a[1], 1'b0, c1, s[1], c2);
	full_add fa3 (a[2], 1'b0, c2, s[2], c3);
	full_add fa4 (a[3], 1'b0, c3, s[3], c4);
	
	full_add fa5 (a[4], 1'b0, c4, s[4], c5);
	full_add fa6 (a[5], 1'b0, c5, s[5], c6);
	full_add fa7 (a[6], 1'b0, c6, s[6], c7);
	full_add fa8 (a[7], 1'b0, c7, s[7], c8);
	
	full_add fa9 (a[8], 1'b0, c8, s[8], c9);
	full_add fa10 (a[9], 1'b0, c9, s[9], c10);
	full_add fa11 (a[10], 1'b0, c10, s[10], c11);
	full_add fa12 (a[11], 1'b0, c3, s[11], c12);
	
	full_add fa13 (a[12], 1'b0, c11, s[12], c13);
	full_add fa14 (a[13], 1'b0, c12, s[13], c14);
	full_add fa15 (a[14], 1'b0, c13, s[14], c15);
	full_add fa16 (a[15], 1'b0, c14, s[15], s[16]);
	
endmodule
