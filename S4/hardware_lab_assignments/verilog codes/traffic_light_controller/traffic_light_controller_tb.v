module traffic_light_controller_tb;
	reg clk;
	wire NS;
	wire EW;
	
	traffic_light_controller instance1 (clk, NS, EW)
	
	initial 
		begin
			clk <= 0;
			#5;
			for(i = 0; i < 500; i = i + 1)
				begin
					clk = ~clk;
					#5;
				end
		end
endmodule

