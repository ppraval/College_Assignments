module eight_bit_d_latch(input [7:0] d, input en, reset, output reg [7:0] q, q_, output reg zero);
	always @ (d or en or reset) 
	begin
	  if (!reset) begin
		 q <= 0;
		 zero <= 1;
	  end
	  else 
	  begin
		q <= d; 
		zero <= (d == 0); 
	  end
	end
	always @ (*) 
		q_ <= ~q;
endmodule
