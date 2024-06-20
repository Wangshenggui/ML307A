
CUSTOM_MAIN_DIR := custom/custom_main


OC_FILES += $(CUSTOM_MAIN_DIR)/src/custom_main.c
OC_FILES += $(CUSTOM_MAIN_DIR)/src/uart.c
OC_FILES += $(CUSTOM_MAIN_DIR)/src/cors.c
OC_FILES += $(CUSTOM_MAIN_DIR)/src/cors_socket.c
OC_FILES += $(CUSTOM_MAIN_DIR)/src/nema.c
OC_FILES += $(CUSTOM_MAIN_DIR)/src/mcu_uart.c
OC_FILES += $(CUSTOM_MAIN_DIR)/src/baidumap.c
OC_FILES += $(CUSTOM_MAIN_DIR)/src/coor_trans.c
INC      += -I'$(CUSTOM_MAIN_DIR)/inc'
