module big_reg_file_with_four_reads_and_two_writes #(parameter width = 64, depth = 32) (input [width - 1:0] write_data_1, write_data_2, input [5:0] write_port_1, write_port_2, read_port_1, read_port_2, read_port_3, read_port_4, input write_enable, read_enable, clk, reset, output reg [width - 1:0] read_data_1, read_data_2, read_data_3, read_data_4); 
	reg [width - 1:0] register[0:depth - 1];
	integer i;
	initial 
		begin
		for(i = 0; i < depth; i = i + 1) 
			begin
				register[i] = 0;
			end
	end
	always@(posedge clk or posedge reset) 
		begin
			if(reset) 
				begin
					read_data_1 = 64'bx;
					read_data_2 = 64'bx;
					read_data_3 = 64'bx;
					read_data_4 = 64'bx;
					for(i = 0; i < depth; i = i + 1) 
						begin
							register[i]=0;
						end
				end
			else 
				begin
				if(write_enable && read_enable) 
					begin
						if(write_port_1 == write_port_2) 
							begin
								register[write_port_1] = write_data_1;
								read_data_1 = register[read_port_1];
								read_data_2 = register[read_port_2];
								read_data_3 = register[read_port_3];
								read_data_4 = register[read_port_4];
							end
						else 
							begin
								register[write_port_1] = write_data_1;
								register[write_port_2] = write_data_2;
								read_data_1 = register[read_port_1];
								read_data_2 = register[read_port_2];
								read_data_3 = register[read_port_3];
								read_data_4 = register[read_port_4];
							end
					end
				else if(write_enable) 
					begin
						if(write_port_1 == write_port_2) 
							begin
								register[write_port_1] = write_data_1;
							end
					else 
						begin
							register[write_port_1] = write_data_1;
							register[write_port_2] = write_data_2;
						end
					end
				else if(read_enable) 
					begin
						read_data_1 = register[read_port_1];
						read_data_2 = register[read_port_2];
						read_data_3 = register[read_port_3];
						read_data_4 = register[read_port_4];
					end
				end
		end
endmodule
