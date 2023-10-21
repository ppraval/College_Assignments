transcript on
if {[file exists rtl_work]} {
	vdel -lib rtl_work -all
}
vlib rtl_work
vmap work rtl_work

vlog -vlog01compat -work work +incdir+D:/CollegeAssignments/Verilog\ Codes/4_bit_adder_and_sub {D:/CollegeAssignments/Verilog Codes/4_bit_adder_and_sub/four_bit_add_and_sub.v}

vlog -vlog01compat -work work +incdir+D:/CollegeAssignments/Verilog\ Codes/4_bit_adder_and_sub {D:/CollegeAssignments/Verilog Codes/4_bit_adder_and_sub/four_bit_add_and_sub_test.v}

vsim -t 1ps -L altera_ver -L lpm_ver -L sgate_ver -L altera_mf_ver -L altera_lnsim_ver -L fiftyfivenm_ver -L rtl_work -L work -voptargs="+acc"  four_bit_add_and_sub_test

add wave *
view structure
view signals
run -all
