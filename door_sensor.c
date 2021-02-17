#include <SH79F084B.h>
#include "globalvar.h"
#include "door_sensor.h"
#include "pin_manager.h"

static bit f_66hz_door;
static uint8_t last_door_value;

void door_sensor_init ( void )
{
	last_door_value = 0xFE;
	f_66hz_door = 0;
}

//---------------------------------------------------------------------------------------------------------------
// Драйвер захвата сигнала датчика двери.
// выходной сигнал: current_door_value
// счёт до 7 временных интервалов, т.е. 100мс 
// частота захывата (66Гц)
//---------------------------------------------------------------------------------------------------------------
void door_sensor_capture ( void )
{
	last_door_value<<=1;
	if (REED_PIN) last_door_value|=0x01;
	if (last_door_value == 0x7F) {
		door_state = DOOR_CLOSED;
	}
	if (last_door_value == 0x80) {
		door_state = DOOR_OPENED;
	}
}

void door_sensor_proc ( void )
{
	if (f_66hz_door){
		f_66hz_door = 0;
		door_sensor_capture();
	}
}

void door_sensor_66hz_proc ( void )
{
	f_66hz_door = 1;
}