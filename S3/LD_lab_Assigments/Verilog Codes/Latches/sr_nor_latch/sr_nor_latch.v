module sr_nor_latch(input s, r, output q, q_);
	nor(q_, s, q);
	nor(q, r, q_);
endmodule
