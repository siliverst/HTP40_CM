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
проверить как время SF засекается останавливается.
*/

/*
v2.05 (2021.03.25)
исправил управление счётчиком SX
v2.04 (2021.03.24)
время между оттайками 12часов.
изменнён алгоритм включения экстренной оттайки:
3.4.2.1 Принудительное включение режима оттайки должно осуществ-ляться следующим образом: 
А. при включенном холодильнике перевести ручку регулятора температуры МК в положение «ECO» (если ручка находится в поло-жении «ECO», то повернуть ручку в любое положение, затем возвра-тить в положение «ECO»);
Б. повернуть ручку в крайнее левое положение до упора (MIN), да-лее повернуть в крайнее правое положение до упора (режим «Замора-живание»), далее повторить действия по пункту Б ещё 2 раза.
В случае принудительного включения оттайки испарителя индикатор режима «Замораживание» должен мигать с частотой 2 Гц до завершения пассивной оттаки.
Включение принудительной оттайки испарителя должно осуществлять-ся, если  вышеуказанные операции по пунктам А и Б осуществлены в тече-ние не более одной минут.

3.4.2.4  Выход из принудительного включение оттайки испарителя происходит:
- выполнением операций по 3.4.2.1;
- после завершения оттайки
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
