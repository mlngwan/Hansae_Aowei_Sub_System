#include "Typedefs.h"


typedef struct {
	uint16 received_data;
	unsigned int fMainfault :  1;
	uint32_t MissingCnt;
}TC277_t;

typedef union {
	uint16 data16;
	struct {
		unsigned int data	: 10;
		unsigned int addr	: 5;
		unsigned int r_w 	: 1;
	} field;
} TC277_MOSI;

typedef union {
	uint16 data16;
	struct {
		unsigned int data	: 8;
		unsigned int status	: 8;
	} field;	
} TC277_MISO;

uint16_t TC277_Response(void);
void CheckTC277Status(void);
extern void GetFailSafeStatus(void);

