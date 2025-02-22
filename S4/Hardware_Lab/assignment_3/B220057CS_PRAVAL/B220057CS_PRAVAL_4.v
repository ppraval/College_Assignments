module eight_by_eight_reg_file_with_two_reads_and_one_write #(parameter width = 8, depth = 8) (input [width - 1:0] write_data, input [2:0] read_port_1, read_port_2, write_port, input write_enable, reset, clk, output reg [width - 1:0] read_data_1, read_data_2);
	reg [width - 1:0] register [0:depth - 1];
	integer i;
	initial begin
	for(i = 0; i < depth; i = i + 1) 
		begin
			register[i]=0;
		end
	end
	
	always@(posedge clk or posedge reset) 
	begin
		if(reset) 
			begin
				read_data_1 = 8'bx;
				read_data_2 = 8'bx;
				for(i = 0; i < depth; i = i + 1) 
					begin
					register[i] = 0;
					end
			end
		else 
			begin
				if(write_enable) 
					begin
						register[write_port] = write_data;
					end
				else 
					begin
						read_data_1 = register[read_port_1];
						read_data_2 = register[read_port_2];
					end
			end
	end
endmodule

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