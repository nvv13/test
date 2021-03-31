transcript on
if {[file exists rtl_work]} {
	vdel -lib rtl_work -all
}
vlib rtl_work
vmap work rtl_work

vlog -vlog01compat -work work +incdir+/home/nvv/fpga_prg/m2/up_clk1 {/home/nvv/fpga_prg/m2/up_clk1/up_clk1.v}

