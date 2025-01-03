#include "i2c_helper.h"
#include "stm32g0xx_ll_dma.h"
#include "main.h"
#include <string.h>

uint8_t i2cDmaTxBuff1[DMA_TX_BUFF_LEN_BYTES];
uint8_t i2cDmaRxBuff1[DMA_RX_BUFF_LEN_BYTES];

/* Start I2C function.
 * @param I2Cx 	   Selected I2C
 */
void I2C_Start (I2C_TypeDef *I2Cx)
{
//	LL_I2C_AcknowledgeNextData(I2Cx, LL_I2C_ACK); // Enable the ACK
//  LL_I2C_GenerateStartCondition(I2Cx);          // Generate START
////  while(!LL_I2C_IsActiveFlag_SB(I2Cx)){};
//  while(!LL_I2C_IsActiveFlag_BUSY(I2Cx)){};
}

/* Stop I2C function.
 * @param I2Cx 	   Selected I2C
 */
void I2C_Stop (I2C_TypeDef *I2Cx)
{
//	LL_I2C_GenerateStopCondition(I2Cx);  // Stop I2C
}

/* Only write byte I2C function.
 * @param I2Cx 	   Selected I2C
 */
void I2C_Write (I2C_TypeDef *I2Cx, uint8_t data)
{
//	while(!LL_I2C_IsActiveFlag_TXE(I2Cx)){};    // wait for TXE bit to set
//	LL_I2C_TransmitData8(I2Cx, data);
////	while (!LL_I2C_IsActiveFlag_BTF(I2Cx)) {};  // wait for BTF bit to set
//	while (!LL_I2C_IsActiveFlag_TCR(I2Cx)) {};  // wait for BTF bit to set
}

/* Multy write byte I2C function.

 * @param I2Cx 	   Selected I2C
 * @param data 	   pointer to data
 * @param size 	   size of data

 */
void I2C_MultWrite (I2C_TypeDef *I2Cx, uint8_t Address, uint8_t Reg, uint8_t *data, uint16_t size)
{
//	uint16_t i;
//  while(!LL_I2C_IsActiveFlag_TXE(I2Cx)){};     //wait for TXE bit to set
//  for(i = 0;i < size; i++)
//  {
//    LL_I2C_TransmitData8(I2Cx, data[i]);       //send data
//    while(!LL_I2C_IsActiveFlag_TXE(I2Cx)){};   //wait for TXE bit to set
//  }
	LL_I2C_EnableIT_NACK(I2C1);
	LL_I2C_EnableIT_STOP(I2C1);
	LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_1);
	LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_1, (uint32_t)&i2cDmaTxBuff1, LL_I2C_DMA_GetRegAddr(I2C1, LL_I2C_DMA_REG_DATA_TRANSMIT), LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_CHANNEL_1));
	LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_1);
	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_1, size+1);
//	i2cDmaTxBuff1[0] = Reg;
//	i2cDmaTxBuff1[1] = Data;
	i2cDmaTxBuff1[0] = Reg;
	memcpy(i2cDmaTxBuff1+sizeof(uint8_t), data, size+1);
	isPrevTransferFinished = 0;
	LL_I2C_EnableDMAReq_TX(I2C1);
	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);
	LL_I2C_HandleTransfer(I2C1, Address, LL_I2C_ADDRSLAVE_7BIT, size+1, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_WRITE);


	LL_I2C_EnableDMAReq_TX(I2C1);
	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);
}

/* Choise Slave Adress I2C function.

 * @param I2Cx 	   Selected I2C
 */
void I2C_Address (I2C_TypeDef *I2Cx,uint8_t Address)
{
	//LL_I2C_SetSlaveAddr(I2Cx, Address);

	//LL_I2C_TransmitData8(I2Cx, Address);      //  send the address
	//while(!LL_I2C_IsActiveFlag_ADDR(I2Cx)){}; // wait for ADDR bit to set
	//LL_I2C_ClearFlag_ADDR(I2Cx);              // read SR1 and SR2 to clear the ADDR bit
}

/* Only read from slave I2C function.
 * @param I2Cx 	   Selected I2C

 * @param Address  I2C slave device address
 * @param buffer 	 Data from register

 * @param size 		 How many bytes will read
 */
void I2C_Read (I2C_TypeDef *I2Cx, uint8_t Address, uint8_t *buffer, uint8_t size)
{	
	
	int remaining = size;
	I2C_Address(I2Cx, Address);                           //send the address device
	
	if (size == 1)
	{
		LL_I2C_AcknowledgeNextData(I2Cx, LL_I2C_NACK);      // clear the ACK bit 
		LL_I2C_GenerateStopCondition(I2Cx);                 // Stop I2C
		while(!LL_I2C_IsActiveFlag_RXNE(I2Cx)){};           // wait for RxNE to set
		buffer[size-remaining] = LL_I2C_ReceiveData8(I2Cx); // Read the data from the DATA REGISTER
	}	
	else 
	{
		while (remaining > 2)
		{
			while(!LL_I2C_IsActiveFlag_RXNE(I2Cx)){};          // wait for RxNE to set
			buffer[size-remaining] = LL_I2C_ReceiveData8(I2Cx);// copy the data into the buffer			
			LL_I2C_AcknowledgeNextData(I2Cx, LL_I2C_ACK);      // Set the ACK bit to Acknowledge the data received
		
			remaining--;
		}
		while(!LL_I2C_IsActiveFlag_RXNE(I2Cx)){};          // wait for RxNE to set
		buffer[size-remaining] = LL_I2C_ReceiveData8(I2Cx);// copy the data into the buffer		
		LL_I2C_AcknowledgeNextData(I2Cx, LL_I2C_NACK);     // clear the ACK bit 
		LL_I2C_GenerateStopCondition(I2Cx);                // Stop I2C
		remaining--;
		
		// Read the Last BYTE
		while(!LL_I2C_IsActiveFlag_RXNE(I2Cx)){};          // wait for RxNE to set
		buffer[size-remaining] = LL_I2C_ReceiveData8(I2Cx);// copy the data into the buffer
	}	
}

/** Write 8-bit to device register.
 * @param I2Cx 	   Selected I2C

 * @param Address  I2C slave device address
 * @param Reg   	 In reg write data

 * @param Data 		 Data write into register
 */
void I2C_WriteData(I2C_TypeDef *I2Cx, uint8_t Address, uint8_t Reg, uint8_t Data)
{

//	LL_I2C_Disable(I2C1);
//	LL_I2C_DisableAutoEndMode(I2C1);
//	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_1, 2);
////	LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_1, (uint32_t)&i2cDmaTxBuff1);
////	LL_DMA_SetPeriphAddress(DMA1, LL_DMA_CHANNEL_1, (uint32_t)&I2C1->TXDR);
//	i2cDmaTxBuff1[0] = Reg;
//	i2cDmaTxBuff1[1] = Data;
//	LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_1, (uint32_t)&i2cDmaTxBuff1, LL_I2C_DMA_GetRegAddr(I2C1, LL_I2C_DMA_REG_DATA_TRANSMIT), LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_CHANNEL_1));
//
//	LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_1);
//	I2C_Address (I2Cx, Address);
//	isPrevTransferFinished = 0;
//	LL_I2C_Enable(I2C1);
//	I2C_Start (I2Cx);
//	i2cDmaTxBuff1[0] = Reg;
//	i2cDmaTxBuff1[1] = Data;
//	while(LL_I2C_IsActiveFlag_TC(I2Cx) == 0){
////		wait for dev addr to transfer
//	}
//	LL_mDelay(1);
//	LL_I2C_EnableDMAReq_TX(I2C1);
//	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);


	LL_I2C_EnableIT_NACK(I2C1);
	LL_I2C_EnableIT_STOP(I2C1);
	LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_1);
	LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_1, (uint32_t)&i2cDmaTxBuff1, LL_I2C_DMA_GetRegAddr(I2C1, LL_I2C_DMA_REG_DATA_TRANSMIT), LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_CHANNEL_1));
	LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_1);
	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_1, 2);
	i2cDmaTxBuff1[0] = Reg;
	i2cDmaTxBuff1[1] = Data;
	isPrevTransferFinished = 0;
	LL_I2C_EnableDMAReq_TX(I2C1);
	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);
	LL_I2C_HandleTransfer(I2C1, Address, LL_I2C_ADDRSLAVE_7BIT, 2, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_WRITE);


	LL_I2C_EnableDMAReq_TX(I2C1);
	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);


//	I2C_Write (I2Cx, Reg);
//	I2C_Write (I2Cx, Data);
//	I2C_Stop (I2Cx);
}

/** Multy Write 8-bit to device register.
 * @param I2Cx 	   Selected I2C
 * @param Address  I2C slave device address

 * @param Reg   	 In reg write data
 * @param Data 		 Data write into register

 */
void I2C_MultWriteData(I2C_TypeDef *I2Cx, uint8_t Address, uint8_t Reg, uint8_t *Data, uint16_t size)
{
//	I2C_Start (I2Cx);
//	I2C_Address (I2Cx, Address);
//	I2C_Write (I2Cx, Reg);
	while(!isPrevTransferFinished){
		//wait for prev transfer to stop
	}
	I2C_MultWrite(I2Cx, Address, Reg, Data, size);
//	I2C_Stop(I2Cx);
}

/** Read from device register.
 * @param I2Cx 	   Selected I2C
 * @param Address  I2C slave device address

 * @param Reg   	 Register to read from
 * @param buffer 	 Data from register

 * @param size 		 How many bytes will write
 */
void I2C_ReadData(I2C_TypeDef *I2Cx, uint8_t Address, uint8_t Reg, uint8_t *buffer, uint8_t size)
{
	I2C_Start (I2Cx);
	I2C_Address (I2Cx, Address);
	I2C_Write (I2Cx, Reg);
	I2C_Start (I2Cx);
	I2C_Read (I2Cx, Address+0x01, buffer, size);
	I2C_Stop (I2Cx);
}
