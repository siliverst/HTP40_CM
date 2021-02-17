/*
 * measuring.c
 *
 * Created: 25.08.2020 11:39:10
 *  Author: DeAd_MorOz
 */ 
#include <SH79F084B.h>
#include <intrins.h>
#include "globalvar.h"
#include "measuring.h"
#include "pin_manager.h"

static data uint16_t	local_fc_sensor,local_eva_sensor,local_tr_sensor;
static uint16_t GetADC(uint8_t mux);
static data volatile uint8_t without_fase_detector;
static data volatile uint8_t index_m;

//------------------------------------------------------------------------------
// Типа INLINE функция для управления симисторами
//------------------------------------------------------------------------------
#define triacs_zerocrosing_control()	\
	if(compressor) {COMPR_PIN = 0;}		\
	else {COMPR_PIN =1;}				\
	if (heater) {HEATER_PIN = 0;}		\
	else {HEATER_PIN = 1;}				\
	if (fan) {FAN_PIN = 0;}				\
	else {FAN_PIN = 1;}					

//Инициализация измерительного модуля
void measuring_init ( void )
{	
	uint8_t k;
	// инициализация входов АЦП

	ADCH = ((1<<ADC_CHAN_TRIM)|(1<<ADC_CHAN_EVA)|(1<<ADC_CHAN_FC));			// AN2, AN3, AN4 - АНАЛОГОВЫЕ ВХОДЫ

	// начальная инициализация датчиков температуры
	for (k=0;k<4;k++)
  	{
		gvar.fc_sensor	= GetADC(ADC_CHAN_FC) << 4;
		gvar.eva_sensor 	= GetADC(ADC_CHAN_EVA) << 4;
		gvar.tr_sensor	= GetADC(ADC_CHAN_TRIM) << 4;
	}
	
	// инициализация циклических измерений ADC
	ADCON = bin(10000000)+(ADC_CHAN_TRIM << 1);	// ADON + канал
	ADT = 	bin(11001111);	// 
	for(k=0; k<11; k++){
		_nop_();				//12us
	}

	index_m = 0;
	bitclr(ADCON, ADCIF);
	EADC = 1;					// включаю прерывания от ADC.
	
	// инициализация прерываний фронта сети.
	IT0 = 1;					// работаю по спадающему фронту
	EX0 = 1;					// разрешаю прерывание от сети
	without_fase_detector = 0;	// должно быть 0	
	ext_int_dis = 0;
	
	flags.start_now = 11;		// начальная инициализация регистров 
}

//---------------------------------------------------------------------------------------------------------------
// прерывание АЦП
//---------------------------------------------------------------------------------------------------------------
void adc_interrupt (void) interrupt 6
{	
	uint16_t u;
 	u = ADDH;	// сохраняю значение АЦП во временный регистр.
	u <<=2;
	u+=ADDL;
	bitclr(ADCON, ADCIF);
	
	if (index_m == 0){
		ADCON = bin(10000000)+(ADC_CHAN_FC << 1);
		local_tr_sensor = u;
		bitset(ADCON, GO_DONE);
		
	}else if (index_m == 1){
		ADCON = bin(10000000)+(ADC_CHAN_EVA << 1);
		local_fc_sensor = u;
		bitset(ADCON, GO_DONE);

	}else if (index_m == 2){
		ADCON = bin(10000000)+(ADC_CHAN_TRIM << 1);
		local_eva_sensor = u;
	}
	index_m++;
}

//---------------------------------------------------------------------------------------------------------------
// прерывание по переднему фронту синусоиды запуск текущего периода замера
//---------------------------------------------------------------------------------------------------------------
void zero_crosing_interrupt (void) interrupt 0
{	
	if (ext_int_dis){
		ext_int_dis = 0;
	}else{
		triacs_zerocrosing_control();
	}
	without_fase_detector = 0;	// сброс сторожевого регистра прерывания
}

//---------------------------------------------------------------------------------------------------------------
// функция для запуска преобразования АЦП, в случае, если прерывания от сети не поступают. Тактировать от 15мс.
//---------------------------------------------------------------------------------------------------------------
void reset_mains_int_wd (void)
{
	index_m = 0;			// запуск ADC преобразования в скользящем режиме относительно частоты сети.
	bitset(ADCON, GO_DONE); //ADC GO
	#ifdef D_MODE
	if (without_fase_detector < 5) {
		without_fase_detector++; 
	}
	else {
		triacs_zerocrosing_control();
	}
	#endif
}

//---------------------------------------------------------------------------------------------------------------
// функция обработки измеренных значений и фильтры.
//---------------------------------------------------------------------------------------------------------------
void measure(void)
{ 
	if(index_m == 3)
	{
		index_m = 4;

		gvar.fc_sensor = gvar.fc_sensor - (gvar.fc_sensor >> 4) + local_fc_sensor;
		gvar.eva_sensor = gvar.eva_sensor - (gvar.eva_sensor >> 4) + local_eva_sensor;
		gvar.tr_sensor = gvar.tr_sensor - (gvar.tr_sensor >> 4) + local_tr_sensor;
		if (flags.start_now) flags.start_now--;	// уменьшене регистра первичной инициализации 	
	} 
}

//---------------------------------------------------------------------------------------------------------------
// функция однократного измерения АЦП
//---------------------------------------------------------------------------------------------------------------
static uint16_t GetADC(uint8_t mux)
{
	uint8_t t;
	ADCON =  	bin(10000000)+(mux<<1);		// ADON + канал
	ADT = 		bin(11101111);	
	for(t=0; t<11; t++){
		_nop_();							//12us
	}
	bitset(ADCON, GO_DONE);
	while(bitchk(ADCON, GO_DONE));
	
	ADCON = 0;
	return (((uint16_t)ADDH << 2) + ADDL);
}