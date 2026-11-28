# Power board and e-stop remote

Firmware for the two boards of `obot-cad/electronics`: the power / controller board on the robot's
deck, and the wireless e-stop remote. Three processors, three programs:

| Program | Processor | Where | Does |
|---|---|---|---|
| `power_main` | STM32G564RET6 | power board | measures battery, motor bus and the computer's supplies; switches the 12 V output and its half of the bus eFuse's enable; CAN |
| `power_radio` | STM32WBA5MMG module | power board | always powered: power button, remote, dock, display, lights; starts the converters (and so `power_main`); its own line to hold the bus off |
| `power_remote` | STM32WBA5MMG module | remote | red button (two contacts), pause button, display, lights, one lithium cell |

**None of it has run on hardware**: the boards are not built yet. What decides anything (when the
bus is on, what a button means, what goes over the air and whether it is believed, the pairing,
the screens) is in modules without hardware in them and is tested on a PC. The rest is compiled,
by gcc and by clang, and has never met a board. `Open points` below lists what was assumed.

## How it behaves

- **Off.** Only the radio module is powered (from the battery, through a micropower regulator),
  asleep in Stop mode. Twice a second it wakes for a moment to keep the display alive; once a
  second it says its status on the air and listens for 30 ms. The power button wakes it at once.
- **On**: the power button (short press), or PAUSE held on the remote. The radio module starts
  the two converters, which powers the main controller; the computer's 12 V output follows.
- **The motor bus** is on only while both processors agree. The radio module lets go of the
  eFuse's enable only while an authenticated message from the paired remote, saying its red
  button is out, is less than 300 ms old; the main controller drives the same line high only
  while the radio module, heard in the last 300 ms, allows it and the battery is in range. So a
  remote that is out of range, flat or broken, a wire off the red button, either processor
  hanging (the main controller has a watchdog; a processor in reset drives nothing): bus off.
- **Pause.** After anything that took the bus away the robot is *paused*: letting the red button
  out powers the motors, it does not make them move. PAUSE on the remote toggles it. Pause is a
  bit in the status frame on the CAN bus; the bus stays powered.
- **Off again**: the power button held, or PAUSE held on the remote. The bus goes at once, the
  computer is told (status frame) and keeps its supply for 20 s.
- **The remote** sleeps whenever no robot that is on hears it and nothing was pressed for 30 s.
  A button or the dock wakes it.
- **Pairing is putting the remote on the dock.** The two exchange a key over the dock's data
  contact (`common/dock.cppm`). The pairing button on the board, held 5 s, forgets the remote.
- **Both displays** show the same screen (`common/screen.cppm`): what the robot is doing, its
  battery, the bus, the computer, the remote's cell. The lights: green run, yellow pause, red
  stop, red blinking fault, blue starting / shutting down / looking for the robot.

A wireless stop is a convenience, not a safety-rated function.

## Over the air

No Bluetooth connection, no pairing in the Bluetooth sense. Each node advertises one small frame
again and again (a non-connectable advertisement with the frame as manufacturer data) and
listens only for the other's address (the link layer's accept list):

- remote: a heartbeat every 20 ms: a counter, the red button, pause presses, its cell;
- board: its status every 100 ms (every second while off): a session number, the robot's state.

Every frame ends in a CMAC (AES-128) with the key from the dock. The session number is a random
number of the board's, renewed when the remote has been silent for a second; the remote has to
use it and to count upwards within it. A frame recorded earlier is therefore worth nothing
(`common/messages.cppm`).

Listening is what costs: the board listens all the time while the robot is on and 3 % of the
time while it is off; the remote all the time until it hears the robot and then a fifth of it.

## Layout

    common/     no hardware: frame (display bytes), font, screen, aes (AES-128, CMAC), messages,
                logic (the three state machines, buttons, lights), dock (pairing), ina228
    test/       run.sh: builds common/ for the host and runs test_power.cpp (g++ 14 or newer)
    main/       power_main: bare metal, in the style of the other obot3 firmware (constexpr init
                structs, device modules of motorlib, coroutine tasks); a UART and an I2C module
    wba/        port.h and port.c: pins, display SPI, ADC, UARTs, radio, flash record for the two
                radio programs; wba.cmake; overlay/: three of ST's configuration headers, extended
    radio/      power_radio's application
    remote/     power_remote's application

## Building

`power_main` is built with the rest of obot3 (`cmake --preset gcc` or `llvm`, target
`power_main.elf`).

The two radio programs need ST's STM32CubeWBA package, which is not in this repository:

    obot3/power/fetch_stm32cubewba.sh                   # v1.11.0, 750 MB, to ~/src/STM32CubeWBA
    cmake --preset gcc -DSTM32CUBE_WBA=$HOME/src/STM32CubeWBA
    cmake --build --preset gcc -t power_radio.elf -t power_remote.elf

They are built on ST's example `BLE_BeaconHCI`: its start-up (clocks, RTC, timer server, low
power manager) and ST's Bluetooth LE link layer, used through HCI commands only. ST's files are
compiled from the package unchanged and none is copied here; ST's licence for them (SLA0044)
allows their use on ST's processors. See `wba/wba.cmake` for exactly what is taken from where.

    obot3/power/test/run.sh                             # the tests; a second argument: a directory for pictures of the screens

## On the CAN bus

`power_main` is an obot node at address 0x70: it answers the enum request (`obot3_power`) and
sends a status frame ten times a second (`make_can_status` in `common/messages.cppm`: state,
faults, pause, voltages, currents, energy).

## Open points

Assumed, to be checked on the first board:

- **Radio module pins**: alternate functions are those ST's own examples use on the same pins
  (SPI3 on PA0 / PB8: AF6, LPUART1 on PB5 / PA10: AF8, USART1 on PB12: AF7, PA1 = ADC channel 8).
  The module's datasheet has no alternate function table and the STM32WBA55's was not at hand.
- **ST's example is for the Nucleo board**; the module's start-up file, linker script, device
  name and power table are those of ST's project for its module board. The radio (crystal
  trimming from the module's OTP, power table 0) has not been on the air.
- **Stop mode with our peripherals**: LPUART1, USART1 and SPI3 are only used with Stop mode
  switched off; the two UARTs run from the 16 MHz internal oscillator, assumed to stay on then.
- **Sleep current** has not been measured, nor the 30 ms a second of listening against the
  0.1 mA the board's standby supply was planned for.
- **INA228**: register meanings from the datasheet; ranges 163.84 mV (battery, 3 mOhm) and
  40.96 mV (outputs).
- **The pack**: limits are for 15 cells (45.0 V low, 63.5 V high); the per cent is from voltage.
- **STM32G564**: the independent watchdog is assumed to start its own 32 kHz oscillator, as on
  other STM32. ADC clock 17.5 MHz (its limit was not looked up). No serial number in the enum.

Not written:

- the battery pack's data line (protocol unknown);
- reprogramming the main controller from the radio module (the wiring is there: its reset, boot
  pin and boot loader UART);
- obot3's text api and debug monitor in `power_main`; a CAN boot loader for it;
- charging the remote while the robot is off works by starting the converters (the main
  controller then runs too); nothing limits how long;
- the main controller's CAN status has no sender-side check that the computer has shut down
  before its supply goes.
