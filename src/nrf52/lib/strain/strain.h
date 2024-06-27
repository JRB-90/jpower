#ifndef STRAIN_H__
#define STRAIN_H__

#include <stdint.h>
#include "app_error.h"
#include "nrf_drv_spi.h"
#include "jp_utils.h"

typedef struct
{
    guid_t cal_id;
    float slope;
    float intercept;
    float crank_length_m;
} calibration_data_t;

extern ret_code_t strain_init(
    nrf_drv_spi_t* spi_instance,
    const uint8_t sck_pin,
    const uint8_t mosi_pin,
    const uint8_t miso_pin,
    const uint8_t ss_pin
);
extern void strain_update_10ms(float delta_time_s);
extern void strain_get_calibration(calibration_data_t* const calibration);
extern void strain_set_calibration(calibration_data_t* const calibration);
extern uint32_t strain_get_current_adv_value();
extern float strain_get_current_torque_nm();
extern ret_code_t strain_zero_offset();

#endif // STRAIN_H__