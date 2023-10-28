module four_bit_ripple_adder_test;
reg [3:0] a, b;
reg cin;
wire [4:0] s;

four_bit_ripple_adder gate1 (a, b, cin, s);
integer i;
initial begin
	for (i = 0; i < 512; i = i + 1) 
		begin
			{a, b} = i;
			#20;
		end
	end
endmodule
