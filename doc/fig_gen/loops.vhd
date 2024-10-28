library ieee;
use ieee.std_logic_1164.all;

entity Motor is
  port (
    MotorCommand: in std_logic;
    MotorStatus: out std_logic
  );
end entity Motor;



architecture rtl of Motor is
    component FastLoop is
        port (
            Command: in std_logic;
            FaultAction: in std_logic;
            Status: out std_logic
        );
    end component FastLoop;

    component MainLoop is
        port (
            Command: in std_logic;
            Status: out std_logic;
            FastLoopStatus: in std_logic;
            FastLoopCommand: out std_logic
        );
    end component MainLoop;

    component SystemLoop is
        port (
            MainLoopStatus: in std_logic;
            FaultAction: out std_logic
        );
    end component SystemLoop;

    signal FastLoopCommand: std_logic;
    signal FastLoopStatus: std_logic;
    signal MainLoopStatus: std_logic;
    signal FaultAction: std_logic;
begin
    fast_loop: component FastLoop
        port map (
            Command => FastLoopCommand,
            Status => FastLoopStatus,
            FaultAction => FaultAction
        );
    
    main_loop: component MainLoop
        port map (
            Command => MotorCommand,
            Status => MainLoopStatus,
            FastLoopStatus => FastLoopStatus,
            FastLoopCommand => FastLoopCommand
        );
    
    system_loop: component SystemLoop
        port map (
            MainLoopStatus => MainLoopStatus,
            FaultAction => FaultAction
        );
    
    MotorStatus <= MainLoopStatus;
end architecture rtl;

library ieee;
use ieee.std_logic_1164.all;
entity FastLoop is
    port (
        Command: in std_logic;
        FaultAction: in std_logic;
        Status: out std_logic
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
        Status: out std_logic;
        FastLoopStatus: in std_logic;
        FastLoopCommand: out std_logic
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
        FaultAction: out std_logic
    );
end entity SystemLoop;

architecture rtl of SystemLoop is
begin
end architecture rtl;