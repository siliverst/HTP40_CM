/*
 * test_auto.c v1.0 
 */ 
#include <SH79F084B.h>
#include <intrins.h>
#include <math.h>
#include "globalvar.h"
#include "test_auto.h"
#include "measuring.h"
#include "switch_flags.h"
#include "pin_manager.h"
#include "service_timing.h"
#include "NTC.h"
#include "led_sf.h"
#include "fan.h"

#ifdef TEST_MODE
static uint8_t wait_counter, test_stage, test_error;
static void test_disp_error (void);
static void test_counters ( void );

void test ( void )
{
	
	while(1){
		wdt_reset();
		measure();
		test_counters();
	}
}

void test_enter (void)
{
	test_stage = 0;
}

static void test_disp_error (void)
{
	test_error = test_stage;
	test_stage = 255;
}

static void test_counters ( void )
{
	static uint8_t i = 0;
	uint8_t t;
	
	if (F_8Hz)
	{
		F_8Hz = 0;
 		if (wait_counter) 
 		{
 			wait_counter--;
			return;
 		}			
		switch(test_stage){
			case 0:
				wait_counter = 4;
				test_stage = 1;
				break;
			
			case 1:
				gvar.fc_temp = fCalcTempAir(gvar.fc_sensor);
				if((gvar.fc_temp < -19.7)&&(gvar.fc_temp > -21.7)){
					test_stage = 2;
				}else{
					test_disp_error ();
				}
				
				break;
				
			case 2:
				gvar.eva_temp = fCalcTempAir(gvar.eva_sensor);
				if((gvar.eva_temp < 0.7)&&(gvar.eva_temp > -1.3)){
					test_stage = 3;
				}else{
					test_disp_error ();
				}
				break;
				
			case 3:
				FAN_PIN = 0;
				for(t=0; t<11; t++){
					_nop_();				//12us
				}
/*				if (TACHO_PIN == 0){
					FAN_PIN = 1;
					for(t=0; t<11; t++){
						_nop_();				//12us
					}
					if (TACHO_PIN == 1){
						test_stage = 4;
					}else{
						test_disp_error();
					}
				}else{
					test_disp_error();
				}*/
				break;
				
			case 4:
				if (gvar.tr_sensor < 5*16){
					compressor = 1;
					heater = 0;
					LED_PIN = LED_OFF;
					test_stage = 5;
				}
				break;
			case 5:
				if (gvar.tr_sensor > (1023*16-5*16)){
					compressor = 0;
					heater = 1;
					LED_PIN = LED_OFF;
					test_stage = 6;
				}
				break;
			case 6:
				if((gvar.tr_sensor > (511*16-20*16))&&(gvar.tr_sensor < (511*16+20*16))){
					compressor = 0;
					heater = 0;
					LED_PIN = LED_ON;
					test_stage = 7;
				}
				break;
			case 255:
				led_flash (test_error);
				break;
			default:
				break;
		}
	}	
}
#endif
