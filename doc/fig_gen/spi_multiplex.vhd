library ieee;
use ieee.std_logic_1164.all;

entity spi_multiplex is
    port (
        clk: in std_logic
    );
end spi_multiplex;

architecture rtl of spi_multiplex is
component counter
    generic (
        period: integer := 50
    );
    port (
        clk: in std_logic;
        cnt: out integer
    );
end component;
signal mcnt, cnta: integer;
begin
    mtim: counter generic map(25) port map(clk, mcnt);
    tima: counter generic map(50) port map(clk, cnta);
end rtl;


library ieee;
use ieee.std_logic_1164.all;

entity counter is
    generic (
        period: integer := 50
    );
    port (
        clk: in std_logic;
        cnt: out integer
    );
end counter;

architecture behavior of counter is
    signal cnti: integer := 0;
begin
    process(clk)
    begin
        if rising_edge(clk) then
            if cnti = period - 1 then
                cnti <= 0;
            else
                cnti <= cnti + 1;
            end if;
        end if;
    end process;
    cnt <= cnti;
end behavior;
