-- cpu.vhd: Simple 8-bit CPU (BrainFuck interpreter)
-- Copyright (C) 2015 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): Truong Lai Nhat Tuan (xlainh00)
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(12 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1' a DATA_RDWR='0'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1' a DATA_RDWR='1'
   DATA_RDWR  : out std_logic;                    -- cteni (1) / zapis (0)
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA <- stav klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna
   IN_REQ    : out std_logic;                     -- pozadavek na vstup data
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- LCD je zaneprazdnen (1), nelze zapisovat
   OUT_WE   : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

 -- zde dopiste potrebne deklarace signalu
 type FSM_state is (
   idle, fetch0, fetch1, decode, 
   dataInc0, dataInc1, dataDec0, dataDec1, dataNext, dataPrev, 
   dataPrint, dataInput0, dataInput1,
   tmpsave, tmpload, 
   beginwhile0, beginwhile1, endwhile0, endwhile1,
   skipdecode, whiledecode0, whiledecode1, whiledecode2, whileFetch0, whileFetch1,
   test1, skipchar, halt
 );

 signal present_state                     :  FSM_state;
 signal next_state                        :  FSM_state := fetch0;

 signal instr_pointer                     :  std_logic_vector(12 downto 0) := (others => '0');
 signal instr_reg                         :  std_logic_vector(7 downto 0) := (others => '0');
 signal instr_save, next_instr            :  std_logic := '0';

 signal data_pointer                      :  std_logic_vector(12 downto 0) := (others => '0');
 signal data_reg                          :  std_logic_vector(7 downto 0) := "00000000";
 signal data_save, data_load              :  std_logic := '0';    -- register 
 signal curr_data, next_data, prev_data   :  std_logic := '0';    -- pointer toggles         
 signal data_init, data_inc, data_dec     :  std_logic := '0';    -- operation toggle 
 signal data_input, data_output           :  std_logic := '0';             

 signal while_instr_pointer               :  std_logic_vector(12 downto 0) := "1000000000000";  -- while instr at X"1000"
 signal while_skip, while_prep, while_run :  std_logic := '0';
 signal start_while, curr_while, next_while :  std_logic := '0';    -- while pointer toggle
 signal instr_move                        :  std_logic := '0';    -- instr load

begin

 -- zde dopiste vlastni VHDL kod
---------------------- NEXT STATE LOGIC ----------------------
next_state_logic : process(RESET, CLK)
begin
   if RESET = '1' then
      present_state <= idle;
   elsif rising_edge(CLK) AND EN = '1' then      
      present_state <= next_state;
   end if;
end process;

---------------------- MEMORY POINTERS ----------------------
shared_memory_logic : process(RESET, CLK)
begin
   if RESET = '1' then
      DATA_ADDR <= (others => '0');
      instr_pointer <= (others => '0');
      data_pointer <= "0000000000000";
      while_instr_pointer <= "1000000000000";

   elsif rising_edge(CLK) then

      if next_instr = '1' then
         DATA_ADDR <= instr_pointer + 1;      
         instr_pointer <= instr_pointer + 1;

      elsif start_while = '1' then
         DATA_ADDR <= "1000000000000";
         while_instr_pointer <= "1000000000000";

      elsif curr_while = '1' then 
         DATA_ADDR <= while_instr_pointer;

      elsif next_while = '1' then 
         DATA_ADDR <= while_instr_pointer + 1;
         while_instr_pointer <= while_instr_pointer + 1;

      elsif curr_data = '1' then
         DATA_ADDR <= data_pointer;

      elsif next_data = '1' then
         data_pointer <= data_pointer + 1;

      elsif prev_data = '1' then 
         data_pointer <= data_pointer - 1;
      end if;

   end if;
end process;

---------------------- REGISTER MANIPULATION ----------------------
register_logic : process(RESET, CLK)
begin
   if RESET = '1' then
      instr_reg <= (others => '0');
      data_reg <= "01000001";
   elsif rising_edge(CLK) then

      if instr_save = '1' then
         instr_reg <= DATA_RDATA;

      elsif data_save = '1' then
         data_reg <= DATA_RDATA;

      end if;
   end if;
end process;

---------------------- PREPARE TO REWRITE DATA ----------------------
data_operations : process(CLK)
begin
   if rising_edge(CLK) then
      OUT_WE <= '0';

      if data_init = '1' then
         DATA_WDATA <= "00000000";

      elsif data_load = '1' then    -- load from register
         DATA_WDATA <= data_reg;

      elsif instr_move = '1' then         -- move to other address
         DATA_WDATA <= instr_reg;

      elsif data_input = '1' then   -- save input
         DATA_WDATA <= IN_DATA;

      elsif data_output = '1' then 
         OUT_DATA <= DATA_RDATA;
         OUT_WE <= '1'; 

      elsif data_inc = '1' then
         DATA_WDATA <= DATA_RDATA + 1;

      elsif data_dec = '1' then 
         DATA_WDATA <= DATA_RDATA - 1;
      end if;
   end if;
end process;

---------------------------------------------------------------
fsm : process(present_state, OUT_BUSY, IN_VLD)
begin
   DATA_EN <= '0';
   DATA_RDWR <= '1';

   instr_save <= '0';
   next_instr <= '0';

   data_save <= '0';
   data_load <= '0';

   curr_data <= '0';
   next_data <= '0';
   prev_data <= '0';
   data_init <= '0';
   data_inc <= '0';
   data_dec <= '0';

   start_while <= '0';
   curr_while <= '0';
   next_while <= '0';
   instr_move <= '0';

   data_input <= '0';
   IN_REQ <= '0';
   data_output <= '0';

   case present_state is
      -- - - - - - - - - - - - - - - - - - - - -
      when idle =>
         next_state <= fetch0;
      -- - - - - - - - - - - - - - - - - - - - -
      when fetch0 =>    
         next_state <= fetch1;
         DATA_RDWR <= '1';          -- read instruction from DATA_ADDR
         DATA_EN <= '1';
         data_init <= '1';          -- prepare to rewrite instruction code
      -- - - - - - - - - - - - - - - - - - - - -
      when fetch1 =>          
         instr_save <= '1';         -- save instr in register
         DATA_EN <= '1';
         DATA_RDWR <= '0';          -- rewrite instr code

         if while_skip = '1' then
            next_state <= skipdecode; -- skip next instructions

         elsif while_prep = '1' then
            next_state <= whiledecode0;
            curr_while <= '1';

         else
            next_state <= decode;
            curr_data <= '1';       -- move to current data position
         end if;
      -- - - - - - - - - - - - - - - - - - - - -
      when decode =>
         case instr_reg is
            when "00111110" =>   ---- ">"
               next_state <= dataNext;
               next_data <= '1';

            when "00111100" =>   ---- "<"
               next_state <= dataPrev;
               prev_data <= '1';

            when X"2B" =>        ---- "+"
               next_state <= dataInc0;
               DATA_EN <= '1';
               DATA_RDWR <= '1';       -- load data

            when "00101101" =>   ---- "-"
               next_state <= dataDec0;
               DATA_EN <= '1';
               DATA_RDWR <= '1';       -- load data

            when "01011011" =>   ---- "["
               next_state <= beginwhile0;
               DATA_EN <= '1';
               DATA_RDWR <= '1';

            when "01011101" =>   ---- "]"
               next_state <= endwhile0;
               DATA_EN <= '1';
               DATA_RDWR <= '1';

            when "00101110" =>   ---- "."
               next_state <= dataPrint;
               DATA_EN <= '1';
               DATA_RDWR <= '1';       -- load current data

            when "00101100" =>   ---- ","
               next_state <= dataInput0;
               IN_REQ <= '1';          -- request input

            when "00100100" =>   ---- "$"
               next_state <= tmpsave;
               DATA_EN <= '1';
               DATA_RDWR <= '1';
            
            when "00100001" =>   ---- "!"
               next_state <= tmpload;
               data_load <= '1';
            
            when "00000000" =>   ---- "null"
               next_state <= halt;
            
            when others =>       ---- other chars
               next_state <= skipchar;
         end case;
      -- - - - - - - - - - - - - - - - - - - - -
      when skipdecode =>
         next_state <= fetch0;
         next_instr <= '1';
         if instr_reg = X"5D" then  -- end bracket "]" found
            while_skip <= '0';
         end if;
      -- - - - - - - - - - - - - - - - - - - - -
      when whiledecode0 =>
         next_state <= whiledecode1;
         instr_move <= '1';         -- WDATA <= instruction

      when whiledecode1 =>
         next_state <= whiledecode2;
         DATA_EN <= '1';
         DATA_RDWR <= '0';          -- move instr to "1 0000 0000 0000"
         next_while <= '1';         -- pointer increment

      when whiledecode2 =>
         next_state <= decode;
         curr_data <= '1';
      -- - - - - - - - - - - - - - - - - - - - -
      when dataNext =>
         if while_run = '0' then
            next_state <= fetch0;
            next_instr <= '1'; 
         else 
            next_state <= whileFetch0;
            next_while <= '1';
         end if;

      when dataPrev =>
         if while_run = '0' then
            next_state <= fetch0;
            next_instr <= '1'; 
         else 
            next_state <= whileFetch0;
            next_while <= '1';
         end if;
      -- - - - - - - - - - - - - - - - - - - - -
      when dataInc0 =>
         next_state <= dataInc1;
         data_inc <= '1';  -- increment

      when dataInc1 =>
         DATA_EN <= '1';
         DATA_RDWR <= '0';             -- allow memory rewrite
         if while_run = '0' then
            next_state <= fetch0;
            next_instr <= '1';            -- and prepare next instr
         else 
            next_state <= whileFetch0;
            next_while <= '1';
         end if;
      -- - - - - - - - - - - - - - - - - - - - -
      when dataDec0 =>
         next_state <= dataDec1;
         data_dec <= '1';  -- decrement

      when dataDec1 =>
         DATA_EN <= '1';
         DATA_RDWR <= '0';             -- allow memory rewrite
         if while_run = '0' then
            next_state <= fetch0;
            next_instr <= '1'; 
         else 
            next_state <= whileFetch0;
            next_while <= '1';
         end if;
      -- - - - - - - - - - - - - - - - - - - - -
      when beginwhile0 =>
         next_state <= beginwhile1;
         start_while <= '1';
      -- - - - - - - - - - - - - - - - - - - - -
      when beginwhile1 =>
         next_state <= fetch0;
         next_instr <= '1';
         if DATA_RDATA = "00000000" then
            while_skip <= '1';
         else 
            while_prep <= '1';
         end if;
      -- - - - - - - - - - - - - - - - - - - - -
      when endwhile0 =>
         next_state <= endwhile1;
         while_prep <= '0';
      -- - - - - - - - - - - - - - - - - - - - -
      when endwhile1 =>
         if DATA_RDATA = "00000000" then
            next_state <= fetch0;
            next_instr <= '1';
            while_run <= '0';
         else 
            next_state <= whileFetch0;
            start_while <= '1';
            while_run <= '1';
         end if;
      -- - - - - - - - - - - - - - - - - - - - -
      when whileFetch0 =>
         next_state <= whileFetch1;
         DATA_EN <= '1';
         DATA_RDWR <= '1';
      -- - - - - - - - - - - - - - - - - - - - -
      when whileFetch1 =>
         next_state <= decode;
         instr_save <= '1';
         curr_data <= '1';
      -- - - - - - - - - - - - - - - - - - - - -
      when dataPrint =>
         if OUT_BUSY = '1' then        -- wait till display not busy
            next_state <= dataPrint;
         else                          -- then send data to OUT
            data_output <= '1';

            if while_run = '0' then
               next_state <= fetch0;
               next_instr <= '1'; 
            else 
               next_state <= whileFetch0;
               next_while <= '1';
            end if;
         end if;
      -- - - - - - - - - - - - - - - - - - - - -
      when dataInput0 =>
         if IN_VLD = '0' then
            next_state <= dataInput0;
         else
            next_state <= dataInput1;
            data_input <= '1';         -- save input and prepare to rewrite
         end if;

      when dataInput1 =>
         DATA_EN <= '1';
         DATA_RDWR <= '0';
         if while_run = '0' then
            next_state <= fetch0;
            next_instr <= '1'; 
         else 
            next_state <= whileFetch0;
            next_while <= '1';
         end if;
      -- - - - - - - - - - - - - - - - - - - - -
      when tmpsave =>
         data_save <= '1';    -- save data in register
         if while_run = '0' then
            next_state <= fetch0;
            next_instr <= '1'; 
         else 
            next_state <= whileFetch0;
            next_while <= '1';
         end if;
      -- - - - - - - - - - - - - - - - - - - - -
      when tmpload =>
         DATA_EN <= '1';
         DATA_RDWR <= '0';    -- rewrite current data with data in register
         if while_run = '0' then
            next_state <= fetch0;
            next_instr <= '1'; 
         else 
            next_state <= whileFetch0;
            next_while <= '1';
         end if;
      -- - - - - - - - - - - - - - - - - - - - -
      when halt =>
         next_state <= halt;
      -- - - - - - - - - - - - - - - - - - - - -
      when skipchar =>
         if while_run = '0' then
            next_state <= fetch0;
            next_instr <= '1'; 
         else 
            next_state <= whileFetch0;
            next_while <= '1';
         end if;
      -- - - - - - - - - - - - - - - - - - - - -
      when others => null;
   end case;

end process;

end behavioral;
 
