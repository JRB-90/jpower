set(target jpower)

set(NRF_PROJECT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(NRF5_LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/gcc_nrf52)
set(PRIVATE_KEY ${CMAKE_CURRENT_SOURCE_DIR}/../../keys/dfu_private.key)
set(PUBLIC_KEY ${CMAKE_CURRENT_SOURCE_DIR}/../../keys/dfu_public_key.c)
set(PREVIOUS_SOFTDEVICES FALSE)
set(APP_VALIDATION_TYPE NO_VALIDATION)
set(SD_VALIDATION_TYPE NO_VALIDATION)
set(BOOTLOADER_VERSION 1)
set(DFU_VERSION_STRING "${VERSION_STRING}")

nRF5_addLog()
nRF5_addSeggerRTT()
nRF5_addAppError()
nRF5_addAppButton()
nRF5_addBSP(TRUE TRUE FALSE)

jpow_addJPowMath()
jpow_addI2C()
jpow_addSPI()
jpow_addIMU()
jpow_addFusion()
jpow_addStrain()
jpow_addStorage()
jpow_addCalibrate()
jpow_addUSB()
jpow_addBLE()
jpow_addANTPlus()

list(APPEND SOURCE_FILES
    ${NRF_PROJECT_PATH}/main.c
)

list(APPEND INCLUDE_DIRS
    ${NRF_PROJECT_PATH}
)

nRF5_addExecutable(${target}
    "${SOURCE_FILES}"
    "${INCLUDE_DIRS}"
    "${NRF5_LINKER_SCRIPT}"
    ".rtt"
)

target_compile_definitions(${target} PRIVATE 
    USE_APP_CONFIG
    DEBUG
)

target_link_libraries(${target} PRIVATE
    m
)

set(bootloader_vars "")
nRF5_addSecureBootloader(${target} "${PUBLIC_KEY}" "${bootloader_vars}")
nRF5_addBootloaderSoftDeviceAppMergeTarget(${target} ${DFU_VERSION_STRING} ${PRIVATE_KEY} ${PREVIOUS_SOFTDEVICES} ${APP_VALIDATION_TYPE} ${SD_VALIDATION_TYPE} ${BOOTLOADER_VERSION})
nRF5_addFullFlashTarget(${target}_bl_sd_app_merge "${CMAKE_CURRENT_BINARY_DIR}/${target}_bl_sd_app.hex")
nRF5_addDFU_BL_SD_APP_PkgTarget(${target} ${DFU_VERSION_STRING} ${PRIVATE_KEY} ${PREVIOUS_SOFTDEVICES} ${APP_VALIDATION_TYPE} ${SD_VALIDATION_TYPE} ${BOOTLOADER_VERSION})
nRF5_addDFU_APP_PkgTarget(${target} ${DFU_VERSION_STRING} ${PRIVATE_KEY} ${PREVIOUS_SOFTDEVICES} ${APP_VALIDATION_TYPE})
