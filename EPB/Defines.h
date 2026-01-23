
#include "Typedefs.h"

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE (!FALSE)
#endif


/* Select System Mode */
#define LAB_TEST_MODE			0 /* Lab test mode */
#define VEHICLE_RUN_MODE		1 /* Vehicle run mode, 성능시험기 */x
#define SIMULATOR_RUN_MODE		2 /* Simulator run mode */
#define BLEEDING_MODE			3
#define RUN_TYPE				(LAB_TEST_MODE)

/* Select ECU Mode  */
#define NORMAL_MODE				0
#define BOARD_TEST_MODE			1
#define PERFORMANCE_TEST_MODE	2 /* 시험기 */
#define EPB_OP_MODE				(NORMAL_MODE)

#define MOTOR_TYPE_INFAC 		(0U)
#define MOTOR_TYPE_DAEDONG		(1U)
#define MOTOR_TYPE 				(MOTOR_TYPE_DAEDONG)


