/*
 * measuring.c
 *
 * Created: 25.02.2021 11:39:10
 *  Author: DeAd_MorOz
 */ 
#include "CMS80F231x.h"
#include <intrins.h>
#include "globalvar.h"
#include "measuring.h"
#include "pin_manager.h"

static uint16_t data local_fc_sensor,local_eva_sensor,local_tr_sensor;
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
	// начальная инициализация датчиков температуры
	for (k=0;k<4;k++)
  	{
		gvar.fc_sensor	= GetADC(ADC_CHAN_FC) << 4;
		gvar.eva_sensor = GetADC(ADC_CHAN_EVA) << 4;
		gvar.tr_sensor	= GetADC(ADC_CHAN_TRIM) << 4;
	}
	
	// инициализация циклических измерений ADC
	bitmaskclr(ADCON0,bin(11011100));
	ADCON1 = 0;
	bitmaskset(ADCON0,(ADC_REF_VCC+((ADC_CHAN_TRIM & 0x10)<<3)));// left ajusted, vref = vdd.
	ADCON1 = bin(11000000) + (ADC_CHAN_TRIM&0x0f);
	_nop_();// после смены канала нужна задержка 200ns (4.8 такта при 24МГц) при напряжении 4.5~5.5V
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	index_m = 0;
	bitclr(EIF2,ADCIF);		// стираю флаг прерываний
	bitset(EIE2,ADCIE);		// включаю прерывания от ADC
	
	// инициализация прерываний фронта сети.
	IT0 = 1;				// работаю по спадающему фронту
	IE0 = 0;				// стираю флаг прерывания
	EX0 = 1;				// разрешаю прерывание от сети
	
	without_fase_detector = 0;	// должно быть 0	
	ext_int_dis = 0;
	
	flags.start_now = 11;		// начальная инициализация регистров 
}

//---------------------------------------------------------------------------------------------------------------
// прерывание АЦП
//---------------------------------------------------------------------------------------------------------------
void adc_interrupt (void) interrupt 19
{	
	uint16_t u;
	u = (((uint16_t)ADRESH << 2) + ((uint16_t)ADRESL>>6));
	bitclr(EIF2,ADCIF);		// стираю флаг прерываний
	
	if (index_m == 0){
		bitclr(ADCON0,ADCHS4);
		bitmaskset(ADCON0,((ADC_CHAN_FC& 0x10)<<3));
		ADCON1 = bin(11000000) + (ADC_CHAN_FC&0x0f);
		local_tr_sensor = u;
		bitset(ADCON0, ADGO);
		
	}else if (index_m == 1){
		bitclr(ADCON0,ADCHS4);
		bitmaskset(ADCON0,((ADC_CHAN_EVA& 0x10)<<3));
		ADCON1 = bin(11000000) + (ADC_CHAN_EVA&0x0f);
		local_fc_sensor = u;
		bitset(ADCON0, ADGO);

	}else if (index_m == 2){
		bitclr(ADCON0,ADCHS4);
		bitmaskset(ADCON0,((ADC_CHAN_TRIM & 0x10)<<3));
		ADCON1 = bin(11000000) + (ADC_CHAN_TRIM&0x0f);
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
	bitset(ADCON0, ADGO); 	//ADC GO
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
	bitmaskclr(ADCON0,bin(11011100));
	bitmaskset(ADCON0,(ADC_REF_VCC+((mux & 0x10)<<3)));// left ajusted, vref = vdd.
	ADCON1 = bin(11000000) + (mux&0x0f);
	_nop_();// после смены канала нужна задержка 200ns (4.8 такта при 24МГц) при напряжении 4.5~5.5V
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	bitset(ADCON0, ADGO);
	while(bitchk(ADCON0, ADGO));
	bitmaskclr(ADCON0,bin(11011100));
	ADCON1 = 0;
	return (((uint16_t)ADRESH << 2) + ((uint16_t)ADRESL>>6));
}

/*
13. Analog-to-digital conversion (ADC)
13.1 ADC overview
An analog-to-digital converter (ADC) can convert an analog input signal into a 12-bit binary 
number representing the signal.
The port analog input signal is connected to the input of the 
analog-to-digital converter after passing through the multiplexer.
The analog-to-digital converter uses the successive approximation method to produce 
a 12-bit binary result, and converts the
The result is stored in the ADC result registers (ADRESL and ADRESH).
The ADC reference voltage is always internally generated. 
The ADC can generate an interrupt after the conversion is complete.

13.2 ADC configuration
When configuring and using ADC, the following factors must be considered:
◆ Port configuration;
◆ Channel selection;
◆ ADC conversion clock source;
◆ Interrupt control;
◆ The storage format of the result.
13.2.1 Port configuration Port configuration
ADC can convert both analog and digital signals.
When converting analog signals, you should configure the corresponding ports as analog ports.
For more information, see the corresponding port chapter.
Note: Applying analog voltage to pins defined as digital inputs may cause overcurrent in the input buffer.

13.2.2 Channel selection Channel selection
The ADCHS bit in the ADCON1 register determines which channel is connected to the analog-to-digital converter.
If the channel is changed, a certain delay is required before the next conversion starts.
The ADC delay time is shown in the following table:
Delay time Working voltage
500ns 2.5~4.5V
200ns 4.5~5.5V
For more information, please refer to the "ADC Working Principle" chapter.

13.2.3 ADC reference voltage
The ADC reference voltage is provided by the chip's VDD by default, 
or it can be provided by the internal ADC-LDO. ADC-LDO can choose 4 voltage outputs:
1.2V/2.0V/2.4V/3.0V.

13.2.4 Conversion clock Conversion clock
The ADCKS bit of the ADCON1 register can be set by software to select the conversion clock source.
There are 8 possible clock frequencies to choose from:
◆ F sys /2
◆ F sys /4
◆ F sys /8
◆ F sys /16
◆ F sys /32
◆ F sys /64
◆ F sys /128
◆ F sys /256
The time to complete a bit conversion is defined as T AD. 
A complete 12-bit conversion requires 18.5 T AD cycles 
(the time that ADGO continues to be high to complete a conversion).
Must comply with the corresponding T AD specification to obtain the correct conversion result. 
The following table is a reference example for selecting the ADC clock.
F sys 		T AD
8MHz 		F sys /8
16MHz 		F sys /16
24MHz 		F sys /32
48MHz 		F sys /64
Note: Any change in the system clock frequency will change the ADC clock frequency, 
which will negatively affect the ADC conversion results.

13.2.5 ADC Interrupt
The ADC module allows an interrupt to be generated after the analog-to-digital conversion is completed.
The ADC interrupt flag bit is the ADCIF bit in the EIF2 register.
The ADC interrupt enable bit is the ADCIE bit in the EIE2 register.
The ADCIF bit must be cleared by software.
The ADCIF bit will be set to 1 after each conversion, regardless of whether ADC interrupts are enabled.

13.2.6 Result formatting Formatting
The result of 12-bit A/D conversion can be in two formats: left-justified or right-justified.
The output format is controlled by the ADFM bit in the ADCON0 register.
-When ADFM=0, the AD conversion result is left aligned;
-When ADFM=1, the AD conversion result is right aligned.

13.3 ADC hardware trigger start
In addition to the software triggering the AD conversion, 
the ADC module also provides a hardware trigger start method, 
one is the external port edge trigger, 
and the other is the PWM edge or period trigger.
To use the hardware trigger ADC, ADCEX needs to be set to 1, 
even if the ADC function can be triggered externally.
The hardware trigger signal will set the ADGO bit to 1 after a certain delay, 
and it will be automatically cleared after conversion.
After the hardware trigger function is turned on, 
the software trigger function will not be turned off. When the ADC is idle, 
writing 1 to the ADGO bit can also start AD conversion.

13.3.1 External port edge triggering External port edge triggering ADC
The ADET pin edge automatically triggers AD conversion. 
At this time, ADTGS[1:0] needs to be 11 (select external port edge trigger), 
ADEGS[1:0] can select which edge trigger.

13.3.2 PWM trigger ADC
PWM can choose to trigger ADC conversion by edge or cycle zero/midpoint. 
ADTGS[1:0] selects the PWM channel (PG0, PG2, PG4), ADEGS[1:0] can select the edge type or period type trigger mode.

13.3.3 Delay before hardware trigger is started Delay before hardware trigger is started
After the hardware trigger signal is generated, the AD conversion is not started immediately, 
and ADGO is set to 1 after a certain delay.
The delay is determined by ADDLY[9:0]. The structure diagram is shown in the figure below.
Delay time of hardware trigger signal: Tdelay = (ADDLY+3)*Tsys

13.4 ADC working principle
13.4.1 Start Conversion Conversion
To enable the ADC module, you must first set the ADEN bit of the ADCON1 register, 
and then set the ADGO bit of the ADCON0 register
Start analog-digital conversion (ADGO cannot be set when ADEN is 0).

13.4.2 Complete the conversion Complete the conversion
When the conversion is complete, the ADC module will:
◆ Clear the ADGO bit;
◆ Set the ADCIF flag bit to 1;
◆ Update the ADRESH:ADRESL register with the new conversion result.

13.4.3 Terminate Conversion Terminate Conversion
After the ADC is started, it must wait for the completion of the ADC conversion 
to terminate the ADC conversion. It is forbidden to terminate the ADC conversion 
during the ADC conversion process.

Note: Device reset will force all registers to enter reset state. 
Therefore, a reset will shut down the ADC module and terminate any pending conversions.

13.4.4 A/D conversion steps
The following steps give an example of analog-to-digital conversion using ADC:
1. Port configuration:
⚫ Disable pin output driver (see PxTRIS register);
⚫ Configure the pins as analog input pins.
2. Configure the ADC module:
⚫ Select ADC conversion clock;
⚫ Select ADC input channel;
⚫ Choose the format of the result;
⚫ Start the ADC module.
3. Configure ADC interrupt (optional):
⚫ Clear the ADC interrupt flag bit;
⚫ Allow ADC interrupt;
⚫ Allow peripheral interrupts;
⚫ Allow global interrupts.
4. Wait for the required acquisition time.
5. Set ADGO to start conversion.
6. Wait for the ADC conversion to end by one of the following methods:
⚫ Check the ADGO bit;
⚫ Wait for ADC interrupt (interrupt enabled).
7. Read the ADC result.
8. Clear the ADC interrupt flag bit (if interrupts are enabled, this operation is required).
Note: If the user attempts to resume sequential code execution after 
waking the device from sleep mode, the global interrupt must be disabled.

13.4.5 Sleeping during the conversion process Sleeping during the conversion process
When the system enters dormancy, it must wait for the ADC ongoing conversion to complete 
before entering dormant state. It is forbidden to let the chip go to sleep during ADC conversion.


13.5 ADC related registers
There are mainly 10 registers related to AD conversion, namely:
⚫ AD control registers ADCON0, ADCON1, ADCON2, ADCLDO;
⚫ Comparator control register ADCMPC;
⚫ Delay data register ADDLYL;
⚫ AD result data register ADRESH/L;
⚫ Comparator data register ADCMPH/L.


AD control register ADCON0
0xDF Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
ADCON0 ADCHS4 ADFM - AN31SEL2 AN31SEL1 AN31SEL0 ADGO -
Bit7 ADCHS4 ADC analog channel selection bit 4;
1= Refer to the description in the register ADCON1 below for channel allocation;
0= -
Bit6 ADFM: ADC conversion result format selection bit;
1= Right justified;
0= Justify left.
Bit5 - reserved
Bit4~Bit2 AN31SEL<2:0>: ADC channel 31 input source selection bits;
000= BGR(1.2V);
001= ACMP_VREF (the negative terminal reference voltage of the comparator, see the ACMP chapter for details);
010= OP0_O;
011= OP1_O;
100= PGA_ANA (PGA internal output signal, see PGA module chapter for details);
101 = VSS (ADC reference ground);
110= reserved, prohibited use;
111 = VDD (ADC default reference voltage).
Bit1 ADGO: ADC conversion start bit (ADEN must be 1 when this bit is set to 1, otherwise the operation is invalid);
1= Write: start ADC conversion, (the bit will also be 1 when ADC is triggered by hardware);
Read: ADC is converting.
0=Write: invalid.
Read: ADC is idle/conversion completed;
During ADC conversion (ADGO=1), any software and hardware trigger signals will be ignored.
Bit0 -

AD control register ADCON1
0xDE Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
ADCON1 ADEN ADCKS2 ADCKS1 ADCKS0 ADCHS3 ADCHS2 ADCHS1 ADCHS0
Read and write R/W R/W R/W R/W R/W R/W R/W R/W
Reset value 0 1 0 0 0 0 0 0
Bit7 ADEN: ADC enable bit;
1= Enable ADC;
0 = Disable ADC and consume no operating current.
Bit6~Bit4 ADCKS<2:0>: ADC conversion clock selection bits;
000 = Fsys/2 		100 = Fsys/32
001 = Fsys/4 		101 = Fsys/64
010 = Fsys/8 		110 = Fsys/128
011 = Fsys/16 		111 = Fsys/256
Bit3~Bit0 ADCHS<3:0>: The lower 4 bits of the analog channel selection bit, 
	      and ADCHS<4> form the 5-bit channel selection bit, ADCHS<4:0>;
00000 = AN0 10000 = AN16
00001 = AN1 10001 = AN17
00010 = AN2 10010 = AN18
00011 = AN3 10011 = AN19
00100 = AN4 10100 = AN20
00101 = AN5 10101 = AN21
00110 = AN6 10110 = AN22
00111 = AN7 Other = -
01000 = AN8 11111 = See the description of ADCON0.AN31SEL
01001= AN9
01010= AN10
01011= AN11
01100= AN12
01101= AN13
01110= AN14

AD control register ADCON2
0xE9 Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
ADCON2 ADCEX - ADTGS1 ADTGS0 ADEGS1 ADEGS0 - -
Bit7 ADCEX: ADC hardware trigger enable bit;
1= enable;
0= Disabled.
Bit6 reserved: Must be 0.
Bit5~Bit4 ADTGS: ADC hardware trigger source selection bits;
00= PG0(PWM0);
01= PG2 (PWM2);
10= PG4 (PWM4);
11 = Port pin (ADET).
Bit3~ Bit2 ADEGS: ADC hardware trigger edge selection bits;
00 = falling edge;
01= rising edge;
10 = Period point of PWM period;
11 = zero point of the PWM period.
Bit1~Bit0 -

AD comparator control register ADCMPC
0xD1 Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
ADCMPC ADFBEN ADCMPPS - ADCMPO - - ADDLY9 ADDLY8
Bit7 ADFBEN: ADC comparator result control PWM brake enable bit;
1= enable;
0= Disabled.
Bit6 ADCMPPS: ADC comparator output polarity selection bit;
1= If ADRES<ADCMP, then ADCMPO=1;
0= If ADRES>=ADCMP, then ADCMPO=1.
Bit5 -
Bit4 ADCMPO: ADC comparator result output bit (read only).
This bit outputs the result of the ADC comparator output, 
and it will be updated every time the ADC conversion ends.
Bit3~Bit2 -
Bit1~ Bit0 ADDLY[9:8]: ADC hardware trigger delay data[9:8] bits.

AD hardware trigger delay data register ADDLYL
0xD3 Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
ADDLYL ADDLY7 ADDLY6 ADDLY5 ADDLY4 ADDLY3 ADDLY2 ADDLY1 ADDLY0
Read and write R/W R/W R/W R/W R/W R/W R/W R/W
Reset value 0 0 0 0 0 0 0 0
Bit7~Bit0 ADDLY[7:0]: The lower 8 bits of the ADC hardware trigger delay data.

AD data register high ADRESH, ADFM=0 (left justified)
0xDD Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
ADRESH ADRES11 ADRES10 ADRES9 ADRES8 ADRES7 ADRES6 ADRES5 ADRES4
Read and write R R R R R R R R
Reset value X X X X X X X X
Bit7~Bit0 ADRES<11:4>: ADC result register bits.
The 11-4 digits of the 12-bit conversion result.

AD data register low bit ADRESL, ADFM=0 (left justified)
0xDC Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
ADRESL ADRES3 ADRES2 ADRES1 ADRES0 - - - -
Read and write R R R R - - - -
Reset value X X X X - - - -
Bit7~Bit4 ADRES<3:0>: ADC result register bits.
Bits 3-0 of the 12-bit conversion result.
Bit3~Bit0 are not used.

AD data register high ADRESH, ADFM=1 (right justified)
0xDD Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
ADRESH - - - - ADRES11 ADRES10 ADRES9 ADRES8
Read and write - - - - R R R R
Reset value - - - - X X X X
Bit7~Bit4 are not used.
Bit3~Bit0 ADRES<11:8>: ADC result register bits.
The 11th-8th bits of the 12-bit conversion result.

AD data register low bit ADRESL, ADFM = 1 (right justified)
0xDC Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
ADRESL ADRES7 ADRES6 ADRES5 ADRES4 ADRES3 ADRES2 ADRES1 ADRES0
Read and write R R R R R R R R
Reset value X X X X X X X X
Bit7~Bit0 ADRES<7:0>: ADC result register bits.
Bit 7-0 of the 12-bit conversion result.

AD comparator data register ADCMPH
0xD5 Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
ADCMPH D11 D10 D9 D8 D7 D6 D5 D4
Read and write R/W R/W R/W R/W R/W R/W R/W R/W
Reset value 1 1 1 1 1 1 1 1
Bit7~Bit0 ADCMP<11:4>: The upper 8 bits of ADC comparator data.

AD comparator data register ADCMPL
0xD4 Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
ADCMPL - - - - D3 D2 D1 D0
Read and write R/W R/W R/W R/W R/W R/W R/W R/W
Reset value 1 1 1 1 1 1 1 1
Bit7~Bit4 are not used.
Bit 3~Bit0 ADCMP[3:0]: The lower 4 bits of ADC comparator data.

ADC reference voltage control register ADCLDO
F692H Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
ADCLDO LDOEN VSEL1 VSEL0 - - - - -
Read and write R/W R/W R/W R/W R/W R/W R/W R/W
Reset value 0 0 0 0 0 0 0 0
Bit7 LDOEN ADC_LDO enable;
1= LDO is enabled, the reference voltage can only be the voltage corresponding to VSEL[1:0];
0 = LDO is disabled, the reference voltage is the chip power supply voltage.
Bit 6~Bit5 VSEL[1:0]: ADC reference voltage selection bits;
00= 1.2V;
01= 2.0V;
10= 2.4V;
11= 3.0V.
Bit4 - Reserved, must be 0.
Bit 3~Bit0 -

13.6 ADC result comparison
The ADC module provides a set of digital comparators, 
which are used to compare the ADC result with the pre-loaded {ADCMPH, ADCMPL} value.
The result of each ADC conversion will be compared with the preset value ADCMP. 
The result of the comparison is stored in the ADCMPO flag bit. 
After the conversion is completed, the flag bit will be automatically updated.
The ADCMPPS bit can change the polarity of the output result.
The ADC comparison result can trigger the PWM fault brake. 
To enable this function, you need to set ADFBEN to 1.
When the PWM function is turned on and ADFBEN=1, the AD conversion result 
is compared with the preset value {ADCMPH, ADCMPL}. 
If the comparison result ADCMPO is 1, the PWM will immediately generate a 
fault brake operation and clear the start bits of all PWM channels Zero, 
terminate all PWM channel output.
*/