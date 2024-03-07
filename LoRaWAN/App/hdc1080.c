/**
 * @file hdc1080.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-03-04
 *
 * @copyright Copyright (c) 2024
 *
 */
/***********************************************************************************************************************
 * Pragma directive
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes <System Includes>
 ***********************************************************************************************************************/
#include "hdc1080.h"
#include "i2c.h"
#include "stm32wlxx_hal_i2c.h"
#include "platform.h"
#include "sys_app.h"
#include "lora_app.h"
#include "stm32_seq.h"
#include "stm32_timer.h"
#include "utilities_def.h"
#include "lora_app_version.h"
#include "lorawan_version.h"
#include "subghz_phy_version.h"
#include "lora_info.h"
#include "LmHandler.h"
#include "stm32_lpm.h"
#include "CayenneLpp.h"
#include "sys_sensors.h"
#include "flash_if.h"

/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables and functions
 ***********************************************************************************************************************/
static void hdc1080_power_enable(uint8_t power);
/***********************************************************************************************************************
 * Exported global variables and functions (to be accessed by other files)
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Imported global variables and functions (from other files)
 ***********************************************************************************************************************/
/**
 *
 */
void hdc1080_init(Temp_Reso Temperature_Resolution_x_bit, Humi_Reso Humidity_Resolution_x_bit)
{
    hdc1080_power_enable(POWER_ON);
    hdc1080_enable_bootstrap();
    HAL_Delay(15);
    /* Temperature and Humidity are acquired in sequence, Temperature first
     * Default:   Temperature resolution = 14 bit,
     *            Humidity resolution = 14 bit
     */

    /* Set the acquisition mode to measure both temperature and humidity by setting Bit[12] to 1 */
    uint16_t config_reg_value = 0x1000;
    uint8_t data_send[2];

    if (Temperature_Resolution_x_bit == Temperature_Resolution_11_bit)
    {
        config_reg_value |= (1 << 10); // 11 bit
    }

    switch ((uint8_t)Humidity_Resolution_x_bit)
    {
    case Humidity_Resolution_11_bit:
        config_reg_value |= (1 << 8);
        break;
    case Humidity_Resolution_8_bit:
        config_reg_value |= (1 << 9);
        break;
    }

    data_send[0] = (config_reg_value >> 8);
    data_send[1] = (config_reg_value & 0x00ff);

    int ret = HAL_I2C_Mem_Write(&hi2c2, HDC_1080_ADD << 1, Configuration_register_add, I2C_MEMADD_SIZE_8BIT, data_send, 2, 1000);
}

/**
 * @brief
 *
 * @param hdc1080_data
 * @return int32_t
 */
int32_t hdc1080_event_read(hdc1080_data_t *hdc1080_data)
{
    hdc1080_init(Temperature_Resolution_14_bit, Humidity_Resolution_14_bit);
    HAL_Delay(15);
    int ret = -1;
    uint8_t receive_data[4];
    uint16_t temp_x, humi_x;
    uint8_t send_data = Temperature_register_add;

    ret = HAL_I2C_Master_Transmit(&hi2c2, HDC_1080_ADD << 1, &send_data, 1, 1000);
    if (ret != HAL_OK)
        return ret;
    /* Delay here 15ms for conversion compelete.
     * Note: datasheet say maximum is 7ms, but when delay=7ms, the read value is not correct
     */
    HAL_Delay(15);

    /* Read temperature and humidity */
    ret = HAL_I2C_Master_Receive(&hi2c2, HDC_1080_ADD << 1, receive_data, 4, 1000);
    if (ret != HAL_OK)
        return ret;
    temp_x = ((receive_data[0] << 8) | receive_data[1]);
    humi_x = ((receive_data[2] << 8) | receive_data[3]);

    hdc1080_data->temperature = ((temp_x / 65536.0) * 165.0) - 40.0;
    hdc1080_data->humidity = (uint8_t)((humi_x / 65536.0) * 100.0);
    return ret;
}

/**
 * @brief keep alive when button is pressed
 *
 */
void hdc1080_enable_bootstrap(void)
{
    HAL_GPIO_WritePin(KEEP_ALIVE_GPIO_Port, KEEP_ALIVE_Pin, POWER_ON);
}
/***********************************************************************************************************************
 * static functions
 ***********************************************************************************************************************/
static void hdc1080_power_enable(uint8_t power)
{
    HAL_GPIO_WritePin(SENS_HDC_PWM_GPIO_Port, SENS_HDC_PWM_Pin, power);
}
/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/
