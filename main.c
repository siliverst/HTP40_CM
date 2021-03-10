#include "CMS80F231x.h"
#include <intrins.h>
#include "pin_manager.h"

#include "globalvar.h"

#include "service_timing.h"
//#include "uart.h"
//#include "uart_proc.h"
#include "measuring.h"
#include "trim.h"
#include "switch_flags.h"
#include "led_sf.h"
#include "mk.h"
#include "fan.h"
//#include "test_auto.h"
#include "lamp.h"
#include "door_sensor.h"
#include "eeprom.h"

/*
ToDo
uart
wd
режим тестирования.
звук
*/

//v1.03 (2021.02.04)
// корректировка гистерезиса
//#define T_ON_HYSTERESIS			4.0 -> 2.5
//#define T_OFF_HYSTERESIS			2.0 -> 0.5

void main ()
{
	TA = 0xAA;
	TA = 0x55;
	CLKDIV = 0x00;		// инициализация сис клок, пока 24 МГц (установлена в конфиге)
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	cli();
	pin_manager_init();
	led_sf_init();
	measuring_init();
	trim_init();
	service_timing_init();
	door_sensor_init();
	lamp_init();
//	uart_init();
//	uart_grahp_init ();
	
	/*	wdt_reset();*/
	sei();							// глобальные прерывания разрешены
	
	
	while (flags.start_now)
	{
//		wdt_reset();
		measure();
	}
	
	switch_flags_init();
	
	while (gvar.tr_sector == 0xFF){
		tirm_8hz_proc();
	}
	
	//#ifdef TEST_MODE
	//test_enter();
	//test();
	//#endif
	EEPROM_DataInit();
	fan_init ();
	mk_init ();

	while(1){
		measure();
		service_timing_proc();
		switch_flags_proc();
		mk_proc();
		mk_defrost_proc();
		door_sensor_proc();
		lamp_proc();
		fan_proc();
		//wdt_reset();
	}
	
}
