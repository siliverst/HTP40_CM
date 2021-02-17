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