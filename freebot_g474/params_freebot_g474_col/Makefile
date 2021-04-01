BUILD_PARAM_DIR = ../$(BUILD_DIR)/param

all: $(patsubst %.h, $(BUILD_PARAM_DIR)/%.bin, $(wildcard *.h)) | $(BUILD_PARAM_DIR)

$(BUILD_PARAM_DIR)/%.bin: %.h ../param/param_freebot_g474_col_spi.c | $(BUILD_PARAM_DIR)
	$(CC) --include $< -c ../param/param_freebot_g474_col_spi.c -o $(BUILD_PARAM_DIR)/$(<:h=o)
	$(CP) -O binary -S -j flash_param $(BUILD_PARAM_DIR)/$(<:h=o) $@

$(BUILD_PARAM_DIR): 
	mkdir $@