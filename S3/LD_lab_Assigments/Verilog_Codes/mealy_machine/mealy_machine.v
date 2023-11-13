module mealy_machine(input wire in,rst,clk, output reg out);

	reg[1:0] p;

	always @(posedge clk) begin
		if (rst) begin
			out  = 0;
			p = 2'b01;
		end
		else if (in == 0) begin
				case (p)
					2'b01: begin
						out = 0;
						p = 2'b10;
						end
					2'b10:begin 
						out = 0;
						p = 2'b10;
						end
					2'b11:begin 
						out = 1;
						p = 2'b10;
						end
				endcase
			end
		else if (in == 1) begin
				case (p)
					2'b01:begin
						out = 0;
						p = 2'b11;
						end
					2'b10:begin
						out = 1;
						p = 2'b11;
						end
					2'b11:begin
						out = 0;
						p = 2'b11;
						end
				endcase
			end
		end
endmodule
