module traffic_light_controller(input clk, output reg NS, EW);
	reg state;
	wire [1:0] count;
	three_cycle_counter instance1 (clk, count);
	initial state <= 0;
	
	always@(*)
		begin			
			if(count == 0)
				state = ~state;
		end
	
	always@(*)
		begin
			if(state == 0)
				begin
					NS <= 1;
					EW <= 0;
				end
			else
				begin
					NS <= 0;
					EW <= 1;
				end
		end
endmodule


module three_cycle_counter(input clk, output reg [1:0] count);
	
	initial count = 0;	
	
	always@(posedge clk)
		begin
				if(count == 0)
					count <= 1;
				if(count == 1)
					count <= 2;
				if(count == 2)
					count <= 0;
		end
endmodule
	
		