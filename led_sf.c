#include "CMS80F231x.h"
#include <stdint.h>
#include "globalvar.h"
#include "led_sf.h"
#include "pin_manager.h"
#include "trim.h"

//---------------------------------------------------------------------------------------------------------------
// инициализация индикатора
//---------------------------------------------------------------------------------------------------------------
// видно ли за закрытой дверью ручку и лампочку?????????????

void led_sf_init (void)
{
	LED_PIN = LED_OFF;
}

void led_sf_8hz_proc ( void )
{
	static uint8_t counter = 0;
	
	if (err_fc_sensor){
		led_flash (2);	
	}
	else if (err_eva_sensor){
		led_flash (3);	
	}
	else if (err_eva_overheat){
		led_flash (4);	
	}
	else if (cepb){
		if (++counter >= 2)
		{
			counter = 0;
			if (LED_PIN){
				LED_PIN = LED_OFF;
			}else{
				LED_PIN = LED_ON;
			}
		}
	}
	else if (sf){
		LED_PIN = LED_ON;
	}else if (gvar.tr_sector == TR_SEC_SF){
		if (++counter >=4){
			counter = 0;
			if (LED_PIN){
				LED_PIN = LED_OFF;
			}else{
				LED_PIN = LED_ON;
			}
		}
	}else{
		LED_PIN = LED_OFF;
	}
}

void led_flash (uint8_t times)
{
	static uint8_t counter = 0;
	static uint8_t i = 0;

	if (counter) {
		counter--;
	}
	else{
		if (i)
		{
			if (LED_PIN == LED_ON){
				LED_PIN = LED_OFF;
				i--;
			}else{
				LED_PIN = LED_ON;
			}
			
			if (i) {
				counter=2;
			}else{
				counter=18;
			}
		}
		else{
			LED_PIN = LED_OFF;
			i = times;
		}
	}
}