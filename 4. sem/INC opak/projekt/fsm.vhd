-- fsm.vhd: Finite State Machine
-- Author(s): Truong Lai Nhat Tuan
--xlainh00 : kod1 = 4602308586    kod2 = 46023025760
library ieee;
use ieee.std_logic_1164.all;
-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity fsm is
port(
   CLK         : in  std_logic;
   RESET       : in  std_logic;

   -- Input signals
   KEY         : in  std_logic_vector(15 downto 0);
   CNT_OF      : in  std_logic;

   -- Output signals
   FSM_CNT_CE  : out std_logic;
   FSM_MX_MEM  : out std_logic;
   FSM_MX_LCD  : out std_logic;
   FSM_LCD_WR  : out std_logic;
   FSM_LCD_CLR : out std_logic
);
end entity fsm;

-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of fsm is
   type t_state is (START, TEST1, TEST2, TEST3, TEST4, TEST5, TEST6, TEST7, TEST8, TEST9, TEST10, TEST11, 
                     PRINT_MESSAGE_VALID, PRINT_MESSAGE_INVALID, INVALID, FINISH);
   signal present_state, next_state : t_state;
   signal variant: std_logic; -- '0'= '460230 85 86', '1'= '460230 25 760'
begin
-- -------------------------------------------------------
sync_logic : process(RESET, CLK)
begin
   if (RESET = '1') then
      present_state <= START;
   elsif (CLK'event AND CLK = '1') then
      present_state <= next_state;
   end if;
end process sync_logic;

-- -------------------------------------------------------
next_state_logic : process(present_state, KEY, CNT_OF)
begin
   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - -
   when START =>
      next_state <= START;
      if (KEY(4) = '1') then -- [4]
         next_state <= TEST1;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_INVALID; 
      elsif (KEY(14 downto 0) /= "000000000000000") then 
         next_state <= INVALID;
      end if;

   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST1 =>
      next_state <= TEST1;
      if (KEY(6) = '1') then -- 4[6]
         next_state <= TEST2;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_INVALID; 
      elsif (KEY(14 downto 0) /= "000000000000000") then 
         next_state <= INVALID;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST2 =>
      next_state <= TEST2; 
      if (KEY(0) = '1') then  -- 46[0]
         next_state <= TEST3;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_INVALID; 
      elsif (KEY(14 downto 0) /= "000000000000000") then 
         next_state <= INVALID;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST3 =>
      next_state <= TEST3;
      if (KEY(2) = '1') then -- 460 [2]
         next_state <= TEST4;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_INVALID;  
      elsif (KEY(14 downto 0) /= "000000000000000") then 
         next_state <= INVALID;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST4 =>
      next_state <= TEST4;
      if (KEY(3) = '1') then -- 460 2[3]
         next_state <= TEST5;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_INVALID; 
      elsif (KEY(14 downto 0) /= "000000000000000") then 
         next_state <= INVALID;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST5 =>
      next_state <= TEST5;
      if (KEY(0) = '1') then -- 460 23[0]
         next_state <= TEST6;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_INVALID;  
      elsif (KEY(14 downto 0) /= "000000000000000") then 
         next_state <= INVALID;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST6 =>
      next_state <= TEST6;
      if (KEY(8) = '1') then -- 460 230 [8]
         variant <= '0';
         next_state <= TEST7;
      elsif (KEY(2) = '1') then -- 460 230 [2]
         variant <= '1';
         next_state <= TEST7;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_INVALID; 
      elsif (KEY(14 downto 0) /= "000000000000000") then 
         next_state <= INVALID;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST7 =>
      next_state <= TEST7;
      if (KEY(5) = '1') then -- 460 230 8[5], 460 230 2[5]
         next_state <= TEST8;
      elsif (KEY(15) = '1') then 
         next_state <= PRINT_MESSAGE_INVALID;  
      elsif (KEY(14 downto 0) /= "000000000000000") then 
         next_state <= INVALID;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST8 =>
      next_state <= TEST8;
      if (variant = '0' AND KEY(8) = '1') then -- 460 230 85[8]
         next_state <= TEST9;
      elsif (variant = '1' AND KEY(7) = '1') then -- 460 230 25[7]
         next_state <= TEST9;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_INVALID;  
      elsif (KEY(14 downto 0) /= "000000000000000") then 
         next_state <= INVALID;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST9 =>
      next_state <= TEST9;
      if (variant = '0' AND KEY(6) = '1') then -- 460 230 858 [6]
         next_state <= TEST10;
      elsif (variant = '1' AND KEY(6) = '1') then -- 460 230 257 [6]
         next_state <= TEST10;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_INVALID; 
      elsif (KEY(14 downto 0) /= "000000000000000") then 
         next_state <= INVALID;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST10 =>
      next_state <= TEST10;
      if (KEY(0) = '1') then -- 460 230 257 6[0]
         next_state <= TEST11;
      elsif (KEY(15) = '1') then
         if variant = '0' then  -- 460 230 858 6
            next_state <= PRINT_MESSAGE_VALID; 
         else 
            next_state <= PRINT_MESSAGE_INVALID;             
         end if;
      elsif (KEY(14 downto 0) /= "000000000000000") then 
         next_state <= INVALID;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST11 =>
      next_state <= TEST11;
      if (KEY(15) = '1') then -- 460 230 257 60
         next_state <= PRINT_MESSAGE_VALID; 
      elsif (KEY(14 downto 0) /= "000000000000000") then 
         next_state <= INVALID;
      end if;

   -- - - - - - - - - - - -  - - - - - - - - - - -
   when INVALID =>
      next_state <= INVALID;
      if (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_INVALID; 
      end if;

   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_MESSAGE_VALID =>
      next_state <= PRINT_MESSAGE_VALID;
      if (CNT_OF = '1') then
         next_state <= FINISH;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_MESSAGE_INVALID =>
      next_state <= PRINT_MESSAGE_INVALID;
      if (CNT_OF = '1') then
         next_state <= FINISH;
      end if;

   -- - - - - - - - - - - - - - - - - - - - - - -
   when FINISH =>
      next_state <= FINISH;
      if (KEY(15) = '1') then
         next_state <= START; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
      next_state <= START;
   end case;
end process next_state_logic;

-- -------------------------------------------------------
output_logic : process(present_state, KEY)
begin
   FSM_CNT_CE     <= '0'; -- '1' Clock enable, enable vypis
   FSM_MX_MEM     <= '0'; -- '1' Pristup povolen, '0' Pristup odepren, pamatovy modul
   FSM_MX_LCD     <= '0'; -- '1' vypis spravy z MX_MEM, '0' znak '*'
   FSM_LCD_WR     <= '0'; -- enable write
   FSM_LCD_CLR    <= '0'; -- enable clear

   case (present_state) is

   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_MESSAGE_VALID =>
      FSM_MX_MEM     <= '1';
      FSM_CNT_CE     <= '1'; 
      FSM_MX_LCD     <= '1'; 
      FSM_LCD_WR     <= '1';
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_MESSAGE_INVALID =>
      FSM_CNT_CE     <= '1'; 
      FSM_MX_LCD     <= '1'; 
      FSM_LCD_WR     <= '1';
   -- - - - - - - - - - - - - - - - - - - - - - -
   when FINISH =>
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;

      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;

   end case;
end process output_logic;

end architecture behavioral;

