#include <math.h>
#include "globalvar.h"
#include "NTC.h"

//---------------------------------------------------------------------------------------------------------------
// расчет температуры от значения резистора
// Оперируем значениями температуры после фильра
// - для пущей точности расчёта температуры ибо незачем двигать.
// функцию буду использовать напрямую, для одного преобразования за цикл и сохранения в озу.
//---------------------------------------------------------------------------------------------------------------
/*float fCalcTemp(float RRefVal, uint16_t BVal, uint16_t wPULLapVAL, uint16_t wADCVal)
{
    float fTemp,temp;
    temp = ((float)wPULLapVAL*(1024.0-(float)wADCVal)/(float)wADCVal)/(float)RRefVal;
    fTemp =(1.0/((log(temp)/(float)BVal)+(1.0/298.0)))-298.0;
    return(fTemp);
}*/

float fCalcTempAir(uint16_t wADCVal)
{
	float fTemp,temp;
	temp = ((float)AIR_PULPUP*((1024.0*16)-(float)wADCVal)/(float)wADCVal)/(float)AIR_RREF;
	fTemp =(1.0/((log(temp)/(float)AIR_BVALUE)+(1.0/298.0)))-298.0;
	return(fTemp);
}

/*float fCalcTempAmb(uint16_t wADCVal)
{
	float fTemp,temp;
	temp = ((float)AMB_PULPUP*((1024.0*16)-(float)wADCVal)/(float)wADCVal)/(float)AMB_RREF;
	fTemp =(1.0/((log(temp)/(float)AMB_BVALUE)+(1.0/298.0)))-298.0;
	return(fTemp);
}
*/