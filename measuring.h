/*
 * measuring.h
 *
 * Created: 25.09.2012 11:39:27
 *  Author: DeAd_MorOz
 */ 


#ifndef MEASURING_H_
#define MEASURING_H_

// референс 5V
#define ADC_REF_VCC			0x1C
// Адреса каналов

#define ADC_CHAN_FC			0x01
#define ADC_CHAN_EVA		0x08
#define ADC_CHAN_TRIM		0x04

//биты АЦП ADCON0
#define ADGO				1
#define ADCHS4				7

//биты EIF2
#define ADCIF				4

// биты EIE2
#define ADCIE				4




void measuring_init ( void );
void reset_mains_int_wd (void);
void measure(void);


#endif /* MEASURING_H_ */