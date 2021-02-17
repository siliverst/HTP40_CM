#include <SH79F084B.h>
#include <intrins.h>
#include <string.h>
#include "globalvar.h"
#include "uart.h"
#include "uart_proc.h"
#include "trim.h"

//static uint8_t HexToByte(uint8_t *);
//static void put_1byte_to_buff(uint8_t b, uint8_t *buf);
//static void uart_put_2byte_to_buff(uint16_t b, uint8_t *buf);
//static void print_dec(uint16_t datad, uint8_t *buf);

void uart_grahp_init (void)
{

}

void uart_proc(void)
{
	/*
	#ifdef D_MODE
	uint8_t t;
	
	if (flags.start_now) return;
	if(uart.rx_ready){
        uart.rx_ready = 0;
	}	
	
	print_dec(gvar.d8,&uart.tx_buffer[0]);
	uart.tx_buffer[5] = ',';
	print_dec(gvar.d16,&uart.tx_buffer[6]);
	uart.tx_buffer[11] = ',';
	print_dec(gvar.eva_sensor/2,&uart.tx_buffer[12]);
	uart.tx_buffer[17] = ',';
	print_dec(gvar.tr_sector,&uart.tx_buffer[18]);
	uart.tx_buffer[23] = '\r';
	uart.tx_buffer[24] = '\n';
	uart_trans_buff (25);

	#endif
	*/
}

/*static void print_dec(uint16_t datad, uint8_t *buf) 
{
	int8_t bytes[5];
	int8_t i;
	for (i = 0; i < 5; i++) {
		bytes[i] = 0;
	}
	for (i = 0; i < 5; i++) {
		bytes[i] = datad % 10;
		datad /= 10;
		if (datad == 0) {
			break;
		}
	}	

	for (i = (5-1); i >= 0; i--) {
		*buf = bytes[i] + '0';
		buf++;
	}
}*/


//------------------------------------------------------------------------------
// конвертирует 2 байта по указателю из хекс (по 2 буквы чар) в 1 байт инт.
//------------------------------------------------------------------------------
/*static uint8_t HexToByte(uint8_t *p)
{
  uint8_t result,b;
  b = *p;
  if(b <= '9') b -= '0';
  else b = b - 'A'+10;
  result = b << 4;
  p++;
  b = *p;
  if(b <= '9') b -= '0';
  else b = b - 'A'+10;
  result |= b;
  return(result);
}*/

//------------------------------------------------------------------------------
// кладёт 1 байт в буффер в видде двух байт в перекодированном виде ASCII
// например: байт 0x3A   ->   [3][A]
//------------------------------------------------------------------------------
/*static void put_1byte_to_buff(uint8_t b, uint8_t *buf)
{
  uint8_t c;
  c = b >> 4;
  if (c <= 9) *buf = c + '0';      
  else *buf = c - 10 +'A';
  buf++;
  c = b & 0xF;
  if (c <= 9) *buf = c + '0';      
  else *buf = c - 10 +'A';
}*/

/*static void uart_put_2byte_to_buff(uint16_t b, uint8_t *buf)
{
	put_1byte_to_buff((b>>8), buf);
	buf+=2;
	put_1byte_to_buff((b & 0x00FF), buf);
}*/

//void prints(char *s)
//{
//  while(*s)
//  {
//    while ( !(UCSRA & (1<<UDRE)) );
//    RX_REGISTER = *s++;
//  }
//}

//static void tx_1hex (uint8_t c){
//    if (c <= 9) putch(c + '0');
//    else putch(c - 10 +'A');
//}

//void printb(unsigned char b)
//{
//    tx_1hex(b >> 4);
//    tx_1hex(b & 0xF);
//}
