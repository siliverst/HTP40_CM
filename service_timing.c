#include <SH79F084B.h>
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

static data uint8_t counter_8Hz;
static data uint8_t counter_15ms;
static void service_timing_counters ( void );

//------------------------------------------------------------------------------
// Инициализация системного прерывания 1000Гц
//------------------------------------------------------------------------------
void service_timing_init (void) 
{
	bitclr(TCON1,TCLKS0);		// тактирование таймер 0 от SYSclk
	bitset(TCON1,TCLKP0);		// тактирование таймер 0 от SYSclk делитель на 1
	bitclr(TCON1,TC0);			// отключить компаратор на таймере 0
			
	bitclr(TMOD,M01);			// включить MODE1 таймер 0
	bitset(TMOD,M00);			// включить MODE1 таймер 0
	TL0 = 0x28;		 			//в регистр времени гружу 0xBF28 = 0 - 16600 = (48936)
	TH0 = 0xBF;					//16600 = 16600000/1/1000
	TR0 = 1;					//запускаю таймер
	ET0	= 1; 					//Рарешаю прерывания от таймера
	F_8Hz = 0;
	counter_8Hz = 0;
	test_dis = 0;
}


static void service_timing_counters ( void )
{
	#ifndef D_MODE
	static uint16_t uart_on_counter = (8*60*3);	//3 минуты разрешаю заходить в тестмод
	if(uart_on_counter){
		uart_on_counter--;
		if(!uart_on_counter){
			test_dis = 1;
			uart_deinit();
		}
	}
	#endif
}

void service_timing_proc ( void )
{
	static uint8_t quere8hz;
	
	if (F_8Hz){
		if (++quere8hz >= 10){
			quere8hz = 0;
		}
		switch(quere8hz){
			case 0: switch_flags_fc_sensor_proc();break;
			case 1: switch_flags_eva_sensor_proc();break;
			case 2: tirm_8hz_proc();break;
			case 3: mk_8hz_proc_en();break;
			case 4: led_sf_8hz_proc();break;
			case 5: lamp_8Hz_proc();break;
			case 6: fan_8Hz_proc();break;
			case 7: switch_flags_cepb_proc();break;
			case 8: service_timing_counters();break;
			case 9: uart_proc();
			F_8Hz = 0; break;
			default: break;
		}
	}
}


//------------------------------------------------------------------------------
// Системное прерывание 1000Гц для работы индикции и отсчёта времён (1ms)
//------------------------------------------------------------------------------
void service_timing_proc_int (void)	interrupt 1
{
	TL0 = 0x28;
	TH0 = 0xBF;
	
    if (++counter_8Hz >= PERIOD_8HZ){
        counter_8Hz = 0;
        F_8Hz = 1;
    }
	
	if (++counter_15ms >= PERIOD_15MS){
        counter_15ms = 0;
		reset_mains_int_wd ();
		door_sensor_66hz_proc();
    }
	lamp_1ms_proc();

	uart_rx_timeout_1ms_proc();
}
