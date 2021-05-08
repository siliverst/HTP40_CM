#include "CMS80F231x.h"
#include <intrins.h>
#include "globalvar.h"
#include "sound.h"
#include "service_timing.h"
#include "door_sensor.h"
#include "eeprom.h"
#ifdef SOUND_ON
static bit proc_8Hz_en;
static uint8_t last_sound_state, sound_state;
static uint8_t second;
static uint8_t counter_door_alarm_time;
static void sound_door_alarm_sec_proc ( void );
static void sound ( uint8_t state);

//---------------------------------------------------------------------------------------------------------------
// инициализация звука
//---------------------------------------------------------------------------------------------------------------
void sound_init ( void )
{
	BUZDIV = 47;
	BUZCON = bin(00000011);	//старший бит включает/отключает звук
	sound_state = 0;
	proc_8Hz_en = 0;	
	sound_door_alarm_reset();
}

void sound_8Hz_proc ( void )
{
	proc_8Hz_en = 1;
}

void sound_proc ( void )
{
	if (proc_8Hz_en){
		proc_8Hz_en = 0;

		if (last_sound_state != sound_state)
		{
			if (sound_state)							// если включаю звук
			{
				//if (timer05hz_get() == (f1Hz-1))		// то только когда полностью будет звучать первый отрезок если сигналы длинные???? ЯХЗ у меня звучит всё полностью...
				{
					last_sound_state = sound_state;
				}
			}
			else
			{
				last_sound_state = sound_state;
				sound(SOFF);
			}	
		}
	
		if (last_sound_state)							// звуковая сигнализация 1Hz
		{
			if ((timer05hz_get() == f1Hz)||(timer05hz_get() == (f05Hz|f1Hz)))
			{
				sound(SON);
			}
			if ((timer05hz_get() == 0)||(timer05hz_get() == f05Hz))
			{
				sound(SOFF);
			}
		}	
		if (++second >= 8){
			second = 0;
			sound_door_alarm_sec_proc();
		}
	}
	if (door_state == DOOR_CLOSED){
		sound_door_alarm_reset();
	}
}

static void sound ( uint8_t state)
{
	if (state == SON){
		bitset(BUZCON,7);	//старший бит включает/отключает звук)
	}else{
		bitclr(BUZCON,7);	//старший бит включает/отключает звук)
	}
}

static void sound_door_alarm_sec_proc ( void )
{
	// подсчет времени до звуковой сигнализации открытой двери
	if (door_state == DOOR_OPENED){
		if (counter_door_alarm_time){
			counter_door_alarm_time--;
			if (!counter_door_alarm_time){
				bitset(sound_state,SOUND_ALARM_XK);
			}
		}
	}
}

void sound_door_alarm_reset ( void )
{
	counter_door_alarm_time = SOUND_TIME_TO_ALARM;
	bitclr(sound_state,SOUND_ALARM_XK);
}
#endif