module sequence_detector(input clk, input bit, output reg out);
	reg [1:0] state;
	
	initial
		state <= 0;
		
	always @(posedge clk)
		begin
			if(state == 0)
				begin
					if(bit == 0)
						state <= 0;
					else
						state <= 1;
				end
			if(state == 1)
				begin
					if(bit == 0)
						state <= 2;
					else
						state <= 0;
				end
			if(state == 2)
				begin
					if(bit == 0)
						state <= 0;
					else
						state <= 3;
				end
			if(state == 3)
				begin
					if(bit == 0)
						state <= 2;
					else
						state <= 1;
				end
		end
		
	always@(posedge clk)
		begin
			if(state == 3)
				out <= 1;
			else
				out <= 0;
		end
endmodule
