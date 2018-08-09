library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity fsm is
    Port ( Y : out  STD_LOGIC_VECTOR(3 downto 0);
           CLR : in  STD_LOGIC;
           CLK : in  STD_LOGIC;
			  vehicle_N : in STD_LOGIC;	-- 1 if at least a vehicle is present at Norvic side
			  vehicle_M : in STD_LOGIC;	-- 1 if at least a vehicle is present at Maitighar side
			  vehicle_T : in STD_LOGIC; 	-- 1 if at least a vehicle is present at Tripureshwor side
			  vehicle_B : in STD_LOGIC; 	-- 1 if at least a vehicle is present at Bagmati side
			  urgent_B : in STD_LOGIC;		-- 1 if an urgent vehilce approaches from Bagmati side
			  urgent_M : in STD_LOGIC;		-- 1 if an urgent vehilce approaches from Maitighar side
			  urgent_T : in STD_LOGIC;		-- 1 if an urgent vehilce approaches from Tripureshwor side
			  urgent_N : in STD_LOGIC;		-- 1 if an urgent vehilce approaches from Norvic side
			  time_large : in STD_LOGIC;	-- 1 if counting upto 50s else 0 if it has already overrun
			  time_small : in STD_LOGIC); -- 1 if counting upto 3s else 0 if it has already overrun
end fsm;

architecture Behavioral of fsm is
	type state_type is (ST0,ST1,ST2,ST3,ST4,ST5,ST6,ST7,ST8,ST9,ST10,ST11);	
	signal PS,NS : state_type;
	-- PS is the present state
	-- NS is the next state
begin
	-- this is the sequential circuit
	-- it gets input the next state from the combinational circuit
	
	sync_proc: process(CLK,CLR,NS)
	begin
		if (CLR = '1') then
			PS <= ST0;
		elsif (rising_edge(CLK)) then
			PS <= NS;
		end if;
	end process sync_proc;
	
	-- this is the combinational circuit
	-- it gets the present state as input from the sequential circuit
	
	comb_proc: process(PS, 
						vehicle_N, vehicle_M, vehicle_T, vehicle_B,
						urgent_N, urgent_M, urgent_T, urgent_B,
						time_large, time_small)
	begin
		
		if (PS = ST0) then
		
			-- B1:GREEN, B2:GREEN, T1:RED, T2:GREEN, N1:RED, N2:GREEN, M1:RED, M2:GREEN
			if (urgent_B = '1' or time_large = '1' or 
					(vehicle_T = '0' and vehicle_N = '0' and vehicle_M = '0')) then
				NS <= ST0;
			elsif (urgent_B = '0' and (urgent_T = '1' or urgent_M = '1')) then
				NS <= ST11;
			elsif (time_large = '0' and (vehicle_T = '1' or vehicle_N = '1' or vehicle_M = '1')) then
				NS <= ST1;
			else
				NS <= ST0;
			end if;
		
		elsif (PS = ST1) then 
			
			-- B1:YELLOW, B2:GREEN, T1:RED, T2:GREEN, N1:RED, N2:GREEN, M1:RED, M2:GREEN
			if (time_small = '1') then
				NS <= ST1;
			elsif (time_small = '0') then
				NS <= ST2;
			elsif (urgent_M = '1' or urgent_B = '1' or urgent_T = '1') then
				NS <= ST11;
			else
				NS <= ST1;
			end if;
			
		elsif (PS = ST2) then 
			
			-- B1:RED, B2:GREEN, T1:RED, T2:GREEN, N1:RED, N2:GREEN, M1:YELLOW, M2:GREEN
			if (time_small = '1') then
				NS <= ST2;
			elsif (time_small = '0') then 
				NS <= ST3;
			elsif (urgent_M = '1' or urgent_B = '1' or urgent_T = '1') then
				NS <= ST11;
			else 
				NS <= ST2;
			end if;
			
		elsif (PS = ST3) then
		
			-- B1:RED, B2:GREEN, T1:RED, T2:GREEN, N1:RED, N2:GREEN, M1:GREEN, M2:GREEN
			if (urgent_M = '1' or time_large = '1' or 
					(vehicle_T = '0' and vehicle_N = '0' and vehicle_B = '0')) then
				NS <= ST3;
			elsif (urgent_M = '0' and (urgent_T = '1' or urgent_B = '1')) then
				NS <= ST11;
			elsif (time_large = '0' and (vehicle_T = '1' or vehicle_N = '1' or vehicle_B = '1')) then
				NS <= ST4;
			else
				NS <= ST3;
			end if;
		
		elsif (PS = ST4) then
			
			-- B1:RED, B2:GREEN, T1:RED, T2:GREEN, N1:RED, N2:GREEN, M1:YELLOW, M2:GREEN
			if (time_small = '1') then 
				NS <= ST4;
			elsif (time_small = '0') then
				NS <= ST5;
			elsif (urgent_M = '1' or urgent_B = '1' or urgent_T = '1') then
				NS <= ST11;
			else 
				NS <= ST4;
			end if;

		elsif (PS = ST5) then
			
			-- B1:RED, B2:GREEN, T1:YELLOW, T2:GREEN, N1:RED, N2:GREEN, M1:RED, M2:GREEN
			if (time_small = '1') then
				NS <= ST5;
			elsif (time_small = '0') then
				NS <= ST6;
			elsif (urgent_M = '1' or urgent_B = '1' or urgent_T = '1') then
				NS <= ST11;
			else 
				NS <= ST5;
			end if;

		elsif (PS = ST6) then
			
			-- B1:RED, B2:GREEN, T1:GREEN, T2:GREEN, N1:RED, N2:GREEN, M1:RED, M2:GREEN
			if (urgent_T = '1' or time_large = '1' or 
					(vehicle_M = '0' and vehicle_N = '0' and vehicle_B = '0')) then
				NS <= ST6;
			elsif (urgent_T = '0' and (urgent_M = '1' or urgent_B = '1')) then
				NS <= ST11;
			elsif (time_large = '0' and (vehicle_M = '1' or vehicle_N = '1' or vehicle_B = '1')) then
				NS <= ST7;
			else
				NS <= ST6;
			end if;

		
		elsif (PS = ST7) then

			-- B1:RED, B2:GREEN, T1:YELLOW, T2:GREEN, N1:RED, N2:GREEN, M1:RED, M2:GREEN
			if (time_small = '1') then
				NS <= ST7;
			elsif (time_small = '0') then
				NS <= ST8;
			elsif (urgent_M = '1' or urgent_B = '1' or urgent_T = '1') then
				NS <= ST11;
			else 
				NS <= ST7;
			end if;
		
		elsif (PS = ST8) then
			
			-- careful!
			-- B1:RED, B2:YELLOW, T1:RED, T2:YELLOW, N1:RED, N2:YELLOW, M1:RED, M2:YELLOW
			if (time_small = '1') then
				NS <= ST8;
			elsif (time_small = '0') then
				NS <= ST9;
			elsif (urgent_M = '1' or urgent_B = '1' or urgent_T = '1') then
				NS <= ST11;
			else 
				NS <= ST8;
			end if;

		elsif (PS = ST9) then
		
			-- B1:RED, B2:RED, T1:RED, T2:RED, N1:RED, N2:RED, M1:RED, M2:RED
			if (time_large = '1') then
				NS <= ST9;
			elsif (time_large = '0' and (vehicle_T = '1' or vehicle_M = '1' or vehicle_N = '1' or vehicle_B = '1')) then
				NS <= ST10;
			elsif (urgent_M = '1' or urgent_B = '1' or urgent_T = '1') then
				NS <= ST11;
			else
				NS <= ST9;
			end if;

		elsif (PS = ST10) then
		
			-- B1:YELLOW, B2:YELLOW, T1:RED, T2:YELLOW, N1:RED, N2:YELLOW, M1:RED, M2:YELLOW
			if (time_small = '1') then
				NS <= ST10;
			elsif (time_small = '0') then
				NS <= ST0;
			elsif (urgent_M = '1' or urgent_B = '1' or urgent_T = '1') then
				NS <= ST11;
			else 
				NS <= ST10;
			end if;
		
		elsif (PS = ST11) then

			-- B1:YELLOW, B2:YELLOW, T1:YELLOW, T2:YELLOW, N1:YELLOW, N2:YELLOW, M1:YELLOW, M2:YELLOW
			if (time_small = '1') then
				NS <= ST11;
			elsif (time_small = '0') then
				
				if (urgent_B = '1') then
					NS <= ST0;
				elsif (urgent_M = '1') then
					NS <= ST3;
				elsif (urgent_T = '1') then 
					NS <= ST6;
				else
					NS <= ST11;
				end if;
				
			else
				NS <= ST11;
			end if;
			
		else
			NS <= ST0;
		end if;
		
	end process comb_proc;
	
	with PS select
		Y <= 	"0000" when ST0,
				"0001" when ST1,
				"0011" when ST2,
				"0010" when ST3,
				"0110" when ST4,
				"0111" when ST5,
				"0101" when ST6,
				"0100" when ST7,
				"1100" when ST8,
				"1101" when ST9,
				"1111" when ST10,
				"1110" when ST11,
				"0000" when others;
		
end Behavioral;

