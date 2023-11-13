module four_by_four_multiplier (input [3:0] a, b, output reg [7:0] p);
	reg [7:0] input_a, input_b;
	integer i;
	always @ (*)
		begin
			input_a = {1'b0, 1'b0, 1'b0, 1'b0, a};
			input_b = {1'b0, 1'b0, 1'b0, 1'b0, b};
			for(i = 1; i < input_b; i = i + 1)
				begin
					input_a = input_a + input_a;
				end
			p = input_a;
		end
endmodule
