library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.math_real.all;

entity enc_seq is 
generic (
  period : integer := 170e6*32/40000
);
port (
  clk: in std_logic;
  rst: in std_logic
);
end entity enc_seq;

architecture blah of enc_seq is
    subtype cnt_t is integer range 0 to period;
    signal cnt: cnt_t := 0;
    signal enc1_trg: std_logic := '0';
    signal enc2_trg: std_logic := '0';
    signal enc_diag_trg: std_logic := '0';
    type enc_state_t is (IDLE, ENC1, ENC2, ENC_DIAG);
    signal enc_read : enc_state_t := IDLE;
    signal enc_bus_active : std_logic := '0';
begin
    process(clk, rst)
    begin
        if rst = '1' then
            cnt <= 0;
        elsif rising_edge(clk) then
            if cnt = cnt_t'high -1 then
                cnt <= 0;
            else
                cnt <= cnt + 1;
            end if;
        end if;
    end process;

    enc1_trg <= '1' when cnt = integer(period*0.5) else '0';
    enc2_trg <= '1' when cnt = integer(period*0.8) else '0';
    enc_diag_trg <= '1' when cnt = integer(period*0.2) else '0';
    
    process(clk)
        constant read_time : integer := 6*8*32*16;
        variable cnt : integer := 0;
    begin
        if rising_edge(clk) then
            case enc_read is
                when IDLE =>
                    if enc_diag_trg = '1' then
                        enc_read <= ENC_DIAG;
                        cnt := 0;
                    end if;
                    if enc1_trg = '1' then
                        enc_read <= ENC1;
                        cnt := 0;
                    end if;
                    if enc2_trg = '1' then
                        enc_read <= ENC2;
                        cnt := 0;
                    end if;
                when ENC_DIAG =>
                    if cnt = read_time then
                        enc_read <= IDLE;
                        cnt := 0;
                    else
                        cnt := cnt + 1;
                    end if;
                when ENC1 =>
                    if cnt = read_time then
                        enc_read <= IDLE;
                        cnt := 0;
                    else
                        cnt := cnt + 1;
                    end if;
                when ENC2 =>
                    if cnt = read_time then
                        enc_read <= IDLE;
                        cnt := 0;
                    else
                        cnt := cnt + 1;
                    end if;
            end case;
        end if;
    end process;
    
    enc_bus_active <= '1' when enc_read /= IDLE else '0';

end architecture;
