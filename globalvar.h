/* 
 * File:   globalvar.h
 * Author: DeAd_MorOz
 *
 * Created on 20 Март 2020 г., 12:00
 */

#ifndef GLOBALVAR_H
#define	GLOBALVAR_H

#ifdef	__cplusplus
extern "C" {
#endif
	
#include "typedefs.h"

// Версия FW
#define	FW_VERSION          205

//#define TEST_MODE	
//#define D_MODE



#define bin(a) ((( (a/10000000*128) + \
(((a/1000000)&1)*64) + \
(((a/100000)&1)*32) + \
(((a/10000)&1)*16) + \
(((a/1000)&1)*8) + \
(((a/100)&1)*4) + \
(((a/10)&1)*2) + \
(a&1)) * (a/10000000)) + \
(( ((a/262144)*64) + \
(((a/32768)&1)*32) + \
(((a/4096)&1)*16) + \
(((a/512)&1)*8) + \
(((a/64)&1)*4) + \
(((a/8)&1)*2) + \
(a&1)) * (1-(a/10000000))))


#define _BV(bitno) (1 << (bitno))
#define bitset(var, bitno)    ((var) |= _BV(bitno))
#define bitclr(var, bitno)    ((var) &= ~_BV(bitno))
#define bitchk(var, bitno)    ((var) & _BV(bitno))
#define bitmaskset(var, bitmask)	(var |= bitmask)
#define bitmaskclr(var, bitmask)	(var &= ~bitmask)
#define bitmaskchk(var, bitmask)	(var & bitmask)

#define	hibyte(v1)		((uint8_t)((v1)>>8))
#define	lobyte(v1)		((uint8_t)((v1)&0xff))

//===========================================================================================
//     CPU defs
//===========================================================================================
//wdt period is 1024
//#define wdt_reset()		RSTSTAT=0x01
#define cli()			EA=0
#define sei()			EA=1

//===========================================================================================
extern bit F_8Hz;
extern bit compressor;
extern bit heater;
extern bit fan;
extern volatile bit ext_int_dis;
extern bit sf;
extern bit err_fc_sensor;
extern bit err_eva_sensor;
extern bit err_eva_overheat;
extern bit door_state;
extern bit cepb;
extern bit test_dis;


extern sysf flags;					// системные флаги
extern global gvar;					// глобальные переменные

//double round(double x);

#ifdef	__cplusplus
}
#endif

#endif	/* GLOBALVAR_H */

