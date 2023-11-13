module eight_bit_register_test;
	reg [7:0] a;
	reg clk;
	wire [7:0] q;
	
	eight_bit_register instance1 (a, clk, q);
	
	integer i;
	
	initial 
		begin
			for(i = 0; i < 512; i = i + 1)
				begin
					clk = i;
					a = i;
					#10;					
				end
		end
endmodule
				