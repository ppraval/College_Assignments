module bcd_to_excess_three(input [3:0] bcd, output reg [3:0] ex3);
wire [3:0] wex3;
	bcd_to_x3 g1 (bcd, wex3);
	always @* begin
		if (bcd > 4'b1001)
		begin
			ex3 = 4'bxxxx;
		end
		else
		begin
			ex3 = wex3;
		end
	end
	
endmodule


module bcd_to_x3(input [3:0] bcd, output [3:0] ex3);

		wire w1, w2, w3, w4, w5, b_, c_, d_;
		
		not n1 (b_, bcd[2]);
		not n2 (c_, bcd[1]);
		not n3 (d_, bcd[0]);

		and a1 (w1, b_, bcd[1]);
		and a2 (w2, b_, bcd[0]);
		and a3 (w3, bcd[2], c_, d_);
		and a4 (w4, bcd[2], bcd[1]);
		and a5 (w5, bcd[2], bcd[0]);
		
		or o1 (ex3[3], bcd[3], w4, w5);
		or o2 (ex3[2], w1, w2, w3);

		xnor xn1 (ex3[1], bcd[1], bcd[0]);

		assign ex3[0] = d_;
endmodule

