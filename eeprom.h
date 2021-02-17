/* 
 * File:   EEPROM.h
 * Author: DeAd_MorOz
 *
 * Created on 29 Октября 2019 г., 9:47
 */

#ifndef EEPROM_H
#define	EEPROM_H

#ifdef	__cplusplus
extern "C" {
#endif

void EEPROM_write_byte(uint16_t eaddr, uint8_t edata);
void EEPROM_sector_erase (uint16_t eaddr);	
void EEPROM_DataInit (void);

#ifdef	__cplusplus
}
#endif

#endif	/* EEPROM_H */

