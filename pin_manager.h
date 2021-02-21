#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

#ifdef	__cplusplus
extern "C" {
#endif

sbit		LED_PIN		= P0^2;
sbit 		COMPR_PIN 	= P2^6;
sbit 		FAN_PIN 	= P3^1;
sbit 		HEATER_PIN 	= P3^2;
sbit		REED_PIN	= P0^3;
	
void pin_manager_init (void);
	
	
#ifdef	__cplusplus
}
#endif

#endif