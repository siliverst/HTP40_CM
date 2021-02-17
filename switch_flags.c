#include <math.h>
#include "globalvar.h"
#include "switch_flags.h"
#include "door_sensor.h"
#include "NTC.h"
#include "trim.h"
#include "mk.h"
#include "eeprom.h"

static uint16_t prev_fc_sensor, prev_eva_sensor;	// здесь храню предыдущие значения сенсоров, 
												// что бы лишний раз не колбасить модуль.

void switch_flags_init (void)
{
	err_fc_sensor = 0;
	err_eva_sensor = 0;
	err_eva_overheat = 0;
	prev_eva_sensor = gvar.eva_sensor;
	prev_fc_sensor = gvar.fc_sensor;
	switch_flags_fc_sensor_proc();
	switch_flags_eva_sensor_proc();
	#ifdef D_MODE
	gvar.fc_temp = -21;
	gvar.eva_temp = -20;
	#endif
	
}


void switch_flags_fc_sensor_proc ( void )
{
	#ifndef D_MODE
	// =================================================================================
	// обработка МК сенсора
	// =================================================================================
	if(gvar.fc_sensor != prev_fc_sensor)
	{
		prev_fc_sensor = gvar.fc_sensor;
	}
	else
	{
		if((prev_fc_sensor < (14*16))||(prev_fc_sensor > (1010*16)))
		{
			gvar.fc_temp = 127;
			if (!err_fc_sensor){
				err_fc_sensor = 1;
				sf = 0;
				switch_flags_cepb_dis();
			}
		}
		else
		{	
			if (err_fc_sensor){
				err_fc_sensor = 0;
			}
			gvar.fc_temp = fCalcTempAir(prev_fc_sensor);
			if (gvar.fc_temp > 45)	gvar.fc_temp = 45;
			if (gvar.fc_temp < -45)	gvar.fc_temp = -45;
		}
	}
	#endif
}

void switch_flags_eva_sensor_proc ( void )
{
	#ifndef D_MODE
	// =================================================================================
	// обработка EVA сенсора
	// =================================================================================
	if(gvar.eva_sensor != prev_eva_sensor)
	{
		prev_eva_sensor = gvar.eva_sensor;
	}
	else
	{
		if((prev_eva_sensor < (14*16))||(prev_eva_sensor > (1010*16)))
		{
			gvar.eva_temp = 127;
			if (!err_eva_sensor){
				err_eva_sensor = 1;
				sf = 0;
				switch_flags_cepb_dis();
			}
		}
		else
		{
			if (err_eva_sensor){
				err_eva_sensor = 0;
			}
			gvar.eva_temp = fCalcTempAir(prev_eva_sensor);
			if (gvar.eva_temp > 80)	gvar.eva_temp = 80;
			if (gvar.eva_temp < -45) gvar.eva_temp = -45;

			if (!err_eva_overheat){
				if (gvar.eva_temp > TEMP_HEATER_OVERHEAT){
					err_eva_overheat = 1;
				}
			}			
		}
	}
	#endif
}

void switch_flags_proc (void)
{
	static last_trim_state = 0xFF;
	
	if (last_trim_state != gvar.tr_sector){
		last_trim_state = gvar.tr_sector;
		
		// обработка положения SF
		if (last_trim_state == TR_SEC_SF){
			if (!sf){
				sf = 1;
			}
		}else{
			sf = 0;
		}
	}
}

void switch_flags_cepb_dis (void)
{
	cepb = 0;
	EEPROM_sector_erase(0);
	mk_defrost_init();
}

void switch_flags_cepb_proc (void)
{
	static uint16_t seconds = 0;
	static uint8_t state = 0;
	
	if (seconds) {
		seconds--;
		if (!seconds){
			state = 0;
		}
	}
	
	if (cepb){
		if (!(gvar.tr_sector == TR_SEC_m19||gvar.tr_sector==TR_SEC_m20||gvar.tr_sector==TR_SEC_m21)){
			switch_flags_cepb_dis();
		}
	}
	
	switch(state){
		case 0:
			if (door_state == DOOR_OPENED){
				state = 1;
				seconds = 3*8*60;
			}
			break;
		case 1:
			if (gvar.tr_sensor < 1637){
				if (door_state == DOOR_CLOSED){
					state = 2;
				}
			}
			else {state = 0;}
			break;
		case 2:
			if (gvar.tr_sensor < 1637){
				if (door_state == DOOR_OPENED){
					state = 3;
				}
			}
			else {state = 0;}
			break;
		case 3:
			if (gvar.tr_sensor < 1637){
				if (door_state == DOOR_CLOSED){
					state = 4;
				}
			}
			else {state = 0;}
			break;
		case 4:
			if (gvar.tr_sensor < 1637){
				if (door_state == DOOR_OPENED){
					state = 5;
				}
			}
			else {state = 0;}
			break;
		case 5:
			if (gvar.tr_sensor < 1637){
				if (door_state == DOOR_CLOSED){
					state = 6;
				}
			}
			else {state = 0;}
			break;
		case 6:
			if (gvar.tr_sensor < 1637){
				if (door_state == DOOR_OPENED){
					state = 7;
				}
			}
			else {state = 0;}
			break;
		case 7:
			if (door_state == DOOR_OPENED){
				if (gvar.tr_sensor > 14731){
					state = 8;
				}
			}
			else {state = 0;}
			break;
		case 8:
			if (door_state == DOOR_OPENED){
				if (gvar.tr_sensor < 1637){
					state = 9;
				}
			}
			else {state = 0;}
			break;
		case 9:
			if (door_state == DOOR_OPENED){
				if (gvar.tr_sector == TR_SEC_m19||gvar.tr_sector==TR_SEC_m20||gvar.tr_sector==TR_SEC_m21){
					state = 0;
					EEPROM_write_byte(0,1);
					mk_defrost_start();
					cepb = 1;	
				}
			}
			else {state = 0;}
			break;
		default: break;
	}
	
	#ifdef		D_MODE
	gvar.d8 = state;
	gvar.d16 = seconds/8;
	#endif
}

