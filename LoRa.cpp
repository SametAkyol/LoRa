/*
 * LoRa.cpp
 *
 *  Created on: May 15, 2023
 *      Author: Samet
 */

#include "LoRa.h"

LoRa::LoRa(UART_HandleTypeDef *huart, uint16_t M0_pin, uint16_t M1_pin) :
		huartx(huart), pin_M0(M0_pin), pin_M1(M1_pin) {
}

LoRa_Status LoRa::writeCommand(uint8_t *cmd, int len) {
	if (HAL_OK != HAL_UART_Transmit(huartx, cmd, len, 50)) {
		return Status_Error;
	}
	return Status_OK;
}

LoRa_Status LoRa::readCommand(uint8_t *cmd, uint8_t *buffer, int len) {
	writeCommand(cmd, 3);
	if (HAL_OK != HAL_UART_Receive(huartx, buffer, len, 100)) {
		return Status_Error;
	}
	return Status_OK;
}

LoRa_Status LoRa::Initialize(LoRa_Init_t &cfg) {
	uint8_t buffer[11]{ 0 };
	buffer[0] = 0xC0; //write command
	buffer[1] = 0x00; //register base address
	buffer[2] = 0x08; //length
	buffer[3] = cfg.ADDRESS.ADDRH; //address hgh register
	buffer[4] = cfg.ADDRESS.ADDRL; //address low register
	buffer[5] = cfg.REG0.set(); //
	buffer[6] =cfg.REG1.set();
	buffer[7] =cfg.REG2.CH;
	buffer[8] =cfg.REG3.set();
	buffer[9] =cfg.cryptoreg.CRYPT_H;
	buffer[10] =cfg.cryptoreg.CRYPT_L;
	if (Status_OK != writeCommand(buffer, 11)) {
		return Status_Error;
	}
	return Status_OK;
}

LoRa_Status LoRa::sendData_Fixed(uint16_t targetAddr, uint8_t targetChannel,
		uint8_t* pData,uint16_t size) {

	uint8_t targetInfo[3]{ 0 };

	targetInfo[0] = targetAddr >> 8;
	targetInfo[1] = targetAddr & 0x00FF;
	targetInfo[2] = targetChannel;

	
	if (txStatus==false)
	{
		HAL_UART_Transmit(huartx, targetInfo, sizeof(targetInfo), 1);
		HAL_UART_Transmit_IT(huartx, pData, size);
		txStatus = true;
	}
	return Status_OK;
}

LoRa_Status LoRa::sendData_Broadcasting(uint8_t targetChannel, uint8_t* pData, uint16_t size) {
	uint8_t targetInfo[3]{ 0 };
	targetInfo[0] = 0xFF;
	targetInfo[1] = 0xFF;
	targetInfo[2] = targetChannel;
	
	if (txStatus==false)
	{
		HAL_UART_Transmit(huartx, targetInfo, sizeof(targetInfo), 1);
		HAL_UART_Transmit_IT(huartx, pData, size);
		txStatus = true;
		return Status_OK;
	}
	return Status_Error;
	
}

LoRa_Status LoRa::readData() {
	HAL_UART_Receive_IT(huartx, recvbuffer, recvBufferSize);
	return Status_OK;
}

void LoRa::setMode(LoRa_Mode mode) {
	 switch (mode) {
	 case Mode_0:
	 HAL_GPIO_WritePin(GPIOA, pin_M0, GPIO_PIN_RESET);
	 HAL_GPIO_WritePin(GPIOA, pin_M1, GPIO_PIN_RESET);
	 break;
	 case Mode_1:
	 HAL_GPIO_WritePin(GPIOA, pin_M0, GPIO_PIN_SET);
	 HAL_GPIO_WritePin(GPIOA, pin_M1, GPIO_PIN_RESET);
	 break;
	 case Mode_2:
	 HAL_GPIO_WritePin(GPIOA, pin_M0, GPIO_PIN_RESET);
	 HAL_GPIO_WritePin(GPIOA, pin_M1, GPIO_PIN_SET);
	 break;
	 case Mode_3:
	 HAL_GPIO_WritePin(GPIOA, pin_M0, GPIO_PIN_SET);
	 HAL_GPIO_WritePin(GPIOA, pin_M1, GPIO_PIN_SET);
	 break;
	 default:
	 break;
	 }
	 HAL_Delay(2);
}


bool LoRa::sendStruct_Fixed(uint16_t targetAddress, uint8_t channel,void * obj,const uint16_t sizeOfstruct) {
	
	if (obj == NULL)
	{
		return false;
	}
	uint8_t* pStruct = reinterpret_cast<uint8_t*>(obj);
	LoRa_Status status= sendData_Fixed(targetAddress, channel, pStruct, sizeOfstruct);
	return (status==Status_OK);

}


bool LoRa::sendStruct_BroadCasting(uint16_t targetAddress, uint8_t channel, void* obj, const uint16_t sizeOfstruct) {

	if (obj == NULL)
	{
		return false;
	}

	uint8_t* pStruct = reinterpret_cast<uint8_t*>(obj);
	LoRa_Status status = sendData_Broadcasting( channel, pStruct, sizeOfstruct);
	return (status == Status_OK);
}

