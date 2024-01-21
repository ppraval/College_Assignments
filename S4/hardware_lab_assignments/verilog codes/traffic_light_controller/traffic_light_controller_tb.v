module traffic_light_controller_tb;
	reg clk;
	reg bit;
	wire NS;
	wire EW;
	integer i;
	
	traffic_light_controller instance1 (bit, clk, NS, EW);
	
	initial clk <= 0;
	
	always #5 clk = ~clk;
	
	initial 
		begin
			bit <= 0;
			#5;
			for(i = 0; i < 500; i = i + 1)
				begin
					bit = ~bit;
					#10;
				end
		end
endmodule

