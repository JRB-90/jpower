set(name "s340_6.1.1")
set(dir ${SDK_ROOT}/components/softdevice)
set(${name}_INCLUDE_DIRS
    "${dir}/s340/headers/"
    "${dir}/s340/headers/nrf52/")
set(${name}_HEX_FILE
    "${dir}/s340/hex/s340_nrf52_6.1.1_softdevice.hex")
set(${name}_VERSION "6.1.1")
set(${name}_MAJOR_VERSION "6")
set(${name}_DEFINES
    -DS340
    -DSOFTDEVICE_PRESENT
    -DNRF_SD_BLE_API_VERSION=${${name}_MAJOR_VERSION})
