/*
 * Lamp.h
 *
 * Created: 25.07.2013 16:53:14
 *  Author: DeAd_MorOz
 */ 


#ifndef LED_SF_H_
#define LED_SF_H_



void led_sf_init (void);
void led_sf_8hz_proc ( void );
void led_flash (uint8_t times);

enum led_states {
	LED_ON,
	LED_OFF
};


#endif /* LED_SF_H_ */