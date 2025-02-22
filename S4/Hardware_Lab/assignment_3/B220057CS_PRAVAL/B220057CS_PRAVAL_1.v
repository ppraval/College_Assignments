module single_read_single_write_reg  #(parameter width = 16) (input [width - 1:0] write_data, input clk, reset, write_enable, output reg [width - 1:0] read_data);
	
	reg [width - 1:0] register;
	initial register = 16'b0;
	
	always@(posedge clk or posedge reset) begin
		if(reset) 
			begin
				register = 16'b0;
				read_data = 16'bx;
			end
		else 
			begin
				if(write_enable) 
					begin
						register = write_data;
					end
				else 
					begin
						read_data = register;
					end
			end
		end
endmodule

module single_read_single_write_reg_tb #(parameter width=16);
	reg [width - 1:0] write_data;
	reg write_enable, clk, reset;
	wire [width - 1:0] read_data;
	single_read_single_write_reg instance1 (write_data, clk, reset, write_enable, read_data);
	
	initial clk = 1'b1;
	
	initial 
		begin
			write_data = 25;
			reset = 0;
			write_enable = 1;
			#10;
			
			write_data = 50;
			reset = 0;
			write_enable = 0;
			#10;
			
			write_data = 75;
			reset = 0;
			write_enable = 1;
			
			#10;
		end

	always #5 clk<=~clk;
	
endmodule