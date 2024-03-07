/**
 * @file hdc1080.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-03-04
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef HDC1080_H_
#define HDC1080_H_

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "stdint.h"
#include "stm32wlxx_hal.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define HDC_1080_ADD 0x40
#define Configuration_register_add 0x02
#define Temperature_register_add 0x00
#define Humidity_register_add 0x01
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef struct
{
    float temperature;
    float humidity;
} hdc1080_data_t;

typedef enum
{
    POWER_OFF = 0,
    POWER_ON = 1,
}e_hdc1080_power;
/**
 * @brief 
 * 
 */
typedef enum
{
    Temperature_Resolution_14_bit = 0,
    Temperature_Resolution_11_bit = 1
} Temp_Reso;

/**
 * @brief 
 * 
 */
typedef enum
{
    Humidity_Resolution_14_bit = 0,
    Humidity_Resolution_11_bit = 1,
    Humidity_Resolution_8_bit = 2
} Humi_Reso;
/****************************************************************************/
/***         Exported global functions                                     ***/
/****************************************************************************/
void hdc1080_init(Temp_Reso Temperature_Resolution_x_bit,Humi_Reso Humidity_Resolution_x_bit);


/**
 * @brief 
 * 
 * @param hdc1080_data 
 * @return int32_t 
 */
int32_t hdc1080_event_read(hdc1080_data_t *hdc1080_data);

#endif /* HDC1080_H_ */
