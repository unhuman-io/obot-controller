library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity tb_center_pwm is
end entity tb_center_pwm;

architecture test of tb_center_pwm is
    constant period : integer := 170e6*32/50000/2;
    signal clk: std_logic := '0';
    signal clk_adc : std_logic := '0';
    signal clk_cpu : std_logic := '0';
    signal rst: std_logic := '0';
    signal pwm: std_logic;
    signal cmp1: integer range 0 to period := period/2;
    signal cmp1s: integer range 0 to period := period/2;
    signal cnt: integer range 0 to period;
    signal adc_trg : std_logic := '0';
    signal adc_eos : std_logic := '0';
    signal update_trg : std_logic := '0';
    type foc_exec_t is (IDLE, RUNNING);
    signal foc_exec : foc_exec_t := IDLE;
begin
    uut: entity work.center_pwm
        generic map (
            period => period
        )
        port map (
            clk => clk,
            rst => rst,
            pwm => pwm,
            duty => cmp1s,
            cnt => cnt
        );

    uut2: entity work.adc_seq
        port map (
            clk => clk_adc,
            rst => rst,
            trg => adc_trg,
            eos => adc_eos
        );
    
    process
        constant clk_mult : integer := 32;
        constant clk_frequency : integer := 170e6;
        constant period : time := 1 sec/(clk_mult * clk_frequency);
    begin
        clk <= '0';
        wait for period/2;
        clk <= '1';
        wait for period/2;
    end process;

    process (clk)
        constant adc_div : integer := 32*4;
        variable cnt : integer range 0 to adc_div := 0;
    begin
        if rising_edge(clk) then
            cnt := cnt + 1;
            if cnt = adc_div/2 then
                clk_adc <= '0';
            end if;
            if cnt = adc_div then
                cnt := 0;
                clk_adc <= '1';
            end if;
        end if;
    end process;

    process (clk)
    constant cpu_div : integer := 32;
    variable cnt : integer range 0 to cpu_div := 0;
    begin
        if rising_edge(clk) then
            cnt := cnt + 1;
            if cnt = cpu_div/2 then
                clk_cpu <= '0';
            end if;
            if cnt = cpu_div then
                cnt := 0;
                clk_cpu <= '1';
            end if;
        end if;
    end process;
    
    process
    begin
        rst <= '1';
        wait for 1 ps;
        rst <= '0';
        wait;
    end process;

    adc_trg <= '1' when cnt <= 1 else '0';
    update_trg <= '1' when cnt = 0 else '0';
    
    process (clk)
    begin
        if rising_edge(clk) then
            if update_trg = '1' then
                cmp1s <= cmp1;
            end if;
        end if;
    end process;

    process (clk_cpu)
        variable cnt : integer range 0 to 900 := 0;
    begin
        if rising_edge(clk_cpu) then
            if adc_eos = '1' then
                foc_exec <= RUNNING;
                cnt := 0;
            end if;
            if foc_exec = RUNNING then
                cnt := cnt + 1;
                if cnt = 900 then
                    foc_exec <= IDLE;
                    cmp1 <= cmp1 + 10000;
                end if;
            end if;
        end if;
    end process;


end architecture test;

