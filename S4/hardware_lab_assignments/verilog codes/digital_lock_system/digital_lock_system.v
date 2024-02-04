module edge_det(input edge_in, input clk, output reg det);
   reg edge_prev;
	
	always@(posedge clk)
		begin
			edge_prev = edge_in;
			det = edge_in & ~edge_prev;
		end
endmodule


module mux_4_by_1(input [1:0] s, input i0, i1, i2, i3, output reg mux_out);
	always@(*)
		begin
		if(s == 0)
			mux_out <= i0;
		if(s == 1)
			mux_out <= i1;
		if(s == 2)
			mux_out <= i2;
		if(s == 3)
			mux_out <= i3;
		end
endmodule

module timer(input start, clk, output reg timeout);
	reg [3:0] count;
	initial timeout <= 0;
	initial count <= 0;
	
	always@(posedge clk)
		begin
			if(timeout == 1)
				count <= 0;
			else if(start == 1)
				begin
					timeout = 0;
					count <= count + 1;
				end
			if(count == 10)
				begin
					timeout <= 1;
					count <= 0;
				end
		end
endmodule 

module lock_fsm(output reg [1:0] selsw, input timeout, codesw, anysw, clk, reset, output alarm, locked, entimer, output reg [2:0] curr_state);
	reg [2:0] state;
	initial state <= 0;
		
	always@(*)
		begin
			if(reset == 1)
				state <= 0;
			else
				begin
					case(state) 
						0: 
							begin
								if({codesw, anysw} == 2'b01) 
									begin
										state <= 4;
									end
								if({codesw, anysw} == 2'b11) 
									begin
										state <= 1;
									end
								if({codesw, anysw} == 2'b00) 
									begin
										state <= 0;
									end
								if({codesw, anysw} == 2'b10) 
									begin
										state <= 0;
									end
							end
						1: 
							begin
								if({codesw, anysw} == 2'b01) 
									begin
										state <= 4; 
									end
								if({codesw, anysw} == 2'b11) 
									begin
										state <= 2;
									end
								if({codesw, anysw} == 2'b00) 
									begin
										state <= 1;
									end
								if({codesw, anysw} == 2'b10) 
									begin
										state <= 1;
									end
								end
						2: 
							begin
								if({codesw, anysw} == 2'b01) 
									begin
										state <= 4;
									end
								if({codesw, anysw} == 2'b11) 
									begin
										state <= 3;
									end
								if({codesw, anysw} == 2'b00) 
									begin
										state <= 2;
									end
								if({codesw, anysw} == 2'b10) 
									begin
										state <= 2;
									end
							end
						3: 
							begin
								if({codesw, anysw} == 2'b01) 
									begin
										state <= 4;
									end
								if({codesw, anysw} == 2'b11) 
									begin
										state <= 5;
									end
								if({codesw, anysw} == 2'b00) 
									begin
										state <= 3;
									end 
								if({codesw, anysw} == 2'b10) 
									begin
										state <= 3;
									end 
							end
						4:
							state <= 4;
						5: 
							begin
								if(timeout == 1) 
									begin
										state <= 0;
									end
								if(timeout == 0)
									begin
										state <= 5;
									end
								end
						endcase
				end
		end
	always@(state)
		begin
			case (state)
				0: 
					begin
//						alarm <= 1;
//						locked <= 1;
//						entimer <= 0;
						selsw <= 0;
					end
				1: 
					begin
//						alarm <= 1;
//						locked <= 1;
//						entimer <= 0;
						selsw <= 1;
					end
				2: 
					begin
//						alarm <= 1;
//						locked <= 1;
//						entimer <= 0;
						selsw <= 2;
					end
				3: 
					begin
//						alarm <= 1;
//						locked <= 1;
//						entimer <= 0;
						selsw <= 3;
					end
				4: 
					begin
//						alarm <= 0;
//						locked <= 1;
//						entimer <= 0;
						selsw <= 0;
					end
				5: 
					begin
//						alarm <= 1;
//						locked <= 0;
//						entimer <= 1;
						selsw <= 0;
					end
			endcase		
		end
		
		assign locked = (state == 5) ? 0 : 1;
		assign alarm = (state == 4) ? 0 : 1;
		assign entimer = (state == 5) ? 1 : 0;
		
		always@(state)
			curr_state <= state;
endmodule

module digital_lock_system (input sw0, sw1, sw2, sw3, sw4, sw5, sw6, sw7, input clk, clear, output alarm, locked, output [2:0] lockfsm_state, output reg det1, det2, timer_out, allsw_out, mux_out_wire);
	wire allsw, timeout, codesw, anysw, entimer, mux_out;
	wire [1:0] selsw;
	
	assign allsw = sw0 & sw1 & sw2 & sw3 & sw3 & sw4 & sw5 & sw6 & sw7;
	
//	assign allsw = sw0 | sw1 | sw2 | sw3 | sw3 | sw4 | sw5 | sw6 | sw7;
	
	mux_4_by_1 instance1 (selsw, sw0, sw1, sw2, sw3, mux_out);	

	lock_fsm instance2 (selsw, timeout, codesw, anysw, clk, clear, alarm, locked, entimer, lockfsm_state);
	
	edge_det instance3 (mux_out, clk, codesw);
	edge_det instance4 (allsw, clk, anysw);
	
	timer instance5 (entimer, clk, timeout);
	
	always@(*)
		begin
			det1 <= codesw;
			det2 <= anysw;
			timer_out <= timeout;
			mux_out_wire <= mux_out;
			allsw_out <= allsw;
		end
	
endmodule
