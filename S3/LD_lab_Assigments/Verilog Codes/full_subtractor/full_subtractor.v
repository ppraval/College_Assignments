module half_subtractor(input a, b, output diff, borr);
	wire _b;
	xor x1 (diff, a, b);
	not n1 (_b, b);
	and a1(borr, a, _b);
endmodule

module full_subtractor(input a, b, borrin, output diff, borrout);
	wire diff1, borr1;
	half_subtractor h1(a, b, diff1, borr1);
	half_subtractor h2(diff1, borrin, diff, borrout);
endmodule
