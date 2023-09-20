module half_subtractor(input a, b, output diff, borr);
	wire _b;
	xor x1 (diff, a, b);
	not n1 (_b, b);
	and a1(borr, a, _b);
endmodule
