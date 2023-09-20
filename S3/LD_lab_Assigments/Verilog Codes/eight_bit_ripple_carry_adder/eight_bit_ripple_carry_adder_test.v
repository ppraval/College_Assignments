module eight_bit_ripple_carry_adder_test;
reg [7:0] a, b;
reg cin;
wire [8:0] s;

eight_bit_ripple_carry_adder gate1 (a, b, cin, s);
integer i;
initial begin
	for (i = 0; i < 65536; i = i + 1) 
		begin
			{a, b} = i;
			#20;
		end
	end
endmodule
