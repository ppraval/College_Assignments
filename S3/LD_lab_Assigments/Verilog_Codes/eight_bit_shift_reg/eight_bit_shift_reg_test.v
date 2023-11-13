module eight_bit_shift_reg_test;
	reg [7:0] a;
	reg in, clk;
	wire [7:0] q;
	
	always #10 clk = ~clk;
	
	eight_bit_shift_reg instance1 (in, clk, q);
	
  initial begin
	clk = 0;
	a = 0;
  end
  
  integer i;
  
  initial begin
	a = 8'b10000000;
	for(i = 0; i < 8; i = i + 1)
		begin
			in = a[i];
			#10;
		end
		$finish;
  end
endmodule
