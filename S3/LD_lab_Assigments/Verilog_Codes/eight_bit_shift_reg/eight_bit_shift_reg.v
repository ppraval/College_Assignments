module d_ff (input d, clk, output reg q);
  always @ (posedge clk)
  begin
    q <= d;
  end
endmodule

module eight_bit_shift_reg (input a, input clk, output [7:0] q);
	wire [7:0] b;
	d_ff d0 (a, clk, b[0]);
	d_ff d1 (b[0], clk, b[1]);
	d_ff d2 (b[1], clk, b[2]);
	d_ff d3 (b[2], clk, b[3]);
	d_ff d4 (b[3], clk, b[4]);
	d_ff d5 (b[4], clk, b[5]);
	d_ff d6 (b[5], clk, b[6]);
	d_ff d7 (b[6], clk, b[7]);
	assign q = b;
endmodule
