# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# compile CXX with /home/javierfr/microros_ws/uros_ws/firmware/toolchain/espressif/tools/xtensa-esp32-elf/esp-2020r2-8.2.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-g++
CXX_FLAGS = -mlongcalls -Wno-frame-address -mlongcalls -Wno-frame-address -g   -Wall -std=gnu++14

CXX_DEFINES = -DLWIP_IPV4 -DLWIP_IPV6 -DROS_PACKAGE_NAME=\"geometry_msgs\"

CXX_INCLUDES = -I/home/javierfr/microros_ws/uros_ws/firmware/freertos_apps/microros_esp32_extensions/build/config -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/newlib/platform_include -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/protocomm/include -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/esp_rom/include -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/driver/include -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/wear_levelling/include -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/vfs/include -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/log/include -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/freertos/include -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/wifi_provisioning/include -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/pthread/include -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/libsodium/libsodium/src/libsodium/include -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/coap/port/include -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/coap/libcoap/include -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/tcp_transport/include -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/unity/include -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/esp32/include -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/asio/port/include -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/asio/asio/asio/include -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/app_update/include -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/esp_ringbuf/include -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/soc/esp32/include -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/soc/include -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/efuse/test/include -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/efuse/esp32/include -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/efuse/include -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/mbedtls/port/include -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/mbedtls/mbedtls/include -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/xtensa/esp32/include -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/xtensa/include -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/app_trace/include -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/nvs_flash/include -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/jsmn/include -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/esp_eth/include -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/spiffs/include -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/ulp/include -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/mdns/include -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/freemodbus/modbus/include -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/freemodbus/common/include -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/esp_event/include -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/expat/port/include -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/bootloader_support/include -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/heap/include -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/wpa_supplicant/port/include -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/wpa_supplicant/include -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/tcpip_adapter/include -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/openssl/include -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/sdmmc/include -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/esp_local_ctrl/include -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/esp_common/include -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/lwip/lwip/src/include/lwip/apps -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/lwip/lwip/src/include/compat/posix -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/lwip/lwip/src/include -I/home/javierfr/microros_ws/uros_ws/firmware/toolchain/esp-idf/components/lwip/port/esp32/include -I/home/javierfr/microros_ws/uros_ws/firmware/mcu_ws/build/geometry_msgs/rosidl_generator_c -I/home/javierfr/microros_ws/uros_ws/firmware/mcu_ws/build/geometry_msgs/rosidl_typesupport_c -I/home/javierfr/microros_ws/uros_ws/firmware/mcu_ws/build/geometry_msgs/rosidl_typesupport_microxrcedds_c -isystem /home/javierfr/microros_ws/uros_ws/firmware/mcu_ws/install/include -isystem /home/javierfr/microros_ws/uros_ws/firmware/mcu_ws/install/share/tinydir_vendor/cmake/../../../include 

