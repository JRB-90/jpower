# set(NRF5_LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/gcc_${NRF_FAMILY})

# # DFU requirements
# # List the softdevice versions previously used, or use FALSE if no previous softdevices
# set(PREVIOUS_SOFTDEVICES FALSE)
# # Set the location to the DFU private key
# set(PRIVATE_KEY ${CMAKE_CURRENT_SOURCE_DIR}/../../keys/dfu_private.key)
# set(PUBLIC_KEY ${CMAKE_CURRENT_SOURCE_DIR}/../../keys/dfu_public_key.c)
# # Set the App validation type. [NO_VALIDATION|VALIDATE_GENERATED_CRC|VALIDATE_GENERATED_SHA256|VALIDATE_ECDSA_P256_SHA256]
# set(APP_VALIDATION_TYPE NO_VALIDATION)
# # Set the Soft Device validation type. [NO_VALIDATION|VALIDATE_GENERATED_CRC|VALIDATE_GENERATED_SHA256|VALIDATE_ECDSA_P256_SHA256]
# set(SD_VALIDATION_TYPE NO_VALIDATION)
# # The bootloader version (user defined)
# set(BOOTLOADER_VERSION 1)
# # The DFU version string (firmware version string)
# set(DFU_VERSION_STRING "${VERSION_STRING}")
# # IC Type being used (NRF51 | NRF52)
# set(IC_TYPE NRF52)
# # The COM port that the NRF BLE dongle is attached to
# set(COM_PORT COM6)
# # The BLE advertising device name
# set(DEVICE_NAME "JPower")

# # Set the target name
# set(target blinky)

# # add the required libraries for this example
# nRF5_addLog()
# nRF5_addSeggerRTT()
# nRF5_addAppError()
# #nRF5_addHardfaultHandler()
# #nRF5_addPowerMgmt()
# nRF5_addBSP(FALSE FALSE FALSE)
# nRF5_addAppButton()
# #nRF5_addAppTimer()
# #nRF5_addAppUART()
# nRF5_addLegacyGPIOTE()

# # include files
# list(APPEND SOURCE_FILES
#     main.c
# )
# list(APPEND INCLUDE_DIRS
#     "${CMAKE_CURRENT_SOURCE_DIR}"
# )

# nRF5_addExecutable(${target} "${SOURCE_FILES}" "${INCLUDE_DIRS}" "${NRF5_LINKER_SCRIPT}")

# # make sdk_config.h import app_config.h
# target_compile_definitions(${target} PRIVATE USE_APP_CONFIG)

# target_link_libraries(${target} PRIVATE m)

# # Here you can set a list of user variables to be defined in the bootloader makefile (which you have modified yourself)
# set(bootloader_vars "")

# # add the secure bootloader build target
# nRF5_addSecureBootloader(${target} "${PUBLIC_KEY}" "${bootloader_vars}")
# # add the bootloader merge target
# nRF5_addBootloaderMergeTarget(${target} ${DFU_VERSION_STRING} ${PRIVATE_KEY} ${PREVIOUS_SOFTDEVICES} ${APP_VALIDATION_TYPE} ${SD_VALIDATION_TYPE} ${BOOTLOADER_VERSION})
# # add the bootloader merged flash target
# nRF5_addFlashTarget(bl_merge_${target} "${CMAKE_CURRENT_BINARY_DIR}/${target}_bl_merged.hex")
# # Add the Bootloader + SoftDevice + App package target
# #nRF5_addDFU_BL_SD_APP_PkgTarget(${target} ${DFU_VERSION_STRING} ${PRIVATE_KEY} ${PREVIOUS_SOFTDEVICES} ${APP_VALIDATION_TYPE} ${SD_VALIDATION_TYPE} ${BOOTLOADER_VERSION})
# # Add the App package target
# #nRF5_addDFU_APP_PkgTarget(${target} ${DFU_VERSION_STRING} ${PRIVATE_KEY} ${PREVIOUS_SOFTDEVICES} ${APP_VALIDATION_TYPE})
# # Add BLE DFU target
# #nRF5_addBLE_DFU_BL_SD_APP_PkgTarget(${target} ${DFU_VERSION_STRING} ${PRIVATE_KEY} ${PREVIOUS_SOFTDEVICES} ${APP_VALIDATION_TYPE} ${IC_TYPE} ${COM_PORT} ${DEVICE_NAME})

# # print the size of consumed RAM and flash - does not yet work on Windows
# if(NOT ${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
#     nRF5_print_size(${target} ${NRF5_LINKER_SCRIPT} TRUE)
# endif()