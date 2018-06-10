/*************************************************************/
/**
 *  @file EEPROMDriver_main.c
 *  @brief Simple example for using the EEPROM driver
 *  @author Timo Bayer
 *
 *  @date 25.05.2016
 *  @version 1.0
 *************************************************************/
#include "stm8s.h"
#include "eepromDriver_main.h"

void GPIO_Configuration(void);

uint32_t delayCounter;

void main(void){
	uint16_t i;
	uint8_t result = 50;
	GPIO_Configuration();
	EEPROM_Init(0xA0);
	EEPROM_Write(4, 0x1000);
	for(delayCounter=0; delayCounter<0x0FFF;delayCounter++);
	EEPROM_Read(0x1000, &result);
		
  for (i = 0; i < result; i++){
		GPIO_WriteReverse(GPIOD, GPIO_PIN_0);
		for(delayCounter=0; delayCounter<0xFFFF;delayCounter++);
  } 
}

void GPIO_Configuration(void){
  GPIO_DeInit(GPIOD);
  GPIO_Init(GPIOD, GPIO_PIN_0, GPIO_MODE_OUT_PP_LOW_FAST);
}
