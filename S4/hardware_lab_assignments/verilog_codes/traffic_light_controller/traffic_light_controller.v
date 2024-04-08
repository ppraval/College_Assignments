module traffic_light_contoller(input clk)
	reg [1:0] state;
	
	initial state <= 0;
	always @(posedge clk)
		begin
			if(state == 0)
				begin
					if(clk == 1)
						state <= 1;
				end
			if(state == 1)
				begin
					if(clk == 0)
						state <= 2;
				end
			if(state == 2)
				begin
					if(clk == 1)
						state <= 3;
				end
			if(state == 3)
				begin
					if(clk == 0)
						state <= 0;						
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
	
endmodule
