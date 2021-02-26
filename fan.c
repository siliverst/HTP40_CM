#include "CMS80F231x.h"
#include "stdint.h"
#include "globalvar.h"
#include "fan.h"
#include "door_sensor.h"
#include "mk.h"

static bit proc_8Hz_en;
static uint8_t second;

//---------------------------------------------------------------------------------------------------------------
// инициализация вентилятора
//---------------------------------------------------------------------------------------------------------------
void fan_init (void)
{
	proc_8Hz_en = 0;
	fan = 0;
}

void fan_proc ( void )
{
	if (err_eva_overheat||err_eva_sensor||err_fc_sensor){
		fan = 0;
	}
	else if (mk_get_defrost_status() > DEFROST_WAIT_DECREASE_EVA_TEMP){
		fan = 0;
	}
	else if (door_state == DOOR_OPENED){
		fan = 0;
	}
	else if (compressor){
		fan = 1;
		second = 8*25;
	}else{
		if (!second){
			fan = 0;
		}
	}
	
	if (proc_8Hz_en){
		proc_8Hz_en = 0;
		if (second) second--;
	}
}

//---------------------------------------------------------------------------------------------------------------
// функция установки флага разрешения обсчёта времени 8Гц.
//---------------------------------------------------------------------------------------------------------------
void fan_8Hz_proc ( void )
{
	proc_8Hz_en = 1;
}

