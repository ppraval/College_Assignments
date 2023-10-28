module alu(input [15:0] x, y, 
				input [3:0] ctrl, 
				output [16:0] result);
		
		wire [16:0] result_1, result_2, result_3, result_4, 
						result_5, result_6, result_7, result_8, 
						result_9, result_10, result_11, result_12, 
						result_13, result_14, result_15, result_16;								
		wire [15:0] temp_y, temp_x;
		wire [15:0] temp_1 = 16'b1111111111111110;

		sixteen_bit_negator n1(temp_y, y);
		sixteen_bit_negator n2(temp_x, x);

		sixteen_adder a (result_1, x, y, 1'b0);
		sixteen_adder b (result_2, x, temp_y, 1'b1);
		sixteen_adder c (result_3, y, temp_x, 1'b1);
		assign result_4 = 17'b00000000000000000;
		assign result_5 = 17'b00000000000000001;
		assign result_6 = 17'b01111111111111111;
		sixteen_adder d (result_7, result_4[15:0], temp_x, 1'b1);		
		sixteen_adder e (result_8, result_4[15:0], temp_y, 1'b1);
		assign result_9 = temp_x;
		assign result_10 = temp_y;
		sixteen_adder f (result_11, result_4[15:0], x, 1'b1);
		sixteen_adder g (result_12, result_4[15:0], y, 1'b1);
		sixteen_adder h (result_13, x, temp_1, 1'b1);
		sixteen_adder i (result_14, y, temp_1, 1'b1);
		sixteen_bit_and j (result_15, x, y);
		sixteen_bit_or  k (result_16, x, y);
		
		multiplexer m0 (result, result_1, result_2, result_3, result_4, result_5, result_6, result_7, result_8, result_9, result_10, result_11, result_12, result_13, result_14, result_15, result_16, ctrl);
endmodule 


module andg (output wire c,  input wire a, b);
	wire w1;
	nand(w1, a, b);
	nand(c, w1, w1);
endmodule

module org (output wire c,  input wire a, b);
	wire a_, b_;
	nand(a_, a, a);
	nand(b_, b, b);
	nand(c, a_, b_);
endmodule 


module xorg (output wire c,  input wire a, b);
	wire tr1, tr2;
	org o1 (tr1, a, b);
	nand(tr2, a, b);
	andg a1(c, tr1, tr2);
endmodule

module fulladder (output wire s, cout,  input wire a, b, cin);
	wire w1, w2, w3;
	xorg xor1(w1, a, b);
	xorg xor2(s, w1, cin);
	andg a1(w2, a, b);
	andg a2(w3, w1, cin);
	org a3(cout, w2, w3);
endmodule

module notg(output wire b,  input wire a);
	nand(b, a, a);
endmodule

module sixteen_adder(output [16:0] result,  input [15:0] x, y,  input cin);
	wire c0, c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14;
	fulladder f0 (result[0], c0, x[0], y[0], cin);
	fulladder f1 (result[1], c1, x[1], y[1], c0);
	fulladder f2 (result[2], c2, x[2], y[2], c1);
	fulladder f3 (result[3], c3, x[3], y[3], c2);
	fulladder f4 (result[4], c4, x[4], y[4], c3);
	fulladder f5 (result[5], c5, x[5], y[5], c4);
	fulladder f6 (result[6], c6, x[6], y[6], c5);
	fulladder f7 (result[7], c7, x[7], y[7], c6);
	fulladder f8 (result[8], c8, x[8], y[8], c7);
	fulladder f9 (result[9], c9, x[9], y[9], c8);
	fulladder f10 (result[10], c10, x[10], y[10], c9);
	fulladder f11 (result[11], c11, x[11], y[11], c10);
	fulladder f12 (result[12], c12, x[12], y[12], c11);
	fulladder f13 (result[13], c13, x[13], y[13], c12);
	fulladder f14 (result[14], c14, x[14], y[14], c13);
	fulladder f15 (result[15], result[16], x[15], y[15], c14);
endmodule

module sixteen_bit_negator(output [15:0] b,  input [15:0] a);
	notg g1(b[0], a[0]);
	notg g2(b[1], a[1]);
	notg g3(b[2], a[2]);
	notg g4(b[3], a[3]);
	
	notg g5(b[4], a[4]);
	notg g6(b[5], a[5]);
	notg g7(b[6], a[6]);
	notg g8(b[7], a[7]);
	
	notg g9(b[8], a[8]);
	notg g10(b[9], a[9]);
	notg g11(b[10], a[10]);
	notg g12(b[11], a[11]);
	
	notg g13(b[12], a[12]);
	notg g14(b[13], a[13]);
	notg g15(b[14], a[14]);
	notg g16(b[15], a[15]);
	
endmodule 

module sixteen_bit_and(output [16:0] c,  input [15:0] a,  b);
	andg g1(c[0], b[0], a[0]);
	andg g2(c[1], b[1], a[1]);
	andg g3(c[2], b[2], a[2]);
	andg g4(c[3], b[3], a[3]);
	
	andg g5(c[4], b[4], a[4]);
	andg g6(c[5], b[5], a[5]);
	andg g7(c[6], b[6], a[6]);
	andg g8(c[7], b[7], a[7]);
	
	andg g9(c[8], b[8], a[8]);
	andg g10(c[9], b[9], a[9]);
	andg g11(c[10], b[10], a[10]);
	andg g12(c[11], b[11], a[11]);
	
	andg g13(c[12], b[12], a[12]);
	andg g14(c[13], b[13], a[13]);
	andg g15(c[14], b[14], a[14]);
	andg g16(c[15], b[15], a[15]);
	assign c[16]=1'b0;
endmodule


module sixteen_bit_or(output [16:0] c,  input [15:0] a,  b);
	org g1(c[0], b[0], a[0]);
	org g2(c[1], b[1], a[1]);
	org g3(c[2], b[2], a[2]);
	org g4(c[3], b[3], a[3]);
	
	org g5(c[4], b[4], a[4]);
	org g6(c[5], b[5], a[5]);
	org g7(c[6], b[6], a[6]);
	org g8(c[7], b[7], a[7]);
	
	org g9(c[8], b[8], a[8]);
	org g10(c[9], b[9], a[9]);
	org g11(c[10], b[10], a[10]);
	org g12(c[11], b[11], a[11]);
	
	org g13(c[12], b[12], a[12]);
	org g14(c[13], b[13], a[13]);
	org g15(c[14], b[14], a[14]);
	org g16(c[15], b[15], a[15]);
	assign c[16]=1'b0;
endmodule

module four_bit_and(output [3:0] c,  input [3:0] a,  b);
	andg g1(c[0], b[0], a[0]);
	andg g2(c[1], b[1], a[1]);
	andg g3(c[2], b[2], a[2]);
	andg g4(c[3], b[3], a[3]);
endmodule

module mux(output wire ou,  input wire in1, in2, ctrl);
	
		wire w1, w2, ctrl_;
		notg n1 (ctrl_, ctrl);
		andg a1 (w1, in1, ctrl_);
		andg a2 (w2, in2, ctrl);
		org o1 (ou, w1, w2);
endmodule

module sixteen_mux(output o, input in1, in2, in3, in4, in5, in6, in7, in8, in9, in10, in11, in12, in13, in14, in15, in16,  
										input [3:0] ctrl);
	wire w11, w12, w13, w14, w15, w16, w17, w18;
	wire w21, w22, w23, w24;
	wire w31, w32;
	mux m1(w11, in1, in2, ctrl[0]);
	mux m2(w12, in3, in4, ctrl[0]);
	mux m3(w13, in5, in6, ctrl[0]);
	mux m4(w14, in7, in8, ctrl[0]);
	mux m5(w15, in9, in10, ctrl[0]);
	mux m6(w16, in11, in12, ctrl[0]);
	mux m7(w17, in13, in14, ctrl[0]);
	mux m8(w18, in15, in16, ctrl[0]);
	mux m9(w21, w11, w12, ctrl[1]);
	mux m10(w22, w13, w14, ctrl[1]);
	mux m11(w23, w15, w16, ctrl[1]);
	mux m12(w24, w17, w18, ctrl[1]);
	mux m13(w31, w21, w22, ctrl[2]);
	mux m14(w32, w23, w24, ctrl[2]);
	mux m15(o, w31, w32, ctrl[3]);
endmodule

module multiplexer (output [16:0] result, input [16:0] result_1, result_2, result_3, result_4, 
										result_5, result_6, result_7, result_8, 
										result_9, result_10, result_11, result_12, 
										result_13, result_14, result_15, result_16, 
										input[3:0] ctrl);
				sixteen_mux m1(result[0], result_1[0], result_2[0], result_3[0], result_4[0], 
										result_5[0], result_6[0], result_7[0], result_8[0], 
										result_9[0], result_10[0], result_11[0], result_12[0], 
										result_13[0], result_14[0], result_15[0], result_16[0], ctrl);
				sixteen_mux m2(result[1], result_1[1], result_2[1], result_3[1], result_4[1], 
										result_5[1], result_6[1], result_7[1], result_8[1], 
										result_9[1], result_10[1], result_11[1], result_12[1], 
										result_13[1], result_14[1], result_15[1], result_16[1], ctrl);
				sixteen_mux m3(result[2], result_1[2], result_2[2], result_3[2], result_4[2], 
										result_5[2], result_6[2], result_7[2], result_8[2], 
										result_9[2], result_10[2], result_11[2], result_12[2], 
										result_13[2], result_14[2], result_15[2], result_16[2], ctrl);
				sixteen_mux m4(result[3], result_1[3], result_2[3], result_3[3], result_4[3], 
										result_5[3], result_6[3], result_7[3], result_8[3], 
										result_9[3], result_10[3], result_11[3], result_12[3], 
										result_13[3], result_14[3], result_15[3], result_16[3], ctrl);
				sixteen_mux m5(result[4], result_1[4], result_2[4], result_3[4], result_4[4], 
										result_5[4], result_6[4], result_7[4], result_8[4], 
										result_9[4], result_10[4], result_11[4], result_12[4], 
										result_13[4], result_14[4], result_15[4], result_16[4], ctrl);
				sixteen_mux m6(result[5], result_1[5], result_2[5], result_3[5], result_4[5], 
										result_5[5], result_6[5], result_7[5], result_8[5], 
										result_9[5], result_10[5], result_11[5], result_12[5], 
										result_13[5], result_14[5], result_15[5], result_16[5], ctrl);
				sixteen_mux m7(result[6], result_1[6], result_2[6], result_3[6], result_4[6], 
										result_5[6], result_6[6], result_7[6], result_8[6], 
										result_9[6], result_10[6], result_11[6], result_12[6], 
										result_13[6], result_14[6], result_15[6], result_16[6], ctrl);
				sixteen_mux m8(result[7], result_1[7], result_2[7], result_3[7], result_4[7], 
										result_5[7], result_6[7], result_7[7], result_8[7], 
										result_9[7], result_10[7], result_11[7], result_12[7], 
										result_13[7], result_14[7], result_15[7], result_16[7], ctrl);
				sixteen_mux m9(result[8], result_1[8], result_2[8], result_3[8], result_4[8], 
										result_5[8], result_6[8], result_7[8], result_8[8], 
										result_9[8], result_10[8], result_11[8], result_12[8], 
										result_13[8], result_14[8], result_15[8], result_16[8], ctrl);
				sixteen_mux m10(result[9], result_1[9], result_2[9], result_3[9], result_4[9], 
										result_5[9], result_6[9], result_7[9], result_8[9], 
										result_9[9], result_10[9], result_11[9], result_12[9], 
										result_13[9], result_14[9], result_15[9], result_16[9], ctrl);
				sixteen_mux m11(result[10], result_1[10], result_2[10], result_3[10], result_4[10], 
										result_5[10], result_6[10], result_7[10], result_8[10], 
										result_9[10], result_10[10], result_11[10], result_12[10], 
										result_13[10], result_14[10], result_15[10], result_16[10], ctrl);
				sixteen_mux m12(result[11], result_1[11], result_2[11], result_3[11], result_4[11], 
										result_5[11], result_6[11], result_7[11], result_8[11], 
										result_9[11], result_10[11], result_11[11], result_12[11], 
										result_13[11], result_14[11], result_15[11], result_16[11], ctrl);
				sixteen_mux m13(result[12], result_1[12], result_2[12], result_3[12], result_4[12], 
										result_5[12], result_6[12], result_7[12], result_8[12], 
										result_9[12], result_10[12], result_11[12], result_12[12], 
										result_13[12], result_14[12], result_15[12], result_16[12], ctrl);
				sixteen_mux m14(result[13], result_1[13], result_2[13], result_3[13], result_4[13], 
										result_5[13], result_6[13], result_7[13], result_8[13], 
										result_9[13], result_10[13], result_11[13], result_12[13], 
										result_13[13], result_14[13], result_15[13], result_16[13], ctrl);
				sixteen_mux m15(result[14], result_1[14], result_2[14], result_3[14], result_4[14], 
										result_5[14], result_6[14], result_7[14], result_8[14], 
										result_9[14], result_10[14], result_11[14], result_12[14], 
										result_13[14], result_14[14], result_15[14], result_16[14], ctrl);
				sixteen_mux m16(result[15], result_1[15], result_2[15], result_3[15], result_4[15], 
										result_5[15], result_6[15], result_7[15], result_8[15], 
										result_9[15], result_10[15], result_11[15], result_12[15], 
										result_13[15], result_14[15], result_15[15], result_16[15], ctrl);
				sixteen_mux m17(result[16], result_1[16], result_2[16], result_3[16], result_4[16], 
										result_5[16], result_6[16], result_7[16], result_8[16], 
										result_9[16], result_10[16], result_11[16], result_12[16], 
										result_13[16], result_14[16], result_16[16], result_16[16], ctrl);
				
endmodule

		


 