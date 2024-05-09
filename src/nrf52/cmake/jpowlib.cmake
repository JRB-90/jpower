# Add Math Library
macro(jpow_addJPowMath)
    list(APPEND INCLUDE_DIRS
        "${CMAKE_SOURCE_DIR}/lib/math"
    )

    list(APPEND SOURCE_FILES
        "${CMAKE_SOURCE_DIR}/lib/math/jpow_math.c"
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
    list(APPEND INCLUDE_DIRS
        "${CMAKE_SOURCE_DIR}/lib/spi"
    )

    list(APPEND SOURCE_FILES
        "${CMAKE_SOURCE_DIR}/lib/spi/spi_helper.c"
    )
endmacro()

# Add IMU Library
macro(jpow_addLSM6DS)
    jpow_addJPowMath()
    jpow_addI2C()
    list(APPEND INCLUDE_DIRS
        "${CMAKE_SOURCE_DIR}/lib/lsm6ds"
    )

    list(APPEND SOURCE_FILES
        "${CMAKE_SOURCE_DIR}/lib/lsm6ds/imu.c"
        "${CMAKE_SOURCE_DIR}/lib/lsm6ds/lsm6ds3tr-c_reg.c"
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
        "${CMAKE_SOURCE_DIR}/lib/ad779x"
    )

    list(APPEND SOURCE_FILES
        "${CMAKE_SOURCE_DIR}/lib/ad779x/strain.c"
        "${CMAKE_SOURCE_DIR}/lib/ad779x/ad779x.c"
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