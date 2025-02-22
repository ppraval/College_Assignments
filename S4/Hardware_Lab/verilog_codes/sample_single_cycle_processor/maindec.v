`timescale 1ns / 1ps
module maindec(
input [5:0] op, funct,
output memwrite,
output branch, alusrc,
output regwrite, spregwrite,
output [1:0] regdst, 
output memtoreg,
output jump, jal,
output [3:0] aluop,
output reg spra,
output readhilo
);
reg [14:0] controls;
assign {regwrite, regdst, alusrc,branch, memwrite, memtoreg, jump, jal, aluop, spregwrite, readhilo} = controls;
always @(*)
   case(op)
      6'b000000: 
      	begin
      		case(funct)
      			6'b011000: controls <= 15'b101000000001010; //mult
      			6'b011010: controls <= 15'b101000000001010; //div
      			default:   
      			  begin
      			    case(funct)
      			      6'b010000: 
      			        begin
      			          spra <= 1'b1;
      			          controls <= 15'b101000000001001;
      			        end
      			      6'b010010: 
      			        begin
      			          spra <= 1'b0;
      			          controls <= 15'b101000000001001;
      			        end
      			      default: controls <= 15'b101000000001000; //other R-type
      			    endcase
      			  end
      		endcase
      	end
      6'b100011: controls <= 15'b100100100000000; //LW
      6'b101011: controls <= 15'b000101000000000; //SW
      6'b000100: controls <= 15'b000010000000100; //BEQ
      6'b001000: controls <= 15'b100100000000000; //ADDI
      6'b000010: controls <= 15'b000000010000000; //J
      6'b000011: controls <= 15'b111000011000000; //JAL
      6'b001100: controls <= 15'b100100000010000; //ANDI
      6'b001101: controls <= 15'b100100000010100; //ORI
      6'b001010: controls <= 15'b100100000011100; //SLTI
      6'b001111: controls <= 15'b100100000100000; //LUI
      default:   controls <= 15'bxxxxxxxxxxxxxx; //???
    endcase
endmodule
