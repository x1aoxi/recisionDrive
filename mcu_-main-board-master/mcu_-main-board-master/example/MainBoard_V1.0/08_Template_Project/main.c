#include "driver_icm20608.h"
#include "usart.h"

void usart1_send_char(uint8_t data)
{
    HAL_UART_Transmit(&huart1, &data, 1, 0xFFFF);
}

int main(void)
{
    float pitch, roll, yaw;
    int16_t gyro[3], accel[3];
    float gyro_dps[3], accel_g[3];
    float temp_degrees;
    int16_t temp_raw;
    uint8_t res;
    
    while (1)
    {
        res = icm20608_read_gyroscope(&gs_handle, gyro, gyro_dps);
        if (res == 0)
        {
            res = icm20608_read_accelerometer(&gs_handle, accel, accel_g);
            if (res == 0)
            {
                res = icm20608_read_temperature(&gs_handle, &temp_raw, &temp_degrees);
                if (res == 0)
                {
                    icm20608_send_raw_data(accel[0], accel[1], accel[2],
                                         gyro[0], gyro[1], gyro[2]);
                }
            }
        }
        
        res = icm20608_mpl_get_euler(&gs_handle, &pitch, &roll, &yaw);
        if (res == 0)
        {
        }
        
        HAL_Delay(10);
    }
} 