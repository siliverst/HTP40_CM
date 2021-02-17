/*
 * Lamp.h
 *
 * Created: 25.07.2013 16:53:14
 *  Author: DeAd_MorOz
 */ 


#ifndef LAMP_H_
#define LAMP_H_

	
extern volatile uint8_t LampTo;

void lamp_init (void);
void lamp_proc (void);
void lamp_safe_counter_reload ( void );
void lamp_1ms_proc (void);
void lamp_8Hz_proc ( void );
void lamp ( uint8_t go);
void lamp_set ( uint8_t state);

// 15 минут время до отключения лампы при открытой двери
#define LAMP_TIME_TO_STBY				(15*60)

#define LAMP_FADE_DELAY					4
#define LAMP_FADE_INTERVALS				512
#define LAMP_MAX_DUTY					255
#define LAMP_OFF_DUTY					0
#define LAMP_FADE_IN_START_INTERVAL		70
#define LAMP_FADE_OUT_CONST				2
#define LAMP_FADE_IN_CONST				1023

enum lamp_states{
	LOFF,
	LON
};


enum lamp_go_states
{
	UNCHANGED,
	GO_TO_MAX,
	GO_TO_OFF
};












#endif /* LAMP_H_ */