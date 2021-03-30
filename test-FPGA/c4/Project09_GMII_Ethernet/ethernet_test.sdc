#************************************************************
# THIS IS A WIZARD-GENERATED FILE.                           
#
# Version 12.1 Build 177 11/07/2012 SJ Full Version
#
#************************************************************

# Copyright (C) 1991-2012 Altera Corporation
# Your use of Altera Corporation's design tools, logic functions 
# and other software and tools, and its AMPP partner logic 
# functions, and any output files from any of the foregoing 
# (including device programming or simulation files), and any 
# associated documentation or information are expressly subject 
# to the terms and conditions of the Altera Program License 
# Subscription Agreement, Altera MegaCore Function License 
# Agreement, or other applicable license agreement, including, 
# without limitation, that your use is for the sole purpose of 
# programming logic devices manufactured by Altera and sold by 
# Altera or its authorized distributors.  Please refer to the 
# applicable agreement for further details.



# Clock constraints

create_clock -name "e_rxc" -period 8.000ns [get_ports {e_rxc}] -waveform {0.000 4.000}


# Automatically constrain PLL and other generated clocks
derive_pll_clocks -create_base_clocks

# Automatically calculate clock uncertainty to jitter and other effects.
derive_clock_uncertainty

# tsu/th constraints

set_input_delay -clock "e_rxc" -max 6ns [get_ports {e_rxd[0]}] 
set_input_delay -clock "e_rxc" -min 6.000ns [get_ports {e_rxd[0]}] 
set_input_delay -clock "e_rxc" -max 6ns [get_ports {e_rxd[1]}] 
set_input_delay -clock "e_rxc" -min 6.000ns [get_ports {e_rxd[1]}] 
set_input_delay -clock "e_rxc" -max 6ns [get_ports {e_rxd[2]}] 
set_input_delay -clock "e_rxc" -min 6.000ns [get_ports {e_rxd[2]}] 
set_input_delay -clock "e_rxc" -max 6ns [get_ports {e_rxd[3]}] 
set_input_delay -clock "e_rxc" -min 6.000ns [get_ports {e_rxd[3]}] 
set_input_delay -clock "e_rxc" -max 6ns [get_ports {e_rxd[4]}] 
set_input_delay -clock "e_rxc" -min 6.000ns [get_ports {e_rxd[4]}] 
set_input_delay -clock "e_rxc" -max 6ns [get_ports {e_rxd[5]}] 
set_input_delay -clock "e_rxc" -min 6.000ns [get_ports {e_rxd[5]}] 
set_input_delay -clock "e_rxc" -max 6ns [get_ports {e_rxd[6]}] 
set_input_delay -clock "e_rxc" -min 6.000ns [get_ports {e_rxd[6]}] 
set_input_delay -clock "e_rxc" -max 6ns [get_ports {e_rxd[7]}] 
set_input_delay -clock "e_rxc" -min 6.000ns [get_ports {e_rxd[7]}] 
set_input_delay -clock "e_rxc" -max 6ns [get_ports {e_rxdv}] 
set_input_delay -clock "e_rxc" -min 6.000ns [get_ports {e_rxdv}] 


# tco constraints

set_output_delay -clock "e_rxc" -max 2ns [get_ports {e_txd[0]}] 
set_output_delay -clock "e_rxc" -min -0.000ns [get_ports {e_txd[0]}] 
set_output_delay -clock "e_rxc" -max 2ns [get_ports {e_txd[1]}] 
set_output_delay -clock "e_rxc" -min -0.000ns [get_ports {e_txd[1]}] 
set_output_delay -clock "e_rxc" -max 2ns [get_ports {e_txd[2]}] 
set_output_delay -clock "e_rxc" -min -0.000ns [get_ports {e_txd[2]}] 
set_output_delay -clock "e_rxc" -max 2ns [get_ports {e_txd[3]}] 
set_output_delay -clock "e_rxc" -min -0.000ns [get_ports {e_txd[3]}] 
set_output_delay -clock "e_rxc" -max 2ns [get_ports {e_txd[4]}] 
set_output_delay -clock "e_rxc" -min -0.000ns [get_ports {e_txd[4]}] 
set_output_delay -clock "e_rxc" -max 2ns [get_ports {e_txd[5]}] 
set_output_delay -clock "e_rxc" -min -0.000ns [get_ports {e_txd[5]}] 
set_output_delay -clock "e_rxc" -max 2ns [get_ports {e_txd[6]}] 
set_output_delay -clock "e_rxc" -min -0.000ns [get_ports {e_txd[6]}] 
set_output_delay -clock "e_rxc" -max 2ns [get_ports {e_txd[7]}] 
set_output_delay -clock "e_rxc" -min -0.000ns [get_ports {e_txd[7]}] 
set_output_delay -clock "e_rxc" -max 2ns [get_ports {e_txen}] 
set_output_delay -clock "e_rxc" -min -0.000ns [get_ports {e_txen}] 


# tpd constraints

set_max_delay 1.000ns -from [get_ports {e_rxc}] -to [get_ports {e_gtxc}]


