library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity adc_seq is
    port (
        clk: in std_logic;
        rst: in std_logic;
        trg: in std_logic;
        eos: out std_logic
    );
end entity adc_seq;

architecture blah of adc_seq is
    type state_t is (IDLE, SMP1, CNV1);
    signal state: state_t := IDLE;
    signal cnt: integer range 0 to 32*4 := 0;
begin
    process(clk, rst)
    begin
        if rst = '1' then
            state <= IDLE;
            cnt <= 0;
        elsif rising_edge(clk) then
            eos <= '0';
            case state is
                when IDLE =>
                    cnt <= 0;
                    if trg = '1' then
                        state <= SMP1;
                    end if;
                when SMP1 =>
                    if cnt = 12 then
                        state <= CNV1;
                        cnt <= 0;
                    else
                        cnt <= cnt + 1;
                    end if;
                when CNV1 =>
                    if cnt = 12 then
                        state <= IDLE;
                        cnt <= 0;
                        eos <= '1';
                    else
                        cnt <= cnt + 1;
                    end if;
            end case;
        end if;
    end process;
end architecture;