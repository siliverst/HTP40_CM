#include "CMS80F231x.h"
#include <intrins.h>
#include "globalvar.h"
#include "service_timing.h"
#include "uart.h"
#include "measuring.h"
#include "uart_proc.h"
#include "trim.h"
#include "switch_flags.h"
#include "led_sf.h"
#include "mk.h"
#include "lamp.h"
#include "fan.h"
#include "door_sensor.h"
#include "sound.h"

static uint8_t data counter_8Hz;
static uint8_t data counter_15ms;
#ifdef SOUND_ON
static uint8_t data tfor05hz;
#endif
static void service_timing_counters ( void );

//------------------------------------------------------------------------------
// Инициализация системного прерывания 1000Гц
//------------------------------------------------------------------------------
void service_timing_init (void) 
{
	bitclr(T34MOD,TR4);		// останавливаю таймер
	bitclr(T34MOD,T4M);		// делитель SYSclk/12
	bitclr(T34MOD,T4M1);	// включить MODE1 таймер 4
	bitset(T34MOD,T4M0);	// включить MODE1 таймер 4
	TL4 = 0x30;				// в регистр времени гружу 0xF830 = 65536 - 2000 = (63536)
	TH4 = 0xF8;				// 2000 = 24000000/12/1000
	
	bitclr(EIF2,EF4);		// стираю флаг прерывания
	bitset(EIE2,ET4);		// разрешаю прерывания от таймера
	bitset(T34MOD,TR4);		// запускаю таймер

	F_8Hz = 0;
	counter_8Hz = 0;
	#ifdef SOUND_ON
	tfor05hz = 0;
	#endif
	test_dis = 0;
	
}


static void service_timing_counters ( void )
{
	#ifndef D_MODE
/*	static uint16_t uart_on_counter = (8*60*3);	//3 минуты разрешаю заходить в тестмод
	if(uart_on_counter){
		uart_on_counter--;
		if(!uart_on_counter){
			test_dis = 1;
			uart_deinit();
		}
	}*/
	#endif
}

void service_timing_proc ( void )
{
	static uint8_t quere8hz;
	
	if (F_8Hz){
		quere8hz++;
		switch(quere8hz){
			case 1: switch_flags_fc_sensor_proc();break;
			case 2: switch_flags_eva_sensor_proc();break;
			case 3: tirm_8hz_proc();break;
			case 4: mk_8hz_proc_en();break;
			case 5: led_sf_8hz_proc();break;
			case 6: lamp_8Hz_proc();break;
			case 7: fan_8Hz_proc();break;
			case 8: switch_flags_cepb_proc();break;
			case 9: service_timing_counters();break;
			#ifdef SOUND_ON
			case 10: sound_8Hz_proc();break;
			#endif
			case 11: /*uart_proc();*/
			F_8Hz = 0; quere8hz = 0;break;
			default: break;
		}
	}
}


//------------------------------------------------------------------------------
// Системное прерывание 1000Гц для работы индикции и отсчёта времён (1ms)
//------------------------------------------------------------------------------
void service_timing_proc_int (void)	interrupt 16
{
	TL4 = 0x30;				// в регистр времени гружу 0xF830 = 65536 - 2000 = (63536)
	TH4 = 0xF8;				// 2000 = 24000000/12/1000
	
    if (++counter_8Hz >= PERIOD_8HZ){
        counter_8Hz = 0;
        F_8Hz = 1;
		#ifdef SOUND_ON
		if (++tfor05hz >= 16) {
			tfor05hz = 0;						//прошла два секунда	
		}
		#endif
    }
	
	if (++counter_15ms >= PERIOD_15MS){
        counter_15ms = 0;
		reset_mains_int_wd ();
		door_sensor_66hz_proc();
    }
	lamp_1ms_proc();

	//uart_rx_timeout_1ms_proc();
}

#ifdef SOUND_ON
uint8_t timer05hz_get (void)
{
	return tfor05hz;
}
#endif