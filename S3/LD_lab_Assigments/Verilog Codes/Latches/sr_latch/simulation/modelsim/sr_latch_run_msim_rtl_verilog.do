transcript on
if {[file exists rtl_work]} {
	vdel -lib rtl_work -all
}
vlib rtl_work
vmap work rtl_work

vlog -vlog01compat -work work +incdir+D:/CollegeAssignments/Verilog\ Codes/Latches/sr_latch {D:/CollegeAssignments/Verilog Codes/Latches/sr_latch/sr_latch.v}

