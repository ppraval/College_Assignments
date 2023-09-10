transcript on
if {[file exists rtl_work]} {
	vdel -lib rtl_work -all
}
vlib rtl_work
vmap work rtl_work

vlog -vlog01compat -work work +incdir+D:/CollegeAssignments/Verilog\ Codes/eight_by_one_mux {D:/CollegeAssignments/Verilog Codes/eight_by_one_mux/eight_by_one_mux.v}

vlog -vlog01compat -work work +incdir+D:/CollegeAssignments/Verilog\ Codes/eight_by_one_mux {D:/CollegeAssignments/Verilog Codes/eight_by_one_mux/eight_by_one_mux_test.v}

vsim -t 1ps -L altera_ver -L lpm_ver -L sgate_ver -L altera_mf_ver -L altera_lnsim_ver -L fiftyfivenm_ver -L rtl_work -L work -voptargs="+acc"  eight_by_one_mux_test

add wave *
view structure
view signals
run -all
