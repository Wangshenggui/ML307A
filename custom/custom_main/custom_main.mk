
CUSTOM_MAIN_DIR := custom/custom_main


OC_FILES += $(CUSTOM_MAIN_DIR)/src/custom_main.c
OC_FILES += $(CUSTOM_MAIN_DIR)/src/uart.c
OC_FILES += $(CUSTOM_MAIN_DIR)/src/tcp.c
OC_FILES += $(CUSTOM_MAIN_DIR)/src/gps.c
INC      += -I'$(CUSTOM_MAIN_DIR)/inc'