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
#define	FW_VERSION          206

//#define TEST_MODE	
//#define D_MODE
#define SOUND_ON


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
/*----------------------------------------------------------------------------
 **WDT делители
/*---------------------------------------------------------------------------*/
#define WDT_CLK_131072 			(0x00)		/*溢出时间= Tsys * 131072  (2^17 * Tsys)*/
#define WDT_CLK_262144 			(0x01)		/*溢出时间= Tsys * 262144  (2^18 * Tsys)*/
#define WDT_CLK_524288 			(0x02)		/*溢出时间= Tsys * 524288  (2^19 * Tsys)*/
#define WDT_CLK_1048576			(0x03)		/*溢出时间= Tsys * 1048576  (2^20 * Tsys)*/
#define WDT_CLK_2097152 		(0x04)		/*溢出时间= Tsys * 2097152  (2^21 * Tsys)*/
#define	WDT_CLK_4194304			(0x05)		/*溢出时间= Tsys * 4194304  (2^22 * Tsys)*/
#define	WDT_CLK_16777216		(0x06)		/*溢出时间= Tsys * 16777216 (2^24 * Tsys)*/
#define	WDT_CLK_67108864		(0x07)		/*溢出时间= Tsys * 67108864 (2^26 * Tsys)*/

/*-----------------------------------------------------------------
**WDT
------------------------------------------------------------------*/
/*------WDCON-----------------------------------------------------*/
#define WDT_WDCON_SWRST_Pos			(7)
#define WDT_WDCON_SWRST_Msk			(0x1 << WDT_WDCON_SWRST_Pos)
#define WDT_WDCON_PORF_Pos			(6)
#define WDT_WDCON_PORF_Msk			(0x1 << WDT_WDCON_PORF_Pos)
#define WDT_WDCON_WDTIF_Pos			(3)
#define WDT_WDCON_WDTIF_Msk			(0x1 << WDT_WDCON_WDTIF_Pos)
#define WDT_WDCON_WDTRF_Pos			(2)
#define WDT_WDCON_WDTRF_Msk			(0x1 << WDT_WDCON_WDTRF_Pos)
#define WDT_WDCON_WDTRE_Pos			(1)
#define WDT_WDCON_WDTRE_Msk			(0x1 << WDT_WDCON_WDTRE_Pos)
#define WDT_WDCON_WDTCLR_Pos		(0)
#define WDT_WDCON_WDTCLR_Msk		(0x1 << WDT_WDCON_WDTCLR_Pos)

/*------CKCON-----------------------------------------------------*/
#define TMR_CKCON_WTSn_Pos				(5)
#define TMR_CKCON_WTSn_Msk				(0x7 << TMR_CKCON_WTSn_Pos)
#define TMR_CKCON_T1M_Pos				(4)
#define TMR_CKCON_T1M_Msk				(0x1 << TMR_CKCON_T1M_Pos)
#define TMR_CKCON_T0M_Pos				(3)
#define TMR_CKCON_T0M_Msk				(0x1 << TMR_CKCON_T0M_Pos)

void wdt_init (uint8_t div);
void wdt_reset(void);
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

