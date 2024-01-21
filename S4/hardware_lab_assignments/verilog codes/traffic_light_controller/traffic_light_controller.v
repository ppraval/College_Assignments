module traffic_light_controller(input bit, input clk, output reg NS, EW);
	reg [1:0] state;
	
	initial state <= 0;
	always @(posedge clk)
		begin
			if(state == 0)
				begin
					if(bit == 1)
						state <= 1;
				end
			if(state == 1)
				begin
					if(bit == 0)
						state <= 2;
				end
			if(state == 2)
				begin
					if(bit == 1)
						state <= 3;
				end
			if(state == 3)
				begin
					if(bit == 0)
						state <= 0;						
				end
		end
		
	always@(posedge clk)
		begin
			if(state == 3)
				begin
					NS <= 0;
					EW <= 1;
				end
			if(state == 0)
				begin
					NS <= 1;
					EW <= 0;
				end
		end
endmodule
