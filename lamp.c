/*
 * Lamp.c
 *
 * Created: 25.11.2019 16:52:56
 *  Author: DeAd_MorOz
 */ 
#include <SH79F084B.h>
#include <stdint.h>
#include <intrins.h>
#include <math.h>
#include "globalvar.h"
#include "lamp.h"
#include "door_sensor.h"

static bit proc_8Hz_en;
static bit lamp_fade_en;
static uint16_t lamp_safe_counter;
static uint8_t second, last_lamp_state;
static uint8_t lamp_to;
static uint8_t lamp_1ms_counter;
static uint16_t lamp_fade_current_interval;

static void lamp_fade_proc (void);
static void lamp_pwm_init ( void );
//---------------------------------------------------------------------------------------------------------------
// макрос получения скважности  PWM
//---------------------------------------------------------------------------------------------------------------
#define lamp_pwm_duty_get()		PWMD

//---------------------------------------------------------------------------------------------------------------
// макрос установки скважности  PWM
//---------------------------------------------------------------------------------------------------------------
#define lamp_pwm_duty_set(val) 	(PWMD = (uint8_t)(val))


//---------------------------------------------------------------------------------------------------------------
// инициализация лампы
//---------------------------------------------------------------------------------------------------------------
void lamp_init (void)
{
	lamp_pwm_init();
	lamp_safe_counter_reload();
	proc_8Hz_en = 0;
	lamp_fade_en = 0;
	second = 0;
	last_lamp_state = 0;
	lamp_fade_current_interval = 0;
	lamp_to = UNCHANGED;
	lamp_set(LOFF);
}

//---------------------------------------------------------------------------------------------------------------
// подсчёт делителя, для фэйда, вызывается в прерывании
//---------------------------------------------------------------------------------------------------------------
void lamp_1ms_proc (void)
{ 
	if (++lamp_1ms_counter >= LAMP_FADE_DELAY)
	{
		lamp_1ms_counter = 0;
		lamp_fade_en = 1;
	}
}

//---------------------------------------------------------------------------------------------------------------
// функция установки флага разрешения обсчёта времени 8Гц.
//---------------------------------------------------------------------------------------------------------------
void lamp_8Hz_proc ( void )
{
	proc_8Hz_en = 1;
}

//---------------------------------------------------------------------------------------------------------------
// обработчик лампы вызывается в мэйнлуп
//---------------------------------------------------------------------------------------------------------------
void lamp_proc (void)
{
	if (err_eva_overheat){
		lamp_set(LOFF);
		return;
	}
	
	lamp_fade_proc();
	
	if (door_state == DOOR_CLOSED){
		lamp_safe_counter_reload();
		lamp(GO_TO_OFF);
	}else{
		if (lamp_safe_counter){
			lamp(GO_TO_MAX);
		} else {
			lamp(GO_TO_OFF);
		}
	}
	
	if (proc_8Hz_en){
		proc_8Hz_en = 0;
		
		if (++second >= 8){
			second = 0;
			if (lamp_safe_counter) lamp_safe_counter--;
		}
	}
}

//---------------------------------------------------------------------------------------------------------------
// перезагрузка счётчика разрешённого горения лампы
//---------------------------------------------------------------------------------------------------------------
void lamp_safe_counter_reload ( void )
{
	lamp_safe_counter = LAMP_TIME_TO_STBY;
}

//---------------------------------------------------------------------------------------------------------------
// функция плавного перехода лампы во включённое или отключённое состояние
//---------------------------------------------------------------------------------------------------------------
void lamp ( uint8_t go)
{
	if (last_lamp_state != go){

		if (go == GO_TO_OFF){
			// это нужно для того, что бы в процессе перехода с зажигания лампы на потухание не было скачка яркости
			lamp_fade_current_interval = lamp_pwm_duty_get()*LAMP_FADE_OUT_CONST;	
		}else if (go == GO_TO_MAX){
			// это нужно для того, что бы в процессе перехода с потухания лампы на загорание не было скачка яркости
			if (lamp_fade_current_interval) lamp_fade_current_interval = sqrt(((uint32_t)lamp_pwm_duty_get()-1)*LAMP_FADE_IN_CONST);
			else lamp_fade_current_interval = LAMP_FADE_IN_START_INTERVAL;	// это нужно что бы с выключеннога состояния
																			// яркость наростала не с 0 
																			//а с какого-то стартового значения
		}

		last_lamp_state = go;
		lamp_to = go;
	}
}

//---------------------------------------------------------------------------------------------------------------
// функция резкого перехода лампы во включённое или отключённое состояние
//---------------------------------------------------------------------------------------------------------------
void lamp_set ( uint8_t state)
{
	if (state == LON){
		lamp_pwm_duty_set(LAMP_MAX_DUTY);
		lamp_fade_current_interval = LAMP_FADE_INTERVALS;
		lamp_to = UNCHANGED;
		last_lamp_state = UNCHANGED;
	}else{
		lamp_pwm_duty_set(LAMP_OFF_DUTY);
		lamp_fade_current_interval = 0;
		lamp_to = UNCHANGED;
		last_lamp_state = UNCHANGED;
	}
}

//---------------------------------------------------------------------------------------------------------------
// функция инициализации PWM
//---------------------------------------------------------------------------------------------------------------
static void lamp_pwm_init ( void )
{
	//init PWM	
	PWMP = 0xFF;
	//PWMD = 0x00;
	lamp_pwm_duty_set(LAMP_OFF_DUTY);
	PWMCON = bin(11110001);	// частота делится на 16, частота получается несколько выше 4кГц... но это, я думаю, ничего.
}

//---------------------------------------------------------------------------------------------------------------
// обработчик плавного перехода яркости лампы
//---------------------------------------------------------------------------------------------------------------
static void lamp_fade_proc (void)
{
	if (lamp_fade_en){
		lamp_fade_en = 0;
		if (lamp_to == GO_TO_MAX)
		{
			if (lamp_fade_current_interval < LAMP_FADE_INTERVALS){
				lamp_fade_current_interval++;
				lamp_pwm_duty_set((pow (lamp_fade_current_interval, 2)/LAMP_FADE_IN_CONST)-1);
			}else{
				lamp_to = UNCHANGED;
				lamp_fade_current_interval = LAMP_FADE_INTERVALS;
			}
		}
		else if (lamp_to == GO_TO_OFF)
		{
			if (lamp_fade_current_interval > 0){
				--lamp_fade_current_interval;
				lamp_pwm_duty_set(lamp_fade_current_interval/LAMP_FADE_OUT_CONST);
			}else{
				lamp_to = UNCHANGED;
				lamp_fade_current_interval = 0;
			}
		}
	}
}

