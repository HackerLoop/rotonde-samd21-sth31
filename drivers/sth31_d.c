/*
 * sth31_d.c
 *
 * Created: 21/01/2016 20:35:33
 *  Author: GroundLayer
 */ 

#include "modules/drivers/sth31_d.h"

enum status_code STH31_init(void){
	return STH31_reset();
}

enum status_code STH31_reset(void){
	unsigned char data[2] = {SHT31_SOFTRESET >> 8 , SHT31_SOFTRESET & 0xFF };
	return HAL_I2C_write(SHT31_DEFAULT_ADDR ,2 , data);
}

bool STH31_D_read_temp_humi(double* temperature , double* humidity){
	unsigned char data[6] = {SHT31_MEAS_HIGHREP >> 8 , SHT31_MEAS_HIGHREP & 0xFF };
	HAL_I2C_write(SHT31_DEFAULT_ADDR ,2 , data);
	
	vTaskDelay(500);
	if(HAL_I2C_read(SHT31_DEFAULT_ADDR , 6 , data) != STATUS_OK) {
		return false;
	}
	uint16_t temp = data[0] << 8 | data[1];
	uint16_t humi = data[3] << 8 | data[4];
	
	*temperature = (double) temp;
	*temperature *= 175;
	*temperature /= 0xffff;
	*temperature = *temperature - 45;
	*humidity = (double) humi;
	*humidity *= 100;
	*humidity /= 0xffff;
	
	return true;
	
}
