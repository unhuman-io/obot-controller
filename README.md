# Obot-controller
The obot-controller project is motor driver software for an embedded microcontroller. It has two components: project specific software and a motorlib folder. The project specific folders were generated with STMCubeMX software with generic makefile output but effort has been made to remove most of the ST code on the Obot project. Projects can have multiple configurations to support different hardware and sensors. The configuration is selected at compile time. A configuration of a project can have muliple parameter files to support the same hardware but with different sensor calibrations and control parameters. Parameters are stored in a struct in a specific location in flash memory and can be loaded independently from the main firmware.

# Installation

## Repo

This repository uses [git submodules](https://git-scm.com/book/en/v2/Git-Tools-Submodules). To checkout all the necessary code begin with the following command:
```console
> git clone --recurse-submodules https://github.com/unhuman-io/obot-controller.git
```
To keep the submodules tidy it is encouraged to setup your new git repo with the following options:
```bash
git config --global submodule.recurse true
git config --global pull.ff only
```
Finally, checkout the latest branch and make sure it is up-to-date:
```console
> git checkout --recurse-submodules develop
```

## Build
The firmware is designed around a **gcc** toolchain for ARM and the compiler (`arm-none-eabi`) can be manually downloaded from the [ARM website](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads) or automatically with a script.

Prepare the environment by installing the following modules:
```console
> sudo apt-get install curl wget xz-utils dfu-util make
```
Then run the install script from within the root `obot-controller/` directory, which will install **gcc** into `obot-controller/motorlib/gcc/`:
```console
> ./motorlib/scripts/install_gcc.sh
```

## Debugging
The firmware is designed to communicate via USB, which allows interactive debugging and tuning via a command-line utility `motor_util`. Installation of the utility can be achieved by:

```bash
sudo apt install -y linux-headers-$(uname -r)
curl https://raw.githubusercontent.com/unhuman-io/obot/main/install-obot.sh > install-obot.sh
chmod +x install-obot.sh
./install-obot.sh
```

This will install a realtime USB kernel module for communicating with the motor controller and install a set of utilities for working with the device. The utilities can be upgraded with `update-obot`.

# Development
## Repo
This repository uses [git submodules](https://git-scm.com/book/en/v2/Git-Tools-Submodules). It is useful to periodically sync the various repos after the initial `clone` with:
```console
> git submodule update --init --recursive
```
Similarly, when doing `pull` or `checkout` it can be handy to operate on all submodules of the repo:
```console
> git pull --recurse-submodules
> git checkout --recurse-submodules
```

## Build and Deploy
Currently the code is written to support STM32 micros and therefore the build-chain utilizes `gcc`, since it is supported by STMCubeMX. On successful installation of the compiler it should appear on your path and will have a prefix of `arm-none-eabi`, so for example `arm-none-eabi-gcc`.

Compilation steps are managed with `make` and the rules can be executed from the project folder. As an example, the firmware for a hall-based motor instance can be compiled as follows:
```console
obot-controller/obot_g474> make -j C_DEFS=-DR4 CONFIG=motor_hall
```

For this example the compiled artifacts will be placed into the `obot-controller/obot_g474/build/motor_hall` directory. Inside the directory will be the firmware, parameter blob, and two shell helper scripts for deploying the firmware.

Deploying the firmware to the device is then as simple as executing the appropriate shell script:

```console
obot-controller/obot_g474> ./build/motor_hall/load_motor_hall.sh
```

## Flashing firmware artifacts

In general, firmware can usually be deployed using the shell scripts from the build step. However, it can be useful to manually load or upgrade through USB using the Device Firmware Upgrade (DFU) mode. DFU mode is built into the ST bootloader. Interacting with a board in DFU mode can be accomplished using the `dfu-util` software package.

 If the board is initially unprogrammed or has corrupted firmware, the ST bootloader can be accessed through setting the appropriate hardware pins, e.g. obot_g474 press the reset button on power-up. A board that is already programmed can be switched into DFU mode at runtime by sending a DFU_DETACH packet over USB.

 An example of manually flashing the firmware (the first step of the deploy script above):
```console
obot-controller/obot_g474> dfu-util -a0 -s 0x8000000 -D ./build/motor_hall/motor_hall_noparam.bin
```

Various motor control parameters that may be specific to the application are placed in a separate flash block that can be reprogrammed independently of the main firmware. The format is the struct [Param](/obot_g474/param/param_obot_g474.h). The built parameters can be flashed as follows:
```console
obot-controller/obot_g474> dfu-util -a0 -s 0x8060000:leave -D ./build/motor_hall/motor_hall_param_only.bin
```

## Prebuilt firmware
Some examples of prebuilt firmware are included in the github [releases](https://github.com/unhuman-io/obot-controller/releases/develop). For example one can download a `.tgz` file, extract the build artifacts, and load the firmware:
```bash
tar xf motor_enc.tgz
./motor_enc/load_motor_enc.sh
```

## Tuning and debugging
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

### Text api
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

# Host communication
The assumed use case for this software is for a central control host to connect to multiple motor drivers. The currently recommended communication system is USB to a linux host and there are custom drivers, utilities, and example code for this in the repositories usb_rt_driver and realtime-tmp. The host pc software is centrally linked in the adjacent [obot](https://github.com/unhuman-io/obot) repository.

# Software downloads
Software downloads as of 11/2020:
- [STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html)
- [gcc-arm](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads)
  - Unzip and place on path. E.g. `sudo chown $USER:$USER /opt; tar -C /opt xf gcc-arm-none-eabi-9-2019-q4-major-x86_64-linux.tar.bz2; echo "export PATH = $PATH:/opt/gcc-arm-none-eabi-9-2019-q4-major-x86_64-linux >> ~/.bashrc`
- [Visual Studio Code](https://code.visualstudio.com/download)
  - Get the Cortex Debug extension in Visual Studio Code, View menu&rarr;Extensions
- [SEGGER J-Link Software](https://www.segger.com/downloads/jlink/#J-LinkSoftwareAndDocumentationPack)
- [Host PC software](https://raw.githubusercontent.com/unhuman-io/obot/master/install-obot.sh)

# Hardware

The nominal motor-controller design is also open source and available [here](https://a360.co/3vo8SC4) and corresponds to the **obot_g474** variant. Further description of the board and some useful pin-out information is found in this [document](https://docs.google.com/document/d/1p9xV1iErwQMZhck7lHwQfRsM6e2xIIJ8QxJ62v68LWk/edit).
