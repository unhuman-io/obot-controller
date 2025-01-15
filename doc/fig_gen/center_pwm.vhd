library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.numeric_unsigned.all;

entity center_pwm is port (
  clk: in std_logic;
  rst: in std_logic;
  pwm: out std_logic;
  duty: in std_logic_vector(3 downto 0)
);
end entity center_pwm;

architecture blah of center_pwm is
  signal cnt: unsigned := (others => '0');
  signal up: std_logic := '1';
  signal adc_trigger: std_logic;
  signal fl_trigger: std_logic;
  signal adc_zero_trigger: std_logic;
  signal cnt2: unsigned := (others => '0');
  signal enc_trigger: std_logic;
begin
    process(clk, rst)
    begin
        if rst = '1' then
            cnt <= (others => '0');
        elsif rising_edge(clk) then
            if up = '1' then
                if cnt = 20 then
                    up <= '0';
                end if;
                cnt <= cnt + 1;
            else
                if cnt = 0 then
                    up <= '1';
                end if;
                cnt <= cnt - 1;
            end if;
        end if;
    end process;

    process(clk, rst)
    begin
        if rst = '1' then
            cnt2 <= (others => '0');
        elsif rising_edge(clk) then
            cnt2 <= cnt2 + 1;
        end if;
    end process;

    adc_trigger <= '1' when cnt = 0 else '0';
    fl_trigger <= '1' when cnt = 1 else '0';

    adc_zero_trigger <= '1' when cnt = 20 else '0';
    enc_trigger <= '1' when cnt2 = 10 else
                   '1' when cnt2 = 5 else
                   '0';

    --pwm <= '1' when cnt < to_integer(duty) else '0';
end architecture;
