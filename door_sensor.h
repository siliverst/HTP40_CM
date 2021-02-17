/* 
 * File:   DoorSensor.h
 * Author: DeAd_MorOz
 *
 * Created on 5 Апрель 2014 г., 17:59
 */

#ifndef DOORSENSOR_H
#define	DOORSENSOR_H

#ifdef	__cplusplus
extern "C" {
#endif

#define DOOR_OPENED	1
#define DOOR_CLOSED	0

void door_sensor_init ( void );
void door_sensor_capture ( void );
void door_sensor_proc ( void );
void door_sensor_66hz_proc ( void );
	
	
	
#ifdef	__cplusplus
}
#endif

#endif	/* DOORSENSOR_H */

