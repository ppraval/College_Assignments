module four_bit_negator_test;
reg [3:0] a;
wire [3:0] b;

four_bit_negator gate1 (a, b);

integer i;
initial begin
	for (i = 0; i < 16; i = i + 1) 
		begin
			{a} = i;
			#20;
		end
	end
endmodule
