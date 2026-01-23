/* --------{ EB Automotive C Source File }-------- */

/*
 * !LINKSTO OsekCore.Platforms.Board.012, 1
 */

/*define to exclude C Code in header files*/
#define BOARD_ASM    1

/* Must be first */
#include <Platforms_asm_PA.h>
#include <board.h>


	Platforms_file(cstartup.s)

#define BOOT_WTE		0x04		/* Watchdog Timer enable */
#define BOOT_PS0		0x02		/* Port size for external boot mode */
#define BOOT_FLAGS		0x00		/* May contain BOOT_WTE and BOOT_PS0 */
#define BOOT_IDENTIFIER 0x5A    /* Magic Number for the BAM */
#define BOOT_VLE 0x01    /* Magic Number for the BAM */



#define STARTUP_MSR		0x21200		/* The MSR at startup: CE|ME|DE */


  Platforms_global	__entry

  Platforms_extern	BoardInit
  Platforms_extern	main
  Platforms_extern  BoardStart
#if (BOARD_INIT_SRAM == BOARD_TRUE)
  Platforms_global  _InitRam
#endif
  /* These symbols are defined in the linker script
  */
  Platforms_extern   _BSS_START_
  Platforms_extern   _BSS_END_
  Platforms_extern   _DATA_START_
  Platforms_extern   _DATA_END_
  Platforms_extern   _INIT_
  Platforms_extern   _SDA_BASE_
  Platforms_extern   _SDA2_BASE_
  Platforms_extern   __SP_INIT
  Platforms_extern   _EXCEPTION_TABLE_START
  Platforms_extern   _INTC_TABLE_START
  Platforms_extern   __IVPR
/*
 * reset section
 *
 * Define RCHW (reset configuration half word) for BAM code.
 * The RCHW is a mark at a specific place in memory to tell the BAM (boot assist module) if there
 * exists an application ready to run after reset.
 *
*/
#if(PLATFORMS_TOOLCHAIN == PLATFORMS_mwerks)
PLATFORMS_ASM_SECTION_RESET
  Platforms_byte    BOOT_VLE,BOOT_IDENTIFIER,BOOT_FLAGS,BOOT_FLAGS
  PLATFORMS_ASM_VLE
  Platforms_long    __entry

  /* Startup code
   * The processor runs this code first after a reset.
   * The _start symbol is the default entry address for the Gnu linker
*/
PLATFORMS_ASM_SECTION_RESET_CODE
#else
  PLATFORMS_ASM_SECTION_RESET
  PLATFORMS_ASM_VLE



  Platforms_align(1,0)
  /*
   * !LINKSTO OsekCore.Platforms.Startup.001, 1
   * !LINKSTO OsekCore.Platforms.Startup.002, 1
   */
  PLATFORMS_ASM_SECTION_RESET_CODE
  Platforms_byte    BOOT_FLAGS,BOOT_IDENTIFIER,BOOT_FLAGS,BOOT_IDENTIFIER
  Platforms_long    __entry

  /* Startup code
   * The processor runs this code first after a reset.
   * The _start symbol is the default entry address for the Gnu linker
*/
#endif
__entry:

    /* Initialise the MSR
    *
    * !LINKSTO OsekCore.Platforms.Startup.004, 1
    */

  lis   r3, Platforms_hi(STARTUP_MSR)
  addi  r3, r3, Platforms_lo(STARTUP_MSR)
  mtmsr r3


#if (BOARD_INIT_CPU_REG == BOARD_TRUE)
  /*  Initialise all CPU registers. This is needed in lock step mode.
   *  If it is not done the processor could detect a difference between the two cores and report a failure.
   *
   * !LINKSTO OsekCore.Platforms.Startup.003, 1
   */
  bl    __CpuRegInitStart
#endif



#if (BOARD_WDG_USED == BOARD_TRUE)
  /* disable SWT watchdog
   *
   * !LINKSTO OsekCore.Platforms.Startup.005, 1
   */
  lis   r7, Platforms_hi(0xFF00000A)
  addi  r7, r7, Platforms_lo(0xFF00000A)
  lis   r4, Platforms_hi(0xFFF38000)
  addi  r4, r4, Platforms_lo(0xFFF38000)
  li    r3, Platforms_lo(0xc520)
  li    r2, Platforms_lo(0xd928)
  stw   r3, 0x10(r4)
  stw   r2, 0x10(r4)
  stw   r7, 0(r4)
#endif

  /* Add a default exception handler for all excpetions
   *
   * !LINKSTO OsekCore.Platforms.Startup.006, 1
   */
  bl    __SetupExceptions


#if (BOARD_CHECK_SECTION_BASEADDR == BOARD_TRUE)

  lis       r3, Platforms_hi(_EXCEPTION_TABLE_START)
  addi      r3, r3, Platforms_lo(_EXCEPTION_TABLE_START)
  e_and2i.  r3,Platforms_lo(EXCEPTION_TABLE_AGLINMENT)   /* Check if _EXCEPTION_TABLE_START aligned on EXCEPTION_TABLE_AGLINMENT */

loop1:
  bne       loop1

  lis   r3, Platforms_hi(_EXCEPTION_TABLE_START)
  addi  r3, r3, Platforms_lo(_EXCEPTION_TABLE_START)

  lis   r4, Platforms_hi(_INTC_TABLE_START)
  addi  r4, r4,Platforms_lo(_INTC_TABLE_START )
  
  sub  r4,r3 /* Substract  _INTC_TABLE_START with _EXCEPTION_TABLE_START*/
  
  lis   r5, Platforms_hi(INTC_TABLE_OFFSET)
  addi  r5, r5, Platforms_lo(INTC_TABLE_OFFSET)
  
  cmpw  r4, r5  /* Check offset is  INTC_TABLE_OFFSET */

loop2:
  bne       loop2

#endif
#if (BOARD_INIT_SRAM == BOARD_TRUE)
  /* Initialize SRAM
   *
   * SRAM must be initialized after a cold reset. See target reference manual.
   * If running from RAM INIT_SRAM must be left undefined so the following code does not erase
   * the loaded code.
   *
   * !LINKSTO OsekCore.Platforms.Startup.007, 1
   */
#if ((INIT_SRAM_START & 0xFFFF) != 0)
#error "INIT_SRAM_START must be aligned to 64KB"
#endif
  /* always Init SRAM */  
  lis    r3, (INIT_SRAM_START >> 16)    /* base  address  of the SRAM           */
  ori    r3, r3, 0                      /* not needed for this address          */
  li    r4, (INIT_SRAM_SIZE >> 6)      /* loop  counter  to get all of SRAM    */
  bl    _InitRam
  
  lis       r31,Platforms_hi(BOARD_MC_RGM_BASE)           /* set r31 to register base 0xc3fe0000      */
  e_or2i    r31,0x4002           /* add offset 0x4002 for RGM_DES access     */
  lhz       r12,0x0(r31)         /* load halfword content of RGM_DES reg     */
                                 /* in r12                                   */
  e_and2i.  r12,0xC011           /* compare the F_POR, F_LVD12, F_LVD27_VREG */
                                 /* and F_SOFT_DEST                          */

  bne       init_sram_noinit     /* do no ram init if it is not a POR        */

  lis       r31,Platforms_hi(BOARD_MC_RGM_BASE)          /* set r31 to register base 0xc3fe0000      */
  e_or2i    r31,0x4000           /* add offset 0x4002 for RGM_FES access     */
  lhz       r12,0x0(r31)         /* load halfword content of RGM_DES reg     */
                                 /* in r12                                   */
  e_and2i.  r12,0x0000           /*  no flags for Bolero in FES to be checked*/

  bne       init_sram_noinit     /* do no ram init if it is not a F_EXR      */

  b         do_not_init_sram     /* default case */

  /* Init SRAM NoInit only in certain cases
   *
   * !LINKSTO OsekCore.Platforms.Startup.013, 1
   */ 
init_sram_noinit:
  lis     r3, (INIT_SRAM_START_NOINIT >> 16)        /* base  address  of the SRAM  */
  e_or2i  r3, Platforms_lo(INIT_SRAM_START_NOINIT)  /*0x9F00 */
  ori     r3, r3, 0                                 /* not needed for this address */
  li      r4, (INIT_SRAM_SIZE_NOINIT / 128)         /* loop  counter  to get all of SRAM */
  bl      _InitRam

do_not_init_sram:

#endif /* #if (BOARD_INIT_SRAM == BOARD_TRUE) */


    /* Use the kernel stack for running the startup code. */
    /* The initial SP for the kernel, with 8 bytes (hole+frame) already allocated*/
    /* !LINKSTO OsekCore.Platforms.Startup.007, 1*/
    /* !LINKSTO OsekCore.Platforms.Startup.012, 1*/

    lis     r1, Platforms_hi((__SP_INIT-8))
    addi    r1, r1, Platforms_lo((__SP_INIT-8))

    /* Load Small Data Area Pointer to variable data to R13 */
    /* !LINKSTO OsekCore.Platforms.Startup.007, 1*/
  lis   r13, Platforms_hi( _SDA_BASE_)
  addi  r13, r13, Platforms_lo(_SDA_BASE_)

    /* Load Small Data Area Pointer to const data to R2 */
    /* !LINKSTO OsekCore.Platforms.Startup.007, 1*/
  lis   r2, Platforms_hi(_SDA2_BASE_)
  addi  r2, r2, Platforms_lo(_SDA2_BASE_)

  /* BoardInit() is in board.c
   *
   * !LINKSTO OsekCore.Platforms.Startup.008, 1
   */
  bl    BoardInit

  /* Clear BSS
   * Note: the "-1" on the start and end addresses is there because
   * stbu updates before storing.
   *
   * !LINKSTO OsekCore.Platforms.Startup.009, 1
   */
  li        r5, 0                               /* Initial value for bss */
  lis   r3, Platforms_hi((_BSS_START_-1))
  addi  r3, r3, Platforms_lo((_BSS_START_-1))       /* Start of bss */
  lis   r4, Platforms_hi((_BSS_END_-1))
  addi  r4, r4, Platforms_lo((_BSS_END_-1))     /* Limit of bss */
  b     __clear_bss

__clear_loop:
  stbu  r5, 1(r3)

__clear_bss:
  cmpw  r3, r4
  bne       __clear_loop

  /* Copy data sections
   * Note: the "-1" on the start and end addresses is there because
   * stbu updates before storing.
  */
  lis       r3, Platforms_hi((_DATA_START_-1))
  addi  r3, r3, Platforms_lo((_DATA_START_-1))      /* Start of data section */
  lis       r4, Platforms_hi((_DATA_END_-1))
  addi  r4, r4, Platforms_lo((_DATA_END_-1))        /* Limit of data section */
  lis       r5, Platforms_hi((_INIT_-1))
  addi  r5, r5, Platforms_lo((_INIT_-1))            /* Start of initial values in ROM */
  b     __copy_data

__copy_loop:
  lbzu  r0,1(r5)
  stbu  r0,1(r3)

__copy_data:
  cmpw  r3, r4
  bne       __copy_loop

    /* BoardStart() is in board.c */
    /*
    * !LINKSTO OsekCore.Platforms.Startup.010, 1
    */
    bl      BoardStart

    /* main() is wherever the user puts it */
    /*
    * !LINKSTO OsekCore.Platforms.Startup.010, 1
    */
  bl        main

    /* If main() ever returns we just go round again */
    /*
    * !LINKSTO OsekCore.Platforms.Startup.011, 1
    */
  b     __entry
  PLATFORMS_ASM_FUNCTION(__entry)
  PLATFORMS_ASM_SIZE(__entry)


#if (BOARD_INIT_CPU_REG == BOARD_TRUE)
    /* CPU register initialization for lock step processor mode (LSM)
    ** The following registers are initialized to avoid the CPU to go to safe-mode.
    ** If these registers are stored in memory before they have been set to identical
    ** initial values on all cores in LSM the CPUs safety mechanism detects a failure.
  */

  /*
   * !LINKSTO OsekCore.Platforms.Startup.003, 1
   */
__CpuRegInitStart:
  li    r0, 0
  li    r1, 0
  li    r2, 0
  li    r3, 0
  li    r4, 0
  li    r5, 0
  li    r6, 0
  li    r7, 0
  li    r8, 0
  li    r9, 0
  li    r10, 0
  li    r11, 0
  li    r12, 0
  li    r13, 0
  li    r14, 0
  li    r15, 0
  li    r16, 0
  li    r17, 0
  li    r18, 0
  li    r19, 0
  li    r20, 0
  li    r21, 0
  li    r22, 0
  li    r23, 0
  li    r24, 0
  li    r25, 0
  li    r26, 0
  li    r27, 0
  li    r28, 0
  li    r29, 0
  li    r30, 0
  li    r31, 0

/* reset of selected registers */
  mtcrf 0xFF,r0   /* CR */

  mtspr 272,r0    /* SPRG0 */
  mtspr 273,r0    /* SPRG1 */

  mtspr 26,r0     /* SRR0 */
  mtspr 27,r0     /* SRR1 */

  mtspr 58,r0     /* CSRR0 */
  mtspr 59,r0     /* CSRR1 */

  mtspr 63,r0     /* IVPR */
  mtspr 61,r0     /* DEAR */
  mtspr 62,r0     /* ESR */

  blr
#endif

__SetupExceptions:

    /* Initialize exceptions: only need to load IVPR */
  lis r3, Platforms_hi(__IVPR)
  addi r3,r3, Platforms_lo(__IVPR)
  mtivpr r3

  li r4,0x0
  blr

#if (BOARD_INIT_SRAM == BOARD_TRUE)
/* _InitRam(void * base, int count) - Initialize RAM
 *
 * This function initialises a block of RAM by writing to every location.
 * It is assumed that the block is a multiple of 128 bytes.
 * The function does not use any stack.
 *
 * Parameters:
 *  base (r3) - base address of RAM
 *  count (r4) - no. of 128-byte blocks to initialise.
*/
_InitRam:
  mtctr r4

_InitRam_loop:
  stmw  r16, 0(r3)     /* write all 32 GPRs to RAM */
  addi  r3, r3, 64     /* 32 GPRs * 4 bytes = 128 */
  bdnz  _InitRam_loop /* loop for 64k of L2SRAM */

  blr
#endif /* (BOARD_INIT_SRAM == BOARD_TRUE) */


/* dummy data section
 *
 * If the datasection is empty (contains no data) Trace32 fails to load the ELF-image to the target
 *
*/
    PLATFORMS_ASM_SECTION_DATA
    Platforms_byte	0x0,0x0,0x0,0x0
