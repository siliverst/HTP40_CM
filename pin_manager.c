#include "CMS80F231x.h"
#include "globalvar.h"
#include "pin_manager.h"

void pin_manager_init (void)
{
	//TRIM_PIN
	bitclr(P0TRIS,4);		// вход
	P04CFG = 0x01;			// аналоговый вход AN4
	
	//FC_TEMP_PIN
	bitclr(P0TRIS,1);		// вход
	P01CFG = 0x01;			// аналоговый вход AN1
	
	//EVA_TEMP_PIN
	bitclr(P2TRIS,2);		// вход
	P22CFG = 0x01;			// аналоговый вход AN8
		
	//LED_PIN
	P02CFG = 0x00;			// обычный GPIO
	bitset(P0TRIS,2);		// выход
	bitclr(P0OD,2);			// push-pull
	bitclr(P0DR,2);			// сильный выход
	bitset(P0SR,2);			// медленный фронт
	bitclr(P0DS,2);			// читаю пин статус в режиме входа, и защёлку в режиме выхода
	LED_PIN = 1;			// в высокое состояние
	
	//COMPR_PIN
	P26CFG = 0x00;			// обычный GPIO
	bitset(P2TRIS,6);		// выход
	bitclr(P2OD,6);			// push-pull
	bitclr(P2DR,6);			// сильный выход
	bitclr(P2SR,6);			// быстрый фронт
	bitclr(P2DS,6);			// читаю пин статус в режиме входа, и защёлку в режиме выхода
	COMPR_PIN = 1;			// в высокое состояние
	
	//FAN_PIN
	P31CFG = 0x00;			// обычный GPIO
	bitset(P3TRIS,1);		// выход
	bitclr(P3OD,1);			// push-pull
	bitclr(P3DR,1);			// сильный выход
	bitclr(P3SR,1);			// быстрый фронт
	bitclr(P3DS,1);			// читаю пин статус в режиме входа, и защёлку в режиме выхода
	FAN_PIN = 1;			// в высокое состояние
	
	//HEATER_PIN
	P32CFG = 0x00;			// обычный GPIO
	bitset(P3TRIS,2);		// выход
	bitclr(P3OD,2);			// push-pull
	bitclr(P3DR,2);			// сильный выход
	bitclr(P3SR,2);			// быстрый фронт
	bitclr(P3DS,2);			// читаю пин статус в режиме входа, и защёлку в режиме выхода
	HEATER_PIN = 1;			// в высокое состояние
	
	//REED_PIN
	P03CFG = 0x00;			// обычный GPIO
	bitclr(P0TRIS,3);		// вход
	bitclr(P0OD,3);			// push-pull
	bitclr(P0DR,3);			// сильный выход
	bitclr(P0SR,3);			// быстрый фронт
	bitclr(P0DS,3);			// читаю пин статус в режиме входа, и защёлку в режиме выхода
	REED_PIN = 1;			// в высокое состояние
	
	//BUZZER_PIN
	P05CFG = 0x18;			// выход BEEP
	bitset(P0TRIS,5);		// выход
	bitclr(P0OD,5);			// push-pull
	bitclr(P0DR,5);			// сильный выход
	bitclr(P0SR,5);			// быстрый фронт
	bitclr(P0DS,5);			// читаю пин статус в режиме входа, и защёлку в режиме выхода
	BUZZER_PIN = 0;			// в низкое состояние
	// временно инициализация звука тут	3989Гц.			
	BUZDIV = 47;
	BUZCON = bin(00000011);	//старший бит включает/отключает звук
	
	//LAMP_PIN
	P24CFG = 0x12;			// выход PWM0
	bitset(P2TRIS,4);		// выход
	bitclr(P2OD,4);			// push-pull
	bitclr(P2DR,4);			// сильный выход
	bitclr(P2SR,4);			// быстрый фронт
	bitclr(P2DS,4);			// читаю пин статус в режиме входа, и защёлку в режиме выхода
	LAMP_PIN = 1;			// в низкое состояние
	
	//FASE_PIN
	P30CFG = 0x00;			// обычный GPIO
	bitclr(P3TRIS,0);		// вход
	bitclr(P3OD,0);			// push-pull
	bitclr(P3DR,0);			// сильный выход
	bitclr(P3SR,0);			// быстрый фронт
	bitclr(P3DS,0);			// читаю пин статус в режиме входа, и защёлку в режиме выхода
	FASE_PIN = 1;			// в высокое состояние
	PS_INT0 = 0x30;			// вход подключаю к прерыванию 0
	
	//TEST_PIN
	P23CFG = 0x00;			// обычный GPIO
	bitset(P2TRIS,3);		// выход
	bitclr(P2OD,3);			// push-pull
	bitclr(P2DR,3);			// сильный выход
	bitclr(P2SR,3);			// быстрый фронт
	bitclr(P2DS,3);			// читаю пин статус в режиме входа, и защёлку в режиме выхода
	TEST_PIN = 0;			// в низкое состояние
	
}

/*
6.1 GPIO function
The chip has four sets of I/O ports: PORT0, PORT1, PORT2, and PORT3. 
The readable and writable port data registers can directly access these ports.
PORTx is a bidirectional port. Its corresponding data direction register is PxTRIS.
Set a bit of PxTRIS to 1 (= 1) to configure the corresponding pin as an output.
Clear a bit of PxTRIS (= 0) to configure the corresponding PORTx pin as an input.
When PORTx is used as an output port, writing to the Px register will write to the port latch, and all write operations are read-modify-write operations. 
Therefore, writing a port means first reading the pin level of the port, then modifying the read value, and finally writing the modified value to the port data latch.
When PORTx is used as an output port, reading the Px register is related to the 
setting of the PxDS register.
A bit of PxDS is 1 (=1), the corresponding bit of Px read is the status of the pin, 
a bit of PxDS is cleared (=0), the corresponding bit of Px read is the status of the port data latch; 
PORTx is used as When the input port is used, reading the Px register reads the status of the pin, 
which has nothing to do with the setting of the PxDS register.
When using the PORTx pin as an analog input, the user must ensure that 
the bits in the PxTRIS register remain set to 0.
I/O pins configured as analog inputs always read 0.
The registers related to PORTx include Px, PxTRIS, PxOD, PxUP, PxRD, PxDS, etc.

6.1.1 PORTx data register Px
Register P0 address: 0x80; Register P1 address: 0x90; Register P2 address: 0xA0; Register P3 address: 0xB0.
Bit7~Bit0 Px<7:0>: Px I/O pin bits.
1= Port pin level>V IH;
0 = Port pin level <V IL.

6.1.2 PORTx direction register PxTRIS
Register P0TRIS address: 0x9A; register P1TRIS address: 0xA1; register P2TRIS address: 0xA2; register P3TRIS address: 0xA3.
Bit7~Bit0 PxTRIS<7:0>: Three-state control bits.
1= The pin is configured as an output;
0 = The pin is configured as an input (tri-state).
Note:
1. After the port is set as an output port, read the data of the port as the value of the output register.
2. After the port is set as an input port, the <read-modify-write> type of instruction for the port is actually an operation on the output register.

6.1.3 PORTx open drain control register PxOD
Register P0OD address: F009H; Register P1OD address: F019H;
Register P2OD address: F029H; Register P3OD address: F039H.
Bit7~Bit0 PxOD<7:0>: Open drain control bits.
1= The pin is configured as an open-drain state (the output is an open-drain output);
0 = The pin is configured as a normal state (the output is a push-pull output).

6.1.4 PORTx pull-up resistor control register PxUP
Register P0UP address: F00AH; Register P1UP address: F01AH;
Register P2UP address: F02AH; Register P3UP address: F03AH.
Bit7~Bit0 PxUP<7:0>: Pull-up resistor control bits;
1 = The pin pull-up resistor is turned on;
0 = The pin pull-up resistor is disabled.

6.1.5 PORTx pull-down resistor control register PxRD
Register P0RD address: F00BH; Register P1RD address: F01BH;
Register P2RD address: F02BH; Register P3RD address: F03BH.
Bit7~Bit0 PxRD<7:0>: Pull-down resistor control bits;
1= The pin pull-down resistor is turned on;
0 = The pin pull-down resistor is off.
Note: The control of the pull-down resistor has nothing to do with GPIO configuration and multiplexing functions, and is controlled separately by the PxRD register.

6.1.6 PORTx drive current control register PxDR
Register P0DR address: F00CH; Register P1DR address: F01CH;
Register P2DR address: F02CH; Register P3DR address: F03CH;
Bit7~Bit0 PxDR<7:0>: Drive current control bit (valid when the port is configured as output state).
1= Drive is weak drive;
0= The drive is a strong drive.

6.1.7 PORTx Slope Control Register PxSR
Register P0SR address: F00DH; Register P1SR address: F01DH;
Register P2SR address: F02DH; Register P3SR address: F03DH;
Bit7~Bit0 PxSR<7:0>: Px slope control register (valid when the port is configured as output state).
1= Px pin is slow slope;
0 = Px pin has fast slope.

6.1.8 PORTx data input selection register PxDS
Register P0DS address: F00EH; Register P1DS address: F01EH;
Register P2DS address: F02EH; Register P3DS address: F03EH.
Bit7~Bit0 PxDS<7:0>: Data input selection bits. 
When configured as GPIO, it affects the value of Px register.
1 = 
Read pin status in both output/input mode
(The Schmitt circuit remains open when the port is set to output state);
0= 
Output mode: read as the data latch status,
Input mode: read as pin status.
Note: If you need to read the pin status when the port is a multiplex function input structure, 
you need to set the port direction control to input mode.

6.2 Multiplexing function
6.2.1 Port Port Multiplexing Function Configuration Register Register
PORTx function configuration register PxnCFG
Bit7~Bit5 - reserved, all must be 0.
Bit4~Bit0
PxnCFG <4:0>: Function configuration bits, the default is GPIO function. 
See the port function configuration description for details.
There are 8 Px function configuration registers, 
including Px0CFG~Px7CFG, which control the function configuration of x0~Px7 respectively.
Each port has a function configuration register. 
Through the function configuration register PxnCFG, 
each port can be configured as any digital function.
For example, the function configuration register of port P00 is P00CFG, and different values ​​of the register settings correspond to different digital functions.
For example: To set P2.4 as the BEEP buzzer function, the configuration is as follows:
Assembly: MOV #P24CFG, #18H
C: P24CFG = 0x18;
When the port is used for multiplexing functions, 
it is not necessary to configure the port direction register PxTRIS. 
In addition to the SCL and SDA functions, 
other multiplexed functions are forced to turn off the pull-up resistor 
and open-drain output by hardware, that is, the pull-up resistor PxUP or the open-drain output PxOD is invalid by software.
When the port is multiplexed with SCL and SDA functions, 
the hardware forces the port to be an open-drain output, 
and the pull-up resistor PxUP can be set by software.

The digital functions corresponding to different configuration values are as follows:
Configuration value | Features | direction | Function Description
0x00 				| GPIO     | I/O 	   | General IO port, configure input and output through registers, pull up and pull down and other functions
0x01 				| ANALOG   |		   | simulation function
0x02 - - -
0x03 - - -
0x04 				| CC0      | O         |Timer2 compare output channel 0
..............................................................................и так далее смотри табличку в даташите

Note:
1) The configuration value marked as "--" in the table is reserved and prohibited from use.
2) The function configuration register defaults to 0x00, and the port functions as GPIO. 
	Different functions can be set through the GPIO function register in Chapter 6.1.
3) When the function configuration register is set to 0x01, 
	the hardware turns off the digital circuit to reduce power consumption, 
	and the GPIO function related register settings are invalid.
The port supports multiple analog functions, as described in the table below.
4) The ports used as output functions in the multiplexing function are not restricted by the priority order. 
	If there are multiple ports configured with the same output function, the function will be output at these ports at the same time.
5) The ports used as input functions in the multiplexing function are restricted by priority order. 
	If there are two or more ports configured with the same input function at the same time, 
	configure the selection according to the priority order of P0.0, P0.1,..., P3.2, P3.5 from high to low.
For example, configure P00 and P32 as RXD0 ports at the same time: P00CFG = 0x09; P32CFG = 0x09.
Since P0.0 has a higher priority, the RXD0 signal source is actually connected to the input of P0.0 port. 
Even if there is a data waveform on the P3.2 port, it will not be used as the RXD0 signal source.

The analog functions corresponding to the port are as follows:
PIN | CONFIG| 0 (GPIO) | 1(ANALOG) 					|Priority of other digital functions
P0.0|		|		   | AN0| C0P1	|		|		| Highest
........................................................................................ и так далее смотри табличку в даташите

6.2.2 Port Port input function allocation register Input function allocation register
There are digital functions with only input status inside the chip, such as INT0/INT1... etc. 
This type of digital input function has nothing to do with the port multiplexing status. 
As long as the assigned port supports digital input (such as RXD0 as digital input and GPIO as input function), the port supports this function.
The input function port allocation register is as follows:
Input function 			|
configuration register	| address | Features |Function description
PS_INT0 				| F0C0H   |   INT0   | external interrupt 0 input port allocation register
PS_INT1 				| F0C1H   |  INT1    | external interrupt 1 input port allocation register
PS_T0 					| F0C2H   |    T0    | Timer0 external clock input port allocation register
........................................................................................ и так далее смотри табличку в даташите

PS_XX Input function port allocation register PS_XX (as described in the above table)
Reset value 1 1 1 1 1 1 1 1

Bit7~Bit6 --: reserved, must be 0
Bit5~Bit0 PS_XX<5:0>: Input function allocation control bit
(Subject to the actual port of the chip, the unused value is reserved and prohibited from being used);
0x00= allocated to port P00;
0x01= allocated to P01 port;
…… ……
0x14= allocated to P14 port;
0x15= allocated to port P15;
…… ……
0x35= allocated to P35 port;
0x36= allocated to P36 port;
…… ……
0x3F= No port assigned;

1) The input function assignment structure supports multiple input functions assigned to the same port. 
For example, INT0 and CAP0 can be allocated to P00 at the same time
The configuration is as follows:
P00CFG = 0x00; //P00 port is configured as GPIO function
P0TRIS = 0x00;//P00 is used as GPIO input function
PS_INT0 = 0x00;//P00 port is configured as INT0 function
PS_CAP0 = 0x00;//P00 port is configured as CAP0 function
2) The input function allocation structure is relatively independent, 
and can support simultaneous use with other multiplexed function ports. 
At this time, there is no need to configure the direction of the corresponding port
Register, if RXD0 and INT0 can be allocated to port P00 at the same time, the configuration is as follows:
P00CFG = 0x09;//P00 port is configured as RXD0 function of UART0
PS_INT0 = 0x00;//P00 port is configured as INT0 function
3) The input function configuration structure can also be used simultaneously with the port external interrupt function. 
If CAP0 and GPIO interrupt functions can be assigned to P00 port, the configuration is as follows:
P00CFG = 0x00; //P00 port is configured as GPIO function
P0TRIS = 0x00;//P00 is used as GPIO input function
PS_CAP0 = 0x00;//P00 port is configured as CAP0 function
P00EICFG = 0x01; //P00 port is configured as rising edge trigger interrupt
P0EXTIE = 0x01;//Enable P00 port external interrupt

6.2.3 Port external interrupt control register
When using an external interrupt, you need to configure the port as a GPIO function and set the direction as an input port. 
Or the multiplexing function is input port (such as RXD0, RXD1).
PORTx External Interrupt Control Register PxNEICFG
Bit7~Bit2 are not used.
Bit1~Bit0 Px N EICFG<1:0>: Px N external interrupt control bit;
00 = External interrupt is disabled;
01 = rising edge triggers interrupt;
10= Falling edge trigger interrupt;
11= Both rising or falling edges trigger interrupts.
There are 8 external interrupt control registers of Px, including Px0EICFG~Px7EICFG, which control external interrupts of Px0~Px7 respectively.

6.2.4 Alternate Function Application Note
1) The input of the multiplexing function is relatively independent of the external interrupt (GPIO interrupt) of the port and the structure of the port input function.
For example, configure port P01 as RXD0, and configure the GPIO interrupt trigger mode of P01 as rising edge trigger and interrupt enable. When P01
When the input changes from low level to high level, it will trigger the P01 GPIO interrupt.
2) The input structure of the digital signal is not affected by the system configuration status.
For example, if the P01 port is powered on and configured as an external reset port, the input module of this port will be opened. If P01 is configured as INT0 in the program at the same time,
When the interrupt enable is turned on, before the reset signal sampling time is valid, the interrupt service routine is executed first, and then the reset operation is generated.
3) It should be noted that in the debugging mode, if the multiplexing function is configured on the DSDA port, its input function is also valid.
In this mode, do not configure related multiplexing functions to the DSDA port.

*/