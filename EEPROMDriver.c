/*************************************************************/
/**
 *  @file EEPROMDriver.c
 *  @brief Simple driver for using a EEPROM over I2C on STM8
 *  @author Timo Bayer
 *
 *  @date 25.05.2016
 *  @version 1.0
 *************************************************************/
#include "stm8s.h"

static char deviceAddress;
static uint32_t delayCounter;

/**
 * @brief Initialises the EEPROM
 *
 * This function initialises the EEPROM by setting the device address and the TWI registers
 *
 * @param[in] data Specifies the device address
 */
void EEPROM_Init(char address){
	uint8_t Input_Clock = 0;
	deviceAddress = address;
	Input_Clock = CLK_GetClockFreq()/1000000;
	I2C_DeInit();
	I2C_Init(100000, 0xFF, I2C_DUTYCYCLE_2, I2C_ACK_CURR, I2C_ADDMODE_7BIT, Input_Clock);
	I2C_Cmd(ENABLE);
}

/**
 * @brief Writes a byte on the EEPROM
 *
 * This function writes a byte on the specified EEPROM address
 *
 * @param[in] data Specifies the data to be stored
 * @param[in] address Specifies the target address 
 */
void EEPROM_Write (char data, uint16_t address){
	I2C_GenerateSTART(ENABLE);
	while(!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(deviceAddress, I2C_DIRECTION_TX);
	while(!I2C_CheckEvent( I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	I2C_SendData(address & 0xFF);
	while(!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_SendData(address >> 8);
	while(!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_SendData(data);
	while(!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_GenerateSTOP(ENABLE);
}

/**
 * @brief Reads a byte from the EEPROM
 *
 * This function reads a byte from the EEPROM on the specified address
 *
 * @param[in] address The address to be read
 * @param[out] ptr Contains the data which is read from the EEPROM
 */
void EEPROM_Read(uint16_t address, char* ptr){
	I2C_GenerateSTART(ENABLE);
	while(!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(deviceAddress, I2C_DIRECTION_TX);
	while(!I2C_CheckEvent( I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	I2C_SendData(address & 0xFF);
	while(!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_SendData(address >> 8);
	while(!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_GenerateSTOP(ENABLE);
	while(I2C->SR3 & I2C_SR3_BUSY);
	I2C_GenerateSTART(ENABLE);
	while(!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(deviceAddress, I2C_DIRECTION_RX);
	while(!I2C_CheckEvent( I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	for(delayCounter=0; delayCounter<0xFFFF;delayCounter++);
	*ptr = I2C_ReceiveData();
	for(delayCounter=0; delayCounter<0xFFFF;delayCounter++);
	I2C_GenerateSTOP(ENABLE);
}