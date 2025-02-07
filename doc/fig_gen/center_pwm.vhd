library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity center_pwm is 
generic (
  period : integer := 170e6*32/40000/2
);
port (
  clk: in std_logic;
  rst: in std_logic;
  pwm: out std_logic;
  duty: in integer range 0 to period;
  cnt: inout integer range 0 to period
);
end entity center_pwm;

architecture blah of center_pwm is
  subtype cnt_t is integer range 0 to period;
  type upd_t is (UP, DOWN);
  signal upd: upd_t := UP;
begin
    process(clk, rst)
    begin
        if rst = '1' then
            cnt <= 0;
        elsif rising_edge(clk) then
            if upd = UP then
                if cnt = cnt_t'high -1 then
                    upd <= DOWN;
                end if;
                cnt <= cnt + 1;
            else
                if cnt = 1 then
                    upd <= UP;
                end if;
                cnt <= cnt - 1;
            end if;
        end if;
    end process;

    pwm <= '1' when cnt >= duty else '0';
end architecture;
