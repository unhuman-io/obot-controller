library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity tb_center_pwm is
end entity tb_center_pwm;

architecture test of tb_center_pwm is
  signal clk: std_logic := '0';
  signal rst: std_logic := '0';
  signal pwm: std_logic;
  signal duty: std_logic_vector(3 downto 0) := "0000";
begin
    uut: entity work.center_pwm
        port map (
            clk => clk,
            rst => rst,
            pwm => pwm,
            duty => duty
        );
    
    process
    begin
        clk <= '0';
        wait for 5 ns;
        clk <= '1';
        wait for 5 ns;
    end process;
    
    process
    begin
        rst <= '1';
        wait for 10 ns;
        rst <= '0';
        wait for 10 ns;
        wait;
    end process;

    process
    begin
        wait for 100 ns;
        duty <= "0001";
        wait for 100 ns;
        duty <= "0010";
        wait for 100 ns;
        duty <= "0011";
        wait for 100 ns;
        duty <= "0100";
        wait for 100 ns;
        wait;
    end process;
end architecture test;

