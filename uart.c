#include <SH79F084B.h>
#include <intrins.h>
#include "globalvar.h"
#include "uart.h"

uart_drv uart;
static void uart_init(void);

//------------------------------------------------------------------------------
// Инициализация UART
//------------------------------------------------------------------------------
void uart_init(void)
{
	// 19200
	// baudrate = U_SYS_DEF/(32*(256-TH1));
	// TH1 = 256 - (U_SYS_DEF/32*baudrate);
	
	TCLK = 0;				// таймер 1 используется в качестве генератора опорной частоты UART
	RCLK = 0;				// таймер 1 используется в качестве генератора опорной частоты UART
	TR1 = 0;				// остановить таймер 1
	bitset(TMOD,M11);		// включить MODE2 таймер 1
	bitclr(TMOD,M10);		// включить MODE2 таймер 1
	TH1 = 229;
	TL1 = 0x00;
	bitclr(TCON1,TCLKS1);	// тактирование таймер 1 от SYSclk
	bitset(TCON1,TCLKP1);	// тактирование таймер 1 от SYSclk делитель на 1
	bitclr(TCON1,TC1);		// отключить компаратор на таймере 1
	//baudrate
	TR1 = 1;				// запустить таймер 1
	
	
	bitclr(PCON,SMOD);					// выключаю удвоение частоты uart
	bitclr(PCON,SSTAT);					// выключаю детектор ошибок приёма
	SCON  = bin(01010000);				// включаю MODE1 и приёмник
	bitset(PCON,SSTAT);					// включаю детектор ошибок приёма
	ES0 = 1;							// включаю прерывания по UART1
    uart.rx_ready = 0;
    uart.rx_ptr = 0; 
} 

void uart_deinit (void)
{
	ES0 = 0;
	SCON = 0;
	TR1 = 0;
	bitmaskclr(P3,0x03);	//выводы UART притягиваю к 0
	bitmaskclr(P3M0,0x03);
	bitmaskset(P3M1,0x03);	
}

//------------------------------------------------------------------------------
// Функция для обработки таймаута UART.
// Если данные не поступают в приёмник в течение времени таймаута
// нужно завершить приём буфера
//------------------------------------------------------------------------------
void uart_rx_timeout_1ms_proc (void)
{
	if (uart.rx_pause_timeout){
		uart.rx_pause_timeout--;
		if (!uart.rx_pause_timeout){
			uart.rx_ready = 1;
			uart.rx_ptr = 0;
		}
	}
	
}

//------------------------------------------------------------------------------
// Прерывание UART
//------------------------------------------------------------------------------
void UART_ISR ( void )	interrupt 4
{
	static uint8_t data b;

	if (TI == 1){
		TI = 0;
		if(uart.tx_ptr < uart.tx_count){
			SBUF = uart.tx_buffer[uart.tx_ptr];
			++uart.tx_ptr;
		} else {
			RI = 0;
			REN = 1;
		}
	}
	else if (RI == 1){
		b = SBUF;
		RI = 0;
		if (SM0_FE == 1)
		{
			SM0_FE = 0;
			SM1_RXOV = 0;
			REN = 0;
			_nop_();
			REN = 1;
		} else {
			if(b == '\r'){        //'\r' если принят возврат каретки
				uart.rx_ready = 1;
				uart.rx_ptr = 0;
				uart.rx_pause_timeout = 0;
			}else if(uart.rx_ptr < BUFF_IN_BYTES){
				uart.rx_buffer[uart.rx_ptr] = b;
				uart.rx_ptr++;
				uart.rx_pause_timeout = UART_RX_TIMEOUT_VALUE;
			}
		}
	}
}

//------------------------------------------------------------------------------
// передаёт положенные в буфер данные в количестве "bytes"
//------------------------------------------------------------------------------
void uart_trans_buff (uint8_t bytes)
{
	while(TI);
	REN = 0;
	uart.tx_count = bytes;
	SBUF = uart.tx_buffer[0];
	uart.tx_ptr = 1;
}

//------------------------------------------------------------------------------
// Функция возвращает 1 если UART занят, 0 если свободен
// Вызывается в Верхнем софте
//------------------------------------------------------------------------------
/*uint8_t UART_is_Busy ( void )
{
    if(uart.tx_ptr < uart.tx_count) return 1;
    return 0;
}*/


/*
16. BRT module
16.1 Introduction
There is a 16-bit baut-rate timer inside the chip, which mainly provides the clock for the UART module.
16.2 Function description
There is a 16-bit counter inside the BRT. The initial value of the counter is loaded by {BRTDH, BRTDL}. When BRTEN=1, the counter starts
Work, the clock is derived from the prescaler circuit, and the prescaler clock is determined by BRTCKDIV.
The working mode of the counter is up counting. When the value of the 16-bit counter is equal to FFFFH, the BRT counter overflows. After overflow {BRTDH,
The value of BRTDL} is automatically loaded into the counter, and then counts again.
The overflow signal of the BRT counter is specifically provided to the UART module as the baud rate clock source. No interrupt will be generated on overflow. There is no corresponding
Interrupt structure.
When the BRT is in debug mode, its clock will not stop. If the UART module has started to send or receive data, even if the chip enters the pause state
UART will also complete the entire sending or receiving process.

BRTCON register
F5C0H Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
BRTCON BRTEN - - - - CnNS0 CnPS1 CnPS0
Bit7
BRTEN: BRT timer enable bit;
1= enable;
0= Disabled.
Bit6~ Bit3 -
Bit2~Bit0 BRTCKDIV<2:0> BRT timer prescaler selection bits;
000= Fsys/1;
001= Fsys/2;
010= Fsys/4;
011=Fsys/8;
100=Fsys/16;
101=Fsys/32;
110=Fsys/64;
111=Fsys/128.

BRTDL register
F5C1H Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
BRTDL BRTDL7 BRTDL6 BRTDL5 BRTDL4 BRTDL3 BRTDL2 BRTDL1 BRTDL0
Bit7~Bit0 BRTDL<7:0>: BRT timer load value lower 8 bits;

BRTDH register
F5C2H Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
BRTDH BRTDH7 BRTDH6 BRTDH5 BRTDH4 BRTDH3 BRTDH2 BRTDH1 BRTDH0
Bit7~Bit0 BRTDH<7:0>: BRT timer load value high 8 bits;

The following table shows part of the baud rate related information when the baud rate timer overflow rate is used as the UART clock source in variable baud rate mode:
Fsys=24MHz
SMODn=0, BRTCKDIV=0
19200 -> 65497 -> 19231 error -0.16%
Fsys=24MHz
SMODn=1，BRTCKDIV=0
19200 -> 65458  -> 19231 error -0.16%

17. UARTn module (n=0,1)
17.1 Introduction
The chip contains two UART modules: UART0 and UART1, which have exactly the same functions.
The serial port is full-duplex, meaning it can transmit and receive at the same time. The receiving module has double buffering, which means it can start receiving the next byte before reading the last received byte from the receiving register.
The data written to SBUFn is loaded into the sending register, while the reading of SBUFn is to access a receiving register with an independent physical address.
The serial port has four operating modes: one synchronous and three asynchronous modes.
Modes 2 and 3 have special functions for multi-processor communication, which can be enabled by setting the SMn2 bit in the SCONn register to 1.
The host processor first sends the address byte that identifies the target slave.
The address byte is different from the data byte, because the 9th bit in the address byte is 1 and the data byte is 0.
When SMn2=1, the slave will not be interrupted by the data byte.
The address byte will interrupt all slaves. The addressed slave will clear its SMn2 bit and prepare to receive the upcoming data byte.
The unaddressed slave sets SMn2 and ignores the incoming data.

17.2 UARTn port configuration
Before using the UARTn module, you need to configure the corresponding ports as TXDn and RXDn channels of UARTn (any GPIO can be configured as a UARTn channel), for example:
P25CFG = 0x08;//Select P2.5 to configure as TXD0 channel
P26CFG =0x09;//Select P2.6 to configure the RXD0 channel, the port is automatically configured as open drain and has a pull-up resistor in the master synchronization mode
P35CFG = 0x0A; //Select P3.5 to configure as TXD1 channel
P21CFG =0x0B;//Select P2.1 to configure as RXD1 channel, the port is automatically configured as open-drain and has a pull-up resistor in master synchronization mode
When using, it is recommended to set the working mode first, and then configure the corresponding port as a serial port.

17.3 UARTn baud rate
When UARTn is in mode 0, the baud rate is fixed to the system clock divided by 12 (Fsys/12); in mode 2, the baud rate is fixed to the system clock divided by 32 or 64 (Fsys/12). /32, Fsys/64);
In mode 1 and mode 3, the baud rate is determined by the timer Timer1 or Timer4
Or it is generated by Timer2 or BRT module. Which timer the chip selects as the baud rate clock source is determined by the register FUNCCR.
{FUNCCR[2],FUNCCR[0]}=00, select Timer1 as the baud rate generator of UART0;
{FUNCCR[2],FUNCCR[0]}=01, select Timer4 as the baud rate generator of UART0;
{FUNCCR[2],FUNCCR[0]}=10, select Timer2 as the baud rate generator of UART0;
When {FUNCCR[2],FUNCCR[0]}=11, select BRT as the baud rate generator for UART0.
{FUNCCR[3],FUNCCR[1]}=00, select Timer1 as the baud rate generator of UART1;
{FUNCCR[3],FUNCCR[1]}=01, select Timer4 as the baud rate generator of UART1;
When {FUNCCR[3],FUNCCR[1]}=10, select Timer2 as the baud rate generator of UART1;
When {FUNCCR[3],FUNCCR[1]}=11, select BRT as the baud rate generator of UART1.

17.4 UARTn Register
UARTn has the same functions as the standard 8051 UART. UARTn related registers are: SBUFn, SCONn, PCON (0x87), IE (0xA8) and IP (0xB8). The UARTn data buffer (SBUFn) consists of two independent registers: transmit and receive registers.
The data written to SBUFn will set this data in the UARTn output register and start transmission; the data read of SBUFn will read data from the UARTn receiving register.
The SCON0 register supports bit addressing operations, and the SCON1 register does not support bit addressing operations.
Be careful when using assembly language.

UART buffer register SBUFn
Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
SBUFn BUFFERn7 BUFFERn6 BUFFERn5 BUFFERn4 BUFFERn3 BUFFERn2 BUFFERn1 BUFFERn0
Read and write R/W R/W R/W R/W R/W R/W R/W R/W
Reset value X X X X X X X X
Register SBUF address: 0x99; Register SBUF1 address: 0xEB;
Bit7~Bit0 BUFFERn<7:0>: Buffer data register.
Write: UARTn starts to send data.
Read: Read the received data.

UART control register SCONn
Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
SCONn UnSM0 UnSM1 UnSM2 UnREN UnTB8 UnRB8 TIn RIn
Register SCON0 address: 0x98; register SCON1 address: 0xEA.
Bit7~Bit6 UnSM0- UnSM1: Multi-machine communication control bits;
00 = master synchronization mode;
01= 8-bit asynchronous mode with variable baud rate;
10 = 9-bit asynchronous mode, the baud rate is Fsys/32 or Fsys/64;
11= 9-bit asynchronous mode with variable baud rate.
Bit5 UnSM2: Multi-machine communication control bit;
1= enable;
0= Disabled.
Bit4 UnREN: Receive enable bit;
1= enable;
0= Disabled.
Bit3 UnTB8: The 9th bit of the sending data, mainly used for sending in 9-bit asynchronous mode;
1 = The 9th bit data is 1;
0 = The 9th bit data is 0.
Bit2 UnRB8: The 9th bit of the received data, which is mainly used for sending in 9-bit asynchronous mode;
1= The 9th bit of data received is 1;
0 = The 9th bit of data received is 0.
Bit1 Tln: Transmit interrupt flag bit (requires software to clear);
1= Indicates that the sending buffer is empty, and the next frame data can be sent.
0= -
Bit0 RIn: Receive interrupt flag bit (requires software to clear);
1= Indicates that the receiving buffer is full and the next frame of data can be received after reading.
0= -

The UARTn mode is as follows:
SMn0 |SMn1 | Mode | Description 	| Baud rate
0	 | 0   |  0   | Shift register  | Fsys/12
0 	 | 1   |  1   | 8-Bit UART      | variable
1    | 0   |  2   | 9-Bit UART      | Fsys/32 or /64
1    | 1   |  3   | 9-Bit UART      | variable

UARTn baud rate
Mode  	 |  Baud rate
Mode0 	 |  Fsys/12
Mode1, 3 | are controlled by Timer4/Timer1/Timer2/BRT, see section 16.3
Mode2	 | SMODn=0: Fsys/64 OR SMODn=1: Fsys/32

The SMODn bit is in the power management control register PCON register:
0x87 Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
PCON SMOD0 SMOD1 - - - SWE STOP IDLE
Bit7 SMOD0: UART0 baud rate multiplier bit;
1=Double the baud rate of UART0;
0 = The baud rate of UART0 is normal.
Bit6 SMOD1: UART1 baud rate multiplier bit;
1=Double the baud rate of UART1;
0 = The baud rate of UART1 is normal.
Bit5 -
Bit4~Bit3 Reserved bits: Must be 0.
Bit2 SWE: STOP status function wake-up enable bit;
(Regardless of the value of SWE, the system can be restarted by power-down reset or enabled external reset)
0=Disable function wake-up;
1= Enable function wake-up (wake-up by external interrupt and timed wake-up).
Bit1 STOP: Sleep state control bit;
1= enter sleep state (automatically cleared when exiting STOP mode);
0= not enter sleep state.
Bit0 IDLE: Idle state control bit;
1= Enter the idle state (automatically clear when exiting IDLE mode);
0 = Not entering idle state.

17.5 UARTn interrupt
The interrupt number of UART0 is 4, and its interrupt vector is 0x0023. The interrupt number of UART1 is 6, and its interrupt vector is 0x0033. Enable UARTn
The interrupt must have its enable bit ESn set to 1, and the overall interrupt enable bit EA must be set.
If UARTn related interrupt enable is turned on, when TIn=1 or RIn=1, the CPU will enter the corresponding interrupt service routine. TIn/RIn and
The state of ESn is irrelevant. And need to be cleared by software.
Interrupt mask register IE
0xA8 Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
IE EA ES1 ET2 ES0 ET1 EX1 ET0 EX0
Bit7 EA: Global interrupt enable bit;
1= Allow all unshielded interrupts;
0= Disable all interrupts.
Bit6 ES1: UART1 interrupt enable bit;
1= Enable UART1 interrupt;
0= Disable UART1 interrupt.
Bit5 ET2: TIMER2 total interrupt enable bit;
1= Allow all interrupts of TIMER2;
0= Disable all interrupts of TIMER2.
Bit4 ES0: UART0 interrupt enable bit;
1= Enable UART0 interrupt;
0 = Disable UART0 interrupt.
Bit3 ET1: TIMER1 interrupt enable bit;
1= Enable TIMER1 interrupt;
0= Disable TIMER1 interrupt.
Bit2 EX1: External interrupt 1 interrupt enable bit;
1=Enable external interrupt 1 interrupt;
0= Disable external interrupt 1 interrupt.
Bit1 ET0: TIMER0 interrupt enable bit;
1= Enable TIMER0 interrupt;
0= Disable TIMER0 interrupt.
Bit0 EX0: External interrupt 0 interrupt enable bit;
1=Enable external interrupt 0 interrupt;
0 = Disable external interrupt 0 interrupt.

Interrupt priority control register IP
0xB8 Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
IP - PS1 PT2 PS0 PT1 PX1 PT0 PX0
Bit7 -
Bit6 PS1: UART1 interrupt priority control bit;
1= Set as advanced interrupt;
0= Set as low-level interrupt.
Bit5 PT2: TIMER2 interrupt priority control bit;
1= Set as advanced interrupt;
0= Set as low-level interrupt.
Bit4 PS0: UART0 interrupt priority control bit;
1= Set as advanced interrupt;
0= Set as low-level interrupt.
Bit3 PT1: TIMER1 interrupt priority control bit;
1= Set as advanced interrupt;
0= Set as low-level interrupt.
Bit2 PX1: External interrupt 1 interrupt priority control bit;
1= Set as advanced interrupt;
0= Set as low-level interrupt.
Bit1 PT0: TIMER0 interrupt priority control bit;
1= Set as advanced interrupt;
0= Set as low-level interrupt.
Bit0 PX0: External interrupt 0 interrupt priority control bit;
1= Set as advanced interrupt;
0= Set as low-level interrupt.

UART control register SCONn
Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
SCONn SMn0 SMn1 SMn2 RENn TBn8 RBn8 TIn RIn
Register SCON0 address: 0x98; register SCON1 address: 0xEA.
Bit7~Bit6 SMn0-SMn1: Multi-machine communication control bits;
00 = master synchronization mode;
01= 8-bit asynchronous mode with variable baud rate;
10 = 9-bit asynchronous mode, the baud rate is Fsys/32 or Fsys/64;
11= 9-bit asynchronous mode with variable baud rate.
Bit5 SMn2: Multi-machine communication control bit;
1= enable;
0= Disabled.
Bit4 RENn: Receive enable bit;
1= enable;
0= Disabled.
Bit3 TBn8: The 9th bit of the transmitted data, mainly used for the transmission of 9-bit asynchronous mode;
1 = The 9th bit data is 1;
0 = The 9th bit data is 0.
Bit2 RBn8: The 9th bit of received data, mainly used for 9-bit asynchronous mode transmission;
1= The 9th bit of data received is 1;
0 = The 9th bit of data received is 0.
Bit1 Tin: Send interrupt flag bit (requires software to clear);
1= Indicates that the sending buffer is empty, and the next frame data can be sent.
0= -
Bit0 RIn: Receive interrupt flag bit (requires software to clear);
1= Indicates that the receiving buffer is full and the next frame of data can be received after reading.
0= -

17.6 UARTn mode (n=0/1)
17.6.1 Mode Mode 0- Synchronous Mode
Pin RXDn is used as input or output, and TXDn is used as clock output. The TXDn output is a shift clock. 
The baud rate is fixed at 1/12 of the system clock frequency. The 8-bit transmission is LSB first. 
Initialize the reception by setting the flag in SCONn, set as follows: RIn = 0 and RENn = 1.

17.6.2 Mode Mode 1-8 bit asynchronous mode (variable baud rate)
Pin RXDn is used as input and TXDn is used as serial output. Send 10 bits: start bit (always 0), 
8-bit data (LSB first) and stop bit (always 1). When receiving, the start bit is transmitted synchronously, 
8 data bits can be obtained by reading SBUFn, and the stop bit is set to the flag RBn8 in SCONn. 
The baud rate is variable and depends on the TIMER1, TIMER2, TIMER4 and BRT mode.

7.6.3 Mode Mode 2-9 bit asynchronous mode (fixed baud rate)
This mode is similar to mode 1, but there are two differences. 
The baud rate is fixed at 1/32 or 1/64 of the CLK clock frequency, 11-bit transmission and reception: 
start bit (0), 8-bit data (LSB first), programmable 9th bit and stop bit (1). 
The 9th bit can be used to control the parity of the UARTn interface:
When transmitting, bit TBn8 in SCONn is output as the 9th bit, and when receiving, the 9th bit affects RBn8 in SCONn.

17.6.4 Mode Mode 3-9 bit asynchronous mode (variable baud rate)
The only difference between Mode 2 and Mode 3 is that the baud rate in Mode 3 is variable.
When REN0=1, data reception is enabled. The baud rate is variable and depends on the timer 1 or timer 4 mode.
*/
