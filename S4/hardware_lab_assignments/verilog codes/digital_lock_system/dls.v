`timescale 1ns/1ns
module dls(clock,clear,switch,alarm,locked);
input wire clock,clear;
input wire[7:0] switch;
output reg alarm,locked;

wire mux_out,allsw,anysw,codesw,timeout,entimer,sel,alarmc,lockedc;
wire[1:0] selsw;

and a1(allsw,switch[7],switch[6],switch[5],switch[4],switch[3],switch[2],switch[1],switch[0]);
multi m(switch[3:0],selsw,mux_out);

edge_det e1(mux_out,clock,codesw);
edge_det e2(allsw,clock,anysw);

ctrl c1(timeout,codesw,anysw,clock,clear,alarmc,lockedc,entimer,selsw);
cntr cn1(entimer,clock,timeout);

always @* begin
locked<=lockedc;
alarm<=alarmc;
end
endmodule

module ctrl(timeout,codesw,anysw,clock,reset,alarm,locked,entimer,selsw);
input wire timeout,codesw,anysw,clock,reset;
output reg alarm,locked,entimer;
output reg[1:0] selsw;
reg[2:0] state;

initial begin
state<=0;
end

always @(posedge clock) begin
	if(reset) begin
		state<=0;
	end
	else begin
		case(state) 
			0: begin
				if({codesw,anysw}==2'b01) begin
					state<=4;
					end
				if({codesw,anysw}==2'b11) begin
					state<=1;
					end
				if({codesw,anysw}==2'b00) begin
					state<=0;
					end
				if({codesw,anysw}==2'b10) begin
					state<=0;
					end
				end
			1: begin
				if({codesw,anysw}==2'b01) begin
					state<=4;
					end
				if({codesw,anysw}==2'b11) begin
					state<=2;
					end
				if({codesw,anysw}==2'b00) begin
					state<=1;
					end
				if({codesw,anysw}==2'b10) begin
					state<=1;
					end
				end
			2: begin
				if({codesw,anysw}==2'b01) begin
					state<=4;
					end
				if({codesw,anysw}==2'b11) begin
					state<=3;
					end
				if({codesw,anysw}==2'b00) begin
					state<=2;
					end
				if({codesw,anysw}==2'b10) begin
					state<=2;
					end
				end
			3: begin
				if({codesw,anysw}==2'b01) begin
					state<=4;
					end
				if({codesw,anysw}==2'b11) begin
					state<=5;
					end
				if({codesw,anysw}==2'b00) begin
					state<=3;
					end
				if({codesw,anysw}==2'b10) begin
					state<=3;
					end
				end
			5: begin
				if(timeout) begin
					state<=0;
					end
				if(!timeout) begin
					state<=5;
					end
				end
			4:state<=4;
			default:state<=state;
			endcase
	end
end
always @(state) begin
	case (state) 
		0: begin
			alarm<=1;
			locked<=1;
			entimer<=0;
			selsw<=0;
			end
		1: begin
			alarm<=1;
			locked<=1;
			entimer<=0;
			selsw<=1;
			end
		2: begin
			alarm<=1;
			locked<=1;
			entimer<=0;
			selsw<=2;
			end
		3: begin
			alarm<=1;
			locked<=1;
			entimer<=0;
			selsw<=3;
			end
		4: begin
			alarm<=0;
			locked<=1;
			entimer<=0;
			selsw<=0;
			end
		5: begin
			alarm<=1;
			locked<=0;
			entimer<=1;
			selsw<=0;
			end
		default:begin
					end
		endcase
end
endmodule


module cntr(start,clock,timeout);
input wire start,clock;
output reg timeout;
reg[1:0] count;
initial begin
	count<=0;
	timeout<=0;
	end

always @(posedge clock) begin
	if(timeout) begin
		count<=0;
	end
	else if (start) begin
		count<=count+2'b01;
		timeout<=0;
	end
	if(count==2'b11) begin
		timeout<=1;
		end
end
endmodule


module edge_det(edge_in,clock,detected);

input wire edge_in,clock;
output reg detected;
reg prev=0;
always @(posedge clock) begin

	if(edge_in!=prev) begin
		detected<=1;
		prev<=edge_in;
	end
	else begin
		prev<=edge_in;
		detected<=0;
		end
end

endmodule

module multi(in,sel,out);
input wire[3:0] in;
input wire[1:0] sel;
output reg out;

always @* begin
	case (sel)
		0:out=in[0];
		1:out=in[1];
		2:out=in[2];
		3:out=in[3];
	endcase
end
endmodule


module dls_tb();
reg clock,clear;
reg[7:0] switch;
wire alarm,locked;
dls d1(clock,clear,switch,alarm,locked);
always #5 clock=~clock;
initial begin
clock<=1;
clear<=1;
switch<=8'hxx;
#10
clear<=0;
switch<=8'b11111111;
#10
switch<=8'b10111111;
#10
switch<=8'b11111101;
#10
switch<=8'b11011101;
#10
switch<=8'b10111110;
#10
switch<=8'b11110111;
end
endmodule


