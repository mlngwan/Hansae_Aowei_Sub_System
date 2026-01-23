#ifndef BOARD_PUB_H
#define BOARD_PUB_H

/*============================== PUBLIC DEFINITIONS ==============================*/
/*                                                                                */
/*--------------------------------------------------------------------------------*/
/* - Public Function Prototypes                                                   */

/** \brief Realize Board initial startup conditions for correct main execution
 **
 ** This function shall call :
 **         1- BoardWatchdogDeactivation();
 **         2- BoardDisableInterrupts();
 **         3- BoardStartupClocks();
 **         4- BoardInitializationTimers(u16 uwTime);
 **         5- BoardPortsSettings();
 **         6- BoardInitInterrupts();
 **         7- BoardEnableInterrupts();
 **
 **/
extern void BoardStart(void);


/** \brief This function is used to configure and start the clocks (oscillators, PLLs)
 **
 **
 **/
extern void BoardStartupClocks(void);

/** \brief Board initialize timers
 **
 ** This function is used to set to configure and start the timers
 ** \param[in] uwTimer timer value.
 ** \return RetVal
 ** \retval BOARD_TRUE Timer configuration successful
 ** \retval BOARD_FALSE Timer configuration failed
 **
 **/
extern uint8 BoardInitializationTimers(uint16 uwTimer);


/** \brief Scheduling Timer
 **
 ** used to have a time reference (ex: flag is set every uwTimer, parameter passed to BoarsInitialisationTimers(uwTimer))
 ** \return ubFlag
 **
 **/
extern uint8 BoardSchedulingTimer(void);


/** \brief Port settings
 **
 ** initialize the port for the project 
 ** (depending on the project, it can be Can/Lin/Flexray etc..)
 **
 **/
extern void BoardPortsSettings(void);


/** \brief Interrupt initialization
 **
 ** This function is used to initialize the interrupt controller
 ** and sets a default priority in all interrupt vectors,
 ** integrator shall ensure that each integrated software using interrupts
 ** shall verify the interrupt settings or set them to a correct value
 **
 **/
extern void BoardInitInterrupts(void);


/** \brief Invalid interrupts handler
 **
 ** This function is necessary to catch all uninitialized interrupts that would be unexpectedly triggered
 ** \param[in] id is the interrupt identification number.
 **
 **/
extern void BoardInvalidInterrupt(uint32 id);


/** \brief Invalid exceptions handler
 **
 ** This function is necessary to catch unhandled exceptions (for example ECC ! )
 ** \param[in] id is the exception id.
 ** \param[in] sp is current stack pointer.
 **
 **/
extern void BoardInvalidException(uint32 id, uint32 sp);


/** \brief Interrupt enabling
 **
 ** This function is used to manage interruptions enabling by
 ** taking into consideration the interrupt nesting
 **
 **/
extern void BoardEnableInterrupts(void);


/** \brief Interrupt disabling
 **
 ** This function is used to manage interruptions disabling by
 ** taking into consideration the interrupt nesting
 **
 **/
extern void BoardDisableInterrupts(void);


/** \brief Perform reset
 **
 ** This function is used to generate a reset of type passed in paramater.
 ** This is necessary to generate a reset to switch to/from Application/Bootlaoder
 ** \param[in] ubResetType reset type :
 **                 - PLATFORMS_SW_RESET: software reset
 **                 - PLATFORMS_POWER_ON_RESET: power on reset
 **                 - PLATFORMS_RESET_UNDEFINED: other reset
 **
 **/
extern void BoardPerformReset(Platforms_ResetType ubResetType);


/** \brief Get last reset type
 **
 ** This function is used to get the reset reason
 ** \return Reset type
 ** \retval PLATFORMS_SW_RESET: software reset
 ** \retval PLATFORMS_POWER_ON_RESET: power on reset
 ** \retval PLATFORMS_RESET_UNDEFINED: other reset
 **
 **/
extern Platforms_ResetType BoardGetResetType(void);


/** \brief Change MCU Core state
 **
 ** This function is used to set state of the CPU.
 ** In case of SLEEP or STANDY, an infinite loop is set to
 ** wait for MCU to go to sleep
 ** \param[in] ubState CPU state:
 **                 - PLATFORMS_MCU_SLEEP
 **                 - PLATFORMS_MCU_STANDBY
 **                 - PLATFORMS_MCU_IDLE
 **
 **/
extern void BoardSetState(Platforms_PowerType ubState);


/** \brief Get random number
 **
 ** This function is used to get a pseudo-random value
 ** \return ulBoardRandom : 32bits pseudo-random value
 **
 **/
extern uint32 BoardGetRandom(void);


/** \brief Board initialization
 **
 ** This function performs platform specific pre-initialization before clear
 ** Data/Ram calls in startup, copy data section, init of PLL and branch to main
 ** 
 ** BoardInit() is called by the startup code (cstartup.s) after data and bss
 ** sections have been initialized (global variables may now be used).
 ** The BoardInit() can be used to start up drivers etc.
 **
 **/
extern void BoardInit(void);


/** \brief Watchdog deactivation
 **
 ** This function is used to deactivate the watchdog
 **
 **/
extern void BoardWatchdogDeactivation(void);


/** \brief Led initialization
 **
 ** Initalize the LED used on the demoboard (can be 1 or more LEDs depending on usage)
 **
 **/
extern void BoardInitLed(void);


/** \brief Toggle led
 **
 ** This function is used to control LED (can be used
 ** to toggle a LED in the main while{} loop to proove SW running
 ** \param[in] ubLed : value of the LED (OFF : 0b, ON: 1b).
 **
 **/
//1 extern void BoardToggleLed(uint8 ubLed);  //Commented by Gabriel because unintended PC jump.
/*                                                                               */
/*===============================================================================*/


#endif      /* BOARD_PUB_H    */
