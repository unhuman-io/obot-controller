BUILD_PARAM_DIR = ../$(BUILD_DIR)/param

all: $(patsubst %.h, $(BUILD_PARAM_DIR)/%.bin, $(wildcard *.h)) $(BUILD_PARAM_DIR)

$(BUILD_PARAM_DIR)/%.bin: %.h ../param_freebot_g474_col.c
	$(CC) --include $< -c ../param_freebot_g474_col.c -o $(BUILD_PARAM_DIR)/$(<:h=o)
	$(CP) -O binary -S -j flash_param $(BUILD_PARAM_DIR)/$(<:h=o) $@

$(BUILD_PARAM_DIR): 
	mkdir -p $@

.PHONY: $(BUILD_PARAM_DIR)
