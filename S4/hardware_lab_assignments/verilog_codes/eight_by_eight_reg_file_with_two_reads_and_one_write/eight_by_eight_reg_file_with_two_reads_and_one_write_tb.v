module eight_by_eight_reg_file_with_two_reads_and_one_write_tb #(parameter width = 8);
	reg [width - 1:0] write_data;
	reg [2:0] read_port_1, read_port_2, write_port;
	
	reg write_enable, reset, clk;
	wire [width - 1:0] read_data_1, read_data_2;
	
	eight_by_eight_reg_file_with_two_reads_and_one_write instance1 (write_data, read_port_1, read_port_2, write_port, write_enable, reset, clk, read_data_1, read_data_2);
	
	initial clk = 1'b1;
	initial 
		begin
			write_data = 100;
			write_port = 3'b000;
			read_port_1 = 3'b000;
			read_port_2 = 3'b000;
			write_enable = 1;
			reset = 0;
			#10;
			
			write_data = 50;
			write_port = 3'b001;
			read_port_1 = 3'b000;
			read_port_2 = 3'b000;
			write_enable = 1;
			reset = 0;
			#10;
			
			write_data = 100;
			write_port = 3'b010;
			read_port_1 = 3'b000;
			read_port_2 = 3'b001;
			write_enable = 0;
			reset = 0;
			#10;
			
			write_data = 20;
			write_port = 3'b011;
			read_port_1 = 3'b000;
			read_port_2 = 3'b001;
			write_enable = 0;
			reset = 1;
			#10;
			
			write_data = 100;
			write_port = 3'b010;
			read_port_1 = 3'b000;
			read_port_2 = 3'b001;
			write_enable = 0;
			reset = 0;
			#10;
		end
	
	always #5 clk <= ~clk;
endmodule
