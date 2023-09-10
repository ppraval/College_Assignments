module one_by_eight_demux ( input d, s0, s1, s2, output y0, y1, y2, y3, y4, y5, y6, y7);

wire ns0, ns1, ns2;
	
	not n1 (ns0, s0);
	not n2 (ns1, s1);
	not n3 (ns2, s2);
	
	and (y0, d, ns0, ns1, ns2);
	and (y1, d, ns0, ns1, s2);
	and (y2, d, ns0, s1, ns2);
	and (y3, d, ns0, s1, s2);
	and (y4, d, s0, ns1, ns2);
	and (y5, d, s0, ns1, s2);
	and (y6, d, s0, s1, ns2);
	and (y7, d, s0, s1, s2);
	
endmodule
