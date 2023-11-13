module d_ff (input d, clk, output reg q);
  always @ (posedge clk)
  begin
    q <= d;
  end
endmodule

module eight_bit_register(input [7:0] a, input clk, output [7:0] q);
	wire [7:0] b;
	d_ff d0 (a[0], clk, b[0]);
	d_ff d1 (a[1], clk, b[1]);
	d_ff d2 (a[2], clk, b[2]);
	d_ff d3 (a[3], clk, b[3]);
	d_ff d4 (a[4], clk, b[4]);
	d_ff d5 (a[5], clk, b[5]);
	d_ff d6 (a[6], clk, b[6]);
	d_ff d7 (a[7], clk, b[7]);
	assign q = b;
endmodule
