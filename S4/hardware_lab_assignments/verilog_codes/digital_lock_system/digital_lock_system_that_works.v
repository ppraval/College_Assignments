module dff(output reg q, input d, clk);
	always @(posedge clk) 
		q <= d;
endmodule

module edgedet(input edge_in, output detected, input clock);
	wire q0, q1;
	dff dff0(.q(q0), .d(edge_in), .clk(clock));
	dff dff1(.q(q1), .d(q0), .clk(clock));
	
	assign detected = q0 & ~q1;
endmodule


module Timer(input Clock, Start, output Timeout);
	localparam ten_cycles = 1000;
	reg [8:0] q;
	always @(posedge Clock)
	begin
	if (!Start||(q == ten_cycles))
		q <= 9'b0;
	else
		q <= q + 1;
	end
assign Timeout = (q == ten_cycles);
endmodule

module lockfsm(input clock, reset,codesw, anysw, output reg [1:0] selsw, output locked, alarm, entimer, input timeout);
	localparam s0 = 3'b000, s1 = 3'b001, s2 = 3'b010, s3 = 3'b011, wrong = 3'b100, unlock = 3'b101;
	reg [2:0] lockstate;
	always @(posedge clock or posedge reset)
		begin
	 if (reset == 1'b1)
		lockstate <= s0;
	 else
		case (lockstate)
			s0 : 
				if (anysw & codesw)
					lockstate <= s1;
				else if (anysw)
					lockstate <= wrong;
				else
					lockstate <= s0;
					
			s1 :
				if (anysw & codesw)
					lockstate <= s2;
				else if (anysw)
					lockstate <= wrong;
				else
					lockstate <= s1;
					
			s2:
				if (anysw & codesw)
					lockstate <= s3;
				else if (anysw)
					lockstate <= wrong;
				else
					lockstate <= s2;
				
			s3: 
				if (anysw & codesw)
					lockstate <= unlock;
				else if (anysw)
					lockstate <= wrong;	
				else
					lockstate <= s3;
				
			wrong: lockstate <= wrong;
			
			unlock: 
				if (timeout)
					lockstate <= s0;
				else
					lockstate <= unlock;
			
		 default: lockstate <= 3'bx;
		 endcase
	 end
	 
	always @(lockstate)
		begin
			case(lockstate)
				s0: selsw = 0;
				s1: selsw = 1;
				s2: selsw = 2;
				s3: selsw = 3;
				wrong: selsw = 0;
				unlock: selsw = 0;
			default: selsw = 2'bx;
			endcase
		end
	assign locked = (lockstate == unlock) ? 0: 1;
	assign alarm = (lockstate == wrong) ? 0: 1;
	assign entimer = (lockstate == unlock) ? 1: 0;
endmodule

module comblock(input clock, clear,
	input [7:0] switches,
	output alarm, locked)

	wire mux_out, anysw, codesw, allsw, entimer, timeout;
	wire [1:0] selsw;
	
	assign mux_out = selsw == 0 ? switches[0]:
	 (selsw == 1 ? switches[1]:
	 (selsw == 2 ? switches[2]:
	 (selsw == 3 ? switches[3]: 1'b0)));
	
	assign allsw = &switches;
	
	edgedet det1(.edge_in(mux_out), .detected(codesw), .clock(clock));
	edgedet det2(.edge_in(allsw), .detected(anysw), .clock(clock));
	
	Timer t1(.Clock(clock), .Start(entimer), .Timeout(timeout));
	
	lockfsm controller(.clock(clock), .reset(clear), .codesw(codesw), .anysw(anysw), .selsw(selsw), .locked(locked), .alarm(alarm), .entimer(entimer), .timeout(timeout));
endmodule
