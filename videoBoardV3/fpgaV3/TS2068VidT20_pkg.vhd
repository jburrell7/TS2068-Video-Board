
library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;
use ieee.std_logic_unsigned.all;

package TS2068VidT20_pkg is

constant DAT_TO_TS		: std_logic := '1';
constant DAT_FROM_TS		: std_logic := '0';
constant ADR_TO_TS		: std_logic := '1';
constant ADR_FRoM_TS		: std_logic := '0';
constant CTL_TO_TS		: std_logic := '1';
constant CTL_FROM_TS		: std_logic := '0';


	
constant DOSSTK_PORT			: std_logic_vector(7 downto 0) := x"C0";	-- 192
constant DOSSTKPTRL_PORT	: std_logic_vector(7 downto 0) := x"C1";	-- 193
constant DOSSTKPTRH_PORT	: std_logic_vector(7 downto 0) := x"C2";	-- 194
constant FPGA_REG				: std_logic_vector(7 downto 0) := x"C3";	-- 195
constant VERREG				: std_logic_vector(7 downto 0) := x"C4";	-- 196

constant PORT_MAPADR			: std_logic_vector(7 downto 0) := x"C6";	-- 198
constant PORT_MAPDAT			: std_logic_vector(7 downto 0) := x"C7";	-- 199
constant		HOME_0				: std_logic_vector(7 downto 0) := x"00";
constant		HOME_1				: std_logic_vector(7 downto 0) := x"01";
constant		HOME_2				: std_logic_vector(7 downto 0) := x"02";
constant		HOME_3				: std_logic_vector(7 downto 0) := x"03";
constant		HOME_4				: std_logic_vector(7 downto 0) := x"04";
constant		HOME_5				: std_logic_vector(7 downto 0) := x"05";
constant		HOME_6				: std_logic_vector(7 downto 0) := x"06";
constant		HOME_7				: std_logic_vector(7 downto 0) := x"07";
constant		EXROM_0				: std_logic_vector(7 downto 0) := x"08";
constant		EXROM_1				: std_logic_vector(7 downto 0) := x"09";
constant		EXROM_2				: std_logic_vector(7 downto 0) := x"0A";
constant		EXROM_3				: std_logic_vector(7 downto 0) := x"0B";
constant		EXROM_4				: std_logic_vector(7 downto 0) := x"0C";
constant		EXROM_5				: std_logic_vector(7 downto 0) := x"0D";
constant		EXROM_6				: std_logic_vector(7 downto 0) := x"0E";
constant		EXROM_7				: std_logic_vector(7 downto 0) := x"0F";

constant		DOCK_0				: std_logic_vector(7 downto 0) := x"10";
constant		DOCK_1				: std_logic_vector(7 downto 0) := x"11";
constant		DOCK_2				: std_logic_vector(7 downto 0) := x"12";
constant		DOCK_3				: std_logic_vector(7 downto 0) := x"13";
constant		DOCK_4				: std_logic_vector(7 downto 0) := x"14";
constant		DOCK_5				: std_logic_vector(7 downto 0) := x"15";
constant		DOCK_6				: std_logic_vector(7 downto 0) := x"16";
constant		DOCK_7				: std_logic_vector(7 downto 0) := x"17";

constant		MAP_CTL				: std_logic_vector(7 downto 0) := x"18";



constant SPI_DREG				: std_logic_vector(7 downto 0) := x"C8";	-- 200
constant SPI_SELREG			: std_logic_vector(7 downto 0) := x"C9";	-- 201
constant SPI_STAT				: std_logic_vector(7 downto 0) := x"CA";	-- 202


component T80s
	generic(
		Mode : integer := 0;	-- 0 => Z80, 1 => Fast Z80, 2 => 8080, 3 => GB
		T2Write : integer := 0;	-- 0 => WR_n active in T3, /=0 => WR_n active in T2
		IOWait : integer := 1	-- 0 => Single cycle I/O, 1 => Std I/O cycle
	);
	port(
		RESET_n		: in std_logic;
		CLK_n		: in std_logic;
		WAIT_n		: in std_logic;
		INT_n		: in std_logic;
		NMI_n		: in std_logic;
		BUSRQ_n		: in std_logic;
		M1_n		: out std_logic;
		MREQ_n		: out std_logic;
		IORQ_n		: out std_logic;
		RD_n		: out std_logic;
		WR_n		: out std_logic;
		RFSH_n		: out std_logic;
		HALT_n		: out std_logic;
		BUSAK_n		: out std_logic;
		A			: out std_logic_vector(15 downto 0);
		DI			: in std_logic_vector(7 downto 0);
		DO			: out std_logic_vector(7 downto 0)
	);
end component;


component ReuI2c is
	generic(
	constant REG_I2C_CMD		: std_logic_vector(7 downto 0) := x"00";
	constant REG_I2CDATWR	: std_logic_vector(7 downto 0) := x"01";
	constant REG_I2CDATRD	: std_logic_vector(7 downto 0) := x"02";

	constant I2CCMD_NOP		: std_logic_vector(7 downto 0) := x"00";
	constant I2CCMD_START	: std_logic_vector(7 downto 0) := x"01";
	constant I2CCMD_STOP		: std_logic_vector(7 downto 0) := x"02";
	constant I2CCMD_SEND		: std_logic_vector(7 downto 0) := x"03";
	constant I2CCMD_RXACK	: std_logic_vector(7 downto 0) := x"04";
	constant I2CCMD_RXNAK	: std_logic_vector(7 downto 0) := x"05"
	);
	PORT
	(	
	clock100		: in std_logic;
	rst_n			: in std_logic;
	
	syncRnW		: in std_logic;
	syncIorq_n	: in std_logic;
	
	F_ADR			: in std_logic_vector(7 downto 0);
	i2cItsMe		: out std_logic;
	i2cDatD		: in  std_logic_vector(7 downto 0);
	i2cDatQ		: out std_logic_vector(7 downto 0);
	
-- I2C
	RTC_SDA		: inout std_logic;
	RTC_SCL		: inout std_logic
	);
end component;

component tsVidTop is
generic (
	ULA_A				: std_logic_vector(15 downto 0) := X"BF3B";
	ULA_D				: std_logic_vector(15 downto 0) := X"FF3B";
	VDP_REGADR		: std_logic_vector(7 downto 0) := x"A1";
	VDP_RAMADR		: std_logic_vector(7 downto 0) := x"A0";
	TEXT_PORT		: std_logic_vector(7 downto 0) := x"A2"
);
port (
	clk50			: in std_logic;
	clk100		: in std_logic;
	clk21			: in std_logic;
	pll1Locked	: in std_logic;
	
	reset_n		: in std_logic;
	
	cpuClk		: in std_logic;
	cpuAdr		: in std_logic_vector(15 downto 0);
	cpuD_i		: in std_logic_vector( 7 downto 0);
	cpuD_o		: out std_logic_vector( 7 downto 0);
	cpuMrqn		: in std_logic;
	cpuIorqn		: in std_logic;
	cpuRdn		: in std_logic;
	cpuWrn		: in std_logic;
	
	itsMe			: out std_logic;

	portFE		: in std_logic_vector(7 downto 0);
	portFF		: in std_logic_vector(7 downto 0);
	isHome		: in std_logic;
	isExrom		: in std_logic;
	isDock		: in std_logic;

	-- Video signals
	RED			: out std_logic_vector(2 downto 0);
	GRN			: out std_logic_vector(2 downto 0);
	BLU			: out std_logic_vector(2 downto 0);
	HSYNC			: out std_logic;
	VSYNC			: out std_logic;

	debug			: out std_logic_vector(7 downto 0)
);
end component;


COMPONENT bootRom
	GENERIC ( DATA_WIDTH : INTEGER := 8; 
	ADDR_WIDTH : INTEGER := 8;
	ROMDATA : STRING := "default.txt");
	PORT
	(
		addr		:	 IN STD_LOGIC_VECTOR(ADDR_WIDTH-1 DOWNTO 0);
		clk		:	 IN STD_LOGIC;
		q		:	 OUT STD_LOGIC_VECTOR(DATA_WIDTH-1 DOWNTO 0)
	);
END COMPONENT;


component mySpi is
generic(
	SPI_DREG			: std_logic_vector(7 downto 0) := x"00";
	SPI_SELREG		: std_logic_vector(7 downto 0) := x"01";
	SPI_STAT			: std_logic_vector(7 downto 0) := x"02"
);
port(
	rst_n			: in std_logic;

	cpuClk			: in std_logic;
	cpuAdr			: in std_logic_vector(7 downto 0);
	cpuIorq_n		: in std_logic;
	cpuWr_n			: in std_logic;
	cpuMosi			: in std_logic_vector(7 downto 0);
	cpuMiso			: out std_logic_vector(7 downto 0);
	
	itsMe				: out std_logic;
	
	spiCs_n			: out std_logic_vector(3 downto 0);
	spiMosi			: out std_logic;
	spiClk			: out std_logic;
	spiMiso			: in std_logic;
	spiReset			: out std_logic;
	
	spiBusy			: in std_logic;
	spiIn1			: in std_logic;
	spiIn2			: in std_logic;
	spiIn3			: in std_logic;
	
	debug				: Out std_logic_vector(7 downto 0)
);
end component;


component memMgr is
generic(
	PORT_MAPADR		: std_logic_vector := x"00";
	PORT_MAPDAT		: std_logic_vector := x"01"
);
port(
	rst_n			: in std_logic;

	cpuClk			: in std_logic;
	cpuAdr			: in std_logic_vector(15 downto 0);
	cpuIorq_n		: in std_logic;
	cpuMrq_n			: in std_logic;
	cpuRd_n			: in std_logic;
	cpuWr_n			: in std_logic;
	cpuMosi			: in std_logic_vector(7 downto 0);
	cpuMiso			: out std_logic_vector(7 downto 0);
	beIn				: in std_logic;

	isHome			: in std_logic;
	isExrom			: in std_logic;
	isDock			: in std_logic;

	isIoRd			: out std_logic;
	isMemRd			: out std_logic;
	useMap			: out std_logic;
	memDir			: out std_logic;
	nBe_o				: out std_logic;
	nBe_i				: in std_logic;

	sramAdrHi		: out std_logic_vector(5 downto 0);
	sramOen			: out std_logic;
	sramWen			: out std_logic;
	
	debug				: Out std_logic_vector(7 downto 0)
);
end component;


COMPONENT dosStack
	GENERIC ( DATA_WIDTH : INTEGER := 8; ADDR_WIDTH : INTEGER := 6 );
	PORT
	(
		data		:	 IN STD_LOGIC_VECTOR(DATA_WIDTH-1 DOWNTO 0);
		addr		:	 IN STD_LOGIC_VECTOR(ADDR_WIDTH-1 DOWNTO 0);
		we		:	 IN STD_LOGIC;
		clk		:	 IN STD_LOGIC;
		q		:	 OUT STD_LOGIC_VECTOR(DATA_WIDTH-1 DOWNTO 0)
	);
END COMPONENT;

component SBCTextDisplayRGB is
	generic(
		constant EXTENDED_CHARSET 		: integer := 0; -- 1 = 256 chars, 0 = 128 chars
		constant COLOUR_ATTS_ENABLED 	: integer := 1; -- 1=Colour for each character, 0=Colour applied to whole display
		-- VGA 640x480 Default values
		constant VERT_CHARS 				: integer := 25;
		constant HORIZ_CHARS 			: integer := 80;
		constant CLOCKS_PER_SCANLINE 	: integer := 1600; -- NTSC/PAL = 3200
		constant DISPLAY_TOP_SCANLINE : integer := 35+40;
		constant DISPLAY_LEFT_CLOCK 	: integer := 288; -- NTSC/PAL = 600+
		constant VERT_SCANLINES 		: integer := 525; -- NTSC=262, PAL=312
		constant VSYNC_SCANLINES 		: integer := 2; -- NTSC/PAL = 4
		constant HSYNC_CLOCKS 			: integer := 192;  -- NTSC/PAL = 235
		constant VERT_PIXEL_SCANLINES : integer := 2;
		constant CLOCKS_PER_PIXEL 		: integer := 2; -- min = 2
		constant H_SYNC_ACTIVE 			: std_logic := '0';
		constant V_SYNC_ACTIVE 			: std_logic := '0';
		
		constant DEFAULT_ATT 			: std_logic_vector(7 downto 0) := "00001111"; -- background iBGR | foreground iBGR (i=intensity)
		constant ANSI_DEFAULT_ATT 		: std_logic_vector(7 downto 0) := "00000111" -- background iBGR | foreground iBGR (i=intensity)
	);
	port (
		n_reset	: in std_logic;
		clk    	: in  std_logic;
		
		cpu_clk	: in std_logic;
		n_wr		: in  std_logic;
		n_rd		: in  std_logic;
--		regSel	: in  std_logic;
		dataIn	: in  std_logic_vector(7 downto 0);
		dataOut	: out std_logic_vector(7 downto 0);
--		n_int		: out std_logic; 
		
		-- RGB video signals
		videoR0	: out std_logic;
		videoR1	: out std_logic;
		videoG0	: out std_logic;
		videoG1	: out std_logic;
		videoB0	: out std_logic;
		videoB1	: out std_logic;
		hSync  	: buffer  std_logic;
		vSync  	: buffer  std_logic;
		
		-- Monochrome video signals
		video		: buffer std_logic;
		sync  	: out  std_logic
 );
end component;


COMPONENT mmMem
	GENERIC ( DATA_WIDTH : INTEGER := 8; ADDR_WIDTH : INTEGER := 6 );
	PORT
	(
		data_a		:	 IN STD_LOGIC_VECTOR(DATA_WIDTH-1 DOWNTO 0);
		data_b		:	 IN STD_LOGIC_VECTOR(DATA_WIDTH-1 DOWNTO 0);
		addr_a		:	 IN STD_LOGIC_VECTOR(ADDR_WIDTH-1 DOWNTO 0);
		addr_b		:	 IN STD_LOGIC_VECTOR(ADDR_WIDTH-1 DOWNTO 0);
		we_a		:	 IN STD_LOGIC;
		we_b		:	 IN STD_LOGIC;
		clk_a		:	 IN STD_LOGIC;
		clk_b		:	 IN STD_LOGIC;
		q_a		:	 OUT STD_LOGIC_VECTOR(DATA_WIDTH-1 DOWNTO 0);
		q_b		:	 OUT STD_LOGIC_VECTOR(DATA_WIDTH-1 DOWNTO 0)
	);
END COMPONENT;

end package;

package body TS2068VidT20_pkg is

	-- Type Declaration (optional)

	-- Subtype Declaration (optional)

	-- Constant Declaration (optional)

	-- Function Declaration (optional)

	-- Function Body (optional)

	-- Procedure Declaration (optional)

	-- Procedure Body (optional)



	
	
end TS2068VidT20_pkg;
