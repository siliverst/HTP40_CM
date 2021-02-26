/*
 * Lamp.c
 *
 * Created: 25.11.2019 16:52:56
 *  Author: DeAd_MorOz
 */ 
#include "CMS80F231x.h"
#include "stdint.h"
#include <intrins.h>
#include <math.h>
#include "globalvar.h"
#include "lamp.h"
#include "door_sensor.h"

static bit proc_8Hz_en;
static bit lamp_fade_en;
static uint16_t lamp_safe_counter;
static uint8_t second, last_lamp_state;
static uint8_t lamp_to;
static uint8_t lamp_1ms_counter;
static uint16_t lamp_fade_current_interval;
static void lamp_pwm_duty_set(uint16_t val);
static void lamp_fade_proc (void);
static void lamp_pwm_init ( void );
//---------------------------------------------------------------------------------------------------------------
// макрос получения скважности  PWM
//---------------------------------------------------------------------------------------------------------------
#define lamp_pwm_duty_get()		(((uint16_t)PWMD0H<<8) + (uint16_t)PWMD0L)

//---------------------------------------------------------------------------------------------------------------
// макрос установки скважности  PWM (8bit)
//---------------------------------------------------------------------------------------------------------------
static void lamp_pwm_duty_set(uint16_t val)
{
	PWMD0H=hibyte(val);
	PWMD0L=lobyte(val);
	bitset(PWMLOADEN,0);
}

//---------------------------------------------------------------------------------------------------------------
// инициализация лампы
//---------------------------------------------------------------------------------------------------------------
void lamp_init (void)
{
	lamp_pwm_init();
	lamp_safe_counter_reload();
	proc_8Hz_en = 0;
	lamp_fade_en = 0;
	second = 0;
	last_lamp_state = 0;
	lamp_fade_current_interval = 0;
	lamp_to = UNCHANGED;
	lamp_set(LOFF);
}

//---------------------------------------------------------------------------------------------------------------
// подсчёт делителя, для фэйда, вызывается в прерывании
//---------------------------------------------------------------------------------------------------------------
void lamp_1ms_proc (void)
{ 
	if (++lamp_1ms_counter >= LAMP_FADE_DELAY)
	{
		lamp_1ms_counter = 0;
		lamp_fade_en = 1;
	}
}

//---------------------------------------------------------------------------------------------------------------
// функция установки флага разрешения обсчёта времени 8Гц.
//---------------------------------------------------------------------------------------------------------------
void lamp_8Hz_proc ( void )
{
	proc_8Hz_en = 1;
}

//---------------------------------------------------------------------------------------------------------------
// обработчик лампы вызывается в мэйнлуп
//---------------------------------------------------------------------------------------------------------------
void lamp_proc (void)
{
	if (err_eva_overheat){
		lamp_set(LOFF);
		return;
	}
	
	lamp_fade_proc();
	
	if (door_state == DOOR_CLOSED){
		lamp_safe_counter_reload();
		lamp(GO_TO_OFF);
	}else{
		if (lamp_safe_counter){
			lamp(GO_TO_MAX);
		} else {
			lamp(GO_TO_OFF);
		}
	}
	
	if (proc_8Hz_en){
		proc_8Hz_en = 0;
		
		if (++second >= 8){
			second = 0;
			if (lamp_safe_counter) lamp_safe_counter--;
		}
	}
}

//---------------------------------------------------------------------------------------------------------------
// перезагрузка счётчика разрешённого горения лампы
//---------------------------------------------------------------------------------------------------------------
void lamp_safe_counter_reload ( void )
{
	lamp_safe_counter = LAMP_TIME_TO_STBY;
}

//---------------------------------------------------------------------------------------------------------------
// функция плавного перехода лампы во включённое или отключённое состояние
//---------------------------------------------------------------------------------------------------------------
void lamp ( uint8_t go)
{
	if (last_lamp_state != go){

		if (go == GO_TO_OFF){
			// это нужно для того, что бы в процессе перехода с зажигания лампы на потухание не было скачка яркости
			//lamp_fade_current_interval = lamp_pwm_duty_get()*LAMP_FADE_OUT_CONST;	//8bit
			lamp_fade_current_interval = lamp_pwm_duty_get()/LAMP_FADE_OUT_CONST;	//10bit
		}else if (go == GO_TO_MAX){
			// это нужно для того, что бы в процессе перехода с потухания лампы на загорание не было скачка яркости
			if (lamp_fade_current_interval) lamp_fade_current_interval = sqrt(((uint32_t)lamp_pwm_duty_get()-1)*LAMP_FADE_IN_CONST);
			else lamp_fade_current_interval = LAMP_FADE_IN_START_INTERVAL;	// это нужно что бы с выключеннога состояния
																			// яркость наростала не с 0 
																			//а с какого-то стартового значения
		}

		last_lamp_state = go;
		lamp_to = go;
	}
}

//---------------------------------------------------------------------------------------------------------------
// функция резкого перехода лампы во включённое или отключённое состояние
//---------------------------------------------------------------------------------------------------------------
void lamp_set ( uint8_t state)
{
	if (state == LON){
		lamp_pwm_duty_set(LAMP_MAX_DUTY);
		lamp_fade_current_interval = LAMP_FADE_INTERVALS;
		lamp_to = UNCHANGED;
		last_lamp_state = UNCHANGED;
	}else{
		lamp_pwm_duty_set(LAMP_OFF_DUTY);
		lamp_fade_current_interval = 0;
		lamp_to = UNCHANGED;
		last_lamp_state = UNCHANGED;
	}
}

//---------------------------------------------------------------------------------------------------------------
// функция инициализации PWM
//---------------------------------------------------------------------------------------------------------------
static void lamp_pwm_init ( void )
{
	/*
	//8bit
	//init PWM	
	PWMCON = bin(00000000);
	PWM01PSC = 45; 				// делитель будет равен PWM01PSC+1, т.е. 46
	PWM0DIV = bin(00000100);	// частота канала ноль не делится относительно PSC
	PWMP0H = 0;
	PWMP0L = 0xFF;
	bitset(PWMLOADEN,0);		// разрешаю загрузку периода и скважности для канала 0
	lamp_pwm_duty_set(LAMP_OFF_DUTY);
	bitset(PWMCNTM,0); 			// разрешаю автозагрузку периода и скважности для канала 0
	bitset(PWMPINV,0);			// инвертирую выход 0
	bitset(PWMOE,0);			// включаю выход 0
	bitclr(PWMDTE,0);			// отклчаю мёртвое время между 0 и 1 выходом
	//bitclr(PWMZIE,0);			// отключаю прерывания при переходе через 0
	//bitclr(EIE2,PWMIE);		// отключаю прерывания от модуля PWM
	bitset(PWMCNTE,0);			// включаю таймер канала 0;
	*/

	//10bit
	//init PWM	
	PWMCON = bin(00000000);
	PWM01PSC = 11; 				// делитель будет равен PWM01PSC+1, т.е. 12
	PWM0DIV = bin(00000100);	// частота канала ноль не делится относительно PSC
	PWMP0H = 0x03;
	PWMP0L = 0xFF;
	bitset(PWMLOADEN,0);		// разрешаю загрузку периода и скважности для канала 0
	lamp_pwm_duty_set(LAMP_OFF_DUTY);
	bitset(PWMCNTM,0); 			// разрешаю автозагрузку периода и скважности для канала 0
	bitset(PWMPINV,0);			// инвертирую выход 0
	bitset(PWMOE,0);			// включаю выход 0
	bitclr(PWMDTE,0);			// отклчаю мёртвое время между 0 и 1 выходом
	//bitclr(PWMZIE,0);			// отключаю прерывания при переходе через 0
	//bitclr(EIE2,PWMIE);		// отключаю прерывания от модуля PWM
	bitset(PWMCNTE,0);			// включаю таймер канала 0;
	
}

//---------------------------------------------------------------------------------------------------------------
// обработчик плавного перехода яркости лампы
//---------------------------------------------------------------------------------------------------------------
static void lamp_fade_proc (void)
{
	if (lamp_fade_en){
		lamp_fade_en = 0;
		if (lamp_to == GO_TO_MAX)
		{
			if (lamp_fade_current_interval < LAMP_FADE_INTERVALS){
				lamp_fade_current_interval++;
				lamp_pwm_duty_set((pow (lamp_fade_current_interval, 2)/LAMP_FADE_IN_CONST)-1);
			}else{
				lamp_to = UNCHANGED;
				lamp_fade_current_interval = LAMP_FADE_INTERVALS;
			}
		}
		else if (lamp_to == GO_TO_OFF)
		{
			if (lamp_fade_current_interval > 0){
				--lamp_fade_current_interval;
				//lamp_pwm_duty_set(lamp_fade_current_interval/LAMP_FADE_OUT_CONST);	8bit
				lamp_pwm_duty_set(lamp_fade_current_interval*LAMP_FADE_OUT_CONST);	//10bit
			}else{
				lamp_to = UNCHANGED;
				lamp_fade_current_interval = 0;
			}
		}
	}
}

/*
14. Enhanced PWM module
14.1 Overview
The enhanced PWM module supports 6 PWM generators,
It can be configured as 6 independent PWM outputs (PG0-PG5),
It can also be configured as 3 pairs of complementary PWM (PG0-PG1, PG2-PG3, PG4-PG5) 
with programmed dead-band generators.
Each pair of PWM shares an 8-bit prescaler, and there are 6 sets of clock dividers, 
providing 5 frequency division coefficients (1, 1/2, 1/4, 1/8, 1/16).
Each PWM output is controlled by an independent 16-bit counter, 
and another 16-bit comparator is used to adjust the duty cycle.
The 6-channel PWM generator provides 24 interrupt flags. 
When the period or duty cycle of the relevant PWM channel matches the counter, 
an interrupt flag will be generated. Each PWM channel has a separate enable bit.
Each PWM can be configured in single mode (generating a PWM signal cycle) 
or cyclic mode (continuous output of PWM waveform).

14.2 Features
The enhanced PWM module has the following features:
⚫ 6 independent 16-bit PWM control modes.
-6 independent outputs: PG0, PG1, PG2, PG3, PG4, PG5;
-3 sets of complementary PWM pairs: (PG0-PG1), (PG2-PG3), (PG4-PG5), programmable dead time can be inserted;
-3 groups of synchronous PWM pairs: (PG0-PG1), (PG2-PG3), (PG4-PG5), each group of PWM pairs are pin synchronized.
⚫ Support group control, PG0, PG2, PG4 output synchronization, PG1, PG3, PG5 output synchronization.
⚫ Single mode or auto-load mode.
⚫ Support two modes: edge alignment and center alignment.
⚫ Center alignment mode supports symmetric counting and asymmetric counting.
⚫ In complementary PWM, it supports programmable dead zone generator.
⚫ Each PWM has independent polarity control.
⚫ Hardware brake protection (external FB trigger, support software trigger).
⚫ ADC comparison event can trigger hardware brake protection.
⚫ The PWM edge or cycle can trigger the start of AD conversion.

14.3 Port configuration
Before using the enhanced PWM module, you need to configure the corresponding port as a PWM channel 
(any GPIO can be configured as a PWM channel).
The channels are marked with PG0-PG5 on the multiplex function allocation diagram, 
corresponding to PWM channels 0-5 respectively.
The allocation of PWM channels is controlled by the corresponding port configuration registers, for example:
P13CFG=0x12;//Select P13 to configure as PG0 channel
P14CFG=0x13;//Select P14 to configure as PG1 channel
P15CFG=0x14; //Select P15 and configure it as PG2 channel
P16CFG=0x15;//Select P16 to configure as PG3 channel
P17CFG=0x16;//Select P17 to configure as PG4 channel
P22CFG=0x17;//Select P22 to configure as PG5 channel

14.4.3 Complementary mode and dead time delay Complementary mode and dead time delay
6-channel PWM can be set as 3 complementary pairs.
In complementary mode, the period and duty cycle of PWM1, PWM3, and PWM5 
are determined by PWM0, PWM2, PWM4 related registers, 
and the dead-time delay register can also affect the duty cycle of PWM complementary pair.
At this time, except for the corresponding output enable control bit (PWMnOE), 
the output waveform of PWM1/PWM3/PWM5 is no longer controlled by its own register.
In the complementary mode, each group of complementary PWM pairs supports the insertion of a dead time delay, 
and the inserted dead time is as follows:
PWM0/1 dead time: (PWM01DT+1)*T PWM0
PWM2/3 dead time: (PWM23DT+1)*T PWM2
PWM4/5 dead time: (PWM45DT+1)*T PWM4
T PWM0 /T PWM2 /T PWM4 are the clock source periods of PWM0/PWM2/PWM4 respectively.
Both center alignment and edge alignment support complementary modes.
14.4.4 Brake function Brake function
The signal sources that can trigger the PWM brake are as follows:
1. External trigger port FB;
2. ADC result comparison output;
After the brake is triggered, the brake flag is set to 1, the counter enable bits of all channels are cleared, 
and the PWM outputs the preset brake data.
To restore normal output, you need to clear the brake flag and re-enable the PWM channel counter.


PWM control register PWMCON
F120H Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
PWMCON -- -- PWMMODE1 PWMMODE0 GROUPEN ASYMEN CNTTYPE --
Bit7~ Bit6 -
Bit5~Bit4 PWMMODE: PWM mode control bits;
00 = independent mode;
01 = complementary mode;
10=Sync mode;
11= Reserved.
Bit3 GROUPEN: PWM group function enable bit;
1=PG0 controls PG2, PG4; PG1 controls PG3, PG5;
0 = All PWM channel signals are independent of each other.
Bit2 ASYMEN: Asymmetric counting enable bit in PWM center alignment mode;
1= Asymmetric counting is enabled;
0 = symmetric counting is enabled.
Bit1 CNTTYPE: PWM counting alignment selection bit;
1= Center alignment;
0= Edge alignment.
Bit0 -

PWM output enable control register PWMOE
F121H Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
PWNOE - - PWM5OE PWM4OE PWM3OE PWM2OE PWM1OE PWM0OE
Bit7~Bit6 -
Bit5 PWM5OE: PWM channel 5 output enable bit;
1= enable;
0= Disabled.
Bit4 PWM4OE: PWM channel 4 output enable bit;
1= enable;
0= Disabled.
Bit3 PWM3OE: PWM channel 3 output enable bit;
1= enable;
0= Disabled.
Bit2 PWM2OE: PWM channel 2 output enable bit;
1= enable;
0= Disabled.
Bit1 PWM1OE: PWM channel 1 output enable bit;
1= enable;
0= Disabled.
Bit0 PWM0OE: PWM channel 0 output enable bit;
1= enable;
0= Disabled.

PWM0/1 clock prescaler control register PWM01PSC
F123H Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
PWM01PSC PWM01PSC7 PWM01PSC6 PWM01PSC5 PWM01PSC4 PWM01PSC3 PWM01PSC2 PWM01PSC1 PWM01PSC0
Bit7~Bit0 PWM01PSC<7:0>: PWM channel 0/1 prescaler control bits;
00 = The prescaler clock stops, and the counter of PWM0/1 stops;
Others = (PWM01PSC+1) division of the system clock.

PWM clock divider control register PWMnDIV(n=0-5)
Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
PWMnDIV - - - - - PWMnDIV2 PWMnDIV1 PWMnDIV0
Register PWMnDIV (n=0-5) address: F12AH, F12BH, F12CH, F12DH, F12EH, F12FH.
Bit7~Bit3 -
Bit2~Bit0 PWMnDIV<2:0>: PWM channel n clock frequency division control bit;
000= Fpwmn-PSC/2;
001= Fpwmn-PSC/4;
010= Fpwmn-PSC/8;
011= Fpwmn-PSC/16;
100= Fpwmn-PSC;
Other = Fsys (system clock);
(PSC is the prescaled clock).

PWM data load enable control register PWMLOADEN
F129H Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
PWMLOADEN - - PWM5LE PWM4LE PWM3LE PWM2LE PWM1LE PWM0LE
Bit7~ Bit6 -
Bit5~ Bit0 PWMnLE: Data loading enable bit of PWM channel n (n=0-5) (hardware cleared after loading is completed);
1=Enable load cycle, duty cycle data (PERIODn, CMPn, CMPDn).
0= Writing 0 is invalid.

PWM output polarity control register PWMPINV
F122H Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
PWMPINV - - PWM5PINV PWM4PINV PWM3PINV PWM2PINV PWM1PINV PWM0PINV
Bit7~ Bit6 -
Bit5~ Bit0 PWMnPINV: PWM channel n output polarity control bit (n=0-5);
1= Reverse output;
0= Normal output.

PWM counter mode control register PWMCNTM
F127H Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
PWMCNTM - - PWM5CNTM PWM4CNTM PWM3CNTM PWM2CNTM PWM1CNTM PWM0CNTM
Bit7~ Bit6 -
Bit5~ Bit0 PWMnCNTM: PWM channel n counter mode control bits (n=0-5);
1=Automatic loading mode;
0 = One-shot mode.

PWM counter enable control register PWMCNTE
F126H Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
PWMCNTE - - PWM5CNTE PWM4CNTE PWM3CNTE PWM2CNTE PWM1CNTE PWM0CNTE
Bit7~ Bit6 -
Bit5~ Bit0 PWMnCNTE: PWM channel n counter enable control bit (n=0-5);
1 = PWMn counter is turned on (PWMn starts to output);
0 = PWMn counter stops (software writes 0 to stop the counter and clear the counter value).

PWM counter mode control register PWMCNTCLR
F128H Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
PWMCNTCLR - - PWM5CNTCLR PWM4CNTCLR PWM3CNTCLR PWM2CNTCLR PWM1CNTCLR PWM0CNTCLR
Bit7~ Bit6 -
Bit5~ Bit0 PWMnCNTCLR: PWM channel n counter clearing control bit (n=0-5) (hardware automatic clearing);
1= Clear the PWMn counter;
0= Writing 0 is invalid.

PWM period data register low 8 bits PWMPnL (n=0-5)
Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
PWMPnL PWMPnL7 PWMPnL6 PWMPnL5 PWMPnL4 PWMPnL3 PWMPnL2 PWMPnL1 PWMPnL0
Register PWMPnL (n=0-5) address: F130H, F132H, F134H, F136H, F138H, F13AH.
Bit7~ Bit0 PWMPnL<7:0>: The lower 8 bits of the PWM channel n period data register.

PWM period data register high 8 bits PWMPnH (n=0-5)
Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
PWMPnH PWMPnH7 PWMPnH6 PWMPnH5 PWMPnH PWMPnH3 PWMPnH2 PWMPnH1 PWMPnH0
Register PWMPnH (n=0-5) address: F131H, F133H, F135H, F137H, F139H, F13BH.
Bit7~ Bit0 PWMPnH<7:0>: The upper 8 bits of the PWM channel n period data register.

PWM compare data register low 8 bits PWMDnL (n=0-5)
Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
PWMDnL PWMDnL7 PWMDnL6 PWMDnL5 PWMDnL4 PWMDnL3 PWMDnL2 PWMDnL1 PWMDnL0
Register PWMDnL (n=0-5) address: F140H, F142H, F144H, F146H, F148H, F14AH.
Bit7~ Bit0 PWMDnL<7:0>: The lower 8 bits of the PWM channel n compare data (duty cycle data) register.

PWM compare data register high 8 bits PWMDnH (n=0-5)
Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
PWMDnH PWMDnH7 PWMDnH6 PWMDnH5 PWMDnH4 PWMDnH3 PWMDnH2 PWMDnH1 PWMDnH0
Register PWMDnH (n=0-5) address: F141H, F143H, F145H, F147H, F149H, F14BH.
Bit7~ Bit0 PWMDnH<7:0>: The upper 8 bits of the PWM channel n compare data (duty cycle data) register.

PWM compare data register lower 8 bits PWMDDnL (n=0-5)
Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
PWMDDnL PWMDDnL7 PWMDDnL6 PWMDDnL5 PWMDDnL4 PWMDDnL3 PWMDDnL2 PWMDDnL1 PWMDDnL0
Register PWMDDnL (n=0-5) address: F150H, F152H, F154H, F156H, F158H, F15AH.
Bit7~ Bit0 PWMDDnL<7:0>: The lower 8 bits of the PWM channel n downward 
comparison data (duty cycle data under asymmetric counting) register.

PWM compare data register high 8 bits PWMDDnH (n=0-5)
Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
PWMDDnH PWMDDnH7 PWMDDnH6 PWMDDnH5 PWMDDnH4 PWMDDnH3 PWMDDnH2 PWMDDnH1 PWMDDnH0
Register PWMDDnH (n=0-5) address: F151H, F153H, F155H, F157H, F159H, F15BH.
Bit7~ Bit0 PWMDDnH<7:0>: The upper 8 bits of the PWM channel n downward 
comparison data (duty cycle data under asymmetric counting) register.

PWM Dead Band Enable Control Register PWMDTE
F160H Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
PWMDTE - - - - - PWM45DTE PWM23DTE PWM01DTE
Bit7~Bit3 -
Bit2 PWM45DTE: PWM4/5 channel dead-time delay enable bit;
1= enable;
0= Disabled.
Bit1 PWM23DTE: PWM2/3 channel dead-time delay enable bit;
1= enable;
0= Disabled.
Bit0 PWM01DTE: PWM0/1 channel dead-time delay enable bit;
1= enable;
0= Disabled.

PWM 0/1 Dead Time Delay Data Register PWM01DT
F161H Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
PWM01DT PWM01DT7 PWM01DT6 PWM01DT5 PWM01DT4 PWM01DT3 PWM01DT2 PWM01DT1 PWM01DT0
Bit7~Bit0 PWM01DT<7:0>: PWM channel 0/1 dead-band delay data register.

PWM 2/3 Dead Time Delay Data Register PWM23DT
F162H Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
PWM23DT PWM23DT7 PWM23DT6 PWM23DT5 PWM23DT4 PWM23DT3 PWM23DT2 PWM23DT1 PWM23DT0
Bit7~Bit0 PWM23DT<7:0>: PWM channel 2/3 dead-band delay data register.

PWM4/5 Dead Time Delay Data Register PWM45DT
F163H Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
PWM45DT PWM45DT7 PWM45DT6 PWM45DT5 PWM45DT4 PWM45DT3 PWM45DT2 PWM45DT1 PWM45DT0
Bit7~Bit0 PWM45DT<7:0>: PWM channel 4/5 dead-band delay data register.

PWM mask control register PWMMASKE
F164H Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
PWMMASKE - - PWM5MASKE PWM4MASKE PWM3MASKE PWM2MASKE PWM1MASKE PWM0MASKE
Bit7~ Bit6 -
Bit5~Bit0 PWMnMASKE: PWM channel n mask control enable bit (n=0-5);
1= PWMn channel enable mask data output;
0 = Mask data output is disabled for the PWMn channel (normal output PWM waveform).

PWM mask data register PWMMASKD
F165H Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
PWMMASKD - - PWM5MASKD PWM4MASKD PWM3MASKD PWM2MASKD PWM1MASKD PWM0MASKD
Bit7~ Bit6 -
Bit5~Bit0 PWMnMASKD: PWM channel n mask data bit (n=0-5);
1= PWMn channel output is high;
0 = PWMn channel output is low.

PWM brake control register PWMFBKC
F166H Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
PWMFBKC PWMFBIE PWMFBF PWM5FBCCE PWMFBKSW PWMFBES - PWMFBEN -
Bit7 PWMFBIE: PWM brake interrupt mask bit;
1= enable interrupt;
0= Disable interrupts.
Bit6 PWMFBF: PWM brake flag bit (write 0 to clear);
1=Brake operation occurred (PWM output brake data register value);
0 = No braking operation has occurred.
Bit5 PWMFBCCE: Whether to clear all channel counter selection bits when PWM braking;
1= The counter value will not be cleared when braking;
0 = Clear the channel counter value when braking.
Bit4 PWMFBKSW: PWM software brake signal start bit;
1= PWM generates software brake signal;
0= Disabled.
Bit3 PWMFBES: PWM external hardware brake channel (FB) trigger level selection bit;
1=High level;
0 = low level.
Bit2 - Reserved, must be 0.
Bit1 PWMFBEN: PWM external hardware brake channel (FB) enable bit;
1= enable;
0= Disabled.
Bit0 - reserved, must be 0.

PWM brake data register PWMFBKD
F167H Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
PWMFBKD - - PWM5FBKD PWM4FBKD PWM3FBKD PWM2FBKD PWM1FBKD PWM0FBKD
Bit7~Bit6 are not used.
Bit5~Bit0 PWMnFBKD: PWM channel n brake data bit (n=0-5);
1= PWMn channel outputs high after braking operation.
0 = PWMn channel outputs low after braking operation.

PWM period interrupt mask register PWMPIE
F168H Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
PWMPIE - - PWM5PIE PWM4PIE PWM3PIE PWM2PIE PWM1PIE PWM0PIE
Bit7~Bit6 -
Bit5~Bit0 PWMnPIE: PWM channel n cycle interrupt mask bit (n=0-5);
1= enable interrupt;
0= Disable interrupts.

PWM zero interrupt mask register PWMZIE
F169H Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
PWMZIE - - PWM5ZIE PWM4ZIE PWM3ZIE PWM2ZIE PWM1ZIE PWM0ZIE
Bit7~Bit6 -
Bit5~Bit0 PWMnZIE: PWM channel n zero interrupt mask bit (n=0-5);
1= enable interrupt;
0= Disable interrupts.

PWM up compare interrupt mask register PWMUIE
F16BH Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
PWMUIE - - PWM5UIE PWM4UIE PWM3UIE PWM2UIE PWM1UIE PWM0UIE
Bit7~Bit6 -
Bit5~Bit0 PWMnUIE: PWM channel n upward comparison interrupt mask bit (n=0-5);
1= enable interrupt;
0= Disable interrupts.

PWM Compare Down Interrupt Mask Register PWMDIE
F16BH Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
PWMDIE - - PWM5DIE PWM4DIE PWM3DIE PWM2DIE PWM1DIE PWM0DIE
Bit7~Bit6 -
Bit5~Bit0 PWMnDIE: PWM channel n downward comparison interrupt mask bit (n=0-5);
1= enable interrupt;
0= Disable interrupts.

PWM Period Interrupt Flag Register PWMPIF
F16CH Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
PWMPIF - - PWM5PIF PWM4PIF PWM3PIF PWM2PIF PWM1PIF PWM0PIF
Bit7~Bit6 -
Bit5~Bit0 PWMnPIF: PWM channel n cycle interrupt flag bit (n=0-5);
1= Generate an interrupt (cleared by software);
0 = No interrupt is generated.

PWM zero interrupt flag register PWMZIF
F16DH Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
PWMZIF - - PWM5ZIF PWM4ZIF PWM3ZIF PWM2ZIF PWM1ZIF PWM0ZIF
Bit7~Bit6 -
Bit5~Bit0 PWMnZIF: PWM channel n zero interrupt flag bit (n=0-5);
1= Generate an interrupt (cleared by software);
0 = No interrupt is generated.

PWM up compare interrupt flag register PWMUIF
F16FH Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
PWMUIF - - PWM5UIF PWM4UIF PWM3UIF PWM2UIF PWM1UIF PWM0UIF
Bit7~Bit6 -
Bit5~Bit0 PWMnUIF: PWM channel n upward comparison interrupt flag bit (n=0-5);
1= Generate an interrupt (cleared by software);
0 = No interrupt is generated.

PWM Compare Down Interrupt Flag Register PWMDIF
F16FH Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
PWMDIF - - PWM5DIF PWM4DIF PWM3DIF PWM2DIF PWM1DIF PWM0DIF
Bit7~Bit6 -
Bit5~Bit0 PWMnDIF: PWM channel n downward comparison interrupt flag bit (n=0-5);
1= Generate an interrupt (cleared by software);
0 = No interrupt is generated.
*/