module sixteen_bit_decrementer_test;
reg [15:0] a;
reg cin;
wire [16:0] s;

sixteen_bit_decrementer gate1 (a, cin, s);
integer i;
initial begin
	for (i = 0; i < 65536; i = i + 1) 
		begin
			{a} = i;
			#20;
		end
	end
endmodule
