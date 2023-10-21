module sixteen_bit_or_test;
reg [15:0] a, b;
wire [15:0] c;

sixteen_bit_or gate1 (a, b, c);

integer i;
initial begin
	for (i = 0; i < 65536; i = i + 1) 
		begin
			{a, b} = i;
			#20;
		end
	end
endmodule
