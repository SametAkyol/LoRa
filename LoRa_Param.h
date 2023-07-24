/*
 * LoRa_Param.h
 *
 *  Created on: 15 May 2023
 *      Author: Samet
 */

#ifndef INC_LORA_PARAM_H_
#define INC_LORA_PARAM_H_

#define recvBufferSize 4

typedef enum {
	Baud_1200,
	Baud_2400,
	Baud_4800,
	Baud_9600,
	Baud_38400,
	Baud_57600,
	Baud_115200
} BaudRate_e;


typedef enum {
	ms_500,
	ms_1000,
	ms_1500,
	ms_2000,
	ms_2500,
	ms_3000,
	ms_3500,
	ms_4000
}WOR_Cycle_e; 


typedef enum {
	dBm22, dBm17, dBm13, dBm10
} Tx_Power_e;

typedef enum{
	Disable,
	Enable=1
}Enable_val_e;

typedef enum{
  bytes_200,
  bytes_128,
  bytes_64,
  bytes_32,
}Sub_Packet_e;

typedef enum{
 Method_Transparent,
 Method_Fixed
}Tx_Method_e;

typedef struct {
	uint8_t BaudRate :3;
	uint8_t Parity :2;
	uint8_t AirDataRate :3;
	uint8_t set() {
		return (BaudRate << 5 | Parity << 3 | AirDataRate);
	}
} REG_0;

typedef struct {
	uint8_t Sub_Packet :2;
	uint8_t RSSI_Noise :1;
	uint8_t Reserved :3;
	uint8_t Tx_Power :2;
	uint8_t set() {
		return (Sub_Packet << 6) | (RSSI_Noise << 5) | (Reserved << 2)
				| Tx_Power;
	}
} REG_1;

typedef struct {
	uint8_t CH;
} REG_2;

typedef struct {
	uint8_t RSSI_Byte :1;
	uint8_t Tx_Method :1;
	uint8_t Reserved :1;
	uint8_t LBT :1;
	uint8_t Reserved2 :1;
	uint8_t WOR_Cycle :3;
	uint8_t set() {
		return ((RSSI_Byte << 7) | (Tx_Method << 6) | (Reserved << 5)
				| (LBT << 4) | (Reserved2 << 3) | WOR_Cycle);
	}
} REG_3;

typedef union {
	uint16_t CRYPT;
	struct {
		uint8_t CRYPT_H;
		uint8_t CRYPT_L;
	};
} CRYPT_REG;

typedef enum {
	Parity_NONE, Parity_ODD, Parity_EVEN,
} Parity;

typedef enum {
	Air_2k4_0,
	Air_2k4_1,
	Air_2k4_Default,
	Air_4k8,
	Air_9k6,
	Air_19k2,
	Air_38k4,
	Air_62k5,
} AirDataRate;

typedef enum Lora {
	Status_Error, Status_OK
} LoRa_Status;

typedef union {
	struct {
		uint8_t ADDRL;
		uint8_t ADDRH;
	};
	uint16_t ADDRX;
} ADDR;

typedef enum {
	Mode_0, Mode_1, Mode_2, Mode_3
} LoRa_Mode;

typedef struct {
	REG_0 REG0;
	REG_1 REG1;
	REG_2 REG2;
	REG_3 REG3;
	ADDR ADDRESS;
	CRYPT_REG cryptoreg;
} LoRa_Init_t;




typedef struct {
	bool led1;
	bool led2;
	bool led3;
	bool led4;
}ewe;

#endif /* INC_LORA_PARAM_H_ */
