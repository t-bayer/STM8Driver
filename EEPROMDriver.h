#ifndef _EEPROM_Driver
#define _EEPROM_Driver

void EEPROM_Init(char address);
void EEPROM_Read(uint16_t address, char* ptr);
void EEPROM_Write (char data, uint16_t address);

#endif