	component unnamed is
		port (
			clk         : in    std_logic                     := 'X';             -- clk
			reset       : in    std_logic                     := 'X';             -- reset
			address     : in    std_logic                     := 'X';             -- address
			chipselect  : in    std_logic                     := 'X';             -- chipselect
			byteenable  : in    std_logic_vector(3 downto 0)  := (others => 'X'); -- byteenable
			read        : in    std_logic                     := 'X';             -- read
			write       : in    std_logic                     := 'X';             -- write
			writedata   : in    std_logic_vector(31 downto 0) := (others => 'X'); -- writedata
			readdata    : out   std_logic_vector(31 downto 0);                    -- readdata
			waitrequest : out   std_logic;                                        -- waitrequest
			irq         : out   std_logic;                                        -- irq
			PS2_CLK     : inout std_logic                     := 'X';             -- CLK
			PS2_DAT     : inout std_logic                     := 'X'              -- DAT
		);
	end component unnamed;

	u0 : component unnamed
		port map (
			clk         => CONNECTED_TO_clk,         --                clk.clk
			reset       => CONNECTED_TO_reset,       --              reset.reset
			address     => CONNECTED_TO_address,     --   avalon_ps2_slave.address
			chipselect  => CONNECTED_TO_chipselect,  --                   .chipselect
			byteenable  => CONNECTED_TO_byteenable,  --                   .byteenable
			read        => CONNECTED_TO_read,        --                   .read
			write       => CONNECTED_TO_write,       --                   .write
			writedata   => CONNECTED_TO_writedata,   --                   .writedata
			readdata    => CONNECTED_TO_readdata,    --                   .readdata
			waitrequest => CONNECTED_TO_waitrequest, --                   .waitrequest
			irq         => CONNECTED_TO_irq,         --          interrupt.irq
			PS2_CLK     => CONNECTED_TO_PS2_CLK,     -- external_interface.CLK
			PS2_DAT     => CONNECTED_TO_PS2_DAT      --                   .DAT
		);

