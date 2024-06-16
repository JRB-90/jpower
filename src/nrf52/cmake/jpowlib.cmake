# Add Math Library
macro(jpow_addJPowMath)
    list(APPEND INCLUDE_DIRS
        "${CMAKE_SOURCE_DIR}/lib/math"
    )

    list(APPEND SOURCE_FILES
        "${CMAKE_SOURCE_DIR}/lib/math/jpow_math.c"
    )
endmacro()

# Add JPower Utils Library
macro(jpow_addJPUtils)
    nRF5_addHWRNGLegacy()
    list(APPEND INCLUDE_DIRS
        "${CMAKE_SOURCE_DIR}/lib/utils"
    )

    list(APPEND SOURCE_FILES
        "${CMAKE_SOURCE_DIR}/lib/utils/jp_utils.c"
    )
endmacro()

# Add Storage Library
macro(jpow_addStorage)
    list(APPEND INCLUDE_DIRS
        "${CMAKE_SOURCE_DIR}/lib/storage"
    )

    list(APPEND SOURCE_FILES
        "${CMAKE_SOURCE_DIR}/lib/storage/storage_helper.c"
    )
endmacro()

# Add Calibrate Library
macro(jpow_addCalibrate)
    jpow_addJPUtils()
    jpow_addBLE()
    list(APPEND INCLUDE_DIRS
        "${CMAKE_SOURCE_DIR}/lib/calibrate"
    )

    list(APPEND SOURCE_FILES
        "${CMAKE_SOURCE_DIR}/lib/calibrate/calibrate.c"
        "${CMAKE_SOURCE_DIR}/lib/calibrate/calibrate_srv.c"
    )
endmacro()

# Add JPower State Library
macro(jpow_addJPState)
    jpow_addBLE()
    list(APPEND INCLUDE_DIRS
        "${CMAKE_SOURCE_DIR}/lib/state"
    )

    list(APPEND SOURCE_FILES
        "${CMAKE_SOURCE_DIR}/lib/state/jp_state.c"
        "${CMAKE_SOURCE_DIR}/lib/state/jp_state_srv.c"
    )
endmacro()

# Add I2C Library
macro(jpow_addI2C)
    list(APPEND INCLUDE_DIRS
        "${CMAKE_SOURCE_DIR}/lib/i2c"
    )

    list(APPEND SOURCE_FILES
        "${CMAKE_SOURCE_DIR}/lib/i2c/i2c_helper.c"
    )
endmacro()

# Add SPI Library
macro(jpow_addSPI)
    nRF5_addLegacySpi()
    list(APPEND INCLUDE_DIRS
        "${CMAKE_SOURCE_DIR}/lib/spi"
    )

    list(APPEND SOURCE_FILES
        "${CMAKE_SOURCE_DIR}/lib/spi/spi_helper.c"
    )
endmacro()

# Add IMU Library
macro(jpow_addIMU)
    nRF5_addClock()
    nRF5_addTWILegacy()
    jpow_addJPowMath()
    jpow_addI2C()
    list(APPEND INCLUDE_DIRS
        "${CMAKE_SOURCE_DIR}/lib/imu"
    )

    list(APPEND SOURCE_FILES
        "${CMAKE_SOURCE_DIR}/lib/imu/imu.c"
        "${CMAKE_SOURCE_DIR}/lib/imu/lsm6dso_reg.c"
    )
endmacro()

# Add Fusion Library
macro(jpow_addFusion)
    list(APPEND INCLUDE_DIRS
        "${CMAKE_SOURCE_DIR}/lib/fusion"
    )

    list(APPEND SOURCE_FILES
        "${CMAKE_SOURCE_DIR}/lib/fusion/FusionAhrs.c"
        "${CMAKE_SOURCE_DIR}/lib/fusion/FusionCompass.c"
        "${CMAKE_SOURCE_DIR}/lib/fusion/FusionOffset.c"
    )
endmacro()

# Add Strain Library
macro(jpow_addStrain)
    jpow_addSPI()
    list(APPEND INCLUDE_DIRS
        "${CMAKE_SOURCE_DIR}/lib/strain"
    )

    list(APPEND SOURCE_FILES
        "${CMAKE_SOURCE_DIR}/lib/strain/strain.c"
        "${CMAKE_SOURCE_DIR}/lib/strain/ad779x.c"
    )
endmacro()

# Add USB Library
macro(jpow_addUSB)
    nRF5_addUsbd()
    list(APPEND INCLUDE_DIRS
        "${CMAKE_SOURCE_DIR}/lib/usb"
    )

    list(APPEND SOURCE_FILES
        "${CMAKE_SOURCE_DIR}/lib/usb/usb_subsystem.c"
    )
endmacro()

# Add BLE Library
macro(jpow_addBLE)
    nRF5_addPowerMgmt()
    nRF5_addSoftDeviceSupport()
    nRF5_addBLEService(ble_nus)
    nRF5_addBLEService(ble_lbs)
    nRF5_addBLEService(ble_dfu)
    nRF5_addBLEQWR()
    list(APPEND INCLUDE_DIRS
        "${CMAKE_SOURCE_DIR}/lib/ble"
    )

    list(APPEND SOURCE_FILES
        "${CMAKE_SOURCE_DIR}/lib/ble/ble_subsystem.c"
        "${CMAKE_SOURCE_DIR}/lib/ble/ble_srv_helper.c"
    )
endmacro()

# Add ANT Library
macro(jpow_addANTPlus)
    nRF5_addAppTimer()
    nRF5_addPowerMgmt()
    nRF5_addSoftDeviceSupport()
    nRF5_addANTPlus()
    list(APPEND INCLUDE_DIRS
        "${CMAKE_SOURCE_DIR}/lib/ant_plus"
    )

    list(APPEND SOURCE_FILES
        "${CMAKE_SOURCE_DIR}/lib/ant_plus/ant_subsystem.c"
    )
endmacro()