library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;
use IEEE.std_logic_unsigned.all;
use work.TS2068VidT20_pkg.all;
use work.tsvidTop_pkg.all;


entity ts2068Fpga is
port (
	clk50		: in std_logic;
	
	BLU		: out std_logic_vector(2 downto 0);
	GRN		: out std_logic_vector(2 downto 0);
	RED		: out std_logic_vector(2 downto 0);
	HSYNC		: out std_logic;
	VSYNC		: out std_logic;

	F_A		: in std_logic_vector(15 downto 0);
	F_D_OUT	: out std_logic_vector(7 downto 0);
	F_D_IN	: in std_logic_vector(7 downto 0);
	F_D_OE	: out std_logic_vector(7 downto 0);

	F_CLK		: in std_logic;
	F_nBE_IN	: in std_logic;
	F_BE_OUT	: out std_logic;
	F_nIORQ	: in std_logic;
	F_nM1		: in std_logic;
	F_nMRQ	: in std_logic;
	F_nRD		: in std_logic;
	F_nRST	: in std_logic;
	F_nWR		: in std_logic;
	DAT_DIR	: out std_logic;

	R_A		: out std_logic_vector(18 downto 0);
	R_nOE		: out std_logic;
	R_nWE		: out std_logic;

	FL_MISO	: in std_logic;
	FL_MOSI	: out std_logic;
	FL_SCLK	: out std_logic;
	FL_nSEL	: out std_logic;

	X_OUT		: out std_logic_vector(13 downto 0);
	X_IN		: in std_logic_vector(13 downto 0);
	X_OE		: out std_logic_vector(13 downto 0);

	pll1Locked	: in std_logic;
	clk21pll1	: in std_logic;
	clk50pll1	: in std_logic;
	clk100pll1	: in std_logic
);
end ts2068Fpga;


architecture rtl of ts2068Fpga is
-- used during development debug
constant RAM_DISABLE		: std_logic		:= '1';

constant FPGA_VER		: std_logic_vector(7 downto 0) := std_logic_vector(to_unsigned(31, 8));
constant FD_OEVEC		: std_logic_vector(7 downto 0) := x"FF";



signal cpuWrSync		: std_logic;
signal vramSel_n		: std_logic;
signal vramQ			: std_logic_vector(7 downto 0);

signal sysReset_n		: std_logic;
signal sysReset		: std_logic;

signal portFE			: std_logic_vector(7 downto 0);
signal portFF			: std_logic_vector(7 downto 0);
signal portHSR			: std_logic_vector(7 downto 0);
signal bankVec			: std_logic_vector(2 downto 0);
signal mmuVec			: std_logic_vector(2 downto 0);
signal isLocPort		: std_logic;
signal locPortQ		: std_logic_vector(7 downto 0);
signal regVidSel		: std_logic_vector(1 downto 0);

signal ioRd				: std_logic;
signal ioWr				: std_logic;
signal memRd			: std_logic;
signal memWr			: std_logic;

signal vidDout			: std_logic_vector(7 downto 0);
signal vidItsMe		: std_logic;
signal vidDebug		: std_logic_vector(7 downto 0);

signal bromEna			: std_logic;
signal bromSel			: std_logic := '0';
signal bromQ			: std_logic_vector(7 downto 0);

signal testPort		: std_logic_vector(7 downto 0);
signal testNode		: std_logic;

signal curChunk		: integer;

signal spiQ				: std_logic_vector(7 downto 0);
signal spiItsMe		: std_logic;
signal spiCsn			: std_logic_vector(3 downto 0);

signal spiBusy			: std_logic;
signal spiIn1			: std_logic;
signal spiIn2			: std_logic;
signal spiIn3			: std_logic;
signal spiReset		: std_logic;

signal isHome			: std_logic;
signal isExrom			: std_logic;
signal isDock			: std_logic;

signal rdOeNode		: std_logic_vector(7 downto 0);

signal spiRstNode		: std_logic;

signal chunkNr			: integer range 0 to 7;


-- the dosStack
signal dStkPtr			: std_logic_vector(8 downto 0);
signal dStkWe			: std_logic;
signal dStkQ			: std_logic_vector(7 downto 0);
signal dSPtr			: std_logic_vector(7 downto 0);

signal mapcond			: std_logic;

signal regMemBank		: std_logic_vector(7 downto 0);
signal regMemAdr		: std_logic_vector(15 downto 0);
signal memIn			: std_logic_vector(7 downto 0);

signal termSel			: std_logic;
signal termRdn			: std_logic;
signal termWrn			: std_logic;
signal termDout		: std_logic_vector(7 downto 0);

signal mMgrQ			: std_logic_vector(7 downto 0);
signal mMgrItsMe		: std_logic;
signal mMgrUseMap		: std_logic;
signal mMgrMemDir		: std_logic;
signal mMgrBen			: std_logic;
signal mMgrSramAdr	: std_logic_vector(5 downto 0);
signal mMgrSramOen	: std_logic;
signal mMgrSramWen	: std_logic;


signal fpgaReg			: std_logic_vector(7 downto 0);
signal isHomeVid_n	: std_logic;

signal hrVidAdr		: std_logic_vector(15 downto 0) := x"0000";
signal hrVidD			: std_logic_vector(7 downto 0) := x"00";
signal hrVidQ			: std_logic_vector(7 downto 0) := x"00";
signal fnbeNode		: std_logic;

--signal nbeEna		: std_logic := '0';

signal ddirNode		: std_logic;
signal junk				: std_logic;

begin

	X_OUT		<= (others => '0');
	X_OE		<= (others => '0');


--	X_OUT(22 downto 16)		<= (others => '0');
--	X_OE(22 downto 16)		<= (others => '0');
--
--	X_OE(15 downto 8)		<= (others => '1');	
--	X_OUT(8)					<= F_nBE_IN;
--	X_OUT(9)					<= fnbeNode;
--	X_OUT(10)				<= mMgrUseMap;
--	X_OUT(11)				<= F_nMRQ;
--	X_OUT(12)				<= mMgrSramWen;
--	X_OUT(13)				<= mMgrSramOen;
--	X_OUT(14)				<= mMgrMemDir;
--	X_OUT(15)				<= F_CLK;
	

	junk		<= (not junk) when rising_edge(clk21pll1);

-- convenience signals
	ioRd			<= not (F_nIORQ or F_nRd);
	ioWr			<= not (F_nIORQ or F_nWr);
	memRd			<= not (F_nMRQ or F_nRd);
	memWr			<= not (F_nMRQ or F_nWr);
	
	isHome		<= '1' when (portHSR(to_integer(signed(F_A(15 downto 13)))) = '0') else '0';
	isExrom		<= (not isHome) and portFF(7);
	isDock		<= (not isHome) and (not portFF(7));
	
	
--	exromSel	<= isExrom;

process(clk100pll1, pll1Locked, F_nRST, sysReset_n)
variable rstSync			: std_logic_vector(2 downto 0);
begin
	if ((pll1Locked = '0') or (F_nRST = '0')) then
		rstSync		:= "000";
	elsif rising_edge(clk100pll1) then
		rstSync		:= rstSync(1 downto 0) & '1';
	end if;

	sysReset_n		<= rstSync(2);
	sysReset		<= not sysReset_n;
end process;

	
outMux:process(memRd, vramQ, isLocPort, ioRd, 
					locPortQ, vidItsMe, F_nMRQ, 
					F_nRd, bromSel, bromQ, 
					vidDout, spiItsMe, spiQ, 
					termRdn, termDout, fnbeNode,
					mMgrUseMap, mMgrBen, mMgrMemDir, 
					mMgrItsMe, mMgrQ)
begin

	F_BE_OUT				<= fnbeNode and (not RAM_DISABLE);

	F_D_OE				<= not FD_OEVEC;
	DAT_DIR				<= DAT_FROM_TS;
	F_D_OUT				<= x"00";
-- used for debug
	fnbeNode				<= '0';


	if 	(mMgrUseMap = '1') then
		fnbeNode			<= not mMgrBen;				-- disable TS memory
		F_D_OE			<= not FD_OEVEC;		-- ensure our TS outputs are disabled
		DAT_DIR			<= mMgrMemDir;			-- set the data direction for SRAM
	elsif 	(ioRd = '1') then
		if		(vidItsMe = '1') then
			F_D_OUT		<= vidDout;
			F_D_OE		<= FD_OEVEC;
			DAT_DIR		<= DAT_TO_TS;
		elsif	(isLocPort = '1') then
			F_D_OUT		<= locPortQ;
			F_D_OE		<= FD_OEVEC;
			DAT_DIR		<= DAT_TO_TS;
		elsif (spiItsMe = '1') then
			F_D_OUT		<= spiQ;
			F_D_OE		<= FD_OEVEC;
			DAT_DIR		<= DAT_TO_TS;
		elsif (mMgrItsMe = '1') then
			F_D_OUT		<= mMgrQ;
			F_D_OE		<= FD_OEVEC;
			DAT_DIR		<= DAT_TO_TS;
		end if;
	end if;
end process;




vid:tsVidTop
generic MAP(
	ULA_A			=> ULA_A,
	ULA_D			=> ULA_D,
	VDP_REGADR	=> VDP_REGADR,
	VDP_RAMADR	=> VDP_RAMADR,
	TEXT_PORT	=> TEXT_PORT
)
port map(
	clk50			=> clk50pll1,
	clk100		=> clk100pll1,
	clk21			=> clk21pll1,
	pll1Locked	=> pll1Locked,
	
	reset_n		=> sysReset_n,
	
	cpuClk		=> F_CLK,
	cpuAdr		=> F_A,
	cpuD_i		=> F_D_IN,
	cpuD_o		=> vidDout,
	cpuMrqn		=> F_nMRQ,
	cpuIorqn		=> F_nIORQ,
	cpuRdn		=> F_nRd,
	cpuWrn		=> F_nWr,
	
	itsMe			=> vidItsMe,

	portFE		=> portFE,
	portFF		=> portFF,
	isHome		=> isHome,
	isExrom		=> isExrom,
	isDock		=> isDock,

-- Video signals
	RED			=> RED,
	GRN			=> GRN,
	BLU			=> BLU,
	HSYNC			=> HSYNC,
	VSYNC			=> VSYNC,

	debug		=> vidDebug
);


	FL_nSEL		<= spiCSn(0);
	
spi:mySpi
generic map(
	SPI_DREG			=> SPI_DREG,
	SPI_SELREG		=> SPI_SELREG,
	SPI_STAT			=> SPI_STAT
	)
port map(
	rst_n				=> sysReset_n,

	cpuClk			=> F_CLK,
	cpuAdr			=> F_A(7 downto 0),
	cpuIorq_n		=> F_nIORQ,
	cpuWr_n			=> F_nWr,
	cpuMosi			=> F_D_IN,
	cpuMiso			=> spiQ,
	
	itsMe				=> spiItsMe,
	
	spiCs_n			=> spiCSn,
	spiMosi			=> FL_MOSI,
	spiClk			=> FL_SCLK,
	spiMiso			=> FL_MISO,
	spiReset			=> open,
	
	spiBusy			=> '0',
	spiIn1			=> '0',
	spiIn2			=> '0',
	spiIn3			=> '0',
	
	debug				=> open
);


locRegs:process(sysReset_n, F_CLK, F_A, F_nIORQ, 
					F_nWr, F_nRd, regVidSel, ioRd, 
					bromEna, testPort, dStkQ, 
					memIn, regMemAdr, regMemBank, 
					fpgaReg, dSPtr)
					
variable mapIoAdr			: integer range 0 to 31;
begin

	if (sysReset_n = '0') then
		portFE				<= x"07";
		portFF				<= x"00";
		portHSR				<= x"00";
		testPort				<= x"00";
--		bromEna				<= '1';
--		regEnaRam			<= '0';
--		regDosMode			<= x"00";
--		regDosFlags			<= x"00";
		fpgaReg				<= x"00";
	elsif rising_edge(F_CLK) then
		if (ioWr = '1') then
			case F_A(7 downto 0) is
			when x"FF" =>
				portFF		<= F_D_IN;
			when x"FE" =>
				portFE		<= F_D_IN;
			when x"F4" =>
				portHSR		<= F_D_IN;

			when x"F0" =>
				testPort		<= F_D_IN;

			when FPGA_REG =>	
				fpgaReg		<= F_D_IN;
				
			when others =>
				null;
			end case;
		end if;
	end if;
	
	case F_A(7 downto 0) is
	when x"F3" =>
		isLocPort		<= ioRd;
		locPortQ			<= "0000000" & bromEna;
	when x"F0" =>
		isLocPort		<= ioRd;
		locPortQ			<= testPort;
	when DOSSTK_PORT =>
		isLocPort		<= ioRd;
		locPortQ			<= dStkQ;
	when DOSSTKPTRL_PORT | DOSSTKPTRH_PORT =>
		isLocPort		<= ioRd;
		locPortQ			<= dSPtr;

	when FPGA_REG =>
		isLocPort		<= ioRd;
		locPortQ			<= '0' & fpgaReg(6 downto 0);
	when VERREG =>
		isLocPort		<= ioRd;
		locPortQ			<= FPGA_VER;



	when others =>
		isLocPort		<= '0';
		locPortQ		<= x"00";
	end case;
	
end process;

---------------------------------------------------------------
-- This section implements a 512 byte LIFO stack in the hardware
--		using one of the T20 memory modules. This is intended
--		to be used for the DOS but is available for any other
-- 	purpose
--
dosStkCtl:process(clk100pll1, F_CLK, ioRd, ioWr, F_A, sysReset_n)
variable ioRdSync			: std_logic_vector(1 downto 0);
variable ioWrSync			: std_logic_vector(1 downto 0);
variable state				: integer range 0 to 7;

variable ioRdx				: std_logic;
variable ioWrx				: std_logic;

begin

	if (sysReset_n = '0') then
		ioRdx		:= '0';
		ioWrx		:= '0';
	elsif rising_edge(F_CLK) then
		ioRdx		:= ioRd;
		ioWrx		:= ioWr;
	end if;
	

	if (sysReset_n = '0') then
		ioRdSync	:= "00";
		ioWrSync	:= "00";
	elsif rising_edge(clk100pll1) then
		ioRdSync	:= ioRdSync(0) & ioRdx;
		ioWrSync	:= ioWrSync(0) & ioWrx;
	end if;

	if (sysReset_n= '0') then
		dStkPtr		<= "101010100"; --(others => '0');
		dStkWe		<= '0';
		state		:= 0;
	elsif rising_edge(clk100pll1) then
		dStkWe		<= '0';
		case state is
		when 0 =>
			if (F_A(7 downto 0) = DOSSTK_PORT) then
				if 	(ioRdSync(1) = '1') then
					state		:= 1;
				elsif	(ioWrSync(1) = '1') then
					state		:= 2;
				end if;
			elsif (F_A(7 downto 0) = DOSSTKPTRL_PORT) then
				if	(ioWrSync(1) = '1') then
					dStkPtr(7 downto 0)		<= F_D_IN;
					state		:= 7;
				end if;
				if	(ioRdSync(1) = '1') then
					dSPtr		<= dStkPtr(7 downto 0);
					state		:= 7;
				end if;
			elsif (F_A(7 downto 0) = DOSSTKPTRH_PORT) then
				if	(ioWrSync(1) = '1') then
					dStkPtr(8)					<= F_D_IN(0);
					state		:= 7;
				end if;
				if	(ioRdSync(1) = '1') then
					dSPtr		<= "0000000" & dStkPtr(8);
					state		:= 7;
				end if;
			end if;
	-- reading from the stack
		when 1 =>
			dStkPtr		<= std_logic_vector(unsigned(dStkPtr) - 1);
			state			:= 7;
	-- writing to the stack
		when 2 =>
			dStkWe		<= '1';
			state			:= 3;
		when 3 =>
			state			:= 4;
		when 4 =>
			dStkWe		<= '1';
			state			:= 5;
		when 5 =>
			dStkPtr		<= std_logic_vector(unsigned(dStkPtr) + 1);
			state			:= 6;
		when 7 =>
			if ((ioRdSync(1) = '0') and (ioWrSync(1) = '0')) then
				state		:= 0;
			end if;
			
		when others =>
			state			:= 7;
		end case;

	end if;
end process;



dStack:dosStack
 GENERIC map(
	 DATA_WIDTH	=> 8,
	 ADDR_WIDTH	=> 9
	 )
  PORT map (
	clk	=> clk100pll1,
	we		=> dStkWe,
	addr	=> dStkPtr,
	data	=> F_D_IN,
	q		=> dStkQ
  );
	
	
	R_A			<= mMgrSramAdr & F_A(12 downto 0);
	R_nWE			<= mMgrSramWen or RAM_DISABLE;
	R_nOE			<= mMgrSramOen or RAM_DISABLE;

	
mMgr:memMgr
generic map(
	PORT_MAPADR		=> PORT_MAPADR,
	PORT_MAPDAT		=> PORT_MAPDAT
)
port map(
	rst_n			=> sysReset_n,

	cpuClk		=> F_CLK,
	cpuAdr		=> F_A,
	cpuIorq_n	=> F_nIORQ,
	cpuMrq_n		=> F_nMRQ,
	cpuRd_n		=> F_nRd,
	cpuWr_n		=> F_nWr,
	cpuMosi		=> F_D_IN,
	cpuMiso		=> mMgrQ,
	
	beIn			=> F_nBE_IN,
	
	isHome		=> isHome,
	isExrom		=> isExrom,
	isDock		=> isDock,
	
	isIoRd		=> mMgrItsMe,
	isMemRd		=> open,
	useMap		=> mMgrUseMap,
	memDir		=> mMgrMemDir,
	nBe_o			=> mMgrBen,
	nBe_i			=> F_nBE_IN,

	sramAdrHi	=> mMgrSramAdr,
	sramOen		=> mMgrSramOen,
	sramWen		=> mMgrSramWen,
	
	debug				=> open
);



end rtl;
