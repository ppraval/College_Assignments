module half_add (input a, b, output s, c);
	xor g1(s, a, b);
	and g2(c, a, b);
endmodule

module four_bit_half_adder ( input [3:0] a,b, output [4:0] s);

	wire wc1, wc2, wc3, wc4, w2c1, w2c2, w2c3, w2c4, co0, co1, co2, ws0, ws1, ws2, ws3;
	
	half_add ha1 ( a[0], b[0], ws0, wc1);
	half_add ha2 ( ws0, 1'b0, s[0], w2c1);
	or or1 (co0, wc1, w2c1);
	
	half_add ha3 ( a[1], b[1], ws1, wc2);
	half_add ha4 ( ws1, co0, s[1], w2c2);
	or or2 (co1, wc2, w2c2);
	
	half_add ha5 ( a[2], b[2], ws2, wc3);
	half_add ha6 ( ws2, co1, s[2], w2c3);
	or or3 (co2, wc3, w2c3);
	
	half_add ha7 ( a[3], b[3], ws3, wc4);
	half_add ha8 ( ws3, co2, s[3], w2c4);
	or or4 (s[4], wc4, w2c4);

endmodule

