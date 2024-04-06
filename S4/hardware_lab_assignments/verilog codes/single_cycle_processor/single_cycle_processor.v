//`include "instr.prog";

module instruction_mem(input [15:0] pc, output[15:0] instruction);
	reg [15:0] memory[7:0]; // give this as input then we can look at the pain
	wire [3:0] address = pc[4:1];
	initial 
		begin		
			$readmemb("instr.prog", memory, 0, 7);
//			memory[0] = 16'b0010010010001000;
//			memory[1] = 16'b0100001001000000;
//			memory[2] = 16'b0000000000000000;
//			memory[3] = 16'b0000000000000000;
//			memory[4] = 16'b0000000000000000;
//			memory[5] = 16'b0000000000000000;
//			memory[6] = 16'b0000000000000000;
//			memory[7] = 16'b0000000000000000;		
		end
	assign instruction = memory[address];
endmodule

module register_file(input clk, input pc, input write_en, input [2:0] write_destination, input [15:0] write_data, input [2:0] read_address_1, output [15:0] read_data_1, input [2:0] read_address_2, output [15:0] read_data_2);
	reg [15:0]reg_array[7:0];
	integer i;
	initial begin
		for(i = 0; i < 7; i = i + 1)
			begin
				reg_array[i] = 16'd0;
				reg_array[7] = pc;
			end
	end
	always@(posedge clk)
		begin
			if(write_en)
				begin
					reg_array[write_destination] = write_data;
				end
			end 
	assign read_data_1 = reg_array[read_address_1];
	assign read_data_2 = reg_array[read_address_2];	
endmodule

module data_memory(input clk, input mem_read, input [15:0] mem_read_address, input mem_write, input [15:0] mem_write_data, output [15:0] mem_read_data);
	reg[15:0]mem[7:0];
	wire[2:0] address = mem_read_address[2:0];
	integer f;
	initial 
		begin
			$readmemb("data.data", mem);
			f = $fopen("storage.o");
			$fmonitor(f, "time = %d\n", $time, 
		   "\tmemory[0] = %b\n", mem[0],   
		   "\tmemory[1] = %b\n", mem[1],
		   "\tmemory[2] = %b\n", mem[2],
		   "\tmemory[3] = %b\n", mem[3],
		   "\tmemory[4] = %b\n", mem[4],
		   "\tmemory[5] = %b\n", mem[5],
		   "\tmemory[6] = %b\n", mem[6],
		   "\tmemory[7] = %b\n", mem[7]);
		   #160;
		   $fclose(f);
		end
	always@(posedge clk)
		begin
			if(mem_write)
				begin
					mem[address] = mem_write_data;
				end			
		end
		// I still have to add stuff here the printing registers part into a file or smt
		assign mem_read_data = (mem_read == 1'b1) ? mem[address] : 16'd0;
endmodule

module alu(input [15:0] a, b, input [1:0] alu_control, output reg [15:0] result, output zero);

always @(*)
	begin 
		 case(alu_control)
			 2'b00: result = a + b; // add
			 2'b01: result = a - b; // sub;
			 2'b10: result = ~(a & b); // nand
				default: result = a + b; // add
		endcase
	end
	assign zero = (result == 16'd0) ? 1'b1 : 1'b0;
endmodule

module data_path(input clk, input jump, beq, mem_write, mem_read, alu_src, reg_dest, mem_to_reg, reg_write, input[1:0] alu_op, output[3:0] opcode);
	reg[15:0] pc;
	reg  [15:0] pc_current;
	wire [15:0] pc_next, pc2;
	wire [15:0] instr;
	wire [2:0] reg_write_dest;
	wire [15:0] reg_write_data;
	wire [2:0] reg_read_addr_1;
	wire [15:0] reg_read_data_1;
	wire [2:0] reg_read_addr_2;
	wire [15:0] reg_read_data_2;
	wire [15:0] ext_im, read_data_2;
	wire [1:0] ALU_Control;
	wire [15:0] ALU_out, reg_write_data_another;
	wire zero_flag;
	wire [15:0] PC_j, PC_beq, PC_2beq;
	wire beq_control;
	wire [10:0] jump_shift; // This is 11 bit i think
	wire [15:0] mem_read_data;
	
	initial 
		begin
			pc_current <= 16'b0;
		end
	always@(*)
		begin
			pc_current <= pc_next;
		end
		
	assign pc2 = pc_current + 16'd2;
	
	instruction_mem instance1 (pc_current, instr); // we have to make this work somehow
	
	assign jump_shift = {instr[9:0], 1'b0}; // same with this also has to be 10 bit i think, Might not need this check later tho
	
	assign reg_write_dest = (reg_dest == 1'b1) ? instr[5:3] : instr[8:6];
	
	assign reg_read_addr_1 = instr[11:9];
	assign reg_read_addr_2 = instr[8:6];
	
	register_file instance2 (clk, pc_current, reg_write, reg_write_dest, reg_write_data, reg_read_addr_1, reg_read_data_1, reg_read_addr_2, reg_read_data_2); // we havee to make this work somehow too
	
	assign ext_im = (jump == 1'b1) ? {{6{instr[8]}},instr[8:0]} : {{10{instr[5]}},instr[5:0]}; 
	
	assign read_data_2 = (alu_src == 1'b1) ? ext_im : reg_read_data_2;
	
	assign reg_write_data_another = (jump == 1'b1) ? pc2 : ALU_out;
	
	alu instance3 (reg_read_data_1, read_data_2, alu_op, ALU_out, zero_flag);
	
	assign PC_beq = pc_current + ext_im;
	
	assign beq_control = beq & zero_flag;	
	
	assign PC_2beq = (beq_control == 1'b1) ? PC_beq : pc2;
	
	assign PC_j = pc_current + ext_im;
	
	assign pc_next = (jump == 1'b1) ? PC_j : PC_2beq;
	
	data_memory instance4 (clk, mem_read, ALU_out, mem_write, reg_read_data_2, mem_read_data);
	
	assign reg_write_data = (mem_to_reg == 1'b1) ? mem_read_data : reg_write_data_another;
	
	assign opcode = instr[15:12];
endmodule

module control_fsm (input[3:0] opcode, output reg [2:0] alu_op, output reg jal, beq,  mem_read, mem_write, alu_src, reg_dest, mem_to_reg, reg_write);
	always@(*)
		begin
			case(opcode)
				4'b0000: // Add
					begin
						reg_dest = 1;
						alu_src = 0;
						mem_to_reg = 0;
						reg_write = 1;
						mem_read = 0;
						mem_write = 0;
						beq = 0;
						jal = 0;
						alu_op = 0;
					end
				4'b0010: // NDU
					begin
						reg_dest = 1;
						alu_src = 0;
						mem_to_reg = 0;
						reg_write = 1;
						mem_read = 0;
						mem_write = 0;
						beq = 0;
						jal = 0;
						alu_op = 2;
					end
				4'b0100: // LW
					begin
						reg_dest = 0;
						alu_src = 1;
						mem_to_reg = 1;
						reg_write = 1;
						mem_read = 1;
						mem_write = 0;
						beq = 0;
						jal = 0;
						alu_op = 0;
					end
				4'b0101: // SW
					begin
						reg_dest = 0;
						alu_src = 1;
						mem_to_reg = 0;
						reg_write = 0;
						mem_read = 0;
						mem_write = 1;
						beq = 0;
						jal = 0;
						alu_op = 0;
					end
				4'b1100: // BEQ
					begin
						reg_dest = 0;
						alu_src = 0;
						mem_to_reg = 0;
						reg_write = 0;
						mem_read = 0;
						mem_write = 0;
						beq = 1;
						jal = 0;
						alu_op = 1;
					end
				4'b1000: // JAL
					begin
						reg_dest = 0;
						alu_src = 1;
						mem_to_reg = 0;
						reg_write = 1;
						mem_read = 0;
						mem_write = 0;
						beq = 0;
						jal = 1;
						alu_op = 0;
					end	
				default:
					begin // Add
						reg_dest = 1;
						alu_src = 0;
						mem_to_reg = 0;
						reg_write = 1;
						mem_read = 0;
						mem_write = 0;
						beq = 0;
						jal = 0;
						alu_op = 0;
					end
		endcase
	end
endmodule

module single_cycle_processor(input clk);
	wire jump, beq, mem_write, mem_read, alu_src, reg_dest, mem_to_reg, reg_write;
	wire[1:0] alu_op;
	wire[3:0] opcode;
	
	data_path instance1 (clk, jump, beq, mem_write, mem_read, alu_src, reg_dest, mem_to_reg, reg_write, alu_op, opcode);
	control_fsm instance2 (opcode, alu_op, jump, beq, mem_write, mem_read, alu_src, reg_dest, mem_to_reg, reg_write);	
endmodule
