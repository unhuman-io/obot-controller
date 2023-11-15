######################################
# target
######################################


CONFIG=pz
CONFIG_FILE = config_g474_boost_$(CONFIG).cpp
PARAM ?= $(CONFIG)
PARAM_FILE = param_g474_boost_$(PARAM).c
$(shell touch $(PARAM_FILE)) 

TARGET = $(CONFIG)$(if $(PARAM_OVERRIDE),_$(PARAM_OVERRIDE))
PARAM_OVERRIDE_FILE = $(PARAM_FILE:.c=_$(PARAM_OVERRIDE).h)

C_INCLUDES = \
-IDrivers/STM32G4xx_HAL_Driver/Inc \
-IDrivers/STM32G4xx_HAL_Driver/Inc/Legacy

######################################
# source
######################################
# C sources
C_SOURCES =  \
Src/stm32g4xx_hal_msp.c \
Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_gpio.c \
Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pcd.c \
Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pcd_ex.c \
Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_usb.c \
Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_hrtim.c \
Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_uart.c \
Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_uart_ex.c \
Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_tim.c \
Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_tim_ex.c \
Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_pwr.c \
Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal.c \
Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc.c \
Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc_ex.c \
Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash.c \
Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash_ex.c \
Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash_ramfunc.c \
Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_exti.c \
Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dma.c \
Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dma_ex.c \
Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr.c \
Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr_ex.c \
Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_cortex.c \
Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_adc.c \
Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_adc_ex.c \
Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_adc.c \
Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_spi.c \
Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_spi_ex.c \
Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_opamp.c \
Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_opamp_ex.c


TARGET_MCU := stm32g474

# for submake
export

include ../motorlib/make/main.mk
