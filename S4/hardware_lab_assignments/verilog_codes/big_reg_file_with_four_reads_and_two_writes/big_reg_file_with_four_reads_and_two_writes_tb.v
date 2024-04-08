module big_reg_file_with_four_reads_and_two_writes_tb #(parameter width = 64);
	reg [width - 1:0]write_data_1, write_data_2;
	reg [5:0] write_port_1, write_port_2, read_port_1, read_port_2, read_port_3, read_port_4;
	reg write_enable, read_enable, clk, reset;
	wire [width - 1:0]read_data_1, read_data_2, read_data_3, read_data_4;
	
	big_reg_file_with_four_reads_and_two_writes instance1 (write_data_1, write_data_2, write_port_1, write_port_2, read_port_1, read_port_2, read_port_3, read_port_4, write_enable, read_enable, clk, reset, read_data_1, read_data_2, read_data_3, read_data_4);
	
	initial clk=1'b1;
	
	initial 
		begin
		 write_data_1 = 100;
		 write_data_2 = 75;
		 write_port_1 = 0;
		 write_port_2 = 1;
		 read_port_1 = 0;
		 read_port_2 = 1;
		 read_port_3 = 2;
		 read_port_4 = 3;
		 write_enable = 1;
		 read_enable = 1;
		 reset = 0;
		 #10;
		end
	always #5 clk <= ~clk;
endmodule
