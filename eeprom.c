#include <SH79F084B.h>
#include <intrins.h>
#include "globalvar.h"
#include "eeprom.h"

static bit EA_temp;

//========================================================================================
// Читает из еепром байт данных по адресу eaddr.
//========================================================================================
uint8_t EEPROM_read_byte(uint16_t eaddr)
{
	uint8_t nTemp;
	
	FLASHCON=0x01; 
	nTemp = (*(uint8_t code *)(eaddr));
	FLASHCON=0x00;
	return (nTemp);
}

//========================================================================================
// Пишет в еепром байт данных по адресу eaddr в сектора sect = 512байт.
// В данном варианте закомментированные регистры не меняет!
// это сделано для ускорения операций с буферной записью, т.к. по одному байту
// записывать в данной реализации не предполагаю
//========================================================================================
void EEPROM_write_byte(uint16_t eaddr, uint8_t edata)
{
	EA_temp=EA;
	EA=0;
	FLASHCON =	0x01; 		//Operate EEPROM area
	IB_DATA	=	edata; 		//Write edata
	IB_OFFSET = (uint8_t)eaddr;
	if (eaddr>255) {XPAGE = 0x01;}
	else {XPAGE = 0;}
	IB_CON1 = 0x6E; 		//Write operation
	IB_CON2 = 0x05;
	IB_CON3 = 0x0A;
	IB_CON4 = 0x09;
	IB_CON5 = 0x06;
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_();
	XPAGE=0x00;
	FLASHCON =	0x00; 
	ext_int_dis = 1;
	EA = EA_temp;
}

//========================================================================================
// стирает номер сектора в еепром.
//========================================================================================
void EEPROM_sector_erase (uint16_t eaddr)
{
	EA_temp=EA;
	EA=0;
	FLASHCON = 0x01 ;
	if (eaddr>255) {XPAGE = 0x01;}
	else {XPAGE = 0;}
	IB_CON1  = 0xE6 ; 
	IB_CON2  = 0x05 ;
	IB_CON3  = 0x0A ;
	IB_CON4  = 0x09 ;
	IB_CON5  = 0x06 ;
	_nop_() ;       
	_nop_() ;
	_nop_() ;
	_nop_() ; 
	XPAGE=0x00;
	FLASHCON =	0x00; 
	ext_int_dis = 1;
	EA = EA_temp;
}

void EEPROM_DataInit( void )
{
	if (EEPROM_read_byte(0) > 0){
		cepb = 1;
	}else{
		cepb = 0;
	}
}

