# Obot-controller
The obot-controller project is motor driver software for an embedded microcontroller. It has two components: project specific software and a motorlib folder. The project specific folders were generated with STMCubeMX software with generic makefile output but effort has been made to remove most of the ST code on the Obot project. Projects can have multiple configurations to support different hardware and sensors. The configuration is selected at compile time. A configuration of a project can have muliple parameter files to support the same hardware but with different sensor calibrations and control parameters. Parameters are stored in a struct in a specific location in flash memory and can be loaded independently from the main firmware.

## Submodules
This repository uses git submodules. Ensure to keep them in sync after clone with
```console
> git submodule update --init
```
and when pulling or checking out
```console
> git pull --recurse-submodules
> git checkout --recurse-submodules
```
or for automatic support plus recommended pull ff-only global option
```console
git config --global submodule.recurse true
git config --global pull.ff only
```

## Build
The current build system is `gcc` due to it being supported by STMCubeMX. The specific setup I've been using that works is the build of the `gcc` compiler available on the ARM website. This complier will have a prefix of `arm-none-eabi`, so for example `arm-none-eabi-gcc` should be on your path. Run make in a project folder to build. Then programming can be accomplished either through the built in bootloader or through a debugger connection, both described below.

## Motorlib
Motorlib the core of the firmware. It is described the motorlib readme file.

### Communication
The assumed usage case for this software is multiple motor drivers that are connected to a central control host. The currently recommended communication system is USB to a linux host and there are custom drivers, utilities, and example code for this in the repositories usb_rt_driver and realtime-tmp. The host pc software is centrally linked in the adjacent `obot` repository.

### Programming
The firmware can be upgraded through USB in Device Firmware Upgrade (DFU) mode. DFU mode is built into the ST bootloader. For an unprogrammed or board with corrupted firmware the ST bootloader can be accessed with a combination of external pin settings. For an already programmed board you can switch to DFU mode at runtime by sending a DFU_DETACH packet over USB. This and DFU programming are available through the dfu-util software package. The output of the make process described in build is a bin file. This can be programmed with the command:
```console
> dfu-util -a0 -s 0x8000000 -D build/obot_g474.bin
```
or use the generated script
```console
> ./build/motor_enc/load_motor_enc.sh
```

### Parameters
Various motor control parameters that may be specific to the application are placed in a separate flash block that can be reprogrammed independently of the main firmware. The format is the struct Param.

### Tuning and debugging
With the gcc build system described above I also use Visual Studio Code and the Cortex Debug extension. In addition I prefer the SEGGER JLink debugger as it has a low cost edu mini version that is full featured but with a non commercial license. There is an install file for SEGGER JLink that must be installed as well as having the debugger. The debugger connection allows for fast programming and source debugging through Visual Studio Code. In addition I have also found it very handy to use `arm-none-eabi-gdb` directly, which essentially provides a command line interface to the internals of the software while running with no implementation effort or runtime overhead. To use the command line gdb version separate from Visual Studio Code and Cortex Debug you can run the SEGGER GDBServer executable separately, then run gdb with an example below.
```console
> arm-none-eabi-gdb build/obot_g474.elf
(gdb) monitor remote :2331
(gdb) load
(gdb) monitor reset
(gdb) monitor go
(gdb) print config_items.main_loop.controller_.kp_
(gdb) set config_items.main_loop.controller_.kp_ = 2
```
One caveat to the debugging through gdb is that it is not available when running with link time optimization (LTO) turned on. LTO should be disabled through the Makefile, in order to do the above debugging. However LTO turned off will adversely effect control loop timing.

#### Text api
Support for a separate ascii based communication channel is also provided. A small subset of parameters is provided in motorlib/system.h that may be read and tuned at runtime. For example using motor_util from the `realtime-tmp` repository.
```console
> motor_util --api --no-list
kp
100.0
kp=99
kp set 99
kp
99
```

## Software downloads
Software downloads as of 11/2020:
- [STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html)
- [gcc-arm](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads)
  - Unzip and place on path. E.g. `sudo chown $USER:$USER /opt; tar -C /opt xf gcc-arm-none-eabi-9-2019-q4-major-x86_64-linux.tar.bz2; echo "export PATH = $PATH:/opt/gcc-arm-none-eabi-9-2019-q4-major-x86_64-linux >> ~/.bashrc`
- [Visual Studio Code](https://code.visualstudio.com/download)
  - Get the Cortex Debug extension in Visual Studio Code, View menu&rarr;Extensions
- [SEGGER J-Link Software](https://www.segger.com/downloads/jlink/#J-LinkSoftwareAndDocumentationPack)
- [Host PC software](https://raw.githubusercontent.com/unhuman-io/obot/master/install-obot.sh)
