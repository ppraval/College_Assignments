module eight_by_one_mux ( input i0, i1, i2, i3, i4, i5, i6, i7, a, b, c, output y);

	wire wire0, wire1, wire2, wire3, wire4, wire5, wire6, wire7, na, nb, nc;
	not n1 (na, a);
	not n2 (nb, b);
	not n3 (nc, c);
	
	and a0 ( wire0, na, nb, nc, i0);
	and a1 ( wire1, na, nb, c, i1);
	and a2 ( wire2, na, b, nc, i2);
	and a3 ( wire3, na, b, c, i3);
	and a4 ( wire4, a, nb, nc, i4);
	and a5 ( wire5, a, nb, c, i5);
	and a6 ( wire6, a, b, nc, i6);
	and a7 ( wire7, a, b, c, i7);
	
	or or1 (y, wire0, wire1, wire2, wire3, wire4, wire5, wire6, wire7);	

endmodule
 