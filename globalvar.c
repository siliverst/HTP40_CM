#include "CMS80F231x.h"
#include <math.h>
#include "globalvar.h"

bit F_8Hz;
bit compressor;
bit heater;
bit fan;
volatile bit ext_int_dis;
bit sf;
bit err_fc_sensor;
bit err_eva_sensor;
bit err_eva_overheat;
bit door_state;
bit cepb;
bit test_dis;
sysf flags;
//count counter;					// ошибки
global gvar;
//uint16_t    kkk;

//double round(double x)
//{
//    double truncated,roundedFraction;
//    double fraction = modf(x, &truncated);
//    modf(2.0*fraction, &roundedFraction);
//    return truncated + roundedFraction;
//}

/*
 ** 		  Fsys =16Mhz, 则     (0)WDT_CLK_131072  , 溢出时间= Tsys * 131072 = 8.192ms
 **								  (1)WDT_CLK_262144 , 溢出时间= Tsys * 262144 = 16.384ms
 **								  (2)WDT_CLK_524288 , 溢出时间= Tsys * 524288 = 32.768ms
 **								  (3)WDT_CLK_1048576 , 溢出时间= Tsys * 1048576 = 65.536ms
 **								  (4)WDT_CLK_2097152 , 溢出时间= Tsys * 2097152 = 131.072ms
 **								  (5)WDT_CLK_4194304 , 溢出时间= Tsys * 4194304 = 262.144ms
 **								  (6)WDT_CLK_16777216 , 溢出时间= Tsys * 16777216 = 1.048s 
 **								  (7)WDT_CLK_67108864 , 溢出时间= Tsys * 67108864 = 4.194s 
*/
void wdt_init (uint8_t div)
{
	// частота у меня сейчас 24МГц, следовательно 7 -> будет 2.796с.
	TA = 0xAA;
	TA = 0x55;
	WDCON = WDT_WDCON_WDTCLR_Msk;
	CKCON &= ~(TMR_CKCON_WTSn_Msk);
	CKCON |= (div << TMR_CKCON_WTSn_Pos);
	
	TA = 0xAA;
	TA = 0x55;
	WDCON |= WDT_WDCON_WDTCLR_Msk;
	
	TA = 0xAA;
	TA = 0x55;
	WDCON |= WDT_WDCON_WDTRE_Msk;
}

void wdt_reset(void)
{
	TA = 0xAA;
	TA = 0x55;
	WDCON |= WDT_WDCON_WDTCLR_Msk;
}