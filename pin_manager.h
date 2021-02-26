#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

#ifdef	__cplusplus
extern "C" {
#endif
sbit		TEST_PIN	= P2^3;	
sbit		LED_PIN		= P0^2;
sbit 		COMPR_PIN 	= P2^6;
sbit 		FAN_PIN 	= P3^1;
sbit 		HEATER_PIN 	= P3^2;
sbit		REED_PIN	= P0^3;
sbit 		BUZZER_PIN	= P0^5;
sbit 		LAMP_PIN	= P1^2;
sbit		FASE_PIN	= P3^0;	
// аналоговые входы	
sbit		TRIM_PIN		= P0^4;
sbit		FC_TEMP_PIN		= P0^1;	
sbit		EVA_TEMP_PIN	= P2^2;		

	
void pin_manager_init (void);
	
	
#ifdef	__cplusplus
}
#endif

#endif