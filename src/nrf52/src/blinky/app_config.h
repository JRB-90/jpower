#define NRF_QUEUE_ENABLED 1

#define TWI_ENABLED 1
#define TWI0_ENABLED 1
#define NRFX_TWI_ENABLED 1
#define NRFX_TWIM_ENABLED 1
#define NRF_TWI_MNGR_ENABLED 1

#define BLE_LBS_ENABLED 1
#define BLE_NUS_ENABLED 1
#define BLE_DFU_ENABLED 1
#define NRF_SDH_BLE_SERVICE_CHANGED 1
#define NRF_SDH_BLE_VS_UUID_COUNT 2

#define TIMER_ENABLED 1
#define TIMER1_ENABLED 1

#define NRF_PWR_MGMT_CONFIG_AUTO_SHUTDOWN_RETRY 1

#ifndef NRF_SDH_ANT_ENABLED
#define NRF_SDH_ANT_ENABLED 1
#endif

#ifndef NRF_SDH_ANT_TOTAL_CHANNELS_ALLOCATED
#define NRF_SDH_ANT_TOTAL_CHANNELS_ALLOCATED 1
#endif

#ifndef NRF_SDH_ANT_ENCRYPTED_CHANNELS
#define NRF_SDH_ANT_ENCRYPTED_CHANNELS 0
#endif

#ifndef NRF_SDH_ANT_EVENT_QUEUE_SIZE
#define NRF_SDH_ANT_EVENT_QUEUE_SIZE 32
#endif

#ifndef NRF_SDH_ANT_BURST_QUEUE_SIZE
#define NRF_SDH_ANT_BURST_QUEUE_SIZE 128
#endif

#ifndef NRF_SDH_ANT_OBSERVER_PRIO_LEVELS
#define NRF_SDH_ANT_OBSERVER_PRIO_LEVELS 2
#endif

#ifndef ANTPLUS_NETWORK_NUM
#define ANTPLUS_NETWORK_NUM 0
#endif

#ifndef BPWR_CHANNEL_NUM
#define BPWR_CHANNEL_NUM 0
#endif

#ifndef CALIBRATION_DATA
#define CALIBRATION_DATA 21930
#endif

#ifndef CHAN_ID_DEV_NUM
#define CHAN_ID_DEV_NUM 1
#endif

#ifndef CHAN_ID_TRANS_TYPE
#define CHAN_ID_TRANS_TYPE 5
#endif

#ifndef BPWR_HW_REVISION
#define BPWR_HW_REVISION 127
#endif

#ifndef BPWR_MANUFACTURER_ID
#define BPWR_MANUFACTURER_ID 43690
#endif

#ifndef BPWR_MODEL_NUMBER
#define BPWR_MODEL_NUMBER 21845
#endif

#ifndef BPWR_SW_REVISION_MAJOR
#define BPWR_SW_REVISION_MAJOR 170
#endif

#ifndef BPWR_SW_REVISION_MINOR
#define BPWR_SW_REVISION_MINOR 255
#endif

#ifndef BPWR_SERIAL_NUMBER
#define BPWR_SERIAL_NUMBER 12345678
#endif

// <0=> Power Only Sensor 
// <1=> Wheel Torque Sensor 
// <2=> Crank Torque Sensor 
#ifndef SENSOR_TYPE
#define SENSOR_TYPE 0
#endif




#ifndef ANT_KEY_MANAGER_ENABLED
#define ANT_KEY_MANAGER_ENABLED 1
#endif

#ifndef ANT_CHANNEL_CONFIG_ENABLED
#define ANT_CHANNEL_CONFIG_ENABLED 1
#endif

#ifndef ANT_STATE_INDICATOR_ENABLED
#define ANT_STATE_INDICATOR_ENABLED 1
#endif

#ifndef ANT_STATE_INDICATOR_CONFIG_SHUTDOWN_HANDLER_PRIORITY
#define ANT_STATE_INDICATOR_CONFIG_SHUTDOWN_HANDLER_PRIORITY 1
#endif




#ifndef NRF_SDH_ANT_LOG_ENABLED
#define NRF_SDH_ANT_LOG_ENABLED 1
#endif

// <0=> Off 
// <1=> Error 
// <2=> Warning 
// <3=> Info 
// <4=> Debug 
#ifndef NRF_SDH_ANT_LOG_LEVEL
#define NRF_SDH_ANT_LOG_LEVEL 3
#endif

// <0=> Default 
// <1=> Black 
// <2=> Red 
// <3=> Green 
// <4=> Yellow 
// <5=> Blue 
// <6=> Magenta 
// <7=> Cyan 
// <8=> White 
#ifndef NRF_SDH_ANT_INFO_COLOR
#define NRF_SDH_ANT_INFO_COLOR 0
#endif
#ifndef NRF_SDH_ANT_DEBUG_COLOR
#define NRF_SDH_ANT_DEBUG_COLOR 0
#endif



#ifndef ANT_BPWR_ANT_OBSERVER_PRIO
#define ANT_BPWR_ANT_OBSERVER_PRIO 1
#endif

#ifndef ANT_BSC_ANT_OBSERVER_PRIO
#define ANT_BSC_ANT_OBSERVER_PRIO 1
#endif

#ifndef ANT_ENCRYPT_ANT_OBSERVER_PRIO
#define ANT_ENCRYPT_ANT_OBSERVER_PRIO 1
#endif

#ifndef ANT_HRM_ANT_OBSERVER_PRIO
#define ANT_HRM_ANT_OBSERVER_PRIO 1
#endif

#ifndef ANT_SDM_ANT_OBSERVER_PRIO
#define ANT_SDM_ANT_OBSERVER_PRIO 1
#endif

#ifndef ANT_STATE_INDICATOR_ANT_OBSERVER_PRIO
#define ANT_STATE_INDICATOR_ANT_OBSERVER_PRIO 1
#endif

#ifndef BSP_BTN_ANT_OBSERVER_PRIO
#define BSP_BTN_ANT_OBSERVER_PRIO 1
#endif

#ifndef NRF_DFU_ANT_EVT_HANDLER_PRIO
#define NRF_DFU_ANT_EVT_HANDLER_PRIO 1
#endif

#ifndef NRF_SDH_ANT_STACK_OBSERVER_PRIO
#define NRF_SDH_ANT_STACK_OBSERVER_PRIO 0
#endif




#ifndef ANT_BPWR_ENABLED
#define ANT_BPWR_ENABLED 1
#endif

#ifndef ANT_BPWR_LOG_ENABLED
#define ANT_BPWR_LOG_ENABLED 1
#endif

// <0=> Off 
// <1=> Error 
// <2=> Warning 
// <3=> Info 
// <4=> Debug 
#ifndef ANT_BPWR_LOG_LEVEL
#define ANT_BPWR_LOG_LEVEL 3
#endif

// <0=> Default 
// <1=> Black 
// <2=> Red 
// <3=> Green 
// <4=> Yellow 
// <5=> Blue 
// <6=> Magenta 
// <7=> Cyan 
// <8=> White 
#ifndef ANT_BPWR_INFO_COLOR
#define ANT_BPWR_INFO_COLOR 0
#endif

#ifndef ANT_BPWR_COMMON_LOG_ENABLED
#define ANT_BPWR_COMMON_LOG_ENABLED 1
#endif

// <0=> Off 
// <1=> Error 
// <2=> Warning 
// <3=> Info 
// <4=> Debug 
#ifndef ANT_BPWR_COMMON_LOG_LEVEL
#define ANT_BPWR_COMMON_LOG_LEVEL 3
#endif

// <0=> Default 
// <1=> Black 
// <2=> Red 
// <3=> Green 
// <4=> Yellow 
// <5=> Blue 
// <6=> Magenta 
// <7=> Cyan 
// <8=> White 
#ifndef ANT_BPWR_COMMON_INFO_COLOR
#define ANT_BPWR_COMMON_INFO_COLOR 0
#endif

#ifndef ANT_BPWR_PAGE_TORQUE_LOG_ENABLED
#define ANT_BPWR_PAGE_TORQUE_LOG_ENABLED 1
#endif

// <0=> Off 
// <1=> Error 
// <2=> Warning 
// <3=> Info 
// <4=> Debug 
#ifndef ANT_BPWR_PAGE_TORQUE_LOG_LEVEL
#define ANT_BPWR_PAGE_TORQUE_LOG_LEVEL 3
#endif

// <1=> Black 
// <2=> Red 
// <3=> Green 
// <4=> Yellow 
// <5=> Blue 
// <6=> Magenta 
// <7=> Cyan 
// <8=> White 
#ifndef ANT_BPWR_PAGE_TORQUE_INFO_COLOR
#define ANT_BPWR_PAGE_TORQUE_INFO_COLOR 0
#endif

#ifndef ANT_BPWR_PAGE_1_LOG_ENABLED
#define ANT_BPWR_PAGE_1_LOG_ENABLED 1
#endif
 
// <0=> Off 
// <1=> Error 
// <2=> Warning 
// <3=> Info 
// <4=> Debug 
#ifndef ANT_BPWR_PAGE_1_LOG_LEVEL
#define ANT_BPWR_PAGE_1_LOG_LEVEL 3
#endif

// <0=> Default 
// <1=> Black 
// <2=> Red 
// <3=> Green 
// <4=> Yellow 
// <5=> Blue 
// <6=> Magenta 
// <7=> Cyan 
// <8=> White 
#ifndef ANT_BPWR_PAGE_1_INFO_COLOR
#define ANT_BPWR_PAGE_1_INFO_COLOR 0
#endif

//==========================================================
#ifndef ANT_BPWR_PAGE_16_LOG_ENABLED
#define ANT_BPWR_PAGE_16_LOG_ENABLED 1
#endif
 
// <0=> Off 
// <1=> Error 
// <2=> Warning 
// <3=> Info 
// <4=> Debug 
#ifndef ANT_BPWR_PAGE_16_LOG_LEVEL
#define ANT_BPWR_PAGE_16_LOG_LEVEL 3
#endif

// <0=> Default 
// <1=> Black 
// <2=> Red 
// <3=> Green 
// <4=> Yellow 
// <5=> Blue 
// <6=> Magenta 
// <7=> Cyan 
// <8=> White 
#ifndef ANT_BPWR_PAGE_16_INFO_COLOR
#define ANT_BPWR_PAGE_16_INFO_COLOR 0
#endif

#ifndef ANT_BPWR_PAGE_17_LOG_ENABLED
#define ANT_BPWR_PAGE_17_LOG_ENABLED 1
#endif

// <1=> Error 
// <2=> Warning 
// <3=> Info 
// <4=> Debug 
#ifndef ANT_BPWR_PAGE_17_LOG_LEVEL
#define ANT_BPWR_PAGE_17_LOG_LEVEL 3
#endif

// <0=> Default 
// <1=> Black 
// <2=> Red 
// <3=> Green 
// <4=> Yellow 
// <5=> Blue 
// <6=> Magenta 
// <7=> Cyan 
// <8=> White 
#ifndef ANT_BPWR_PAGE_17_INFO_COLOR
#define ANT_BPWR_PAGE_17_INFO_COLOR 0
#endif

#ifndef ANT_BPWR_PAGE_18_LOG_ENABLED
#define ANT_BPWR_PAGE_18_LOG_ENABLED 1
#endif

// <0=> Off 
// <1=> Error 
// <2=> Warning 
// <3=> Info 
// <4=> Debug 
#ifndef ANT_BPWR_PAGE_18_LOG_LEVEL
#define ANT_BPWR_PAGE_18_LOG_LEVEL 3
#endif

// <0=> Default 
// <1=> Black 
// <2=> Red 
// <3=> Green 
// <4=> Yellow 
// <5=> Blue 
// <6=> Magenta 
// <7=> Cyan 
// <8=> White 
#ifndef ANT_BPWR_PAGE_18_INFO_COLOR
#define ANT_BPWR_PAGE_18_INFO_COLOR 0
#endif

#ifndef ANT_COMMON_PAGE_80_ENABLED
#define ANT_COMMON_PAGE_80_ENABLED 1
#endif

#ifndef ANT_COMMON_PAGE_80_LOG_ENABLED
#define ANT_COMMON_PAGE_80_LOG_ENABLED 1
#endif

// <0=> Off 
// <1=> Error 
// <2=> Warning 
// <3=> Info 
// <4=> Debug 
#ifndef ANT_COMMON_PAGE_80_LOG_LEVEL
#define ANT_COMMON_PAGE_80_LOG_LEVEL 3
#endif

// <0=> Default 
// <1=> Black 
// <2=> Red 
// <3=> Green 
// <4=> Yellow 
// <5=> Blue 
// <6=> Magenta 
// <7=> Cyan 
// <8=> White 
#ifndef ANT_COMMON_PAGE_80_INFO_COLOR
#define ANT_COMMON_PAGE_80_INFO_COLOR 0
#endif

#ifndef ANT_COMMON_PAGE_81_ENABLED
#define ANT_COMMON_PAGE_81_ENABLED 1
#endif

#ifndef ANT_COMMON_PAGE_81_LOG_ENABLED
#define ANT_COMMON_PAGE_81_LOG_ENABLED 1
#endif
 
// <0=> Off 
// <1=> Error 
// <2=> Warning 
// <3=> Info 
// <4=> Debug 
#ifndef ANT_COMMON_PAGE_81_LOG_LEVEL
#define ANT_COMMON_PAGE_81_LOG_LEVEL 3
#endif

// <0=> Default 
// <1=> Black 
// <2=> Red 
// <3=> Green 
// <4=> Yellow 
// <5=> Blue 
// <6=> Magenta 
// <7=> Cyan 
// <8=> White 
#ifndef ANT_COMMON_PAGE_81_INFO_COLOR
#define ANT_COMMON_PAGE_81_INFO_COLOR 0
#endif



#define NRF_QUEUE_ENABLED 1

#ifndef HARDFAULT_HANDLER_ENABLED
#define HARDFAULT_HANDLER_ENABLED 1
#endif

#ifndef NRF_BALLOC_ENABLED
#define NRF_BALLOC_ENABLED 1
#endif

#ifndef NRFX_PRS_ENABLED
#define NRFX_PRS_ENABLED 1
#endif

#ifndef PPI_ENABLED
#define PPI_ENABLED 1
#endif

#ifndef SAADC_ENABLED
#define SAADC_ENABLED 1
#endif

// <0=> 8 bit 
// <1=> 10 bit 
// <2=> 12 bit 
// <3=> 14 bit 
#ifndef SAADC_CONFIG_RESOLUTION
#define SAADC_CONFIG_RESOLUTION 3
#endif

// <0=> Disabled 
// <1=> 2x 
// <2=> 4x 
// <3=> 8x 
// <4=> 16x 
// <5=> 32x 
// <6=> 64x 
// <7=> 128x 
// <8=> 256x 
#ifndef SAADC_CONFIG_OVERSAMPLE
#define SAADC_CONFIG_OVERSAMPLE 0
#endif

#ifndef SAADC_CONFIG_LP_MODE
#define SAADC_CONFIG_LP_MODE 0
#endif

#ifndef SAADC_CONFIG_IRQ_PRIORITY
#define SAADC_CONFIG_IRQ_PRIORITY 6
#endif

#ifndef NRFX_SAADC_ENABLED
#define NRFX_SAADC_ENABLED 1
#endif

// <0=> 8 bit 
// <1=> 10 bit 
// <2=> 12 bit 
// <3=> 14 bit 
#ifndef NRFX_SAADC_CONFIG_RESOLUTION
#define NRFX_SAADC_CONFIG_RESOLUTION 3
#endif

#ifndef NRFX_SAADC_CONFIG_OVERSAMPLE
#define NRFX_SAADC_CONFIG_OVERSAMPLE 0
#endif

#ifndef NRFX_SAADC_CONFIG_LP_MODE
#define NRFX_SAADC_CONFIG_LP_MODE 0
#endif

// <0=> 0 (highest) 
// <1=> 1 
// <2=> 2 
// <3=> 3 
// <4=> 4 
// <5=> 5 
// <6=> 6 
// <7=> 7 
#ifndef NRFX_SAADC_CONFIG_IRQ_PRIORITY
#define NRFX_SAADC_CONFIG_IRQ_PRIORITY 6
#endif

#ifndef NRF_CLOCK_ENABLED
#define NRF_CLOCK_ENABLED 1
#endif

#ifndef CLOCK_CONFIG_LF_SRC
#define CLOCK_CONFIG_LF_SRC 1
#endif

#ifndef CLOCK_CONFIG_LF_CAL_ENABLED
#define CLOCK_CONFIG_LF_CAL_ENABLED 0
#endif

// <i> Priorities 0,2 (nRF51) and 0,1,4,5 (nRF52) are reserved for SoftDevice
// <0=> 0 (highest) 
// <1=> 1 
// <2=> 2 
// <3=> 3 
// <4=> 4 
// <5=> 5 
// <6=> 6 
// <7=> 7 
#ifndef CLOCK_CONFIG_IRQ_PRIORITY
#define CLOCK_CONFIG_IRQ_PRIORITY 6
#endif

#ifndef NRFX_CLOCK_ENABLED
#define NRFX_CLOCK_ENABLED 1
#endif

#ifndef NRFX_CLOCK_CONFIG_LF_SRC
#define NRFX_CLOCK_CONFIG_LF_SRC 1
#endif

#ifndef NRFX_CLOCK_CONFIG_IRQ_PRIORITY
#define NRFX_CLOCK_CONFIG_IRQ_PRIORITY 6
#endif

#define TIMER_ENABLED 1
#define TIMER1_ENABLED 1

#ifndef NRFX_SYSTICK_ENABLED
#define NRFX_SYSTICK_ENABLED 1
#endif

#ifndef NRFX_UARTE_ENABLED
#define NRFX_UARTE_ENABLED 1
#endif

#ifndef NRF_CLI_UART_ENABLED
#define NRF_CLI_UART_ENABLED 1
#endif

#ifndef NRF_MEMOBJ_ENABLED
#define NRF_MEMOBJ_ENABLED 1
#endif

#ifndef NRF_PWR_MGMT_ENABLED
#define NRF_PWR_MGMT_ENABLED 1
#endif

#ifndef POWER_ENABLED
#define POWER_ENABLED 1
#endif

#ifndef NRFX_POWER_ENABLED
#define NRFX_POWER_ENABLED 1
#endif

#ifndef NRFX_POWER_CONFIG_IRQ_PRIORITY
#define NRFX_POWER_CONFIG_IRQ_PRIORITY 6
#endif

#ifndef NRFX_POWER_CONFIG_DEFAULT_DCDCEN
#define NRFX_POWER_CONFIG_DEFAULT_DCDCEN 0
#endif

#ifndef NRFX_POWER_CONFIG_DEFAULT_DCDCENHV
#define NRFX_POWER_CONFIG_DEFAULT_DCDCENHV 0
#endif

#ifndef NRFX_PRS_ENABLED
#define NRFX_PRS_ENABLED 1
#endif

#ifndef NRFX_PRS_BOX_0_ENABLED
#define NRFX_PRS_BOX_0_ENABLED 0
#endif

#ifndef NRFX_PRS_BOX_1_ENABLED
#define NRFX_PRS_BOX_1_ENABLED 0
#endif

#ifndef NRFX_PRS_BOX_2_ENABLED
#define NRFX_PRS_BOX_2_ENABLED 0
#endif

#ifndef NRFX_PRS_BOX_3_ENABLED
#define NRFX_PRS_BOX_3_ENABLED 0
#endif

#ifndef NRFX_PRS_BOX_4_ENABLED
#define NRFX_PRS_BOX_4_ENABLED 1
#endif

#ifndef NRFX_USBD_ENABLED
#define NRFX_USBD_ENABLED 1
#endif

// <0=> 0 (highest) 
// <1=> 1 
// <2=> 2 
// <3=> 3 
// <4=> 4 
// <5=> 5 
// <6=> 6 
// <7=> 7 
#ifndef NRFX_USBD_CONFIG_IRQ_PRIORITY
#define NRFX_USBD_CONFIG_IRQ_PRIORITY 6
#endif

#ifndef NRFX_USBD_CONFIG_DMASCHEDULER_MODE
#define NRFX_USBD_CONFIG_DMASCHEDULER_MODE 0
#endif

#ifndef NRFX_USBD_CONFIG_DMASCHEDULER_ISO_BOOST
#define NRFX_USBD_CONFIG_DMASCHEDULER_ISO_BOOST 1
#endif

#ifndef NRFX_USBD_CONFIG_ISO_IN_ZLP
#define NRFX_USBD_CONFIG_ISO_IN_ZLP 0
#endif

#ifndef APP_USBD_CDC_ACM_ENABLED
#define APP_USBD_CDC_ACM_ENABLED 1
#endif

#ifndef APP_USBD_CDC_ACM_ZLP_ON_EPSIZE_WRITE
#define APP_USBD_CDC_ACM_ZLP_ON_EPSIZE_WRITE 1
#endif

#ifndef USBD_ENABLED
#define USBD_ENABLED 1
#endif

#ifndef USBD_CONFIG_IRQ_PRIORITY
#define USBD_CONFIG_IRQ_PRIORITY 6
#endif

#ifndef USBD_CONFIG_DMASCHEDULER_MODE
#define USBD_CONFIG_DMASCHEDULER_MODE 0
#endif

#ifndef USBD_CONFIG_DMASCHEDULER_ISO_BOOST
#define USBD_CONFIG_DMASCHEDULER_ISO_BOOST 1
#endif

#ifndef USBD_CONFIG_ISO_IN_ZLP
#define USBD_CONFIG_ISO_IN_ZLP 0
#endif

#ifndef NRF_CLI_ENABLED
#define NRF_CLI_ENABLED 1
#endif

#ifndef NRF_CLI_ARGC_MAX
#define NRF_CLI_ARGC_MAX 12
#endif

#ifndef NRF_CLI_BUILD_IN_CMDS_ENABLED
#define NRF_CLI_BUILD_IN_CMDS_ENABLED 1
#endif

#ifndef NRF_CLI_CMD_BUFF_SIZE
#define NRF_CLI_CMD_BUFF_SIZE 128
#endif

#ifndef NRF_CLI_ECHO_STATUS
#define NRF_CLI_ECHO_STATUS 1
#endif

#ifndef NRF_CLI_WILDCARD_ENABLED
#define NRF_CLI_WILDCARD_ENABLED 0
#endif

#ifndef NRF_CLI_METAKEYS_ENABLED
#define NRF_CLI_METAKEYS_ENABLED 0
#endif

#ifndef NRF_CLI_PRINTF_BUFF_SIZE
#define NRF_CLI_PRINTF_BUFF_SIZE 23
#endif

#ifndef NRF_CLI_HISTORY_ENABLED
#define NRF_CLI_HISTORY_ENABLED 1
#endif

#ifndef NRF_CLI_HISTORY_ELEMENT_SIZE
#define NRF_CLI_HISTORY_ELEMENT_SIZE 32
#endif

#ifndef NRF_CLI_HISTORY_ELEMENT_COUNT
#define NRF_CLI_HISTORY_ELEMENT_COUNT 8
#endif

#ifndef NRF_CLI_VT100_COLORS_ENABLED
#define NRF_CLI_VT100_COLORS_ENABLED 1
#endif

#ifndef NRF_CLI_STATISTICS_ENABLED
#define NRF_CLI_STATISTICS_ENABLED 1
#endif

#ifndef NRF_CLI_LOG_BACKEND
#define NRF_CLI_LOG_BACKEND 1
#endif

#ifndef NRF_CLI_USES_TASK_MANAGER_ENABLED
#define NRF_CLI_USES_TASK_MANAGER_ENABLED 0
#endif

#ifndef APP_USBD_ENABLED
#define APP_USBD_ENABLED 1
#endif

#ifndef APP_USBD_VID
#define APP_USBD_VID 0x1915
#endif

#ifndef APP_USBD_PID
#define APP_USBD_PID 0x520F
#endif

#ifndef APP_USBD_DEVICE_VER_MAJOR
#define APP_USBD_DEVICE_VER_MAJOR 1
#endif

#ifndef APP_USBD_DEVICE_VER_MINOR
#define APP_USBD_DEVICE_VER_MINOR 0
#endif

#ifndef APP_USBD_DEVICE_VER_SUB
#define APP_USBD_DEVICE_VER_SUB 0
#endif

#ifndef APP_USBD_CONFIG_SELF_POWERED
#define APP_USBD_CONFIG_SELF_POWERED 1
#endif

#ifndef APP_USBD_CONFIG_MAX_POWER
#define APP_USBD_CONFIG_MAX_POWER 100
#endif

#ifndef APP_USBD_CONFIG_POWER_EVENTS_PROCESS
#define APP_USBD_CONFIG_POWER_EVENTS_PROCESS 1
#endif

#ifndef APP_USBD_CONFIG_EVENT_QUEUE_ENABLE
#define APP_USBD_CONFIG_EVENT_QUEUE_ENABLE 1
#endif

#ifndef APP_USBD_CONFIG_EVENT_QUEUE_SIZE
#define APP_USBD_CONFIG_EVENT_QUEUE_SIZE 32
#endif

#ifndef APP_USBD_CONFIG_SOF_HANDLING_MODE
#define APP_USBD_CONFIG_SOF_HANDLING_MODE 1
#endif

#ifndef APP_USBD_CONFIG_SOF_TIMESTAMP_PROVIDE
#define APP_USBD_CONFIG_SOF_TIMESTAMP_PROVIDE 0
#endif

#ifndef APP_USBD_CONFIG_DESC_STRING_SIZE
#define APP_USBD_CONFIG_DESC_STRING_SIZE 31
#endif

#ifndef APP_USBD_CONFIG_DESC_STRING_UTF_ENABLED
#define APP_USBD_CONFIG_DESC_STRING_UTF_ENABLED 0
#endif

#ifndef APP_USBD_STRINGS_LANGIDS
#define APP_USBD_STRINGS_LANGIDS APP_USBD_LANG_AND_SUBLANG(APP_USBD_LANG_ENGLISH, APP_USBD_SUBLANG_ENGLISH_US)
#endif

#ifndef APP_USBD_STRING_ID_MANUFACTURER
#define APP_USBD_STRING_ID_MANUFACTURER 1
#endif

#ifndef APP_USBD_STRINGS_MANUFACTURER_EXTERN
#define APP_USBD_STRINGS_MANUFACTURER_EXTERN 0
#endif

#ifndef APP_USBD_STRINGS_MANUFACTURER
#define APP_USBD_STRINGS_MANUFACTURER APP_USBD_STRING_DESC("Nordic Semiconductor")
#endif

#ifndef APP_USBD_STRING_ID_PRODUCT
#define APP_USBD_STRING_ID_PRODUCT 2
#endif

#ifndef APP_USBD_STRINGS_PRODUCT_EXTERN
#define APP_USBD_STRINGS_PRODUCT_EXTERN 0
#endif

#ifndef APP_USBD_STRINGS_PRODUCT
#define APP_USBD_STRINGS_PRODUCT APP_USBD_STRING_DESC("nRF52 USB CDC Demo")
#endif

#ifndef APP_USBD_STRING_ID_SERIAL
#define APP_USBD_STRING_ID_SERIAL 3
#endif

#ifndef APP_USBD_STRING_SERIAL_EXTERN
#define APP_USBD_STRING_SERIAL_EXTERN 1
#endif

#ifndef APP_USBD_STRING_SERIAL
#define APP_USBD_STRING_SERIAL g_extern_serial_number
#endif

#ifndef APP_USBD_STRING_ID_CONFIGURATION
#define APP_USBD_STRING_ID_CONFIGURATION 4
#endif

#ifndef APP_USBD_STRING_CONFIGURATION_EXTERN
#define APP_USBD_STRING_CONFIGURATION_EXTERN 0
#endif

#ifndef APP_USBD_STRINGS_CONFIGURATION
#define APP_USBD_STRINGS_CONFIGURATION APP_USBD_STRING_DESC("Default configuration")
#endif

#ifndef APP_USBD_STRINGS_USER
#define APP_USBD_STRINGS_USER X(APP_USER_1, , APP_USBD_STRING_DESC("User 1"))
#endif

#ifndef APP_USBD_CDC_ACM_ENABLED
#define APP_USBD_CDC_ACM_ENABLED 1
#endif

#ifndef APP_USBD_CDC_ACM_ZLP_ON_EPSIZE_WRITE
#define APP_USBD_CDC_ACM_ZLP_ON_EPSIZE_WRITE 1
#endif
