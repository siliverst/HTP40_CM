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











