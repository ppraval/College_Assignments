module four_bit_half_adder_test;
reg [3:0] a, b;
wire [4:0] s;

four_bit_half_adder gate1 (a, b, s);
integer i;
initial begin
	for (i = 0; i < 256; i = i + 1) 
		begin
			{a, b} = i;
			#20;
		end
	end
endmodule
