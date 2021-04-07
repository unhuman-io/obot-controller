BUILD_PARAM_DIR = ../$(BUILD_DIR)/param

all: $(patsubst %.h, $(BUILD_PARAM_DIR)/%.bin, $(wildcard *.h)) | $(BUILD_PARAM_DIR)

$(BUILD_PARAM_DIR)/%.bin: %.h ../param/param_freebot_g474_motor_enc.c | $(BUILD_PARAM_DIR)
	$(CC) --include $< -c ../param/param_freebot_g474_motor_enc.c -o $(BUILD_PARAM_DIR)/$(<:h=o)
	$(CP) -O binary -S -j flash_param $(BUILD_PARAM_DIR)/$(<:h=o) $@

$(BUILD_PARAM_DIR): 
	mkdir $@