I generated the STM32G474.h file from version 2.2 (says 2.2 on the website, 2.1 in the file) of the .svd file downloaded from ST's website on the stm32g474 page under CAD resources. I obtained SVDConv from the arm CMSIS_5 github by downloading and running [fetch_devtools.sh](https://github.com/ARM-software/CMSIS_5/raw/refs/heads/develop/CMSIS/Utilities/fetch_devtools.sh) then running:
```
./Linux64/SVDConv STM32G4_svd/STM32G4_svd_V2.2/STM32G474.svd --fields=struct
```
