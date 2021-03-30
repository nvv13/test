library verilog;
use verilog.vl_types.all;
entity pll_test is
    port(
        clk             : in     vl_logic;
        rst_n           : in     vl_logic;
        clkout1         : out    vl_logic;
        clkout2         : out    vl_logic;
        clkout3         : out    vl_logic;
        clkout4         : out    vl_logic
    );
end pll_test;
