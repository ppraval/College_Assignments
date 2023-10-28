module four_bit_decrementer_test;
reg [3:0] a;
reg cin;
wire [4:0] s;

four_bit_decrementer gate1 (a, cin, s);
integer i;
initial begin
	for (i = 0; i < 16; i = i + 1) 
		begin
			{a} = i;
			#20;
		end
	end
endmodule
