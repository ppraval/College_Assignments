transcript on
if {[file exists rtl_work]} {
	vdel -lib rtl_work -all
}
vlib rtl_work
vmap work rtl_work

vlog -vlog01compat -work work +incdir+D:/CollegeAssignments/Verilog\ Codes/four_bit_ripple_adder {D:/CollegeAssignments/Verilog Codes/four_bit_ripple_adder/four_bit_ripple_adder.v}

vlog -vlog01compat -work work +incdir+D:/CollegeAssignments/Verilog\ Codes/four_bit_ripple_adder {D:/CollegeAssignments/Verilog Codes/four_bit_ripple_adder/four_bit_ripple_adder_test.v}

vsim -t 1ps -L altera_ver -L lpm_ver -L sgate_ver -L altera_mf_ver -L altera_lnsim_ver -L fiftyfivenm_ver -L rtl_work -L work -voptargs="+acc"  four_bit_ripple_adder_test

add wave *
view structure
view signals
run -all
