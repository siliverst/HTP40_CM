/* 
 * File:   typedefs.h
 * Author: DeAd_MorOz
 *
 * Created on 20 Март 2014 г., 12:00
 */

#ifndef TYPEDEFS_H
#define	TYPEDEFS_H

#ifdef	__cplusplus
extern "C" {
#endif
	
#include "stdint.h"	

    typedef struct
    {
		uint8_t start_now;
    }sysf;

//===================================================================================================================
//======================                    Глобальные переменные                         ===========================
//===================================================================================================================
    typedef struct
    {
		float 		fc_temp;
		float 		eva_temp;
        uint16_t	fc_sensor;
		uint16_t	eva_sensor;
		uint16_t	tr_sensor;
		uint8_t 	tr_sector;
    }global;
//===================================================================================================================
#ifdef	__cplusplus
}
#endif

#endif	/* TYPEDEFS_H */

