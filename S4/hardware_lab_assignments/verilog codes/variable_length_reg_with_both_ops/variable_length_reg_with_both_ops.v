module variable_length_reg_with_both_ops #(parameter width = 32) (input [width - 1:0] write_data, input clk, write_enable, read_enable, output reg [width - 1:0] read_data);
	reg [width - 1:0] register;
	initial register = 32'b0;
	always@(posedge clk) 
		begin
			if(write_enable && read_enable) 
				begin
					register = write_data;
					read_data = register;
				end
			else if(write_enable) 
				begin
					register = write_data;
				end
			else if(read_enable) 
				begin
					read_data = register;
				end
		end
endmodule
