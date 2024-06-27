#ifndef SENSOR_SUBSYSTEM_H__
#define SENSOR_SUBSYSTEM_H__

#include <stdint.h>
#include "app_error.h"
#include "nrf_drv_twi.h"
#include "nrf_drv_spi.h"
#include "imu_types.h"

#define TWI_INSTANCE_ID         0                       // Internal TWI device to use
#define SPI_INSTANCE_ID         1                       // Internal SPI device to use

typedef struct
{
    uint8_t i2c_scl_pin;
    uint8_t i2c_sda_pin;
    uint8_t spi_sck_pin;
    uint8_t spi_mosi_pin;
    uint8_t spi_miso_pin;
    uint8_t spi_cs_pin;
    uint8_t wake_pin;
} sensor_config_t;

extern ret_code_t sensor_subsystem_init(const sensor_config_t* const config);
extern void sensor_subsystem_register_activity_event_cb(activity_event_cb callback);
extern void sensor_subsystem_update_10ms(float time_delta_s);
extern void sensor_enable_activity_tracking();
extern void sensor_disable_activity_tracking();

#endif // JPOW_SENSOR_H__