/*
 * measuring.h
 *
 * Created: 25.09.2012 11:39:27
 *  Author: DeAd_MorOz
 */ 


#ifndef MEASURING_H_
#define MEASURING_H_

// референс 5V
// Адреса каналов

#define ADC_CHAN_FC			0x06
#define ADC_CHAN_EVA		0x07
#define ADC_CHAN_TRIM		0x02

//биты АЦП
#define GO_DONE				0
#define ADCIF				6



void measuring_init ( void );
void reset_mains_int_wd (void);
void measure(void);


#endif /* MEASURING_H_ */