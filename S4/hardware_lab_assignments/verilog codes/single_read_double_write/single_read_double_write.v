module single_read_double_write #(parameter width = 16) (input [width - 1:0] write_data_1, write_data_2, input write_enable, clk, reset, output reg [width - 1:0] read_data);
	reg state;
	initial state = 1'b0;
	reg [width - 1:0] register;
	initial register = 16'b0;
	always@(posedge clk or posedge reset) 
		begin
			if(reset) 
				begin
					register = 16'b0;
					read_data = 16'bx;
					state = 1'b0;
				end
			else 
				begin
					if(write_enable) begin
						if(state == 1'b0) 
							begin
								register = write_data_1;
								state = 1'b1;
							end
						else 
							begin
								register = write_data_2;
								state = 1'b0;
							end
					end
					else 
						begin
							read_data = register;
						end
				end
		end
endmodule
