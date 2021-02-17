#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

#ifdef	__cplusplus
extern "C" {
#endif

sbit 		COMPR_PIN 	= P4^1;
sbit 		HEATER_PIN 	= P3^3;
sbit 		FAN_PIN 	= P4^2;
sbit		LED_PIN		= P1^5;
sbit		REED_PIN	= P1^4;

void pin_manager_init (void);
	
	
#ifdef	__cplusplus
}
#endif

#endif