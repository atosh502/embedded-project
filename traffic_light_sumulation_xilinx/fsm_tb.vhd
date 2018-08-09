LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
 
ENTITY fsm_tb IS
END fsm_tb;
 
ARCHITECTURE behavior OF fsm_tb IS 
 
    COMPONENT fsm
    PORT(
         Y : OUT  std_logic_vector(3 downto 0);
         CLR : IN  std_logic;
         CLK : IN  std_logic;
         vehicle_N : IN  std_logic;
         vehicle_M : IN  std_logic;
         vehicle_T : IN  std_logic;
         vehicle_B : IN  std_logic;
         urgent_B : IN  std_logic;
         urgent_M : IN  std_logic;
         urgent_T : IN  std_logic;
         urgent_N : IN  std_logic;
         time_large : IN  std_logic;
         time_small : IN  std_logic
        );
    END COMPONENT;
    

   --Inputs
   signal CLR : std_logic := '0';
   signal CLK : std_logic := '0';
   signal vehicle_N : std_logic := '0';
   signal vehicle_M : std_logic := '0';
   signal vehicle_T : std_logic := '0';
   signal vehicle_B : std_logic := '0';
   signal urgent_B : std_logic := '0';
   signal urgent_M : std_logic := '0';
   signal urgent_T : std_logic := '0';
   signal urgent_N : std_logic := '0';
   signal time_large : std_logic := '0';
   signal time_small : std_logic := '0';

 	--Outputs
   signal Y : std_logic_vector(3 downto 0);

   -- Clock period definitions
   constant CLK_period : time := 10 ns;
 
BEGIN
 
	-- Instantiate the Unit Under Test (UUT)
   uut: fsm PORT MAP (
          Y => Y,
          CLR => CLR,
          CLK => CLK,
          vehicle_N => vehicle_N,
          vehicle_M => vehicle_M,
          vehicle_T => vehicle_T,
          vehicle_B => vehicle_B,
          urgent_B => urgent_B,
          urgent_M => urgent_M,
          urgent_T => urgent_T,
          urgent_N => urgent_N,
          time_large => time_large,
          time_small => time_small
        );

   -- Clock process definitions
   CLK_process :process
   begin
		CLK <= '0';
		wait for CLK_period/2;
		CLK <= '1';
		wait for CLK_period/2;
   end process;
 

   -- Stimulus process
   stim_proc: process
   begin	
			CLR <= '0';
			time_large <= '1';
			vehicle_B <= '1';
		
		wait for 500 ns;	-- ST0 -- vehicle from Bagmati bridge can cross
			time_large <= '0';
			vehicle_T <= '1';
		
		wait for CLK_period;
			time_small <= '1';
		wait for 300 ns;	-- ST1
			time_small <= '0';
		
		wait for CLK_period;
			time_small <= '1';
		wait for 300 ns; 	-- ST2
			time_small <= '0';
			
			time_large <= '1';
			vehicle_M <= '1';
		wait for 500 ns; 	-- ST3 -- vehicles from Maitighar can cross
			time_large <= '0';
			vehicle_T <= '1';
			
		wait for CLK_period;
			time_small <= '1';
		wait for 300 ns; 	-- ST4
			time_small <= '0';
				
		wait for CLK_period;
			time_small <= '1';
		wait for 300 ns; 	-- ST5
			time_small <= '0';
			
		wait for CLK_period;
			time_large <= '1';
			vehicle_T <= '1';
		wait for 500 ns;	-- ST6 -- vehicles from Tripureshwor can cross
			time_large <= '0';
			vehicle_M <= '1';
		
		wait for CLK_period;
			time_small <= '1';
		wait for 300 ns; 	-- ST7
			time_small <= '0';
				
		wait for CLK_period;
			time_small <= '1';
		wait for 300 ns; 	-- ST8
			time_small <= '0';
		
		wait for CLK_period;
			time_large <= '1';
		wait for 500 ns;	-- ST9 -- pedestrains can cross
			time_large <= '0';
			vehicle_B <= '1';
			
		wait for CLK_period;
			time_small <= '1';
		wait for 300 ns;	-- ST10
			time_small <= '0';
			
		wait for CLK_period;
			time_large <= '1';
		wait for 250 ns;	-- ST0
			urgent_M <= '1';
		wait for CLK_period;	-- jumps to ST11
			time_large <= '0';	-- reset the large timer
			time_small <= '1';	-- start the small timer
		wait for 300 ns;	-- ST11
			time_small <= '0';
		
      wait;
   end process;

END;
