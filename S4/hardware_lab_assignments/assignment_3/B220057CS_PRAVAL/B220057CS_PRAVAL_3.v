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

module variable_length_reg_with_both_ops_tb #(parameter width = 32);
	reg [width-1:0] write_data;
	reg write_enable, read_enable, clk;
	wire [width-1:0] read_data;
	variable_length_reg_with_both_ops instance1 (write_data, clk, write_enable, read_enable, read_data);
	initial clk = 1'b1;
	initial 
		begin
			write_data = 100;
			write_enable = 1;
			read_enable = 0;
			#10;
			
			write_data = 50;
			write_enable = 0;
			read_enable = 1;
			#10;
			
			write_data = 50;
			write_enable = 0;
			read_enable = 0;
			#10;
			
			write_data = 1000;
			write_enable = 1;
			read_enable = 1;
			#10;
		end
	always #5 clk <= ~clk;
endmodule

	