/*
 * LoRa.h
 *
 *  Created on: May 15, 2023
 *      Author: Samet
 */

#ifndef INC_LORA_H_
#define INC_LORA_H_
#include "main.h"
#include "LoRa_Param.h"
class LoRa {
public:
	volatile bool txStatus;
	volatile bool rxStatus;
	LoRa(UART_HandleTypeDef *huart, uint16_t M0_pin, uint16_t M1_pin);
	LoRa_Status writeCommand(uint8_t *cmd,int len);
	LoRa_Status readCommand(uint8_t *cmd, uint8_t *buffer, int len);
	LoRa_Status Initialize(LoRa_Init_t &);
	LoRa_Status sendData_Fixed(uint16_t targetAddr, uint8_t targetChannel,uint8_t* pData, uint16_t size);
	LoRa_Status sendData_Broadcasting(uint8_t targetChannel, uint8_t* pData, uint16_t size);
	LoRa_Status readData();
	bool sendStruct_Fixed(uint16_t targetAddress, uint8_t channel, void* strct,  uint16_t sizeOfstruct);
	bool sendStruct_BroadCasting(uint16_t targetAddress, uint8_t channel, void* strct,  uint16_t sizeOfstruct);
	void setMode(LoRa_Mode mode);
	uint8_t recvbuffer[recvBufferSize];
private:
	UART_HandleTypeDef *huartx;
	uint16_t pin_M0, pin_M1;

protected:
	
};

#endif /* INC_LORA_H_ */
