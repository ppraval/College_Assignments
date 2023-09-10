module sr_latch(input s, r, output q, q_);
	nand(q_, r, q);
	nand(q, s, q_);
endmodule

