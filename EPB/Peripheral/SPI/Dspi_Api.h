
#include "Typedefs.h"


void initDSPI1(void);
void SPI_EEP_Send_X16(ubyte Address, uint16_t Data, ubyte cmd);
uint16_t SPI_EEP_Read_X16(ubyte Address);
void SpiInitialize(void);
extern void CANSBCInit(void);
extern void ExtEEPROMInit(void);


