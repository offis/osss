LIBRARY IEEE;
USE IEEE.std_logic_1164.all;
USE IEEE.numeric_std.all;
USE std.textio.all;

entity Testbench is
end entity Testbench;

architecture sim of Testbench is

  CONSTANT clk_period : time := 10 ns;
  SIGNAL clk   : BOOLEAN := FALSE;
  SIGNAL reset, alarm : BOOLEAN;

begin
  clk <= not clk after clk_period/2;
  reset <= true, false after 10.5*clk_period;

  dut : entity work.AlarmClock
    PORT MAP
    (
      clock => clk,
      reset => reset,
      alarm => alarm
    );
    
  notify : PROCESS (clk) IS
    VARIABLE tmp : line;
    VARIABLE cyclenum : integer := 0;
  BEGIN
    if clk'event AND clk 
    then
    
      if not reset then 
        cyclenum := cyclenum + 1; 
      end if;      

      if alarm then    
        write(tmp, cyclenum);
        write(tmp, STRING'(": Alarm!"));
        writeline(output, tmp);
      end if;
    end if;
  
  END PROCESS notify;

end architecture sim;