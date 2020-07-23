--Author: Truong Lai Nhat Tuan
--xlainh00
---------------------
library IEEE;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;
use IEEE.std_logic_1164.all;

-----------------------------------------------------------------
-----------------------------ENTITY------------------------------
-----------------------------------------------------------------
entity ledc8x8 is
	port(
		RESET: in std_logic;
		SMCLK: in std_logic;
		ROW: out std_logic_vector (7 downto 0);
		LED: out std_logic_vector (7 downto 0)
	);
end entity ledc8x8;
-----------------------------------------------------------------
---------------------------ARCHITECTURE--------------------------
-----------------------------------------------------------------
architecture behavior of ledc8x8 is
	signal counter : std_logic_vector(21 downto 0);
	signal row_count : std_logic_vector(7 downto 0) := "10000000";
	signal col_count : std_logic_vector(7 downto 0) := "11111111";
	signal ce : std_logic; 	
	signal switch : std_logic := '0';

begin
--------------------------clock enable---------------------
clk_logic : process(RESET, SMCLK)
begin
	if RESET = '1' then
		counter <= "0000000000000000000000";
	elsif rising_edge(SMCLK) then
		if counter(7 downto 0) = "11111111" then
			ce <= '1';
		else
			ce <= '0';
		end if;

		if counter(21 downto 0) = "1111010000100100000000" then
			switch <= '1';
		end if;

		counter <= counter + 1;
	end if;
end process;

---------------------row rotation----------------------
row_rotate : process(RESET, SMCLK)
begin
	if RESET = '1' then
		row_count <= "10000000";
	elsif rising_edge(SMCLK) then
		if ce = '1' then
			case row_count is
				when "10000000" => row_count <= "01000000";
				when "01000000" => row_count <= "00100000";
				when "00100000" => row_count <= "00010000";
				when "00010000" => row_count <= "00001000";
				when "00001000" => row_count <= "00000100";
				when "00000100" => row_count <= "00000010";
				when "00000010" => row_count <= "00000001";
				when "00000001" => row_count <= "10000000";
				when others => null;
			end case;
		end if;
	end if;

	ROW <= row_count;
end process;

-------------------------------------------------------
disp_logic : process(SMCLK, col_count)
begin
	if rising_edge(SMCLK) then
		if switch = '0' then
			case row_count is
				when "00000001" => col_count <= "11100000"; --R0: C7, C6 .. C0
				when "00000010" => col_count <= "11111011";
				when "00000100" => col_count <= "11111011";
				when "00001000" => col_count <= "11111011";
				when "00010000" => col_count <= "11111011";
				when "00100000" => col_count <= "11111011";
				when "01000000" => col_count <= "11111111";
				when "10000000" => col_count <= "11111111";
				when others => null;
			end case;
		else ---- switch = 1
			case row_count is
				when "00000001" => col_count <= "11100000"; --R0: C7, C6 .. C0
				when "00000010" => col_count <= "11111011";
				when "00000100" => col_count <= "11101011";
				when "00001000" => col_count <= "11101011";
				when "00010000" => col_count <= "11101011";
				when "00100000" => col_count <= "11101011";
				when "01000000" => col_count <= "11101111";
				when "10000000" => col_count <= "00001111";
				when others => null;
			end case;	
		end if;
	end if;
end process;

--------------------------------------------------------
disp_show : process(RESET, SMCLK)
begin
	if RESET = '1' then
		LED <= "11111111";
	elsif rising_edge(SMCLK) then-----------------------------------------------------------------------------------
		LED <= col_count;
	end if;
end process;


end architecture behavior;
