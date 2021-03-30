transcript on
if {[file exists rtl_work]} {
	vdel -lib rtl_work -all
}
vlib rtl_work
vmap work rtl_work

vlog -vlog01compat -work work +incdir+E:/Project/AX530/verilog/6_pll_test/ipcore {E:/Project/AX530/verilog/6_pll_test/ipcore/pll.v}
vlog -vlog01compat -work work +incdir+E:/Project/AX530/verilog/6_pll_test {E:/Project/AX530/verilog/6_pll_test/pll_test.v}
vlog -vlog01compat -work work +incdir+E:/Project/AX530/verilog/6_pll_test/db {E:/Project/AX530/verilog/6_pll_test/db/pll_altpll.v}

vlog -vlog01compat -work work +incdir+E:/Project/AX530/verilog/6_pll_test/simulation/modelsim {E:/Project/AX530/verilog/6_pll_test/simulation/modelsim/pll_test.vt}

vsim -t 1ps -L altera_ver -L lpm_ver -L sgate_ver -L altera_mf_ver -L altera_lnsim_ver -L cycloneive_ver -L rtl_work -L work -voptargs="+acc"  pll_test_tb

add wave *
view structure
view signals
run -all
