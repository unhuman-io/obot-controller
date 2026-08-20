library ieee;
use ieee.std_logic_1164.all;

entity Motor is
  port (
    MotorCommand: in std_logic;
    ASCIICommand: in std_logic;
    CurrentSensor: in std_logic;
    MotorPosition: in std_logic;
    OutputPosition: in std_logic;
    TorqueSensor: in std_logic;
    MotorStatus: out std_logic;
    ASCIIResponse: out std_logic;
    PWM: out std_logic
  );
end entity Motor;



architecture rtl of Motor is
    component FastLoop is
        port (
            Command: in std_logic;
            FaultAction: in std_logic;
            CurrentSensor: in std_logic;
            Status: out std_logic;
            PWM: out std_logic;
            MotorPosition: in std_logic;
            clk: in std_logic
        );
    end component FastLoop;

    component MainLoop is
        port (
            Command: in std_logic;
            Status: out std_logic;
            OutputPosition: in std_logic;
            TorqueSensor: in std_logic;
            FastLoopStatus: in std_logic;
            FastLoopCommand: out std_logic;
            clk: in std_logic
        );
    end component MainLoop;

    component SystemLoop is
        port (
            MainLoopStatus: in std_logic;
            FaultAction: out std_logic;
            clk: in std_logic
        );
    end component SystemLoop;

    component ClockGen is
        port (
            clk_1kHz: out std_logic;
            clk_10kHz: out std_logic;
            clk_40kHz: out std_logic
        );
    end component ClockGen;

    component Main is
        port (
            ASCIICommand: in std_logic;
            ASCIIResponse: out std_logic
        );
    end component Main;

    signal FastLoopCommand: std_logic;
    signal FastLoopStatus: std_logic;
    signal MainLoopStatus: std_logic;
    signal FaultAction: std_logic;
    signal clk_1kHz: std_logic;
    signal clk_10kHz: std_logic;
    signal clk_40kHz: std_logic;
begin
    fast_loop: component FastLoop
        port map (
            Command => FastLoopCommand,
            CurrentSensor => CurrentSensor,
            MotorPosition => MotorPosition,
            Status => FastLoopStatus,
            FaultAction => FaultAction,
            PWM => PWM,
            clk => clk_40kHz
        );
    
    main_loop: component MainLoop
        port map (
            Command => MotorCommand,
            Status => MainLoopStatus,
            OutputPosition => OutputPosition,
            TorqueSensor => TorqueSensor,
            FastLoopStatus => FastLoopStatus,
            FastLoopCommand => FastLoopCommand,
            clk => clk_10kHz
        );
    
    system_loop: component SystemLoop
        port map (
            MainLoopStatus => MainLoopStatus,
            FaultAction => FaultAction,
            clk => clk_1kHz
        );

    clock_gen: component ClockGen
        port map (
            clk_1kHz => clk_1kHz,
            clk_10kHz => clk_10kHz,
            clk_40kHz => clk_40kHz
        );

    main1: component Main
        port map (
            ASCIICommand => ASCIICommand,
            ASCIIResponse => ASCIIResponse
        );
    
    MotorStatus <= MainLoopStatus;
end architecture rtl;

library ieee;
use ieee.std_logic_1164.all;
entity FastLoop is
    port (
        Command: in std_logic;
        CurrentSensor: in std_logic;
        MotorPosition: in std_logic;
        FaultAction: in std_logic;
        Status: out std_logic;
        PWM: out std_logic;
        clk: in std_logic
    );
end entity FastLoop;

architecture rtl of FastLoop is
begin
end architecture rtl;

library ieee;
use ieee.std_logic_1164.all;
entity MainLoop is
    port (
        Command: in std_logic;
        OutputPosition: in std_logic;
        TorqueSensor: in std_logic;
        Status: out std_logic;
        FastLoopStatus: in std_logic;
        FastLoopCommand: out std_logic;
        clk: in std_logic
    );
end entity MainLoop;

architecture rtl of MainLoop is
begin
end architecture rtl;

library ieee;
use ieee.std_logic_1164.all;
entity SystemLoop is
    port (
        MainLoopStatus: in std_logic;
        FaultAction: out std_logic;
        clk: in std_logic
    );
end entity SystemLoop;

architecture rtl of SystemLoop is
begin
end architecture rtl;

library ieee;
use ieee.std_logic_1164.all;
entity ClockGen is
    port (
        clk_1kHz: out std_logic;
        clk_10kHz: out std_logic;
        clk_40kHz: out std_logic
    );
end entity ClockGen;

architecture rtl of ClockGen is
begin
end architecture rtl;

library ieee;
use ieee.std_logic_1164.all;
entity Main is
    port (
        ASCIICommand: in std_logic;
        ASCIIResponse: out std_logic
    );
end entity Main;

architecture rtl of Main is
begin
end architecture rtl;
