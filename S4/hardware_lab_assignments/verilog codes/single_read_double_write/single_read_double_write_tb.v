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

	