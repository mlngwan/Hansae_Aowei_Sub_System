#include "Typedefs.h"

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE (!FALSE)
#endif

#define EEPROM_BITSIZE_11	 11U
#define EEPROM_BITSIZE_9	9U

#define ERAL_CMD    0x90        // ERAL command(Erase All command)
#define EWEN_CMD    0x98        // EWEN command(Write enable command)
#define EWDS_CMD    0x80        // EWDS command
#define WRITE_CMD   0xa0        // WRITE command
#define READ_CMD    0xc0        // READ command

/* Detail information is located in reference manual(From middle of page 460). */
#define PUSHR_CONT	0x80000000			// Continuous peripheral chip select enable
#define PUSHR_EOQ		0x08000000			// End Of Queue
#define PUSHR_ATTR	0x00010000			//CTAR(Clock and Transfer Attribytes Select)
																		//PCS0 Selected
#define PUSHR_CTCNT	0x04000000			// Clear SPI_TCNT
#define	All_PUSHR	(PUSHR_CONT|PUSHR_EOQ|PUSHR_ATTR|PUSHR_CTCNT)
#define	TatooEeprom_Add					0x00
#define EpbStatusLeft_Add				0x01
#define EpbStatusRight_Add				0x02			
#define EEPROM_TEST_1					0x03	
#define EEPROM_TEST_2					0x04		
#define UDS_ResetECU_CheckAdd			0x06					//1 Be careful!!! Must be matched with Bootloader UDS_ResetECU_CheckAdd.
#define	RxOpMode_Add					0x07
#define	EEP_BootWriteAdd				0x0E

#define	Fingerprint_AddOffset		0x10
#define	Fingerprint0_Add		(Fingerprint_AddOffset+1)
#define	Fingerprint1_Add		(Fingerprint_AddOffset+2)
#define	Fingerprint2_Add		(Fingerprint_AddOffset+3)
#define	Fingerprint3_Add		(Fingerprint_AddOffset+4)
#define	Fingerprint4_Add		(Fingerprint_AddOffset+5)

#define	VIN_AddressOffset		0x16					// VIN size 17byte(From 0x16 to 0x1E)
#define DTC_StoreAddOffset	0x20					// Please check DEM_DTC_NUMBER define for DTC size(current is 58U)

typedef struct{
	uint8 SaveWritingStep;	
	uint16 SaveParamCount;
	uint16 SaveFlag;
	uint32_t EEP_ReadData[3];
	uint8 Eeprom_BitSize;
	uint16 SizeCheckCount;
	uint8 SizeCheckStep;
	unsigned int fEepromTimeOut : 1;
}ExtEEPROM_t;


void EepromReadAll(void);
void ApplicationStoredDataRead(void);
void EepromMemorySizeCheck(void);
void Delayms(uint32_t time);
void ExtEEPROMInit(void);

void SaveParamData(void);
extern ubyte Temporary_Failure_Check(ubyte dtcIdx);

