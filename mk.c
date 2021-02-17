#include "globalvar.h"
#include "mk.h"
#include "trim.h"
#include "switch_flags.h"


static bit proc_8Hz_en;
static uint8_t	second = 0;
static uint8_t	minute = 0;

static uint8_t	defrost_status;

static uint16_t disable_compressor_on_counter;
static uint16_t defrost_heater_work_all_time_counter;
static uint16_t mk_next_defrost;
static uint16_t mk_sx_counter;
static uint16_t compressor_run_time;

static void switch_compr (uint8_t mode);
static float calc_on_temp ( void );
static float calc_off_temp ( void );
static void mk_8hz_proc (void);
static void minute_proc (void);
static void second_proc (void);

static int8_t temp_table[] =
{
	-16,
	-17,
	-18,
	-19,
	-20,
	-21,
	-22,
	-23,
	-24,
	-24	/*уставка по умолчанию после SF*/
};

void mk_init (void)
{ 
	// если при перезапуске температура сильно не изменилась, то задержка запуска компрессора 5 минут если сильно то 5 секунд.
	if (gvar.fc_temp >= calc_on_temp()){
		disable_compressor_on_counter =  TIME_DISABLE_COMPR_ON_START_SHORT;
	}else{
		disable_compressor_on_counter =  TIME_DISABLE_COMPR;
	}
	compressor = 0;
	proc_8Hz_en = 0;
	mk_defrost_init();
	mk_sx_counter = SX_TIME;
	sf = 0;
	if(cepb){
		mk_defrost_start();
	}
}

// включение/выключение охлаждения камеры
static void switch_compr (uint8_t mode)
{
	if (mode == COMPR_ON)
	{
		if (!disable_compressor_on_counter)				// если разрешено включение
		{
			compressor = 1;	
		}
	}
	else
	{
 		compressor = 0;	
		disable_compressor_on_counter = TIME_DISABLE_COMPR;
	}
}

void mk_8hz_proc_en ( void )
{
	proc_8Hz_en = 1;
}

static void mk_8hz_proc (void)
{
	if (proc_8Hz_en == 1)
	{
		proc_8Hz_en = 0;
		if (++second >= 8){
			second = 0;
			second_proc();
			if (++minute >= 60)
			{
				minute = 0;
				minute_proc();
			}
		}
	}
}

static void minute_proc (void)
{
	if (sf){
		mk_sx_counter--;
		if (!mk_sx_counter){
			sf = 0;
			mk_sx_counter = SX_TIME;
		}
	}

	if ((compressor)&&(defrost_status == DEFROST_DISABLE))
	{
		if (compressor_run_time < 0xFFFF){
			compressor_run_time++;
		}
	}
	
	if (defrost_status == DEFROST_DISABLE){
		if (compressor_run_time >= mk_next_defrost){
			mk_defrost_start();
		}
	}	
}

static void second_proc (void)
{
	if (disable_compressor_on_counter) disable_compressor_on_counter--;
	if (defrost_heater_work_all_time_counter) defrost_heater_work_all_time_counter--;	
}

void mk_proc (void)
{
	mk_8hz_proc();

	if (err_eva_overheat||err_eva_sensor||err_fc_sensor){
		if (compressor){
			switch_compr(COMPR_OFF);
		}
	}
	else if (defrost_status > DEFROST_WAIT_DECREASE_EVA_TEMP)
	{
		if (compressor){
			switch_compr(COMPR_OFF);
		}
	}
	// если охлаждение в MК отключено:
	else if (!compressor)
	{
 		if (sf){
 			switch_compr(COMPR_ON);
 		}
		else 
		if (gvar.fc_temp >= calc_on_temp()){
			switch_compr(COMPR_ON);
		}
	} 
	else
	{
 		if (sf){
 			return;
 		}	

		if (gvar.fc_temp <= calc_off_temp()){
			switch_compr(COMPR_OFF);
		}
	}
}

static float calc_on_temp ( void )
{
	float t = (float)temp_table[gvar.tr_sector];
	return (t+T_ON_HYSTERESIS);					
}

static float calc_off_temp ( void )
{
	float t = (float)temp_table[gvar.tr_sector];
	return (t+T_OFF_HYSTERESIS);					
}

void mk_defrost_proc (void)
{
	if (err_eva_overheat||err_eva_sensor||err_fc_sensor){
		heater = 0;
		return;
	}
	switch(defrost_status)
	{
		case DEFROST_DISABLE:
			heater = 0;				// Если не оттайка, то незачем включать нагреватель.
			break;
			
		case DEFROST_WAIT_DECREASE_EVA_TEMP:
			if (gvar.eva_temp <= T_EVA_FOR_START_DEFROST){
				defrost_status = DEFROST_COMPRESSOR_FAN_DAMPER_OFF;
			}
			heater = 0;
			break;
			
		case DEFROST_COMPRESSOR_FAN_DAMPER_OFF:
			switch_compr(COMPR_OFF);			
			defrost_status = DEFROST_HEATERS_ON;
			break;
				
		case DEFROST_HEATERS_ON:
			heater = 1;
			defrost_heater_work_all_time_counter = TIME_HEATER_WORK;
			defrost_status = DEFROST_WAIT_EVA_TEMP_STATE;
			break;	
				
		case DEFROST_WAIT_EVA_TEMP_STATE:
			heater = 1;
			if (gvar.eva_temp >= T_EVA_FOR_STOP_DEFROST){
				
				defrost_status = DEFROST_HEATERS_OFF;
			}
			if (!defrost_heater_work_all_time_counter){
				defrost_status = DEFROST_HEATERS_OFF;
			}
			break;	
				
		case DEFROST_HEATERS_OFF:	
			heater = 0;
			if (disable_compressor_on_counter < TIME_PASSIVE_DEFROST) disable_compressor_on_counter = TIME_PASSIVE_DEFROST;	
			defrost_status = DEFROST_COMPRESSOR_ON;
			break;
				
		case DEFROST_COMPRESSOR_ON:
			if (!disable_compressor_on_counter)			// Жду до включения компрессора.
			{
				if (gvar.fc_temp > calc_off_temp()){	//включаю компрессор только в том случае, если температура в мк меньше температуры отключения
					compressor = 1;
				}
				mk_defrost_init();
				if (cepb){
					switch_flags_cepb_dis();					
				}
			}
			break;
						
		default: break;
	}
}


void mk_defrost_start ( void )
{
	defrost_status = DEFROST_WAIT_DECREASE_EVA_TEMP;
}


void mk_defrost_init ( void )
{
	heater = 0;
	mk_next_defrost = TIME_TO_DEFROST;
	defrost_status = DEFROST_DISABLE;
	compressor_run_time = 0;
}

uint8_t mk_get_defrost_status ( void )
{
	return defrost_status;
}

