module alu_tb;
	reg [15:0] X, Y;
	wire[16:0] result;
	reg[3:0] ctrl;
		
	alu a1 (X, Y, ctrl, result);
	integer i;			
	initial 
		begin
			assign X = $random;
			assign Y = $random;
			#10;

			for(i = 0; i < 16; i = i + 1)
				begin
					ctrl = i;
					#10;
				end
		end
endmodule

