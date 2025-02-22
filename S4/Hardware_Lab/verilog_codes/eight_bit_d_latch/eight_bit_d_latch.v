module eight_bit_d_latch(input [7:0] a, input en, reset, output [7:0] d);
	always @ (d or en or rstn) 
	begin
	
	  if (!rstn) begin
		 q <= 0;
		 zero <= 1;
	  end
	  
	  else 
	  begin
		 if (en) 
		 begin
			q <= d; 
			zero <= (d == 0); 
		 end
	  end
	end
endmodule
