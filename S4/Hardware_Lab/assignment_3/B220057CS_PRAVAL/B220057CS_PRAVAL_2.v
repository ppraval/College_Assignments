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

module single_read_double_write_tb #(parameter width = 16);
	reg [width - 1:0] write_data_1, write_data_2;
	reg write_enable, clk, reset;
	wire [width - 1:0] read_data;
	single_read_double_write instance1 (write_data_1, write_data_2, write_enable, clk, reset, read_data);
	initial clk = 1'b1;
	initial 
		begin
			write_data_1 = 25;
			write_data_2 = 50;
			write_enable = 1;
			reset = 0;
			#10;
			
			write_data_1 = 50;
			write_data_2 = 75;
			write_enable = 1;
			reset = 0;
			#10;
			
			write_data_1 = 75;
			write_data_2 = 100;
			write_enable = 0;
			reset = 0;
			#10;
			
			write_data_1 = 100;
			write_data_2 = 125;
			write_enable = 1;
			reset = 0;
			#10;
			
			write_data_1 = 125;
			write_data_2 = 150;
			write_enable = 0;
			reset = 0;
			#10;
		end
	always #5 clk<=~clk;
	
endmodule