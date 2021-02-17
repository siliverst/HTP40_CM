/* 
 * File:   ntc.h
 * Author: DeAd_MorOz
 *
 * Created on 26 Май 2019 г., 7:50
 */

#ifndef NTC_H
#define	NTC_H

#ifdef	__cplusplus
extern "C" {
#endif


//#define AMB_RREF	27680
//#define AMB_BVALUE	3958
//#define AMB_PULPUP	10000
	
	
// NTC SF103F от Sunfull Hanbec
#define AIR_RREF	32460
// NTC SF103F от Sunfull Hanbec
#define AIR_BVALUE	4497
//
#define AIR_PULPUP	33000

//float fCalcTemp(float RRefVal, uint16_t BVal, uint16_t wPULLapVAL, uint16_t wADCVal);
float fCalcTempAir(uint16_t wADCVal);
//float fCalcTempAmb(uint16_t wADCVal);	
#ifdef	__cplusplus
}
#endif

#endif	/* NTC_H */

