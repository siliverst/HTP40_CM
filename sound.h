/*
 * Sound.h
 *
 * Created: 21.01.2019 12:43:18
 *  Author: DeAd_MorOz
 */ 


#ifndef SOUND_H_
#define SOUND_H_

#ifdef SOUND_ON

#define BEEP_COUNT_TO_10MS 10
#define BEEP_1	8
#define BEEP_2s	200

#define SOUND_ALARM_XK	0
#define SOUND_ERR_XK	1
#define SOUND_ERR_MK	2
#define SOUND_ERR_EVA	3
#define SOUND_ERR_HEAT	4
#define SOUND_OUT_TEMP_XK		5
#define SOUND_HI_MK		6

//40 секунд счётчик до сигнала
#define SOUND_TIME_TO_ALARM		40
#define SOUND_TIME_TO_ALARM_2	120

// раскоментировать для прерывистого сигнала
//#define WATER_DISSCONTINUOUS_SOUND

enum on_off_sound {
	SOFF,
	SON
};
	


void sound_init ( void );
void sound_8Hz_proc ( void );
void sound_door_alarm_reset ( void );
void sound_proc ( void );
#endif
#endif /* SOUND_H_ */