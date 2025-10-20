## Build

```
make
# or
make USE_LLVM=1
```

## Rebuild

```
make -C build
# or
make -C build_llvm
```

## stm32 header generation
I generated the STM32G474.h file from version 2.2 (says 2.2 on the website, 2.1 in the file) of the .svd file downloaded from ST's website on the stm32g474 page under CAD resources. I obtained SVDConv from the arm CMSIS_5 github by downloading and running [fetch_devtools.sh](https://github.com/ARM-software/CMSIS_5/raw/refs/heads/develop/CMSIS/Utilities/fetch_devtools.sh) or maybe better [SVDConv release](https://github.com/Open-CMSIS-Pack/devtools/releases/download/tools%2Fsvdconv%2F3.3.48/svdconv-3.3.48-linux-amd64.tbz2) then running:
```
./Linux64/SVDConv STM32G4_svd/STM32G4_svd_V2.2/STM32G474.svd --fields=struct
# or
./svdconv STM32G474.svd --fields=struct --generate=header
```
