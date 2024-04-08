`timescale 1ns / 1ps
module spregfile(
input clk, 
input we, 
input ra, 
input  [31:0] wd0, wd1, 
output [31:0] rd
);
reg [31:0] rf[1:0];
always @(posedge clk)
    if (we == 1'b1)
      begin
        rf[1'b0] <= wd0;
        rf[1'b1] <= wd1;
      end
   assign rd = (ra != 1'b0) ? rf[1'b1] : rf[1'b0];
endmodule
