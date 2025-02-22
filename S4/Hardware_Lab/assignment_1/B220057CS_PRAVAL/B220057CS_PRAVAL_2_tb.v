module traffic_light_controller_tb;
	reg clk;
	wire NS;
	wire EW;
	
	traffic_light_controller instance1 (clk, NS, EW);
	
	initial 
		begin
			clk <= 0;
			#5;
		end
		
	always #5 clk = ~clk;
	
endmodule

