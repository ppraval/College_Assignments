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

module four_bit_decrementer (input [3:0] a, input cin, output [4:0] s);
	wire c1, c2, c3;
	
	full_add fa1 (a[0], 1'b1, 1'b0, s[0], c1);
	full_add fa2 (a[1], 1'b1, c1, s[1], c2);
	full_add fa3 (a[2], 1'b1, c2, s[2], c3);
	full_add fa4 (a[3], 1'b1, c3, s[3], s[4]);
endmodule
