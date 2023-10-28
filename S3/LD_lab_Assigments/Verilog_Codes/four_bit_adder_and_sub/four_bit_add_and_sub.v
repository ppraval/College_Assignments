module half_add (input a, b, output s, c);
	xor g1(s, a, b);
	and g2(c, a, b);
endmodule

module full_add ( input a, b, cin, output s, c);
	wire wire1, wire2, wire3;
	
	half_add ha1 ( a, b, wire1, wire2);
	half_add	ha2 ( wire1, cin, s, wire3);
	or or1 (c, wire2, wire3);
endmodule

module four_bit_add_and_sub ( input [3:0] a, b, input cin, output [4:0] s, output overflow);
	wire c1, c2, c3, x1, x2, x3, x4;
	xor xor1 (x1, b[0], cin);
	xor xor2 (x2, b[1], cin);
	xor xor3 (x3, b[2], cin);
	xor xor4 (x4, b[3], cin);
	
	full_add fa1 ( a[0], x1, cin, s[0], c1);
	full_add fa2 ( a[1], x2, c1, s[1], c2);
	full_add fa3 ( a[2], x3, c2, s[2], c3);
	full_add fa4 ( a[3], x4, c3, s[3], s[4]);
	
	xor xor5 (overflow, c, c3);

endmodule
	
	
	

