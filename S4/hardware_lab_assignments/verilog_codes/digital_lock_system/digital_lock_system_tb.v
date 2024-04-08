module timer_tb;
	reg clk, clear;
	reg s0, s1, s2, s3, s4, s5, s6, s7;
	wire alarm, locked, det1, det2, timeout, mux_out, allsw;
	wire [2:0] curr_state;
	
	digital_lock_system instance1 (s0, s1, s2, s3, s4, s5, s6, s7, clk, clear, alarm, locked, curr_state, det1, det2, timeout, allsw, mux_out);
	
	initial clk = 0;
	
	always #5 clk = ~clk;
	
	initial 
		begin
			#1 clear = 1;
			#9 clear = 0;
			
			#30;
			
			s0 = 1;
//			#10;
//			s0 = 0;
			
			s1 = 1;
//			#10;
//			s1 = 0;
			
			s2 = 1;
//			#10;
//			s2 = 0;
			
			s3 = 1;
//			#10;
//			s3 = 0;
			
			s4 = 0;
//			#10;
//			s4 = 0;
			
			s5 = 0;
//			#10;
//			s5 = 0;
			
			s6 = 0;
//			#10;
//			s6 = 0;
			
			s7 = 0;
//			#10;
//			s7 = 0;
		end
endmodule
