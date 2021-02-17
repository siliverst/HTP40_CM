#ifndef MK_H_
#define MK_H_

#define T_ON_HYSTERESIS				2.5
#define T_OFF_HYSTERESIS			0.5
#define SX_TIME						(24*60)
#define ERROR_TIME					(20*60)
// 12 моточасов до оттайки
#define TIME_TO_DEFROST					(12*60)
// 6 минут пассивной оттайки
#define TIME_PASSIVE_DEFROST			420
#define T_EVA_FOR_START_DEFROST			-8.0
#define T_EVA_FOR_STOP_DEFROST			11.0


// порог повышения температуры в камере при отключенной электроэнергии после которого
// (при включении аппарата в сеть) компрессор включается с минимальной задержкой.
#define T_ON_MK_START_TEMP						3.0
// 5 минут задержки компрессора после остановки/запуска
#define TIME_DISABLE_COMPR						(5*60)
// 5 секунд задержки компрессора при старте в случае 
// если температура сильно изменилась относительно уставки
#define TIME_DISABLE_COMPR_ON_START_SHORT		5
// 40 минут,ограничение времени работы нагревателя
#define TIME_HEATER_WORK						(40*60)
//---------------------------------------------------------------------------------------------------------------
// константы для работы оттайки
//---------------------------------------------------------------------------------------------------------------
enum defrost_statuses
{
	DEFROST_DISABLE,
	DEFROST_WAIT_DECREASE_EVA_TEMP,
	DEFROST_PRE_ON_COMPRESSOR,
	DEFROST_WAIT_COMPRESSOR_OFF,
	DEFROST_COMPRESSOR_FAN_DAMPER_OFF,
	DEFROST_HEATERS_ON,
	DEFROST_WAIT_EVA_TEMP_STATE,
	DEFROST_HEATERS_OFF,
	DEFROST_COMPRESSOR_ON
};


enum compressor_states{
	COMPR_OFF,
	COMPR_ON
};


void mk_init (void);
void mk_proc (void);
void mk_8hz_proc_en ( void );
void mk_defrost_proc (void);
void mk_defrost_start ( void );
void mk_defrost_init ( void );
uint8_t mk_get_defrost_status ( void );
#endif /* MK_H_ */