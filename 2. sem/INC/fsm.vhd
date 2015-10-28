-- fsm.vhd: Finite State Machine
-- Author(s): 
--
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
   type t_state is (START, TEST0, TEST2, TEST3, TEST4, TEST5, TEST6, TEST7, TEST8, INVALID, PRINT_MESSAGE, FINISH);
   signal present_state, next_state : t_state;
   shared variable match: boolean := false;
   shared variable count: integer range 0 to 12;
   signal variant: std_logic;
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
   -----------------------------------------------
   when START =>
      next_state <= START;
      if (KEY(4) = '1') then
         count := count +1;
         next_state <= TEST4;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE; 
      elsif (KEY(14 downto 0) /= "0000000000000000") then
         next_state <= INVALID;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST0 =>
      next_state <= TEST0;
      if (KEY(2) = '1' AND count = 3) then
         count := count + 1;
         next_state <= TEST2;

      elsif (KEY(8) = '1' AND count = 6) then
         count := count + 1;
         variant <= '0';
         next_state <= TEST8;
      elsif (KEY(2) = '1' AND count = 6) then
         count := count + 1;
         variant <= '1';
         next_state <= TEST2;

      elsif (KEY(15) = '1') then
         if (count = 11 AND variant = '1') then match := true; 
		 end if;
         next_state <= PRINT_MESSAGE; 

      elsif (KEY(14 downto 0) /= "0000000000000000") then
         next_state <= INVALID;
      
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST2 =>
      next_state <= TEST2;
      if (KEY(3) = '1' AND count = 4) then
         count := count + 1;
         next_state <= TEST3;
      elsif (KEY(5) = '1' AND count = 7 AND variant = '1') then
         count := count + 1;
         next_state <= TEST5;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE; 

      elsif (KEY(14 downto 0) /= "0000000000000000") then
         next_state <= INVALID;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST3 =>
      next_state <= TEST3;
      if (KEY(0) = '1' AND count = 5) then
         count := count +1;
         next_state <= TEST0;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE; 

      elsif (KEY(14 downto 0) /= "0000000000000000") then
         next_state <= INVALID;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST4 =>
      next_state <= TEST4;
      if (KEY(6) = '1' AND count = 1) then
         count := count +1;
         next_state <= TEST6;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE; 

      elsif (KEY(14 downto 0) /= "0000000000000000") then
         next_state <= INVALID;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST5 =>
      next_state <= TEST5;
      if (KEY(8) = '1' AND count = 8 AND variant = '0') then
         count := count + 1;
         next_state <= TEST8;
      elsif (KEY(7) = '1' AND count = 8 AND variant = '1') then
         count := count + 1;
         next_state <= TEST7;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE;

      elsif (KEY(14 downto 0) /= "0000000000000000") then
         next_state <= INVALID;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST6 =>
      next_state <= TEST6;
      if (KEY(0) = '1' AND count = 2) then
         count := count +1;
         next_state <= TEST0;
      
      elsif (KEY(0) = '1' AND count = 10 AND variant = '1') then
         count := count + 1;
         next_state <= TEST0;
      elsif (KEY(15) = '1') then
         if (count = 10 AND variant = '0') then match := true;
		 end if;
         next_state <= PRINT_MESSAGE;

      elsif (KEY(14 downto 0) /= "0000000000000000") then
         next_state <= INVALID;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST7 =>
      next_state <= TEST7;
      if (KEY(6) = '1') then
         next_state <= TEST6;
      elsif (KEY(6) = '1' AND count = 9 AND variant = '1') then
         count := count + 1;
         next_state <= TEST6;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE;

      elsif (KEY(14 downto 0) /= "0000000000000000") then
         next_state <= INVALID;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST8 =>
      next_state <= TEST8;
      if (KEY(5) = '1' AND count = 7 AND variant = '0') then
         count := count + 1;
         next_state <= TEST5;
      elsif (KEY(6) = '1' AND count = 9 AND variant = '0') then
         count := count + 1;
         next_state <= TEST6;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE;

      elsif (KEY(14 downto 0) /= "0000000000000000") then
         next_state <= INVALID;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when INVALID =>
      next_state <= INVALID;
      if (KEY(15) = '1') then
         match := false;
         next_state <= PRINT_MESSAGE;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_MESSAGE =>
      next_state <= PRINT_MESSAGE;
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
   FSM_CNT_CE     <= '0';
   FSM_MX_MEM     <= '0';
   FSM_MX_LCD     <= '0';
   FSM_LCD_WR     <= '0';
   FSM_LCD_CLR    <= '0';

   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_MESSAGE =>
      if match = true then
         FSM_MX_MEM <= '1';
      end if;
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

