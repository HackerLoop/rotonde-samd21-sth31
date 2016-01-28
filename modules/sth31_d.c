/*
 * sth31_d.c
 *
 * Created: 21/01/2016 20:55:34
 *  Author: GroundLayer
 */ 

#include "modules/drivers/sth31_d.h"
#include "modules/modules/sth31_d.h"

#include "registrar/registrar.h"
#include "tasks/modules.h"

#define MODULE_NAME "STH31_D_TEMP_HUMI"
#define MODULE_DATA_SIZE 2
#define MODULE_TEMP "temperature"
#define MODULE_HUMIDITY "humidity"


data_name STH31_D_names[MODULE_DATA_SIZE] = { MODULE_TEMP , MODULE_HUMIDITY  };
data_type STH31_D_types[] = { DOUBLE , DOUBLE };

void STH31_D_tick(void);
definition *def;

void STH31_D_module_init(void) {
	STH31_init();
	def = register_definition(EVENT , MODULE_NAME , STH31_D_names , STH31_D_types , MODULE_DATA_SIZE , NULL, STH31_D_tick , 1000);
}

void STH31_D_tick(void) {
	double temp , humi;
	
	if(STH31_D_read_temp_humi(&temp , &humi)) {
		instance inst = create_instance(MODULE_NAME);
		if(inst.def != NULL && inst.values != NULL){
			(*(double*) inst.values[0]) = temp;
			(*(double*) inst.values[1]) = humi;
			portBASE_TYPE xStatus = xQueueSendToBack(module_out, &inst , 10 / portTICK_RATE_MS );
			if(xStatus != pdPASS){
				release_instance(inst);
			}
		}
	}
}