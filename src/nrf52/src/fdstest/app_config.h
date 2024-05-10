// The COM port that the NRF BLE dongle is attached to
#define COM_PORT "COM6"
// The BLE advertising device name
#define DEVICE_NAME "JPower"

#define CRC16_ENABLED 1
#define FDS_ENABLED 1
#define FDS_BACKEND 2
#define FDS_CRC_CHECK_ON_READ 1
#define FDS_CRC_CHECK_ON_WRITE 0
#define FDS_MAX_USERS 4
#define FDS_OP_QUEUE_SIZE 4
#define FDS_VIRTUAL_PAGES 3
#define FDS_VIRTUAL_PAGE_SIZE 1024
#define FDS_VIRTUAL_PAGES_RESERVED 0

#define NRF_FSTORAGE_ENABLED 1
#define NRF_FSTORAGE_PARAM_CHECK_DISABLED 0
#define NRF_FSTORAGE_SD_QUEUE_SIZE 4
#define NRF_FSTORAGE_SD_MAX_RETRIES 8
#define NRF_FSTORAGE_SD_MAX_WRITE_SIZE 4096

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

#define NRF_DFU_SVCI_ENABLED 1
#define NRF_DFU_TRANSPORT_BLE 1

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



