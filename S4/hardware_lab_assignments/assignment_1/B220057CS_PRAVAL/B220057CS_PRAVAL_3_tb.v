`timescale 1 ms / 1 ms
 module test_comblock();
 reg clock;
 reg clear;
 reg [7:0] switches;
 wire alarm;
 wire locked;
 comblock UUT( .clock(clock), .clear(clear), .switches(switches), .alarm(alarm), .locked(locked));
 initial
	begin
	clock = 1'b0;
	forever
	#50 clock = ~clock;
 end
 initial
	begin
	 clear = 1'b1;
	 
	 switches = 8'b11111111;
	 
	 repeat(3) @(negedge clock);
		clear = 1'b0;
	 repeat(3) @(negedge clock);
		switches [0] = 1'b0;
	 repeat(2) @(negedge clock);
		switches [0] = 1'b1;
	 repeat(3) @(negedge clock);
		switches [1] = 1'b0;
	 repeat(2) @(negedge clock);
		switches [1] = 1'b1;
	 repeat(3) @(negedge clock);
		switches[2] = 1'b0;
	 repeat(2) @(negedge clock);
		switches [2] = 1'b1;
	 repeat(3) @(negedge clock);
		switches [3] = 1'b0;
	 repeat(2) @(negedge clock);
		switches [3] = 1'b1;
	 
	 
	 repeat(400) @(negedge clock);
		clear = 1'b1;
	 repeat(4) @(negedge clock);
		clear = 1'b0;
	 repeat(3) @(negedge clock);
		switches [0] = 1'b0;
	 repeat(2) @(negedge clock);
		switches [0] = 1'b1;
	 repeat(3) @(negedge clock);
		switches [5] = 1'b0;
	 repeat(2) @(negedge clock);
		switches [5] = 1'b1;
	 repeat(3) @(negedge clock);
		switches [2] = 1'b0;
	 repeat(2) @(negedge clock);
		switches [2] = 1'b1;
	 repeat(3) @(negedge clock);
		switches [3] = 1'b0;
	 repeat(2) @(negedge clock);
		switches [3] = 1'b1;
	 repeat(4) @(negedge clock);
		clear = 1'b1;
	 repeat(4) @(negedge clock);
	 $stop;
end
endmodule
