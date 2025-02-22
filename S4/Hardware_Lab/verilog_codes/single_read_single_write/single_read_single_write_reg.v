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
