transcript on
if {[file exists rtl_work]} {
	vdel -lib rtl_work -all
}
vlib rtl_work
vmap work rtl_work

vlog -vlog01compat -work work +incdir+D:/College_Assignments/S3/LD_lab_Assigments/Verilog\ Codes/BCD_to_excess_three {D:/College_Assignments/S3/LD_lab_Assigments/Verilog Codes/BCD_to_excess_three/bcd_to_excess_three.v}

vlog -vlog01compat -work work +incdir+D:/College_Assignments/S3/LD_lab_Assigments/Verilog\ Codes/BCD_to_excess_three {D:/College_Assignments/S3/LD_lab_Assigments/Verilog Codes/BCD_to_excess_three/bcd_to_excess_three_test.v}

vsim -t 1ps -L altera_ver -L lpm_ver -L sgate_ver -L altera_mf_ver -L altera_lnsim_ver -L cyclonev_ver -L cyclonev_hssi_ver -L cyclonev_pcie_hip_ver -L rtl_work -L work -voptargs="+acc"  bcd_to_excess_three_test

add wave *
view structure
view signals
run -all
