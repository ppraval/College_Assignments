`timescale 1ns / 1ps
module mux3 #(parameter WIDTH = 8)
             (input [WIDTH-1:0] d0, d1, d2,
              input [1:0] s, 
              output [WIDTH-1:0] y);

  assign y = (s == 2'b00) ? d0 : ((s == 2'b01) ? d1 : d2); 
endmodule
