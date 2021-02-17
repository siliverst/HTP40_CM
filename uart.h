#ifndef UART_H
#define UART_H

#ifdef	__cplusplus
extern "C" {
#endif

	
#define	BUFF_IN_BYTES       8
#define	BUFF_OUT_BYTES      32

#define	U_SYS_DEF		16600000UL
#define U_BAUD			19200

#define UART_RX_TIMEOUT_VALUE	8

// IEN1
//бит разрешения прерывания от юарт1
//#define	ES1					3 
	
// TMOD
// бит включающий режим 8 бит автозагрузка для таймера 1	
#define M11						5
#define M10						4
	
//TCON1
//  бит включающий режим тактирования timer1 от sysclk
#define TCLKS1					6
// бит делителя на 12
#define TCLKP1					3
// бит включения компаратора
#define TC1						1


//PCON
// бит удвоения честоты юарт
#define SMOD					7
// бит переключения функции битов 7:5 SCON
#define SSTAT					6
	
typedef struct
{
    volatile uint8_t rx_ready;
    volatile uint8_t rx_ptr;
    uint8_t rx_buffer[BUFF_IN_BYTES];
	volatile uint8_t rx_pause_timeout;
    volatile uint8_t tx_ptr;
    volatile uint8_t tx_count;
    volatile uint8_t tx_in_process;
    uint8_t tx_buffer[BUFF_OUT_BYTES];
    uint8_t connection_status;
}uart_drv;

extern uart_drv uart;

void uart_init(void);
void uart_deinit (void);
void uart_trans_buff (uint8_t bytes);
void uart_rx_timeout_1ms_proc (void);
//uint8_t UART_is_Busy ( void );


#ifdef	__cplusplus
}
#endif

#endif
