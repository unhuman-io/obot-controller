#!/bin/sh
# Fetches what the radio firmware (power_radio, power_remote) is built on: ST's STM32CubeWBA
# package at the version this was written against, with three of its submodules (the device
# header, the HAL, the radio middleware). About 750 MB.
#   fetch_stm32cubewba.sh [directory]      default ~/src/STM32CubeWBA
# Then:  cmake --preset gcc -DSTM32CUBE_WBA=<directory>      (or STM32CUBE_WBA in the environment)
# ST's licence for it (SLA0044) is in the package; it allows use on ST's processors.
set -eu
version=v1.11.0
dir="${1:-$HOME/src/STM32CubeWBA}"
if [ ! -d "$dir/.git" ]; then
    git clone -q --depth 1 --branch "$version" https://github.com/STMicroelectronics/STM32CubeWBA.git "$dir"
fi
cd "$dir"
[ "$(git describe --tags)" = "$version" ] || { echo "$dir is not STM32CubeWBA $version" >&2; exit 1; }
git submodule update --init --depth 1 -q Drivers/CMSIS/Device/ST/STM32WBAxx Drivers/STM32WBAxx_HAL_Driver Middlewares/ST/STM32_WPAN
echo "$dir"
