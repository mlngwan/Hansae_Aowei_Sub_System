/**
 * \file
 *
 * \brief Autosar Demo
 *
 * This file contains the implementation of the Autosar
 * module Demo.
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2010 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*
 *  MISRA deviation report:
 *  - This file violates MISRA-C:2004, Rule 19.6:
 *       "#undef shall not be used"
 *    Reason:
 *       The specification document explicitely describes this mechanism.
 *  - This file violates MISRA-C:2004, Rule 19.15:
 *       "Precautions shall be taken in order to prevent the contents
 *        of a header file being included twice."
 *    Reason:
 *       This file needs to be included more than once in a compilation unit
 */
/* CHECK: NOPARSE */
/*==================[inclusions]=============================================*/
#include <Platform_Types.h>
#include <Platforms_defs.h>
/*==================[macros]=================================================*/

/*------------------[Autosar vendor identification]--------------------------*/


#if (!defined MEM_VENDOR_ID)
/** \brief Autosar module vendor identification
 **
 ** Vendor ID of the dedicated implementation of this module according
 ** to the AUTOSAR vendor list. */
#define MEM_VENDOR_ID 1U /* Elektrobit Automotive GmbH */
#endif

/*------------------[Autosar specification version]--------------------------*/

#if (!defined MEM_AR_MAJOR_VERSION)
/** \brief definition of the major version of the specification */
#define MEM_AR_MAJOR_VERSION 1U
#endif

#if (!defined MEM_AR_MINOR_VERSION)
/** \brief definition of a minor version of the specification */
#define MEM_AR_MINOR_VERSION 1U
#endif

#if (!defined MEM_AR_PATCH_VERSION)
/** \brief definition of a patch version of the specification */
#define MEM_AR_PATCH_VERSION 0U
#endif

/*------------------[Autosar software version]-------------------------------*/

#if (!defined MEM_SW_MAJOR_VERSION)
/** \brief definition of the major version of this implementation */
#define MEM_SW_MAJOR_VERSION 1U
#endif

#if (!defined MEM_SW_MINOR_VERSION)
/** \brief definition of a minor version of this implementation */
#define MEM_SW_MINOR_VERSION 1U
#endif

#if (!defined MEM_SW_PATCH_VERSION)
/** \brief definition of a patch version of this implementation */
#define MEM_SW_PATCH_VERSION 0U
#endif

/*------------------[MemMap error checking]----------------------------------*/

#if (defined MEMMAP_ERROR) /* to prevent double definition */
#error MEMMAP_ERROR defined, wrong MemMap.h usage
#endif /* if (!defined MEMMAP_ERROR) */

/** \brief Checks if this file has been correctly used
 **
 ** This definition checks if this file has been correctly included
 ** the last time.
 ** We do it here, before the big if-elif. */
#define MEMMAP_ERROR


/*------------------[MemMap extensions]--------------------------------------*/

#if (defined MEMMAP_EXTENSION_FILE)
#include <MemMapExt.h>
#endif

/* turn off "no #undef" warnings for this file */
#if (defined __TenDRA__)
#pragma TenDRA error "misra98_092" off
#pragma TenDRA error "misra04_19_6" off
#endif

#if 0 /* to be able to use #elif for every module */

/*------------------[Start of a module]--------------------------------------*/
#elif (defined BOARD_START_STACK_CODE)
  #undef BOARD_START_STACK_CODE
    #if (PLATFORMS_TOOLCHAIN == PLATFORMS_tasking)
      #pragma section farbss "stack"
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_gnu)
      #pragma section ".stack" aw
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_multi)
      #pragma ghs section bss=".stack"
    #endif
  #undef MEMMAP_ERROR

#elif (defined BOARD_STOP_STACK_CODE)
  #undef BOARD_STOP_STACK_CODE
    #if (PLATFORMS_TOOLCHAIN == PLATFORMS_tasking)
      #pragma section farbss restore
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_gnu)
      #pragma section
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_multi)
      #pragma ghs section bss=default
    #endif
  #undef MEMMAP_ERROR

#elif (defined BOARD_START_SEC_VAR_NO_INIT)
   #undef      BOARD_START_SEC_VAR_NO_INIT
    #if (PLATFORMS_TOOLCHAIN == PLATFORMS_tasking)
     #pragma section farbss "section_noinit"
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_gnu)
      /* B : unititialized, w : writable, a : allocatable*/
      #pragma section ".section_noinit" Bwa
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_multi)
      #pragma ghs section bss=".section_noinit"
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_dcc)
      #pragma section DATA ".noinit" ".noinit" RW
      #pragma use_section DATA
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_iar)
      #pragma object_attribute=__no_init
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_mwerks)
      #pragma push
      #pragma section ".noinit" ".noinitbss"
      __declspec (section ".noinit")
    #endif
   #undef MEMMAP_ERROR
#elif (defined BOARD_STOP_SEC_VAR_NO_INIT)
   #undef      BOARD_STOP_SEC_VAR_NO_INIT
    #if (PLATFORMS_TOOLCHAIN == PLATFORMS_tasking)
      #pragma section farbss restore
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_gnu)
      #pragma section
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_multi)
      #pragma ghs section bss=default
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_dcc)
      #pragma section DATA ".data"
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_iar)
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_mwerks)
      #pragma pop
    #endif
   #undef MEMMAP_ERROR

#elif (defined APP_START_SEC_VAR_NO_INIT_BOOT_STATUS)
   #undef      APP_START_SEC_VAR_NO_INIT_BOOT_STATUS
     #if (PLATFORMS_TOOLCHAIN == PLATFORMS_tasking)
       #pragma section farbss ".section_bootstatus"
     #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_gnu)
       /* B : unititialized, w : writable, a : allocatable*/
       #pragma section ".section_bootstatus" Bwa
     #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_multi)
       #pragma ghs section bss=".section_bootstatus"
     #endif
   #undef MEMMAP_ERROR
#elif (defined APP_STOP_SEC_VAR_NO_INIT_BOOT_STATUS)
   #undef      APP_STOP_SEC_VAR_NO_INIT_BOOT_STATUS
     #if (PLATFORMS_TOOLCHAIN == PLATFORMS_tasking)
       #pragma section farbss restore
     #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_gnu)
       #pragma section
     #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_multi)
       #pragma ghs section
     #endif
   #undef MEMMAP_ERROR

/*  Used for flash driver start */
#elif (defined FLASH_FLS_START_SEC_CODE)
   #undef      FLASH_FLS_START_SEC_CODE
    #if (PLATFORMS_TOOLCHAIN == PLATFORMS_dcc)
     #pragma section CODE ".fls_driver"
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_multi)
     #pragma ghs section text = ".fls_driver"
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_mwerks)
      #pragma push
     #pragma section code_type ".fls_driver"
    #endif
   #undef MEMMAP_ERROR
#elif (defined FLASH_FLS_STOP_SEC_CODE)
   #undef      FLASH_FLS_STOP_SEC_CODE
    #if (PLATFORMS_TOOLCHAIN == PLATFORMS_dcc)
     #pragma section CODE
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_multi)
     #pragma ghs section text = default
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_mwerks)
     #pragma pop
    #endif
   #undef MEMMAP_ERROR

#elif (defined FLASH_FLS_START_SEC_CONST)
   #undef      FLASH_FLS_START_SEC_CONST
    #if (PLATFORMS_TOOLCHAIN == PLATFORMS_dcc)
     #pragma section CONST ".fls_driver"
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_multi)
     #pragma ghs section rodata = ".fls_driver"
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_mwerks)
    #pragma push
    #pragma section const_type ".fls_driver"
    #endif
   #undef MEMMAP_ERROR
#elif (defined FLASH_FLS_STOP_SEC_CONST)
   #undef      FLASH_FLS_STOP_SEC_CONST
    #if (PLATFORMS_TOOLCHAIN == PLATFORMS_dcc)
     #pragma section CONST
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_multi)
     #pragma ghs section rodata = default
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_mwerks)
     #pragma pop
    #endif
   #undef MEMMAP_ERROR

/*  Used for flash driver end */

/*  Used for RH850 code needed to copy to RAM */
#elif (defined FLASH_START_R_FCL_CODE_USR)
   #undef      FLASH_START_R_FCL_CODE_USR
    #if (PLATFORMS_TOOLCHAIN == PLATFORMS_multi)
    #pragma ghs section text = ".R_FCL_CODE_USR"
    #endif
   #undef MEMMAP_ERROR
#elif (defined FLASH_STOP_R_FCL_CODE_USR)
   #undef      FLASH_STOP_R_FCL_CODE_USR
    #if (PLATFORMS_TOOLCHAIN == PLATFORMS_multi)
    #pragma ghs section text = default
    #endif
   #undef MEMMAP_ERROR

/* Start of History Record */
#elif (defined FLASH_START_RAM_CODE_USR )
   #undef      FLASH_START_RAM_CODE_USR 
    #if (PLATFORMS_TOOLCHAIN == PLATFORMS_dcc)
     #pragma section DATA ".data"
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_multi)
     #pragma ghs section data = ".data"
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_mwerks)
     #pragma section ".myCodeInRAM"
     __declspec(section ".myCodeInRAM")
    #endif
   #undef MEMMAP_ERROR
#elif (defined FLASH_STOP_RAM_CODE_USR )
   #undef      FLASH_STOP_RAM_CODE_USR 
    #if (PLATFORMS_TOOLCHAIN == PLATFORMS_dcc)
     #pragma section DATA
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_multi)
     #pragma ghs section data = default
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_mwerks)
     /* nothing to do */
    #endif
   #undef MEMMAP_ERROR
/* End of History Record */

/* Start of History Record */
#elif (defined PROG_START_HISTORY_RECORD_SEC_CONST_8 )
   #undef      PROG_START_HISTORY_RECORD_SEC_CONST_8 
    #if (PLATFORMS_TOOLCHAIN == PLATFORMS_dcc)
     #pragma section CONST ".history_record"
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_multi)
     #pragma ghs section rodata = ".history_record"
    #endif
   #undef MEMMAP_ERROR
#elif (defined PROG_STOP_HISTORY_RECORD_SEC_CONST_8 )
   #undef      PROG_STOP_HISTORY_RECORD_SEC_CONST_8 
    #if (PLATFORMS_TOOLCHAIN == PLATFORMS_dcc)
     #pragma section CONST
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_multi)
     #pragma ghs section rodata = default
    #endif
   #undef MEMMAP_ERROR
/* End of History Record */

/*------------------[Start of a module]--------------------------------------*/
#elif (defined APP_COMPSTART_START_SEC_CONST_8)
  #undef APP_COMPSTART_START_SEC_CONST_8
    #if (PLATFORMS_TOOLCHAIN == PLATFORMS_tasking)
      #pragma section farrom "CompStart"
      #pragma protect
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_gnu)
      /*#pragma section ".stack" aw*/
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_multi)
      #pragma ghs section rodata=".CompStart"
    #endif
  #undef MEMMAP_ERROR

#elif (defined APP_COMPSTART_STOP_SEC_CONST_8)
  #undef APP_COMPSTART_STOP_SEC_CONST_8
    #if (PLATFORMS_TOOLCHAIN == PLATFORMS_tasking)
      #pragma section farrom restore
      #pragma endprotect
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_gnu)
      #pragma section
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_multi)
      #pragma ghs section rodata=default
    #endif
  #undef MEMMAP_ERROR

#elif (defined APP_COMPADDRSTART_START_SEC_CONST_8)
  #undef APP_COMPADDRSTART_START_SEC_CONST_8
    #if (PLATFORMS_TOOLCHAIN == PLATFORMS_tasking)
      #pragma section farrom "CompAddrStart"
      #pragma protect
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_gnu)
      /*#pragma section ".stack" aw*/
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_multi)
      #pragma ghs section rodata=".CompAddrStart"
    #endif
  #undef MEMMAP_ERROR

#elif (defined APP_COMPADDRSTART_STOP_SEC_CONST_8)
  #undef APP_COMPADDRSTART_STOP_SEC_CONST_8
    #if (PLATFORMS_TOOLCHAIN == PLATFORMS_tasking)
      #pragma section farrom restore
      #pragma endprotect
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_gnu)
      #pragma section
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_multi)
      #pragma ghs section rodata=default
    #endif
  #undef MEMMAP_ERROR

#elif (defined APP_COMPADDRCOMPSTART_START_SEC_CONST_8)
  #undef APP_COMPADDRCOMPSTART_START_SEC_CONST_8
    #if (PLATFORMS_TOOLCHAIN == PLATFORMS_tasking)
      #pragma section farrom "CompAddCompStart"
      #pragma protect
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_gnu)
      /*#pragma section ".stack" aw*/
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_multi)
      #pragma ghs section rodata=".CompAddCompStart"
    #endif
  #undef MEMMAP_ERROR

#elif (defined APP_COMPADDRCOMPSTART_STOP_SEC_CONST_8)
  #undef APP_COMPADDRCOMPSTART_STOP_SEC_CONST_8
    #if (PLATFORMS_TOOLCHAIN == PLATFORMS_tasking)
      #pragma section farrom restore
      #pragma endprotect
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_gnu)
      #pragma section
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_multi)
      #pragma ghs section rodata=default
    #endif
  #undef MEMMAP_ERROR

#elif (defined APP_COMPADDREND_START_SEC_CONST_8)
  #undef APP_COMPADDREND_START_SEC_CONST_8
    #if (PLATFORMS_TOOLCHAIN == PLATFORMS_tasking)
      #pragma section farrom "CompAddrEnd"
      #pragma protect
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_gnu)
      /*#pragma section ".stack" aw*/
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_multi)
      #pragma ghs section rodata=".CompAddrEnd"
    #endif
  #undef MEMMAP_ERROR

#elif (defined APP_COMPADDREND_STOP_SEC_CONST_8)
  #undef APP_COMPADDREND_STOP_SEC_CONST_8
    #if (PLATFORMS_TOOLCHAIN == PLATFORMS_tasking)
      #pragma section farrom restore
      #pragma endprotect
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_gnu)
      #pragma section
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_multi)
      #pragma ghs section rodata=default
    #endif
  #undef MEMMAP_ERROR

#elif (defined APP_COMPEND_START_SEC_CONST_8)
  #undef APP_COMPEND_START_SEC_CONST_8
    #if (PLATFORMS_TOOLCHAIN == PLATFORMS_tasking)
      #pragma section farrom "CompEnd"
      #pragma protect
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_gnu)
      /*#pragma section ".stack" aw*/
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_multi)
      #pragma ghs section rodata=".CompEnd"
    #endif
  #undef MEMMAP_ERROR

#elif (defined APP_COMPEND_STOP_SEC_CONST_8)
  #undef APP_COMPEND_STOP_SEC_CONST_8
    #if (PLATFORMS_TOOLCHAIN == PLATFORMS_tasking)
      #pragma section farrom restore
      #pragma endprotect
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_gnu)
      #pragma section
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_multi)
      #pragma ghs section rodata=default
    #endif
  #undef MEMMAP_ERROR

#elif (defined APP_COMPCONSTTEST_START_SEC_CONST_8)
  #undef APP_COMPCONSTTEST_START_SEC_CONST_8
    #if (PLATFORMS_TOOLCHAIN == PLATFORMS_tasking)
      #pragma section farrom "CompConstTest"
      #pragma protect
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_gnu)
      /*#pragma section ".stack" aw*/
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_multi)
      #pragma ghs section rodata=".CompConstTest"
    #endif
  #undef MEMMAP_ERROR

#elif (defined APP_COMPCONSTTEST_STOP_SEC_CONST_8)
  #undef APP_COMPCONSTTEST_STOP_SEC_CONST_8
    #if (PLATFORMS_TOOLCHAIN == PLATFORMS_tasking)
      #pragma section farrom restore
      #pragma endprotect
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_gnu)
      #pragma section
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_multi)
      #pragma ghs section rodata=default
    #endif
  #undef MEMMAP_ERROR

#elif (defined APP_COMPCALLOUT_START_SEC_CODE_8)
  #undef APP_COMPCALLOUT_START_SEC_CODE_8
    #if (PLATFORMS_TOOLCHAIN == PLATFORMS_tasking)
      #pragma section code "CompleteCompatibleCallout"
      #pragma protect
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_gnu)
      /*#pragma section ".stack" aw*/
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_multi)
      #pragma ghs section text=".CompleteCompatibleCallout"
    #endif
  #undef MEMMAP_ERROR

#elif (defined APP_COMPCALLOUT_STOP_SEC_CODE_8)
  #undef APP_COMPCALLOUT_STOP_SEC_CODE_8
    #if (PLATFORMS_TOOLCHAIN == PLATFORMS_tasking)
      #pragma section code restore
      #pragma endprotect
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_gnu)
      #pragma section
    #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_multi)
      #pragma ghs section text=default
    #endif
  #undef MEMMAP_ERROR

/*------------------[Start of a module]--------------------------------------*/
#elif defined (PROG_START_SEC_CODE)
   #undef      PROG_START_SEC_CODE
   #undef MEMMAP_ERROR
#elif defined (PROG_STOP_SEC_CODE)
   #undef      PROG_STOP_SEC_CODE
   #undef MEMMAP_ERROR

#elif defined (PROG_GET_DID_F0F3_START_SEC_CODE)
   #undef      PROG_GET_DID_F0F3_START_SEC_CODE
   #undef MEMMAP_ERROR
#elif defined (PROG_GET_DID_F0F3_STOP_SEC_CODE)
   #undef      PROG_GET_DID_F0F3_STOP_SEC_CODE
   #undef MEMMAP_ERROR

#elif defined (PROG_GET_DID_F0F6_START_SEC_CODE)
   #undef      PROG_GET_DID_F0F6_START_SEC_CODE
   #undef MEMMAP_ERROR
#elif defined (PROG_GET_DID_F0F6_STOP_SEC_CODE)
   #undef      PROG_GET_DID_F0F6_STOP_SEC_CODE
   #undef MEMMAP_ERROR

#elif defined (PROG_START_SEC_VAR_INIT_8)
   #undef      PROG_START_SEC_VAR_INIT_8
   #undef MEMMAP_ERROR
#elif defined (PROG_STOP_SEC_VAR_INIT_8)
   #undef      PROG_STOP_SEC_VAR_INIT_8
   #undef MEMMAP_ERROR

#elif defined (PROG_START_SEC_VAR_NO_INIT_8)
   #undef      PROG_START_SEC_VAR_NO_INIT_8

   #undef MEMMAP_ERROR
#elif defined (PROG_STOP_SEC_VAR_NO_INIT_8)
   #undef      PROG_STOP_SEC_VAR_NO_INIT_8
   #undef MEMMAP_ERROR

#elif defined (PROG_START_SEC_VAR_NO_INIT_UNSPECIFIED)
   #undef      PROG_START_SEC_VAR_NO_INIT_UNSPECIFIED
     #if (PLATFORMS_TOOLCHAIN == PLATFORMS_tasking)
       #pragma section farbss ".ram_no_init"
     #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_gnu)
       /* B : unititialized, w : writable, a : allocatable*/
       #pragma section ".ram_no_init" Bwa
     #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_multi)
       #pragma ghs section bss=".ram_no_init"
     #endif
   #undef MEMMAP_ERROR
#elif defined (PROG_STOP_SEC_VAR_NO_INIT_UNSPECIFIED)
   #undef      PROG_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
     #if (PLATFORMS_TOOLCHAIN == PLATFORMS_tasking)
       #pragma section farbss restore
     #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_gnu)
       #pragma section
     #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_multi)
       #pragma ghs section
     #endif
   #undef MEMMAP_ERROR

#elif defined (PROG_START_SEC_CONST_UNSPECIFIED)
   #undef      PROG_START_SEC_CONST_UNSPECIFIED
   #undef MEMMAP_ERROR
#elif defined (PROG_STOP_SEC_CONST_UNSPECIFIED)
   #undef      PROG_STOP_SEC_CONST_UNSPECIFIED
   #undef MEMMAP_ERROR

#elif defined (PROG_START_SEC_CONST_8)
   #undef      PROG_START_SEC_CONST_8
   #undef MEMMAP_ERROR
#elif defined (PROG_STOP_SEC_CONST_8)
   #undef      PROG_STOP_SEC_CONST_8
   #undef MEMMAP_ERROR

#elif defined (PROG_SBL_HEADER_START_SEC_CONST_8)
   #undef      PROG_SBL_HEADER_START_SEC_CONST_8
   #undef MEMMAP_ERROR
   #if (PLATFORMS_TOOLCHAIN == PLATFORMS_tasking)
       #pragma section farrom "SBLHeader"
       #pragma protect
       #pragma noclear
     #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_gnu)
       /* B : unititialized, w : writable, a : allocatable*/
       /*#pragma section ".SBLHeader" Bwa*/
     #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_multi)
       #pragma ghs section rodata=".SBLHeader"
     #endif
#elif defined (PROG_SBL_HEADER_STOP_SEC_CONST_8)
   #undef      PROG_SBL_HEADER_STOP_SEC_CONST_8
   #undef MEMMAP_ERROR
   #if (PLATFORMS_TOOLCHAIN == PLATFORMS_tasking)
     #pragma  section farrom restore
     #pragma endprotect
     #pragma clear
   #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_gnu)
     #pragma section
   #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_multi)
     #pragma ghs section
   #endif

#elif defined (PROG_SBL_VALIDITY_START_SEC_CONST_8)
   #undef      PROG_SBL_VALIDITY_START_SEC_CONST_8
   #undef MEMMAP_ERROR
   #if (PLATFORMS_TOOLCHAIN == PLATFORMS_tasking)
       #pragma section farrom "SBLValidity"
       #pragma protect
       #pragma noclear
     #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_gnu)
       /* B : unititialized, w : writable, a : allocatable*/
       /*#pragma section ".SBLValidity" Bwa*/
     #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_multi)
       #pragma ghs section rodata=".SBLValidity"
     #endif
#elif defined (PROG_SBL_VALIDITY_STOP_SEC_CONST_8)
   #undef      PROG_SBL_VALIDITY_STOP_SEC_CONST_8
   #undef MEMMAP_ERROR
   #if (PLATFORMS_TOOLCHAIN == PLATFORMS_tasking)
     #pragma section farrom restore
     #pragma endprotect
     #pragma clear
   #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_gnu)
     #pragma section
   #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_multi)
     #pragma ghs section
   #endif

#elif defined (PROG_REPROG_FLAG_START_SEC_VAR_NO_INIT_8)
   #undef      PROG_REPROG_FLAG_START_SEC_VAR_NO_INIT_8
     #if (PLATFORMS_TOOLCHAIN == PLATFORMS_tasking)
       #pragma section farbss "prog_sig"
       #pragma protect
     #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_gnu)
       /* B : unititialized, w : writable, a : allocatable*/
       #pragma section ".prog_sig" Bwa
     #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_multi)
       #pragma ghs section bss=".prog_sig"
     #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_dcc)
       #pragma section DATA "" ".prog_sig"
     #endif
   #undef MEMMAP_ERROR
#elif defined (PROG_REPROG_FLAG_STOP_SEC_VAR_NO_INIT_8)
   #undef      PROG_REPROG_FLAG_STOP_SEC_VAR_NO_INIT_8
   #undef MEMMAP_ERROR
   #if (PLATFORMS_TOOLCHAIN == PLATFORMS_tasking)
     #pragma section farbss restore
     #pragma endprotect
   #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_gnu)
     #pragma section
   #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_multi)
     #pragma ghs section
   #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_dcc)
     #pragma section DATA "" ""
   #endif

#elif defined (PROG_TESTER_ADDRESS_FLAG_START_SEC_VAR_NO_INIT_8)
   #undef      PROG_TESTER_ADDRESS_FLAG_START_SEC_VAR_NO_INIT_8
     #if (PLATFORMS_TOOLCHAIN == PLATFORMS_tasking)
       #pragma section farbss "tester_address"
       #pragma protect
     #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_gnu)
       /* B : unititialized, w : writable, a : allocatable*/
       #pragma section ".tester_address" Bwa
     #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_multi)
       #pragma ghs section bss=".tester_address"
     #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_dcc)
       #pragma section DATA "" ".tester_address"
     #endif
   #undef MEMMAP_ERROR
#elif defined (PROG_TESTER_ADDRESS_FLAG_STOP_SEC_VAR_NO_INIT_8)
   #undef      PROG_TESTER_ADDRESS_FLAG_STOP_SEC_VAR_NO_INIT_8
   #undef MEMMAP_ERROR
   #if (PLATFORMS_TOOLCHAIN == PLATFORMS_tasking)
     #pragma section farbss restore
     #pragma endprotect
   #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_gnu)
     #pragma section
   #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_multi)
     #pragma ghs section
   #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_dcc)
     #pragma section DATA "" ""
   #endif

#elif defined (PROG_SUPPPOSITIVERESPONSE_FLAG_START_SEC_VAR_NO_INIT_8)
   #undef      PROG_SUPPPOSITIVERESPONSE_FLAG_START_SEC_VAR_NO_INIT_8
     #if (PLATFORMS_TOOLCHAIN == PLATFORMS_tasking)
       #pragma section farbss "applitoboot_response"
       #pragma protect
     #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_gnu)
       /* B : unititialized, w : writable, a : allocatable*/
       #pragma section ".applitoboot_response" Bwa
     #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_multi)
       #pragma ghs section bss=".applitoboot_response"
     #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_dcc)
       #pragma section DATA "" ".applitoboot_response"
     #endif
   #undef MEMMAP_ERROR
#elif defined (PROG_SUPPPOSITIVERESPONSE_FLAG_STOP_SEC_VAR_NO_INIT_8)
   #undef      PROG_SUPPPOSITIVERESPONSE_FLAG_STOP_SEC_VAR_NO_INIT_8
   #undef MEMMAP_ERROR
   #if (PLATFORMS_TOOLCHAIN == PLATFORMS_tasking)
     #pragma section farbss restore
     #pragma endprotect
   #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_gnu)
     #pragma section
   #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_multi)
     #pragma ghs section
   #elif (PLATFORMS_TOOLCHAIN == PLATFORMS_dcc)
     #pragma section DATA "" ""
   #endif

#elif defined (PROG_BOOT_INFO_START_SEC_CONST_UNSPECIFIED)
   #undef      PROG_BOOT_INFO_START_SEC_CONST_UNSPECIFIED
   #undef MEMMAP_ERROR
#elif defined (PROG_BOOT_INFO_STOP_SEC_CONST_UNSPECIFIED)
   #undef      PROG_BOOT_INFO_STOP_SEC_CONST_UNSPECIFIED
   #undef MEMMAP_ERROR
   
#elif (defined PROG_START_SEC_CODE_FLASH_PAGE)
  #undef PROG_START_SEC_CODE_FLASH_PAGE
  #undef MEMMAP_ERROR
  #if (PLATFORMS_TOOLCHAIN == PLATFORMS_tasking)
    #pragma section code="FlashPage"
    #pragma protect
  #endif

#elif (defined PROG_STOP_SEC_CODE_FLASH_PAGE)
  #undef PROG_STOP_SEC_CODE_FLASH_PAGE
  #undef MEMMAP_ERROR
  #if (PLATFORMS_TOOLCHAIN == PLATFORMS_tasking)
    #pragma section code
    #pragma endprotect
  #endif

/*****************************************************************************
**                                 EB                                       **
******************************************************************************/
#elif (defined EB_BUFF_TP_WRAP_START_SEC_VAR )
  #undef EB_BUFF_TP_WRAP_START_SEC_VAR
  #undef MEMMAP_ERROR
#elif (defined EB_BUFF_TP_WRAP_STOP_SEC_VAR )
  #undef EB_BUFF_TP_WRAP_STOP_SEC_VAR
  #undef MEMMAP_ERROR
  
/*****************************************************************************
**                               BLCR                                       **
******************************************************************************/
#elif (defined BLCR_BUFF_START_SEC_VAR )
  #undef BLCR_BUFF_START_SEC_VAR
  #undef MEMMAP_ERROR
#elif (defined BLCR_BUFF_STOP_SEC_VAR )
  #undef BLCR_BUFF_STOP_SEC_VAR
  #undef MEMMAP_ERROR


/*****************************************************************************
**                                 FLSLOADER                                **
******************************************************************************/

/*
 * To be used for mapping code to application block, boot block,
 * external flash etc
 */
#elif defined (FLSLOADER_START_SEC_CODE)
   #undef      FLSLOADER_START_SEC_CODE
   #undef MEMMAP_ERROR
#elif defined (FLSLOADER_STOP_SEC_CODE)
   #undef      FLSLOADER_STOP_SEC_CODE
   #undef MEMMAP_ERROR


#elif defined (FLSLOADER_START_SEC_WRITE_CODE)
   #undef      FLSLOADER_START_SEC_WRITE_CODE
   #undef MEMMAP_ERROR
#elif defined (FLSLOADER_STOP_SEC_WRITE_CODE)
   #undef      FLSLOADER_STOP_SEC_WRITE_CODE
   #undef MEMMAP_ERROR


#elif defined (FLSLOADER_START_SEC_ERASE_CODE)
   #undef      FLSLOADER_START_SEC_ERASE_CODE
   #undef MEMMAP_ERROR
#elif defined (FLSLOADER_STOP_SEC_ERASE_CODE)
   #undef      FLSLOADER_STOP_SEC_ERASE_CODE
   #undef MEMMAP_ERROR


#elif defined (FLSLOADER_START_SEC_VAR_NOINIT_8BIT)
   #undef      FLSLOADER_START_SEC_VAR_NOINIT_8BIT
   #undef MEMMAP_ERROR
#elif defined (FLSLOADER_STOP_SEC_VAR_NOINIT_8BIT)
   #undef      FLSLOADER_STOP_SEC_VAR_NOINIT_8BIT
   #undef MEMMAP_ERROR


#elif defined (FLSLOADER_START_SEC_VAR_NOINIT_16BIT)
   #undef      FLSLOADER_START_SEC_VAR_NOINIT_16BIT
   #undef MEMMAP_ERROR
#elif defined (FLSLOADER_STOP_SEC_VAR_NOINIT_16BIT)
   #undef      FLSLOADER_STOP_SEC_VAR_NOINIT_16BIT
   #undef MEMMAP_ERROR


#elif defined (FLSLOADER_START_SEC_VAR_NOINIT_32BIT)
   #undef      FLSLOADER_START_SEC_VAR_NOINIT_32BIT
   #undef MEMMAP_ERROR
#elif defined (FLSLOADER_STOP_SEC_VAR_NOINIT_32BIT)
   #undef      FLSLOADER_STOP_SEC_VAR_NOINIT_32BIT
   #undef MEMMAP_ERROR


#elif defined (FLSLOADER_START_SEC_VAR_NOINIT_UNSPECIFIED)
   #undef      FLSLOADER_START_SEC_VAR_NOINIT_UNSPECIFIED
   #undef MEMMAP_ERROR
#elif defined (FLSLOADER_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
   #undef      FLSLOADER_STOP_SEC_VAR_NOINIT_UNSPECIFIED
   #undef MEMMAP_ERROR


#elif defined (FLSLOADER_START_SEC_VAR_POWER_ON_INIT_8BIT)
   #undef      FLSLOADER_START_SEC_VAR_POWER_ON_INIT_8BIT
   #undef MEMMAP_ERROR
#elif defined (FLSLOADER_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
   #undef      FLSLOADER_STOP_SEC_VAR_POWER_ON_INIT_8BIT
   #undef MEMMAP_ERROR


#elif defined (FLSLOADER_START_SEC_VAR_POWER_ON_INIT_16BIT)
   #undef      FLSLOADER_START_SEC_VAR_POWER_ON_INIT_16BIT
   #undef MEMMAP_ERROR
#elif defined (FLSLOADER_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
   #undef      FLSLOADER_STOP_SEC_VAR_POWER_ON_INIT_16BIT
   #undef MEMMAP_ERROR


#elif defined (FLSLOADER_START_SEC_VAR_POWER_ON_INIT_32BIT)
   #undef      FLSLOADER_START_SEC_VAR_POWER_ON_INIT_32BIT
   #undef MEMMAP_ERROR
#elif defined (FLSLOADER_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
   #undef      FLSLOADER_STOP_SEC_VAR_POWER_ON_INIT_32BIT
   #undef MEMMAP_ERROR


#elif defined (FLSLOADER_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
   #undef      FLSLOADER_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
   #undef MEMMAP_ERROR
#elif defined (FLSLOADER_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
   #undef      FLSLOADER_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
   #undef MEMMAP_ERROR


#elif defined (FLSLOADER_START_SEC_VAR_FAST_8BIT)
   #undef      FLSLOADER_START_SEC_VAR_FAST_8BIT
   #undef MEMMAP_ERROR
#elif defined (FLSLOADER_STOP_SEC_VAR_FAST_8BIT)
   #undef      FLSLOADER_STOP_SEC_VAR_FAST_8BIT
   #undef MEMMAP_ERROR


#elif defined (FLSLOADER_START_SEC_VAR_FAST_16BIT)
   #undef      FLSLOADER_START_SEC_VAR_FAST_16BIT
   #undef MEMMAP_ERROR
#elif defined (FLSLOADER_STOP_SEC_VAR_FAST_16BIT)
   #undef      FLSLOADER_STOP_SEC_VAR_FAST_16BIT
   #undef MEMMAP_ERROR


#elif defined (FLSLOADER_START_SEC_VAR_FAST_32BIT)
   #undef      FLSLOADER_START_SEC_VAR_FAST_32BIT
   #undef MEMMAP_ERROR
#elif defined (FLSLOADER_STOP_SEC_VAR_FAST_32BIT)
   #undef      FLSLOADER_STOP_SEC_VAR_FAST_32BIT
   #undef MEMMAP_ERROR


#elif defined (FLSLOADER_START_SEC_VAR_FAST_UNSPECIFIED)
   #undef      FLSLOADER_START_SEC_VAR_FAST_UNSPECIFIED
   #undef MEMMAP_ERROR
#elif defined (FLSLOADER_STOP_SEC_VAR_FAST_UNSPECIFIED)
   #undef      FLSLOADER_STOP_SEC_VAR_FAST_UNSPECIFIED
   #undef MEMMAP_ERROR


#elif defined (FLSLOADER_START_SEC_VAR_8BIT)
   #undef      FLSLOADER_START_SEC_VAR_8BIT
   #undef MEMMAP_ERROR
#elif defined (FLSLOADER_STOP_SEC_VAR_8BIT)
   #undef      FLSLOADER_STOP_SEC_VAR_8BIT
   #undef MEMMAP_ERROR


#elif defined (FLSLOADER_START_SEC_VAR_16BIT)
   #undef      FLSLOADER_START_SEC_VAR_16BIT
   #undef MEMMAP_ERROR
#elif defined (FLSLOADER_STOP_SEC_VAR_16BIT)
   #undef      FLSLOADER_STOP_SEC_VAR_16BIT
   #undef MEMMAP_ERROR


#elif defined (FLSLOADER_START_SEC_VAR_32BIT)
   #undef      FLSLOADER_START_SEC_VAR_32BIT
   #undef MEMMAP_ERROR
#elif defined (FLSLOADER_STOP_SEC_VAR_32BIT)
   #undef      FLSLOADER_STOP_SEC_VAR_32BIT
   #undef MEMMAP_ERROR


#elif defined (FLSLOADER_START_SEC_VAR_UNSPECIFIED)
   #undef      FLSLOADER_START_SEC_VAR_UNSPECIFIED
   #undef MEMMAP_ERROR
#elif defined (FLSLOADER_STOP_SEC_VAR_UNSPECIFIED)
   #undef      FLSLOADER_STOP_SEC_VAR_UNSPECIFIED
   #undef MEMMAP_ERROR


#elif defined (FLSLOADER_START_SEC_CONST_8BIT)
   #undef      FLSLOADER_START_SEC_CONST_8BIT
   #undef MEMMAP_ERROR
#elif defined (FLSLOADER_STOP_SEC_CONST_8BIT)
   #undef      FLSLOADER_STOP_SEC_CONST_8BIT
   #undef MEMMAP_ERROR


#elif defined (FLSLOADER_START_SEC_CONST_16BIT)
   #undef      FLSLOADER_START_SEC_CONST_16BIT
   #undef MEMMAP_ERROR
#elif defined (FLSLOADER_STOP_SEC_CONST_16BIT)
   #undef      FLSLOADER_STOP_SEC_CONST_16BIT
   #undef MEMMAP_ERROR


#elif defined (FLSLOADER_START_SEC_CONST_32BIT)
   #undef      FLSLOADER_START_SEC_CONST_32BIT
   #undef MEMMAP_ERROR
#elif defined (FLSLOADER_STOP_SEC_CONST_32BIT)
   #undef      FLSLOADER_STOP_SEC_CONST_32BIT
   #undef MEMMAP_ERROR


#elif defined (FLSLOADER_START_SEC_CONST_UNSPECIFIED)
   #undef      FLSLOADER_START_SEC_CONST_UNSPECIFIED
   #undef MEMMAP_ERROR
#elif defined (FLSLOADER_STOP_SEC_CONST_UNSPECIFIED)
   #undef      FLSLOADER_STOP_SEC_CONST_UNSPECIFIED
   #undef MEMMAP_ERROR

/*
Fls_Write API : Section
*/
#elif defined (FLSLOADER_START_SEC_WRITE_CODE)
   #undef      FLSLOADER_START_SEC_WRITE_CODE
   #undef MEMMAP_ERROR
#elif defined (FLSLOADER_STOP_SEC_WRITE_CODE)
   #undef      FLSLOADER_STOP_SEC_WRITE_CODE
   #undef MEMMAP_ERROR
/*
Fls Erase API : Section
*/
#elif defined (FLSLOADER_START_SEC_ERASE_CODE)
   #undef      FLSLOADER_START_SEC_ERASE_CODE
   #undef MEMMAP_ERROR
#elif defined (FLSLOADER_STOP_SEC_ERASE_CODE)
   #undef      FLSLOADER_STOP_SEC_ERASE_CODE
   #undef MEMMAP_ERROR

/*****************************************************************************
**                                 MCAL                                      **
******************************************************************************/

/*
 * To be used for mapping code to application block, boot block,
 * external flash etc
 */
#elif defined (MCAL_START_SEC_VAR_FAST_32BIT)
   #undef      MCAL_START_SEC_VAR_FAST_32BIT
   #undef MEMMAP_ERROR
#elif defined (MCAL_STOP_SEC_VAR_FAST_32BIT)
   #undef      MCAL_STOP_SEC_VAR_FAST_32BIT
   #undef MEMMAP_ERROR

/*****************************************************************************
**                                MCAL LIB                                  **
******************************************************************************/

/*
* To be used for mapping code to application block, boot block,
* external flash etc
*/
#elif defined (MCAL_TCLIB_START_SEC_CODE)
   #undef      MCAL_TCLIB_START_SEC_CODE
   #undef MEMMAP_ERROR
#elif defined (MCAL_TCLIB_STOP_SEC_CODE)
   #undef      MCAL_TCLIB_STOP_SEC_CODE
   #undef MEMMAP_ERROR

/* add for tc297 start */
#elif defined (MCAL_TCLIB_START_SEC_VAR_INIT_32BIT)
   #undef      MCAL_TCLIB_START_SEC_VAR_INIT_32BIT
   #undef MEMMAP_ERROR
#elif defined (MCAL_TCLIB_STOP_SEC_VAR_INIT_32BIT)
   #undef      MCAL_TCLIB_STOP_SEC_VAR_INIT_32BIT
   #undef MEMMAP_ERROR
#elif defined (MCAL_WDGLIB_START_SEC_VAR_INIT_32BIT)
   #undef      MCAL_WDGLIB_START_SEC_VAR_INIT_32BIT
   #undef MEMMAP_ERROR
#elif defined (MCAL_WDGLIB_STOP_SEC_VAR_INIT_32BIT)
   #undef      MCAL_WDGLIB_STOP_SEC_VAR_INIT_32BIT
   #undef MEMMAP_ERROR
/* add for tc297 start */
#elif defined (MCAL_TCLIB_START_SEC_VAR_32BIT)
   #undef      MCAL_TCLIB_START_SEC_VAR_32BIT
   #undef MEMMAP_ERROR
#elif defined (MCAL_TCLIB_STOP_SEC_VAR_32BIT)
   #undef      MCAL_TCLIB_STOP_SEC_VAR_32BIT
   #undef MEMMAP_ERROR

#elif defined (MCAL_TCLIB_START_SEC_CONST_32BIT)
   #undef      MCAL_TCLIB_START_SEC_CONST_32BIT
   #undef MEMMAP_ERROR
#elif defined (MCAL_TCLIB_STOP_SEC_CONST_32BIT)
   #undef      MCAL_TCLIB_STOP_SEC_CONST_32BIT
   #undef MEMMAP_ERROR

#elif defined (MCAL_TCLIB_START_SEC_VAR_8BIT)
   #undef      MCAL_TCLIB_START_SEC_VAR_8BIT
   #undef MEMMAP_ERROR
#elif defined (MCAL_TCLIB_STOP_SEC_VAR_8BIT)
   #undef      MCAL_TCLIB_STOP_SEC_VAR_8BIT
   #undef MEMMAP_ERROR

#elif defined (MCAL_WDGLIB_START_SEC_CODE)
   #undef      MCAL_WDGLIB_START_SEC_CODE
   #undef MEMMAP_ERROR
#elif defined (MCAL_WDGLIB_STOP_SEC_CODE)
   #undef      MCAL_WDGLIB_STOP_SEC_CODE
   #undef MEMMAP_ERROR

#elif defined (MCAL_WDGLIB_START_SEC_VAR_32BIT)
   #undef      MCAL_WDGLIB_START_SEC_VAR_32BIT
   #undef MEMMAP_ERROR
#elif defined (MCAL_WDGLIB_STOP_SEC_VAR_32BIT)
   #undef      MCAL_WDGLIB_STOP_SEC_VAR_32BIT
   #undef MEMMAP_ERROR

#elif defined (MCAL_WDGLIB_START_SEC_VAR_8BIT)
   #undef      MCAL_WDGLIB_START_SEC_VAR_8BIT
   #undef MEMMAP_ERROR
#elif defined (MCAL_WDGLIB_STOP_SEC_VAR_8BIT)
   #undef      MCAL_WDGLIB_STOP_SEC_VAR_8BIT
   #undef MEMMAP_ERROR

#elif defined (MCAL_WDGLIB_START_SEC_CONST_32BIT)
   #undef      MCAL_WDGLIB_START_SEC_CONST_32BIT
   #undef MEMMAP_ERROR
#elif defined (MCAL_WDGLIB_STOP_SEC_CONST_32BIT)
   #undef      MCAL_WDGLIB_STOP_SEC_CONST_32BIT
   #undef MEMMAP_ERROR

#elif defined (MCAL_DMALIB_START_SEC_CODE)
   #undef      MCAL_DMALIB_START_SEC_CODE
   #undef MEMMAP_ERROR
#elif defined (MCAL_DMALIB_STOP_SEC_CODE)
   #undef      MCAL_DMALIB_STOP_SEC_CODE
   #undef MEMMAP_ERROR

#elif defined (MCAL_DMALIB_START_SEC_VAR_32BIT)
   #undef      MCAL_DMALIB_START_SEC_VAR_32BIT
   #undef MEMMAP_ERROR
#elif defined (MCAL_DMALIB_STOP_SEC_VAR_32BIT)
   #undef      MCAL_DMALIB_STOP_SEC_VAR_32BIT
   #undef MEMMAP_ERROR

#elif defined (MCAL_DMALIB_START_SEC_VAR_8BIT)
   #undef      MCAL_DMALIB_START_SEC_VAR_8BIT
   #undef MEMMAP_ERROR
#elif defined (MCAL_DMALIB_STOP_SEC_VAR_8BIT)
   #undef      MCAL_DMALIB_STOP_SEC_VAR_8BIT
   #undef MEMMAP_ERROR


/*
 * To be used for mapping code to application block, boot block,
 * external flash etc
 */
#elif defined (MCAL_START_SEC_VAR_32BIT)
   #undef      MCAL_START_SEC_VAR_32BIT
   #undef MEMMAP_ERROR
#elif defined (MCAL_STOP_SEC_VAR_32BIT)
   #undef      MCAL_STOP_SEC_VAR_32BIT
   #undef MEMMAP_ERROR

#elif defined (MCAL_START_SEC_VAR_32BIT)
   #undef      MCAL_START_SEC_VAR_32BIT
   #undef MEMMAP_ERROR
#elif defined (MCAL_STOP_SEC_VAR_32BIT)
   #undef      MCAL_STOP_SEC_VAR_32BIT
   #undef MEMMAP_ERROR

#elif defined (MCAL_START_SEC_VAR_16BIT)
   #undef      MCAL_START_SEC_VAR_16BIT
   #undef MEMMAP_ERROR
#elif defined (MCAL_STOP_SEC_VAR_16BIT)
   #undef      MCAL_STOP_SEC_VAR_16BIT
   #undef MEMMAP_ERROR

#elif defined (MCAL_START_SEC_VAR_8BIT)
   #undef      MCAL_START_SEC_VAR_8BIT
   #undef MEMMAP_ERROR
#elif defined (MCAL_STOP_SEC_VAR_8BIT)
   #undef      MCAL_STOP_SEC_VAR_8BIT
   #undef MEMMAP_ERROR

#elif defined (MCAL_START_SEC_CODE)
   #undef      MCAL_START_SEC_CODE
   #undef MEMMAP_ERROR
#elif defined (MCAL_STOP_SEC_CODE)
   #undef      MCAL_STOP_SEC_CODE
   #undef MEMMAP_ERROR

/*------------------[Start of a module]--------------------------------------*/

#elif (defined ADC_START_SEC_CODE)
  #undef ADC_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined ADC_STOP_SEC_CODE)
  #undef ADC_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined ADC_START_SEC_APPL_CODE)
  #undef ADC_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined ADC_STOP_SEC_APPL_CODE)
  #undef ADC_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined ADC_START_SEC_VAR_NOINIT_8BIT)
  #undef ADC_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined ADC_STOP_SEC_VAR_NOINIT_8BIT)
  #undef ADC_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined ADC_START_SEC_VAR_NOINIT_16BIT)
  #undef ADC_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined ADC_STOP_SEC_VAR_NOINIT_16BIT)
  #undef ADC_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined ADC_START_SEC_VAR_NOINIT_32BIT)
  #undef ADC_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined ADC_STOP_SEC_VAR_NOINIT_32BIT)
  #undef ADC_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined ADC_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef ADC_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined ADC_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef ADC_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined ADC_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef ADC_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined ADC_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef ADC_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined ADC_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef ADC_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined ADC_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef ADC_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined ADC_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef ADC_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined ADC_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef ADC_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined ADC_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef ADC_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined ADC_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef ADC_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined ADC_START_SEC_VAR_FAST_8BIT)
  #undef ADC_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined ADC_STOP_SEC_VAR_FAST_8BIT)
  #undef ADC_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined ADC_START_SEC_VAR_FAST_16BIT)
  #undef ADC_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined ADC_STOP_SEC_VAR_FAST_16BIT)
  #undef ADC_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined ADC_START_SEC_VAR_FAST_32BIT)
  #undef ADC_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined ADC_STOP_SEC_VAR_FAST_32BIT)
  #undef ADC_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined ADC_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef ADC_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined ADC_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef ADC_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined ADC_START_SEC_VAR_8BIT)
  #undef ADC_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined ADC_STOP_SEC_VAR_8BIT)
  #undef ADC_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined ADC_START_SEC_VAR_16BIT)
  #undef ADC_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined ADC_STOP_SEC_VAR_16BIT)
  #undef ADC_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined ADC_START_SEC_VAR_32BIT)
  #undef ADC_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined ADC_STOP_SEC_VAR_32BIT)
  #undef ADC_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined ADC_START_SEC_VAR_UNSPECIFIED)
  #undef ADC_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined ADC_STOP_SEC_VAR_UNSPECIFIED)
  #undef ADC_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined ADC_START_SEC_CONST_8BIT)
  #undef ADC_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined ADC_STOP_SEC_CONST_8BIT)
  #undef ADC_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined ADC_START_SEC_CONST_16BIT)
  #undef ADC_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined ADC_STOP_SEC_CONST_16BIT)
  #undef ADC_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined ADC_START_SEC_CONST_32BIT)
  #undef ADC_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined ADC_STOP_SEC_CONST_32BIT)
  #undef ADC_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined ADC_START_SEC_CONST_UNSPECIFIED)
  #undef ADC_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined ADC_STOP_SEC_CONST_UNSPECIFIED)
  #undef ADC_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined ADC_START_CONFIG_DATA_8BIT)
  #undef ADC_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined ADC_STOP_CONFIG_DATA_8BIT)
  #undef ADC_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined ADC_START_CONFIG_DATA_16BIT)
  #undef ADC_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined ADC_STOP_CONFIG_DATA_16BIT)
  #undef ADC_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined ADC_START_CONFIG_DATA_32BIT)
  #undef ADC_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined ADC_STOP_CONFIG_DATA_32BIT)
  #undef ADC_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined ADC_START_CONFIG_DATA_UNSPECIFIED)
  #undef ADC_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined ADC_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef ADC_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined BASE_START_SEC_CODE)
  #undef BASE_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined BASE_STOP_SEC_CODE)
  #undef BASE_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined BASE_START_SEC_APPL_CODE)
  #undef BASE_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined BASE_STOP_SEC_APPL_CODE)
  #undef BASE_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined BASE_START_SEC_VAR_NOINIT_8BIT)
  #undef BASE_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined BASE_STOP_SEC_VAR_NOINIT_8BIT)
  #undef BASE_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined BASE_START_SEC_VAR_NOINIT_16BIT)
  #undef BASE_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined BASE_STOP_SEC_VAR_NOINIT_16BIT)
  #undef BASE_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined BASE_START_SEC_VAR_NOINIT_32BIT)
  #undef BASE_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined BASE_STOP_SEC_VAR_NOINIT_32BIT)
  #undef BASE_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined BASE_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef BASE_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined BASE_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef BASE_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined BASE_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef BASE_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined BASE_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef BASE_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined BASE_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef BASE_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined BASE_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef BASE_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined BASE_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef BASE_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined BASE_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef BASE_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined BASE_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef BASE_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined BASE_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef BASE_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined BASE_START_SEC_VAR_FAST_8BIT)
  #undef BASE_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined BASE_STOP_SEC_VAR_FAST_8BIT)
  #undef BASE_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined BASE_START_SEC_VAR_FAST_16BIT)
  #undef BASE_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined BASE_STOP_SEC_VAR_FAST_16BIT)
  #undef BASE_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined BASE_START_SEC_VAR_FAST_32BIT)
  #undef BASE_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined BASE_STOP_SEC_VAR_FAST_32BIT)
  #undef BASE_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined BASE_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef BASE_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined BASE_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef BASE_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined BASE_START_SEC_VAR_8BIT)
  #undef BASE_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined BASE_STOP_SEC_VAR_8BIT)
  #undef BASE_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined BASE_START_SEC_VAR_16BIT)
  #undef BASE_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined BASE_STOP_SEC_VAR_16BIT)
  #undef BASE_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined BASE_START_SEC_VAR_32BIT)
  #undef BASE_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined BASE_STOP_SEC_VAR_32BIT)
  #undef BASE_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined BASE_START_SEC_VAR_UNSPECIFIED)
  #undef BASE_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined BASE_STOP_SEC_VAR_UNSPECIFIED)
  #undef BASE_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined BASE_START_SEC_CONST_8BIT)
  #undef BASE_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined BASE_STOP_SEC_CONST_8BIT)
  #undef BASE_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined BASE_START_SEC_CONST_16BIT)
  #undef BASE_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined BASE_STOP_SEC_CONST_16BIT)
  #undef BASE_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined BASE_START_SEC_CONST_32BIT)
  #undef BASE_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined BASE_STOP_SEC_CONST_32BIT)
  #undef BASE_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined BASE_START_SEC_CONST_UNSPECIFIED)
  #undef BASE_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined BASE_STOP_SEC_CONST_UNSPECIFIED)
  #undef BASE_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined BASE_START_CONFIG_DATA_8BIT)
  #undef BASE_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined BASE_STOP_CONFIG_DATA_8BIT)
  #undef BASE_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined BASE_START_CONFIG_DATA_16BIT)
  #undef BASE_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined BASE_STOP_CONFIG_DATA_16BIT)
  #undef BASE_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined BASE_START_CONFIG_DATA_32BIT)
  #undef BASE_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined BASE_STOP_CONFIG_DATA_32BIT)
  #undef BASE_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined BASE_START_CONFIG_DATA_UNSPECIFIED)
  #undef BASE_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined BASE_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef BASE_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined CAL_START_SEC_CODE)
  #undef CAL_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined CAL_STOP_SEC_CODE)
  #undef CAL_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined CAL_START_SEC_APPL_CODE)
  #undef CAL_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined CAL_STOP_SEC_APPL_CODE)
  #undef CAL_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined CAL_START_SEC_VAR_NOINIT_8BIT)
  #undef CAL_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined CAL_STOP_SEC_VAR_NOINIT_8BIT)
  #undef CAL_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined CAL_START_SEC_VAR_NOINIT_16BIT)
  #undef CAL_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined CAL_STOP_SEC_VAR_NOINIT_16BIT)
  #undef CAL_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined CAL_START_SEC_VAR_NOINIT_32BIT)
  #undef CAL_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined CAL_STOP_SEC_VAR_NOINIT_32BIT)
  #undef CAL_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined CAL_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef CAL_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CAL_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef CAL_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CAL_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef CAL_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined CAL_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef CAL_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined CAL_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef CAL_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined CAL_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef CAL_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined CAL_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef CAL_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined CAL_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef CAL_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined CAL_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef CAL_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CAL_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef CAL_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CAL_START_SEC_VAR_FAST_8BIT)
  #undef CAL_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined CAL_STOP_SEC_VAR_FAST_8BIT)
  #undef CAL_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined CAL_START_SEC_VAR_FAST_16BIT)
  #undef CAL_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined CAL_STOP_SEC_VAR_FAST_16BIT)
  #undef CAL_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined CAL_START_SEC_VAR_FAST_32BIT)
  #undef CAL_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined CAL_STOP_SEC_VAR_FAST_32BIT)
  #undef CAL_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined CAL_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef CAL_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CAL_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef CAL_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CAL_START_SEC_VAR_8BIT)
  #undef CAL_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined CAL_STOP_SEC_VAR_8BIT)
  #undef CAL_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined CAL_START_SEC_VAR_16BIT)
  #undef CAL_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined CAL_STOP_SEC_VAR_16BIT)
  #undef CAL_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined CAL_START_SEC_VAR_32BIT)
  #undef CAL_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined CAL_STOP_SEC_VAR_32BIT)
  #undef CAL_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined CAL_START_SEC_VAR_UNSPECIFIED)
  #undef CAL_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CAL_STOP_SEC_VAR_UNSPECIFIED)
  #undef CAL_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CAL_START_SEC_CONST_8BIT)
  #undef CAL_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined CAL_STOP_SEC_CONST_8BIT)
  #undef CAL_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined CAL_START_SEC_CONST_16BIT)
  #undef CAL_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined CAL_STOP_SEC_CONST_16BIT)
  #undef CAL_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined CAL_START_SEC_CONST_32BIT)
  #undef CAL_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined CAL_STOP_SEC_CONST_32BIT)
  #undef CAL_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined CAL_START_SEC_CONST_UNSPECIFIED)
  #undef CAL_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CAL_STOP_SEC_CONST_UNSPECIFIED)
  #undef CAL_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CAL_START_CONFIG_DATA_8BIT)
  #undef CAL_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined CAL_STOP_CONFIG_DATA_8BIT)
  #undef CAL_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined CAL_START_CONFIG_DATA_16BIT)
  #undef CAL_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined CAL_STOP_CONFIG_DATA_16BIT)
  #undef CAL_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined CAL_START_CONFIG_DATA_32BIT)
  #undef CAL_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined CAL_STOP_CONFIG_DATA_32BIT)
  #undef CAL_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined CAL_START_CONFIG_DATA_UNSPECIFIED)
  #undef CAL_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CAL_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef CAL_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined CAN_START_SEC_CODE)
  #undef CAN_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined CAN_STOP_SEC_CODE)
  #undef CAN_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined CAN_START_SEC_APPL_CODE)
  #undef CAN_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined CAN_STOP_SEC_APPL_CODE)
  #undef CAN_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined CAN_START_SEC_VAR_NOINIT_8BIT)
  #undef CAN_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined CAN_STOP_SEC_VAR_NOINIT_8BIT)
  #undef CAN_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined CAN_START_SEC_VAR_NOINIT_16BIT)
  #undef CAN_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined CAN_STOP_SEC_VAR_NOINIT_16BIT)
  #undef CAN_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined CAN_START_SEC_VAR_NOINIT_32BIT)
  #undef CAN_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined CAN_STOP_SEC_VAR_NOINIT_32BIT)
  #undef CAN_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined CAN_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef CAN_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CAN_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef CAN_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CAN_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef CAN_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined CAN_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef CAN_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined CAN_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef CAN_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined CAN_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef CAN_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined CAN_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef CAN_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined CAN_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef CAN_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined CAN_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef CAN_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CAN_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef CAN_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CAN_START_SEC_VAR_FAST_8BIT)
  #undef CAN_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined CAN_STOP_SEC_VAR_FAST_8BIT)
  #undef CAN_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined CAN_START_SEC_VAR_FAST_16BIT)
  #undef CAN_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined CAN_STOP_SEC_VAR_FAST_16BIT)
  #undef CAN_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined CAN_START_SEC_VAR_FAST_32BIT)
  #undef CAN_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined CAN_STOP_SEC_VAR_FAST_32BIT)
  #undef CAN_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined CAN_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef CAN_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CAN_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef CAN_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CAN_START_SEC_VAR_8BIT)
  #undef CAN_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined CAN_STOP_SEC_VAR_8BIT)
  #undef CAN_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined CAN_START_SEC_VAR_16BIT)
  #undef CAN_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined CAN_STOP_SEC_VAR_16BIT)
  #undef CAN_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined CAN_START_SEC_VAR_32BIT)
  #undef CAN_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined CAN_STOP_SEC_VAR_32BIT)
  #undef CAN_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined CAN_START_SEC_VAR_UNSPECIFIED)
  #undef CAN_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CAN_STOP_SEC_VAR_UNSPECIFIED)
  #undef CAN_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CAN_START_SEC_CONST_8BIT)
  #undef CAN_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined CAN_STOP_SEC_CONST_8BIT)
  #undef CAN_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined CAN_START_SEC_CONST_16BIT)
  #undef CAN_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined CAN_STOP_SEC_CONST_16BIT)
  #undef CAN_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined CAN_START_SEC_CONST_32BIT)
  #undef CAN_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined CAN_STOP_SEC_CONST_32BIT)
  #undef CAN_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined CAN_START_SEC_CONST_UNSPECIFIED)
  #undef CAN_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CAN_STOP_SEC_CONST_UNSPECIFIED)
  #undef CAN_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CAN_START_CONFIG_DATA_8BIT)
  #undef CAN_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined CAN_STOP_CONFIG_DATA_8BIT)
  #undef CAN_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined CAN_START_CONFIG_DATA_16BIT)
  #undef CAN_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined CAN_STOP_CONFIG_DATA_16BIT)
  #undef CAN_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined CAN_START_CONFIG_DATA_32BIT)
  #undef CAN_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined CAN_STOP_CONFIG_DATA_32BIT)
  #undef CAN_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined CAN_START_CONFIG_DATA_UNSPECIFIED)
  #undef CAN_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CAN_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef CAN_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined CANIF_START_SEC_CODE)
  #undef CANIF_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined CANIF_STOP_SEC_CODE)
  #undef CANIF_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined CANIF_START_SEC_APPL_CODE)
  #undef CANIF_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined CANIF_STOP_SEC_APPL_CODE)
  #undef CANIF_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined CANIF_START_SEC_VAR_NOINIT_8BIT)
  #undef CANIF_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANIF_STOP_SEC_VAR_NOINIT_8BIT)
  #undef CANIF_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANIF_START_SEC_VAR_NOINIT_16BIT)
  #undef CANIF_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANIF_STOP_SEC_VAR_NOINIT_16BIT)
  #undef CANIF_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANIF_START_SEC_VAR_NOINIT_32BIT)
  #undef CANIF_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANIF_STOP_SEC_VAR_NOINIT_32BIT)
  #undef CANIF_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANIF_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef CANIF_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANIF_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef CANIF_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANIF_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef CANIF_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANIF_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef CANIF_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANIF_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef CANIF_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANIF_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef CANIF_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANIF_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef CANIF_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANIF_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef CANIF_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANIF_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef CANIF_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANIF_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef CANIF_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANIF_START_SEC_VAR_FAST_8BIT)
  #undef CANIF_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANIF_STOP_SEC_VAR_FAST_8BIT)
  #undef CANIF_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANIF_START_SEC_VAR_FAST_16BIT)
  #undef CANIF_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANIF_STOP_SEC_VAR_FAST_16BIT)
  #undef CANIF_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANIF_START_SEC_VAR_FAST_32BIT)
  #undef CANIF_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANIF_STOP_SEC_VAR_FAST_32BIT)
  #undef CANIF_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANIF_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef CANIF_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANIF_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef CANIF_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANIF_START_SEC_VAR_8BIT)
  #undef CANIF_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANIF_STOP_SEC_VAR_8BIT)
  #undef CANIF_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANIF_START_SEC_VAR_16BIT)
  #undef CANIF_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANIF_STOP_SEC_VAR_16BIT)
  #undef CANIF_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANIF_START_SEC_VAR_32BIT)
  #undef CANIF_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANIF_STOP_SEC_VAR_32BIT)
  #undef CANIF_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANIF_START_SEC_VAR_UNSPECIFIED)
  #undef CANIF_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANIF_STOP_SEC_VAR_UNSPECIFIED)
  #undef CANIF_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANIF_START_SEC_CONST_8BIT)
  #undef CANIF_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANIF_STOP_SEC_CONST_8BIT)
  #undef CANIF_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANIF_START_SEC_CONST_16BIT)
  #undef CANIF_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANIF_STOP_SEC_CONST_16BIT)
  #undef CANIF_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANIF_START_SEC_CONST_32BIT)
  #undef CANIF_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANIF_STOP_SEC_CONST_32BIT)
  #undef CANIF_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANIF_START_SEC_CONST_UNSPECIFIED)
  #undef CANIF_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANIF_STOP_SEC_CONST_UNSPECIFIED)
  #undef CANIF_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANIF_START_CONFIG_DATA_8BIT)
  #undef CANIF_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANIF_STOP_CONFIG_DATA_8BIT)
  #undef CANIF_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANIF_START_CONFIG_DATA_16BIT)
  #undef CANIF_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANIF_STOP_CONFIG_DATA_16BIT)
  #undef CANIF_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANIF_START_CONFIG_DATA_32BIT)
  #undef CANIF_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANIF_STOP_CONFIG_DATA_32BIT)
  #undef CANIF_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANIF_START_CONFIG_DATA_UNSPECIFIED)
  #undef CANIF_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANIF_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef CANIF_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined CANNM_START_SEC_CODE)
  #undef CANNM_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined CANNM_STOP_SEC_CODE)
  #undef CANNM_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined CANNM_START_SEC_APPL_CODE)
  #undef CANNM_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined CANNM_STOP_SEC_APPL_CODE)
  #undef CANNM_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined CANNM_START_SEC_VAR_NOINIT_8BIT)
  #undef CANNM_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANNM_STOP_SEC_VAR_NOINIT_8BIT)
  #undef CANNM_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANNM_START_SEC_VAR_NOINIT_16BIT)
  #undef CANNM_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANNM_STOP_SEC_VAR_NOINIT_16BIT)
  #undef CANNM_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANNM_START_SEC_VAR_NOINIT_32BIT)
  #undef CANNM_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANNM_STOP_SEC_VAR_NOINIT_32BIT)
  #undef CANNM_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANNM_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef CANNM_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANNM_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef CANNM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANNM_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef CANNM_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANNM_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef CANNM_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANNM_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef CANNM_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANNM_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef CANNM_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANNM_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef CANNM_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANNM_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef CANNM_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANNM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef CANNM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANNM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef CANNM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANNM_START_SEC_VAR_FAST_8BIT)
  #undef CANNM_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANNM_STOP_SEC_VAR_FAST_8BIT)
  #undef CANNM_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANNM_START_SEC_VAR_FAST_16BIT)
  #undef CANNM_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANNM_STOP_SEC_VAR_FAST_16BIT)
  #undef CANNM_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANNM_START_SEC_VAR_FAST_32BIT)
  #undef CANNM_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANNM_STOP_SEC_VAR_FAST_32BIT)
  #undef CANNM_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANNM_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef CANNM_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANNM_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef CANNM_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANNM_START_SEC_VAR_8BIT)
  #undef CANNM_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANNM_STOP_SEC_VAR_8BIT)
  #undef CANNM_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANNM_START_SEC_VAR_16BIT)
  #undef CANNM_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANNM_STOP_SEC_VAR_16BIT)
  #undef CANNM_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANNM_START_SEC_VAR_32BIT)
  #undef CANNM_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANNM_STOP_SEC_VAR_32BIT)
  #undef CANNM_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANNM_START_SEC_VAR_UNSPECIFIED)
  #undef CANNM_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANNM_STOP_SEC_VAR_UNSPECIFIED)
  #undef CANNM_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANNM_START_SEC_CONST_8BIT)
  #undef CANNM_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANNM_STOP_SEC_CONST_8BIT)
  #undef CANNM_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANNM_START_SEC_CONST_16BIT)
  #undef CANNM_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANNM_STOP_SEC_CONST_16BIT)
  #undef CANNM_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANNM_START_SEC_CONST_32BIT)
  #undef CANNM_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANNM_STOP_SEC_CONST_32BIT)
  #undef CANNM_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANNM_START_SEC_CONST_UNSPECIFIED)
  #undef CANNM_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANNM_STOP_SEC_CONST_UNSPECIFIED)
  #undef CANNM_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANNM_START_CONFIG_DATA_8BIT)
  #undef CANNM_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANNM_STOP_CONFIG_DATA_8BIT)
  #undef CANNM_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANNM_START_CONFIG_DATA_16BIT)
  #undef CANNM_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANNM_STOP_CONFIG_DATA_16BIT)
  #undef CANNM_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANNM_START_CONFIG_DATA_32BIT)
  #undef CANNM_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANNM_STOP_CONFIG_DATA_32BIT)
  #undef CANNM_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANNM_START_CONFIG_DATA_UNSPECIFIED)
  #undef CANNM_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANNM_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef CANNM_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined CANSM_START_SEC_CODE)
  #undef CANSM_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined CANSM_STOP_SEC_CODE)
  #undef CANSM_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined CANSM_START_SEC_APPL_CODE)
  #undef CANSM_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined CANSM_STOP_SEC_APPL_CODE)
  #undef CANSM_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined CANSM_START_SEC_VAR_NOINIT_8BIT)
  #undef CANSM_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANSM_STOP_SEC_VAR_NOINIT_8BIT)
  #undef CANSM_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANSM_START_SEC_VAR_NOINIT_16BIT)
  #undef CANSM_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANSM_STOP_SEC_VAR_NOINIT_16BIT)
  #undef CANSM_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANSM_START_SEC_VAR_NOINIT_32BIT)
  #undef CANSM_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANSM_STOP_SEC_VAR_NOINIT_32BIT)
  #undef CANSM_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANSM_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef CANSM_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANSM_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef CANSM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANSM_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef CANSM_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANSM_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef CANSM_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANSM_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef CANSM_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANSM_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef CANSM_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANSM_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef CANSM_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANSM_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef CANSM_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANSM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef CANSM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANSM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef CANSM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANSM_START_SEC_VAR_FAST_8BIT)
  #undef CANSM_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANSM_STOP_SEC_VAR_FAST_8BIT)
  #undef CANSM_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANSM_START_SEC_VAR_FAST_16BIT)
  #undef CANSM_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANSM_STOP_SEC_VAR_FAST_16BIT)
  #undef CANSM_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANSM_START_SEC_VAR_FAST_32BIT)
  #undef CANSM_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANSM_STOP_SEC_VAR_FAST_32BIT)
  #undef CANSM_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANSM_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef CANSM_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANSM_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef CANSM_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANSM_START_SEC_VAR_8BIT)
  #undef CANSM_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANSM_STOP_SEC_VAR_8BIT)
  #undef CANSM_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANSM_START_SEC_VAR_16BIT)
  #undef CANSM_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANSM_STOP_SEC_VAR_16BIT)
  #undef CANSM_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANSM_START_SEC_VAR_32BIT)
  #undef CANSM_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANSM_STOP_SEC_VAR_32BIT)
  #undef CANSM_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANSM_START_SEC_VAR_UNSPECIFIED)
  #undef CANSM_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANSM_STOP_SEC_VAR_UNSPECIFIED)
  #undef CANSM_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANSM_START_SEC_CONST_8BIT)
  #undef CANSM_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANSM_STOP_SEC_CONST_8BIT)
  #undef CANSM_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANSM_START_SEC_CONST_16BIT)
  #undef CANSM_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANSM_STOP_SEC_CONST_16BIT)
  #undef CANSM_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANSM_START_SEC_CONST_32BIT)
  #undef CANSM_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANSM_STOP_SEC_CONST_32BIT)
  #undef CANSM_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANSM_START_SEC_CONST_UNSPECIFIED)
  #undef CANSM_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANSM_STOP_SEC_CONST_UNSPECIFIED)
  #undef CANSM_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANSM_START_CONFIG_DATA_8BIT)
  #undef CANSM_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANSM_STOP_CONFIG_DATA_8BIT)
  #undef CANSM_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANSM_START_CONFIG_DATA_16BIT)
  #undef CANSM_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANSM_STOP_CONFIG_DATA_16BIT)
  #undef CANSM_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANSM_START_CONFIG_DATA_32BIT)
  #undef CANSM_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANSM_STOP_CONFIG_DATA_32BIT)
  #undef CANSM_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANSM_START_CONFIG_DATA_UNSPECIFIED)
  #undef CANSM_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANSM_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef CANSM_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined CANTP_START_SEC_CODE)
  #undef CANTP_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined CANTP_STOP_SEC_CODE)
  #undef CANTP_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined CANTP_START_SEC_APPL_CODE)
  #undef CANTP_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined CANTP_STOP_SEC_APPL_CODE)
  #undef CANTP_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined CANTP_START_SEC_VAR_NOINIT_8BIT)
  #undef CANTP_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANTP_STOP_SEC_VAR_NOINIT_8BIT)
  #undef CANTP_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANTP_START_SEC_VAR_NOINIT_16BIT)
  #undef CANTP_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANTP_STOP_SEC_VAR_NOINIT_16BIT)
  #undef CANTP_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANTP_START_SEC_VAR_NOINIT_32BIT)
  #undef CANTP_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANTP_STOP_SEC_VAR_NOINIT_32BIT)
  #undef CANTP_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANTP_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef CANTP_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANTP_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef CANTP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANTP_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef CANTP_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANTP_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef CANTP_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANTP_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef CANTP_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANTP_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef CANTP_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANTP_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef CANTP_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANTP_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef CANTP_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANTP_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef CANTP_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANTP_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef CANTP_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANTP_START_SEC_VAR_FAST_8BIT)
  #undef CANTP_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANTP_STOP_SEC_VAR_FAST_8BIT)
  #undef CANTP_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANTP_START_SEC_VAR_FAST_16BIT)
  #undef CANTP_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANTP_STOP_SEC_VAR_FAST_16BIT)
  #undef CANTP_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANTP_START_SEC_VAR_FAST_32BIT)
  #undef CANTP_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANTP_STOP_SEC_VAR_FAST_32BIT)
  #undef CANTP_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANTP_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef CANTP_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANTP_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef CANTP_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANTP_START_SEC_VAR_8BIT)
  #undef CANTP_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANTP_STOP_SEC_VAR_8BIT)
  #undef CANTP_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANTP_START_SEC_VAR_16BIT)
  #undef CANTP_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANTP_STOP_SEC_VAR_16BIT)
  #undef CANTP_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANTP_START_SEC_VAR_32BIT)
  #undef CANTP_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANTP_STOP_SEC_VAR_32BIT)
  #undef CANTP_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANTP_START_SEC_VAR_UNSPECIFIED)
  #undef CANTP_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANTP_STOP_SEC_VAR_UNSPECIFIED)
  #undef CANTP_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANTP_START_SEC_CONST_8BIT)
  #undef CANTP_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANTP_STOP_SEC_CONST_8BIT)
  #undef CANTP_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANTP_START_SEC_CONST_16BIT)
  #undef CANTP_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANTP_STOP_SEC_CONST_16BIT)
  #undef CANTP_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANTP_START_SEC_CONST_32BIT)
  #undef CANTP_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANTP_STOP_SEC_CONST_32BIT)
  #undef CANTP_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANTP_START_SEC_CONST_UNSPECIFIED)
  #undef CANTP_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANTP_STOP_SEC_CONST_UNSPECIFIED)
  #undef CANTP_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANTP_START_CONFIG_DATA_8BIT)
  #undef CANTP_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANTP_STOP_CONFIG_DATA_8BIT)
  #undef CANTP_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANTP_START_CONFIG_DATA_16BIT)
  #undef CANTP_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANTP_STOP_CONFIG_DATA_16BIT)
  #undef CANTP_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANTP_START_CONFIG_DATA_32BIT)
  #undef CANTP_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANTP_STOP_CONFIG_DATA_32BIT)
  #undef CANTP_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANTP_START_CONFIG_DATA_UNSPECIFIED)
  #undef CANTP_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANTP_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef CANTP_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANTP_START_SEC_CONST_ENTRY_JUMP_TABLE)
  #undef CANTP_START_SEC_CONST_ENTRY_JUMP_TABLE
  #undef MEMMAP_ERROR

#elif (defined CANTP_STOP_SEC_CONST_ENTRY_JUMP_TABLE)
  #undef CANTP_STOP_SEC_CONST_ENTRY_JUMP_TABLE
  #undef MEMMAP_ERROR

#elif (defined CANTP_START_SEC_CONST_EXIT_JUMP_TABLE)
  #undef CANTP_START_SEC_CONST_EXIT_JUMP_TABLE
  #undef MEMMAP_ERROR

#elif (defined CANTP_STOP_SEC_CONST_EXIT_JUMP_TABLE)
  #undef CANTP_STOP_SEC_CONST_EXIT_JUMP_TABLE
  #undef MEMMAP_ERROR

#elif (defined CANTP_START_SEC_VAR_ZEROINIT)
  #undef CANTP_START_SEC_VAR_ZEROINIT
  #undef MEMMAP_ERROR

#elif (defined CANTP_STOP_SEC_VAR_ZEROINIT)
  #undef CANTP_STOP_SEC_VAR_ZEROINIT
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined CANTRCV_START_SEC_CODE)
  #undef CANTRCV_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_STOP_SEC_CODE)
  #undef CANTRCV_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_START_SEC_APPL_CODE)
  #undef CANTRCV_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_STOP_SEC_APPL_CODE)
  #undef CANTRCV_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_START_SEC_VAR_NOINIT_8BIT)
  #undef CANTRCV_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_STOP_SEC_VAR_NOINIT_8BIT)
  #undef CANTRCV_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_START_SEC_VAR_NOINIT_16BIT)
  #undef CANTRCV_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_STOP_SEC_VAR_NOINIT_16BIT)
  #undef CANTRCV_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_START_SEC_VAR_NOINIT_32BIT)
  #undef CANTRCV_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_STOP_SEC_VAR_NOINIT_32BIT)
  #undef CANTRCV_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef CANTRCV_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef CANTRCV_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef CANTRCV_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef CANTRCV_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef CANTRCV_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef CANTRCV_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef CANTRCV_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef CANTRCV_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef CANTRCV_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef CANTRCV_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_START_SEC_VAR_FAST_8BIT)
  #undef CANTRCV_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_STOP_SEC_VAR_FAST_8BIT)
  #undef CANTRCV_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_START_SEC_VAR_FAST_16BIT)
  #undef CANTRCV_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_STOP_SEC_VAR_FAST_16BIT)
  #undef CANTRCV_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_START_SEC_VAR_FAST_32BIT)
  #undef CANTRCV_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_STOP_SEC_VAR_FAST_32BIT)
  #undef CANTRCV_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef CANTRCV_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef CANTRCV_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_START_SEC_VAR_8BIT)
  #undef CANTRCV_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_STOP_SEC_VAR_8BIT)
  #undef CANTRCV_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_START_SEC_VAR_16BIT)
  #undef CANTRCV_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_STOP_SEC_VAR_16BIT)
  #undef CANTRCV_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_START_SEC_VAR_32BIT)
  #undef CANTRCV_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_STOP_SEC_VAR_32BIT)
  #undef CANTRCV_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_START_SEC_VAR_UNSPECIFIED)
  #undef CANTRCV_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_STOP_SEC_VAR_UNSPECIFIED)
  #undef CANTRCV_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_START_SEC_CONST_8BIT)
  #undef CANTRCV_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_STOP_SEC_CONST_8BIT)
  #undef CANTRCV_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_START_SEC_CONST_16BIT)
  #undef CANTRCV_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_STOP_SEC_CONST_16BIT)
  #undef CANTRCV_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_START_SEC_CONST_32BIT)
  #undef CANTRCV_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_STOP_SEC_CONST_32BIT)
  #undef CANTRCV_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_START_SEC_CONST_UNSPECIFIED)
  #undef CANTRCV_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_STOP_SEC_CONST_UNSPECIFIED)
  #undef CANTRCV_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_START_CONFIG_DATA_8BIT)
  #undef CANTRCV_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_STOP_CONFIG_DATA_8BIT)
  #undef CANTRCV_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_START_CONFIG_DATA_16BIT)
  #undef CANTRCV_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_STOP_CONFIG_DATA_16BIT)
  #undef CANTRCV_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_START_CONFIG_DATA_32BIT)
  #undef CANTRCV_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_STOP_CONFIG_DATA_32BIT)
  #undef CANTRCV_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_START_CONFIG_DATA_UNSPECIFIED)
  #undef CANTRCV_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef CANTRCV_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined CANTRCV_1_T01_START_SEC_CODE)
  #undef CANTRCV_1_T01_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T01_STOP_SEC_CODE)
  #undef CANTRCV_1_T01_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T01_START_SEC_APPL_CODE)
  #undef CANTRCV_1_T01_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T01_STOP_SEC_APPL_CODE)
  #undef CANTRCV_1_T01_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T01_START_SEC_VAR_NOINIT_8BIT)
  #undef CANTRCV_1_T01_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T01_STOP_SEC_VAR_NOINIT_8BIT)
  #undef CANTRCV_1_T01_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T01_START_SEC_VAR_NOINIT_16BIT)
  #undef CANTRCV_1_T01_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T01_STOP_SEC_VAR_NOINIT_16BIT)
  #undef CANTRCV_1_T01_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T01_START_SEC_VAR_NOINIT_32BIT)
  #undef CANTRCV_1_T01_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T01_STOP_SEC_VAR_NOINIT_32BIT)
  #undef CANTRCV_1_T01_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T01_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef CANTRCV_1_T01_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T01_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef CANTRCV_1_T01_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T01_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef CANTRCV_1_T01_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T01_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef CANTRCV_1_T01_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T01_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef CANTRCV_1_T01_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T01_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef CANTRCV_1_T01_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T01_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef CANTRCV_1_T01_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T01_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef CANTRCV_1_T01_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T01_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef CANTRCV_1_T01_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T01_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef CANTRCV_1_T01_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T01_START_SEC_VAR_FAST_8BIT)
  #undef CANTRCV_1_T01_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T01_STOP_SEC_VAR_FAST_8BIT)
  #undef CANTRCV_1_T01_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T01_START_SEC_VAR_FAST_16BIT)
  #undef CANTRCV_1_T01_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T01_STOP_SEC_VAR_FAST_16BIT)
  #undef CANTRCV_1_T01_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T01_START_SEC_VAR_FAST_32BIT)
  #undef CANTRCV_1_T01_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T01_STOP_SEC_VAR_FAST_32BIT)
  #undef CANTRCV_1_T01_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T01_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef CANTRCV_1_T01_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T01_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef CANTRCV_1_T01_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T01_START_SEC_VAR_8BIT)
  #undef CANTRCV_1_T01_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T01_STOP_SEC_VAR_8BIT)
  #undef CANTRCV_1_T01_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T01_START_SEC_VAR_16BIT)
  #undef CANTRCV_1_T01_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T01_STOP_SEC_VAR_16BIT)
  #undef CANTRCV_1_T01_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T01_START_SEC_VAR_32BIT)
  #undef CANTRCV_1_T01_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T01_STOP_SEC_VAR_32BIT)
  #undef CANTRCV_1_T01_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T01_START_SEC_VAR_UNSPECIFIED)
  #undef CANTRCV_1_T01_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T01_STOP_SEC_VAR_UNSPECIFIED)
  #undef CANTRCV_1_T01_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T01_START_SEC_CONST_8BIT)
  #undef CANTRCV_1_T01_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T01_STOP_SEC_CONST_8BIT)
  #undef CANTRCV_1_T01_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T01_START_SEC_CONST_16BIT)
  #undef CANTRCV_1_T01_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T01_STOP_SEC_CONST_16BIT)
  #undef CANTRCV_1_T01_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T01_START_SEC_CONST_32BIT)
  #undef CANTRCV_1_T01_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T01_STOP_SEC_CONST_32BIT)
  #undef CANTRCV_1_T01_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T01_START_SEC_CONST_UNSPECIFIED)
  #undef CANTRCV_1_T01_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T01_STOP_SEC_CONST_UNSPECIFIED)
  #undef CANTRCV_1_T01_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T01_START_CONFIG_DATA_8BIT)
  #undef CANTRCV_1_T01_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T01_STOP_CONFIG_DATA_8BIT)
  #undef CANTRCV_1_T01_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T01_START_CONFIG_DATA_16BIT)
  #undef CANTRCV_1_T01_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T01_STOP_CONFIG_DATA_16BIT)
  #undef CANTRCV_1_T01_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T01_START_CONFIG_DATA_32BIT)
  #undef CANTRCV_1_T01_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T01_STOP_CONFIG_DATA_32BIT)
  #undef CANTRCV_1_T01_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T01_START_CONFIG_DATA_UNSPECIFIED)
  #undef CANTRCV_1_T01_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T01_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef CANTRCV_1_T01_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined CANTRCV_1_T02_START_SEC_CODE)
  #undef CANTRCV_1_T02_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T02_STOP_SEC_CODE)
  #undef CANTRCV_1_T02_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T02_START_SEC_APPL_CODE)
  #undef CANTRCV_1_T02_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T02_STOP_SEC_APPL_CODE)
  #undef CANTRCV_1_T02_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T02_START_SEC_VAR_NOINIT_8BIT)
  #undef CANTRCV_1_T02_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T02_STOP_SEC_VAR_NOINIT_8BIT)
  #undef CANTRCV_1_T02_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T02_START_SEC_VAR_NOINIT_16BIT)
  #undef CANTRCV_1_T02_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T02_STOP_SEC_VAR_NOINIT_16BIT)
  #undef CANTRCV_1_T02_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T02_START_SEC_VAR_NOINIT_32BIT)
  #undef CANTRCV_1_T02_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T02_STOP_SEC_VAR_NOINIT_32BIT)
  #undef CANTRCV_1_T02_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T02_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef CANTRCV_1_T02_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T02_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef CANTRCV_1_T02_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T02_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef CANTRCV_1_T02_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T02_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef CANTRCV_1_T02_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T02_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef CANTRCV_1_T02_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T02_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef CANTRCV_1_T02_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T02_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef CANTRCV_1_T02_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T02_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef CANTRCV_1_T02_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T02_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef CANTRCV_1_T02_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T02_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef CANTRCV_1_T02_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T02_START_SEC_VAR_FAST_8BIT)
  #undef CANTRCV_1_T02_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T02_STOP_SEC_VAR_FAST_8BIT)
  #undef CANTRCV_1_T02_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T02_START_SEC_VAR_FAST_16BIT)
  #undef CANTRCV_1_T02_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T02_STOP_SEC_VAR_FAST_16BIT)
  #undef CANTRCV_1_T02_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T02_START_SEC_VAR_FAST_32BIT)
  #undef CANTRCV_1_T02_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T02_STOP_SEC_VAR_FAST_32BIT)
  #undef CANTRCV_1_T02_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T02_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef CANTRCV_1_T02_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T02_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef CANTRCV_1_T02_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T02_START_SEC_VAR_8BIT)
  #undef CANTRCV_1_T02_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T02_STOP_SEC_VAR_8BIT)
  #undef CANTRCV_1_T02_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T02_START_SEC_VAR_16BIT)
  #undef CANTRCV_1_T02_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T02_STOP_SEC_VAR_16BIT)
  #undef CANTRCV_1_T02_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T02_START_SEC_VAR_32BIT)
  #undef CANTRCV_1_T02_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T02_STOP_SEC_VAR_32BIT)
  #undef CANTRCV_1_T02_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T02_START_SEC_VAR_UNSPECIFIED)
  #undef CANTRCV_1_T02_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T02_STOP_SEC_VAR_UNSPECIFIED)
  #undef CANTRCV_1_T02_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T02_START_SEC_CONST_8BIT)
  #undef CANTRCV_1_T02_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T02_STOP_SEC_CONST_8BIT)
  #undef CANTRCV_1_T02_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T02_START_SEC_CONST_16BIT)
  #undef CANTRCV_1_T02_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T02_STOP_SEC_CONST_16BIT)
  #undef CANTRCV_1_T02_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T02_START_SEC_CONST_32BIT)
  #undef CANTRCV_1_T02_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T02_STOP_SEC_CONST_32BIT)
  #undef CANTRCV_1_T02_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T02_START_SEC_CONST_UNSPECIFIED)
  #undef CANTRCV_1_T02_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T02_STOP_SEC_CONST_UNSPECIFIED)
  #undef CANTRCV_1_T02_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T02_START_CONFIG_DATA_8BIT)
  #undef CANTRCV_1_T02_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T02_STOP_CONFIG_DATA_8BIT)
  #undef CANTRCV_1_T02_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T02_START_CONFIG_DATA_16BIT)
  #undef CANTRCV_1_T02_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T02_STOP_CONFIG_DATA_16BIT)
  #undef CANTRCV_1_T02_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T02_START_CONFIG_DATA_32BIT)
  #undef CANTRCV_1_T02_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T02_STOP_CONFIG_DATA_32BIT)
  #undef CANTRCV_1_T02_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T02_START_CONFIG_DATA_UNSPECIFIED)
  #undef CANTRCV_1_T02_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CANTRCV_1_T02_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef CANTRCV_1_T02_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined CNM_START_SEC_CODE)
  #undef CNM_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined CNM_STOP_SEC_CODE)
  #undef CNM_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined CNM_START_SEC_APPL_CODE)
  #undef CNM_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined CNM_STOP_SEC_APPL_CODE)
  #undef CNM_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined CNM_START_SEC_VAR_NOINIT_8BIT)
  #undef CNM_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined CNM_STOP_SEC_VAR_NOINIT_8BIT)
  #undef CNM_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined CNM_START_SEC_VAR_NOINIT_16BIT)
  #undef CNM_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined CNM_STOP_SEC_VAR_NOINIT_16BIT)
  #undef CNM_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined CNM_START_SEC_VAR_NOINIT_32BIT)
  #undef CNM_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined CNM_STOP_SEC_VAR_NOINIT_32BIT)
  #undef CNM_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined CNM_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef CNM_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CNM_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef CNM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CNM_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef CNM_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined CNM_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef CNM_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined CNM_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef CNM_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined CNM_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef CNM_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined CNM_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef CNM_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined CNM_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef CNM_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined CNM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef CNM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CNM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef CNM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CNM_START_SEC_VAR_FAST_8BIT)
  #undef CNM_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined CNM_STOP_SEC_VAR_FAST_8BIT)
  #undef CNM_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined CNM_START_SEC_VAR_FAST_16BIT)
  #undef CNM_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined CNM_STOP_SEC_VAR_FAST_16BIT)
  #undef CNM_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined CNM_START_SEC_VAR_FAST_32BIT)
  #undef CNM_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined CNM_STOP_SEC_VAR_FAST_32BIT)
  #undef CNM_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined CNM_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef CNM_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CNM_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef CNM_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CNM_START_SEC_VAR_8BIT)
  #undef CNM_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined CNM_STOP_SEC_VAR_8BIT)
  #undef CNM_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined CNM_START_SEC_VAR_16BIT)
  #undef CNM_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined CNM_STOP_SEC_VAR_16BIT)
  #undef CNM_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined CNM_START_SEC_VAR_32BIT)
  #undef CNM_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined CNM_STOP_SEC_VAR_32BIT)
  #undef CNM_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined CNM_START_SEC_VAR_UNSPECIFIED)
  #undef CNM_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CNM_STOP_SEC_VAR_UNSPECIFIED)
  #undef CNM_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CNM_START_SEC_CONST_8BIT)
  #undef CNM_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined CNM_STOP_SEC_CONST_8BIT)
  #undef CNM_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined CNM_START_SEC_CONST_16BIT)
  #undef CNM_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined CNM_STOP_SEC_CONST_16BIT)
  #undef CNM_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined CNM_START_SEC_CONST_32BIT)
  #undef CNM_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined CNM_STOP_SEC_CONST_32BIT)
  #undef CNM_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined CNM_START_SEC_CONST_UNSPECIFIED)
  #undef CNM_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CNM_STOP_SEC_CONST_UNSPECIFIED)
  #undef CNM_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CNM_START_CONFIG_DATA_8BIT)
  #undef CNM_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined CNM_STOP_CONFIG_DATA_8BIT)
  #undef CNM_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined CNM_START_CONFIG_DATA_16BIT)
  #undef CNM_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined CNM_STOP_CONFIG_DATA_16BIT)
  #undef CNM_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined CNM_START_CONFIG_DATA_32BIT)
  #undef CNM_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined CNM_STOP_CONFIG_DATA_32BIT)
  #undef CNM_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined CNM_START_CONFIG_DATA_UNSPECIFIED)
  #undef CNM_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CNM_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef CNM_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined COM_START_SEC_CODE)
  #undef COM_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined COM_STOP_SEC_CODE)
  #undef COM_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined COM_START_SEC_APPL_CODE)
  #undef COM_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined COM_STOP_SEC_APPL_CODE)
  #undef COM_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined COM_START_SEC_VAR_NOINIT_8BIT)
  #undef COM_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined COM_STOP_SEC_VAR_NOINIT_8BIT)
  #undef COM_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined COM_START_SEC_VAR_NOINIT_16BIT)
  #undef COM_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined COM_STOP_SEC_VAR_NOINIT_16BIT)
  #undef COM_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined COM_START_SEC_VAR_NOINIT_32BIT)
  #undef COM_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined COM_STOP_SEC_VAR_NOINIT_32BIT)
  #undef COM_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined COM_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef COM_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined COM_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef COM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined COM_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef COM_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined COM_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef COM_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined COM_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef COM_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined COM_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef COM_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined COM_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef COM_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined COM_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef COM_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined COM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef COM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined COM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef COM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined COM_START_SEC_VAR_FAST_8BIT)
  #undef COM_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined COM_STOP_SEC_VAR_FAST_8BIT)
  #undef COM_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined COM_START_SEC_VAR_FAST_16BIT)
  #undef COM_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined COM_STOP_SEC_VAR_FAST_16BIT)
  #undef COM_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined COM_START_SEC_VAR_FAST_32BIT)
  #undef COM_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined COM_STOP_SEC_VAR_FAST_32BIT)
  #undef COM_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined COM_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef COM_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined COM_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef COM_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined COM_START_SEC_VAR_8BIT)
  #undef COM_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined COM_STOP_SEC_VAR_8BIT)
  #undef COM_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined COM_START_SEC_VAR_16BIT)
  #undef COM_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined COM_STOP_SEC_VAR_16BIT)
  #undef COM_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined COM_START_SEC_VAR_32BIT)
  #undef COM_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined COM_STOP_SEC_VAR_32BIT)
  #undef COM_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined COM_START_SEC_VAR_UNSPECIFIED)
  #undef COM_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined COM_STOP_SEC_VAR_UNSPECIFIED)
  #undef COM_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined COM_START_SEC_CONST_8BIT)
  #undef COM_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined COM_STOP_SEC_CONST_8BIT)
  #undef COM_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined COM_START_SEC_CONST_16BIT)
  #undef COM_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined COM_STOP_SEC_CONST_16BIT)
  #undef COM_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined COM_START_SEC_CONST_32BIT)
  #undef COM_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined COM_STOP_SEC_CONST_32BIT)
  #undef COM_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined COM_START_SEC_CONST_UNSPECIFIED)
  #undef COM_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined COM_STOP_SEC_CONST_UNSPECIFIED)
  #undef COM_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined COM_START_CONFIG_DATA_8BIT)
  #undef COM_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined COM_STOP_CONFIG_DATA_8BIT)
  #undef COM_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined COM_START_CONFIG_DATA_16BIT)
  #undef COM_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined COM_STOP_CONFIG_DATA_16BIT)
  #undef COM_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined COM_START_CONFIG_DATA_32BIT)
  #undef COM_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined COM_STOP_CONFIG_DATA_32BIT)
  #undef COM_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined COM_START_CONFIG_DATA_UNSPECIFIED)
  #undef COM_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined COM_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef COM_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined COMM_START_SEC_CODE)
  #undef COMM_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined COMM_STOP_SEC_CODE)
  #undef COMM_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined COMM_START_SEC_APPL_CODE)
  #undef COMM_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined COMM_STOP_SEC_APPL_CODE)
  #undef COMM_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined COMM_START_SEC_VAR_NOINIT_8BIT)
  #undef COMM_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined COMM_STOP_SEC_VAR_NOINIT_8BIT)
  #undef COMM_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined COMM_START_SEC_VAR_NOINIT_16BIT)
  #undef COMM_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined COMM_STOP_SEC_VAR_NOINIT_16BIT)
  #undef COMM_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined COMM_START_SEC_VAR_NOINIT_32BIT)
  #undef COMM_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined COMM_STOP_SEC_VAR_NOINIT_32BIT)
  #undef COMM_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined COMM_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef COMM_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined COMM_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef COMM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined COMM_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef COMM_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined COMM_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef COMM_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined COMM_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef COMM_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined COMM_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef COMM_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined COMM_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef COMM_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined COMM_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef COMM_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined COMM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef COMM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined COMM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef COMM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined COMM_START_SEC_VAR_FAST_8BIT)
  #undef COMM_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined COMM_STOP_SEC_VAR_FAST_8BIT)
  #undef COMM_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined COMM_START_SEC_VAR_FAST_16BIT)
  #undef COMM_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined COMM_STOP_SEC_VAR_FAST_16BIT)
  #undef COMM_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined COMM_START_SEC_VAR_FAST_32BIT)
  #undef COMM_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined COMM_STOP_SEC_VAR_FAST_32BIT)
  #undef COMM_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined COMM_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef COMM_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined COMM_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef COMM_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined COMM_START_SEC_VAR_8BIT)
  #undef COMM_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined COMM_STOP_SEC_VAR_8BIT)
  #undef COMM_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined COMM_START_SEC_VAR_16BIT)
  #undef COMM_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined COMM_STOP_SEC_VAR_16BIT)
  #undef COMM_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined COMM_START_SEC_VAR_32BIT)
  #undef COMM_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined COMM_STOP_SEC_VAR_32BIT)
  #undef COMM_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined COMM_START_SEC_VAR_UNSPECIFIED)
  #undef COMM_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined COMM_STOP_SEC_VAR_UNSPECIFIED)
  #undef COMM_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined COMM_START_SEC_CONST_8BIT)
  #undef COMM_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined COMM_STOP_SEC_CONST_8BIT)
  #undef COMM_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined COMM_START_SEC_CONST_16BIT)
  #undef COMM_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined COMM_STOP_SEC_CONST_16BIT)
  #undef COMM_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined COMM_START_SEC_CONST_32BIT)
  #undef COMM_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined COMM_STOP_SEC_CONST_32BIT)
  #undef COMM_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined COMM_START_SEC_CONST_UNSPECIFIED)
  #undef COMM_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined COMM_STOP_SEC_CONST_UNSPECIFIED)
  #undef COMM_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined COMM_START_CONFIG_DATA_8BIT)
  #undef COMM_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined COMM_STOP_CONFIG_DATA_8BIT)
  #undef COMM_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined COMM_START_CONFIG_DATA_16BIT)
  #undef COMM_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined COMM_STOP_CONFIG_DATA_16BIT)
  #undef COMM_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined COMM_START_CONFIG_DATA_32BIT)
  #undef COMM_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined COMM_STOP_CONFIG_DATA_32BIT)
  #undef COMM_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined COMM_START_CONFIG_DATA_UNSPECIFIED)
  #undef COMM_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined COMM_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef COMM_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined CPL_START_SEC_CODE)
  #undef CPL_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined CPL_STOP_SEC_CODE)
  #undef CPL_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined CPL_START_SEC_APPL_CODE)
  #undef CPL_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined CPL_STOP_SEC_APPL_CODE)
  #undef CPL_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined CPL_START_SEC_VAR_NOINIT_8BIT)
  #undef CPL_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined CPL_STOP_SEC_VAR_NOINIT_8BIT)
  #undef CPL_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined CPL_START_SEC_VAR_NOINIT_16BIT)
  #undef CPL_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined CPL_STOP_SEC_VAR_NOINIT_16BIT)
  #undef CPL_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined CPL_START_SEC_VAR_NOINIT_32BIT)
  #undef CPL_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined CPL_STOP_SEC_VAR_NOINIT_32BIT)
  #undef CPL_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined CPL_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef CPL_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CPL_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef CPL_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CPL_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef CPL_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined CPL_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef CPL_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined CPL_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef CPL_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined CPL_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef CPL_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined CPL_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef CPL_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined CPL_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef CPL_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined CPL_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef CPL_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CPL_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef CPL_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CPL_START_SEC_VAR_FAST_8BIT)
  #undef CPL_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined CPL_STOP_SEC_VAR_FAST_8BIT)
  #undef CPL_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined CPL_START_SEC_VAR_FAST_16BIT)
  #undef CPL_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined CPL_STOP_SEC_VAR_FAST_16BIT)
  #undef CPL_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined CPL_START_SEC_VAR_FAST_32BIT)
  #undef CPL_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined CPL_STOP_SEC_VAR_FAST_32BIT)
  #undef CPL_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined CPL_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef CPL_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CPL_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef CPL_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CPL_START_SEC_VAR_8BIT)
  #undef CPL_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined CPL_STOP_SEC_VAR_8BIT)
  #undef CPL_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined CPL_START_SEC_VAR_16BIT)
  #undef CPL_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined CPL_STOP_SEC_VAR_16BIT)
  #undef CPL_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined CPL_START_SEC_VAR_32BIT)
  #undef CPL_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined CPL_STOP_SEC_VAR_32BIT)
  #undef CPL_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined CPL_START_SEC_VAR_UNSPECIFIED)
  #undef CPL_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CPL_STOP_SEC_VAR_UNSPECIFIED)
  #undef CPL_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CPL_START_SEC_CONST_8BIT)
  #undef CPL_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined CPL_STOP_SEC_CONST_8BIT)
  #undef CPL_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined CPL_START_SEC_CONST_16BIT)
  #undef CPL_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined CPL_STOP_SEC_CONST_16BIT)
  #undef CPL_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined CPL_START_SEC_CONST_32BIT)
  #undef CPL_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined CPL_STOP_SEC_CONST_32BIT)
  #undef CPL_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined CPL_START_SEC_CONST_UNSPECIFIED)
  #undef CPL_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CPL_STOP_SEC_CONST_UNSPECIFIED)
  #undef CPL_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CPL_START_CONFIG_DATA_8BIT)
  #undef CPL_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined CPL_STOP_CONFIG_DATA_8BIT)
  #undef CPL_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined CPL_START_CONFIG_DATA_16BIT)
  #undef CPL_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined CPL_STOP_CONFIG_DATA_16BIT)
  #undef CPL_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined CPL_START_CONFIG_DATA_32BIT)
  #undef CPL_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined CPL_STOP_CONFIG_DATA_32BIT)
  #undef CPL_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined CPL_START_CONFIG_DATA_UNSPECIFIED)
  #undef CPL_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CPL_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef CPL_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined CRC_START_SEC_CODE)
  #undef CRC_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined CRC_STOP_SEC_CODE)
  #undef CRC_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined CRC_START_SEC_APPL_CODE)
  #undef CRC_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined CRC_STOP_SEC_APPL_CODE)
  #undef CRC_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined CRC_START_SEC_VAR_NOINIT_8BIT)
  #undef CRC_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined CRC_STOP_SEC_VAR_NOINIT_8BIT)
  #undef CRC_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined CRC_START_SEC_VAR_NOINIT_16BIT)
  #undef CRC_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined CRC_STOP_SEC_VAR_NOINIT_16BIT)
  #undef CRC_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined CRC_START_SEC_VAR_NOINIT_32BIT)
  #undef CRC_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined CRC_STOP_SEC_VAR_NOINIT_32BIT)
  #undef CRC_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined CRC_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef CRC_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CRC_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef CRC_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CRC_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef CRC_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined CRC_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef CRC_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined CRC_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef CRC_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined CRC_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef CRC_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined CRC_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef CRC_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined CRC_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef CRC_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined CRC_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef CRC_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CRC_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef CRC_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CRC_START_SEC_VAR_FAST_8BIT)
  #undef CRC_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined CRC_STOP_SEC_VAR_FAST_8BIT)
  #undef CRC_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined CRC_START_SEC_VAR_FAST_16BIT)
  #undef CRC_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined CRC_STOP_SEC_VAR_FAST_16BIT)
  #undef CRC_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined CRC_START_SEC_VAR_FAST_32BIT)
  #undef CRC_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined CRC_STOP_SEC_VAR_FAST_32BIT)
  #undef CRC_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined CRC_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef CRC_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CRC_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef CRC_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CRC_START_SEC_VAR_8BIT)
  #undef CRC_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined CRC_STOP_SEC_VAR_8BIT)
  #undef CRC_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined CRC_START_SEC_VAR_16BIT)
  #undef CRC_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined CRC_STOP_SEC_VAR_16BIT)
  #undef CRC_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined CRC_START_SEC_VAR_32BIT)
  #undef CRC_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined CRC_STOP_SEC_VAR_32BIT)
  #undef CRC_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined CRC_START_SEC_VAR_UNSPECIFIED)
  #undef CRC_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CRC_STOP_SEC_VAR_UNSPECIFIED)
  #undef CRC_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CRC_START_SEC_CONST_8BIT)
  #undef CRC_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined CRC_STOP_SEC_CONST_8BIT)
  #undef CRC_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined CRC_START_SEC_CONST_16BIT)
  #undef CRC_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined CRC_STOP_SEC_CONST_16BIT)
  #undef CRC_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined CRC_START_SEC_CONST_32BIT)
  #undef CRC_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined CRC_STOP_SEC_CONST_32BIT)
  #undef CRC_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined CRC_START_SEC_CONST_UNSPECIFIED)
  #undef CRC_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CRC_STOP_SEC_CONST_UNSPECIFIED)
  #undef CRC_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CRC_START_CONFIG_DATA_8BIT)
  #undef CRC_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined CRC_STOP_CONFIG_DATA_8BIT)
  #undef CRC_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined CRC_START_CONFIG_DATA_16BIT)
  #undef CRC_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined CRC_STOP_CONFIG_DATA_16BIT)
  #undef CRC_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined CRC_START_CONFIG_DATA_32BIT)
  #undef CRC_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined CRC_STOP_CONFIG_DATA_32BIT)
  #undef CRC_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined CRC_START_CONFIG_DATA_UNSPECIFIED)
  #undef CRC_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CRC_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef CRC_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined CRY_START_SEC_CODE)
  #undef CRY_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_CODE)
  #undef CRY_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif ((defined CRY_START_SEC_APPL_CODE) || (defined CRY_START_SEC_CALLOUT_CODE))
  #undef CRY_START_SEC_APPL_CODE
  #undef CRY_START_SEC_CALLOUT_CODE
  #undef MEMMAP_ERROR

#elif ((defined CRY_STOP_SEC_APPL_CODE) || (defined CRY_STOP_SEC_CALLOUT_CODE))
  #undef CRY_STOP_SEC_APPL_CODE
  #undef CRY_STOP_SEC_CALLOUT_CODE
  #undef MEMMAP_ERROR

#elif ((defined CRY_START_SEC_VAR_NOINIT_BOOLEAN) || (defined CRY_START_SEC_VAR_NO_INIT_BOOLEAN))
  #undef CRY_START_SEC_VAR_NOINIT_BOOLEAN
  #undef CRY_START_SEC_VAR_NO_INIT_BOOLEAN
  #undef MEMMAP_ERROR

#elif ((defined CRY_STOP_SEC_VAR_NOINIT_BOOLEAN) || (defined CRY_STOP_SEC_VAR_NO_INIT_BOOLEAN))
  #undef CRY_STOP_SEC_VAR_NOINIT_BOOLEAN
  #undef CRY_STOP_SEC_VAR_NO_INIT_BOOLEAN
  #undef MEMMAP_ERROR

#elif ((defined CRY_START_SEC_VAR_NOINIT_8BIT) || (defined CRY_START_SEC_VAR_NO_INIT_8))
  #undef CRY_START_SEC_VAR_NOINIT_8BIT
  #undef CRY_START_SEC_VAR_NO_INIT_8
  #undef MEMMAP_ERROR

#elif ((defined CRY_STOP_SEC_VAR_NOINIT_8BIT) || (defined CRY_STOP_SEC_VAR_NO_INIT_8))
  #undef CRY_STOP_SEC_VAR_NOINIT_8BIT
  #undef CRY_STOP_SEC_VAR_NO_INIT_8
  #undef MEMMAP_ERROR

#elif ((defined CRY_START_SEC_VAR_NOINIT_16BIT) || (defined CRY_START_SEC_VAR_NO_INIT_16))
  #undef CRY_START_SEC_VAR_NOINIT_16BIT
  #undef CRY_START_SEC_VAR_NO_INIT_16
  #undef MEMMAP_ERROR

#elif ((defined CRY_STOP_SEC_VAR_NOINIT_16BIT) || (defined CRY_STOP_SEC_VAR_NO_INIT_16))
  #undef CRY_STOP_SEC_VAR_NOINIT_16BIT
  #undef CRY_STOP_SEC_VAR_NO_INIT_16
  #undef MEMMAP_ERROR

#elif ((defined CRY_START_SEC_VAR_NOINIT_32BIT) || (defined CRY_START_SEC_VAR_NO_INIT_32))
  #undef CRY_START_SEC_VAR_NOINIT_32BIT
  #undef CRY_START_SEC_VAR_NO_INIT_32
  #undef MEMMAP_ERROR

#elif ((defined CRY_STOP_SEC_VAR_NOINIT_32BIT) || (defined CRY_STOP_SEC_VAR_NO_INIT_32))
  #undef CRY_STOP_SEC_VAR_NOINIT_32BIT
  #undef CRY_STOP_SEC_VAR_NO_INIT_32
  #undef MEMMAP_ERROR

#elif ((defined CRY_START_SEC_VAR_NOINIT_UNSPECIFIED) || (defined CRY_START_SEC_VAR_NO_INIT_UNSPECIFIED))
  #undef CRY_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef CRY_START_SEC_VAR_NO_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif ((defined CRY_STOP_SEC_VAR_NOINIT_UNSPECIFIED) || (defined CRY_STOP_SEC_VAR_NO_INIT_UNSPECIFIED))
  #undef CRY_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef CRY_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif ((defined CRY_START_SEC_VAR_POWER_ON_INIT_BOOLEAN) || (defined CRY_START_SEC_INTERNAL_VAR_POWER_ON_INIT_BOOLEAN))
  #undef CRY_START_SEC_VAR_POWER_ON_INIT_BOOLEAN
  #undef CRY_START_SEC_INTERNAL_VAR_POWER_ON_INIT_BOOLEAN
  #undef MEMMAP_ERROR

#elif ((defined CRY_STOP_SEC_VAR_POWER_ON_INIT_BOOLEAN) || (defined CRY_STOP_SEC_INTERNAL_VAR_POWER_ON_INIT_BOOLEAN))
  #undef CRY_STOP_SEC_VAR_POWER_ON_INIT_BOOLEAN
  #undef CRY_STOP_SEC_INTERNAL_VAR_POWER_ON_INIT_BOOLEAN
  #undef MEMMAP_ERROR

#elif ((defined CRY_START_SEC_VAR_POWER_ON_INIT_8BIT) || (defined CRY_START_SEC_INTERNAL_VAR_POWER_ON_INIT_8) || (defined CRY_START_SEC_VAR_POWER_ON_INIT_8))
  #undef CRY_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef CRY_START_SEC_INTERNAL_VAR_POWER_ON_INIT_8
  #undef CRY_START_SEC_VAR_POWER_ON_INIT_8
  #undef MEMMAP_ERROR

#elif ((defined CRY_STOP_SEC_VAR_POWER_ON_INIT_8BIT) || (defined CRY_STOP_SEC_INTERNAL_VAR_POWER_ON_INIT_8) || (defined CRY_STOP_SEC_VAR_POWER_ON_INIT_8))
  #undef CRY_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef CRY_STOP_SEC_INTERNAL_VAR_POWER_ON_INIT_8
  #undef CRY_STOP_SEC_VAR_POWER_ON_INIT_8
  #undef MEMMAP_ERROR

#elif ((defined CRY_START_SEC_VAR_POWER_ON_INIT_16BIT) || (defined CRY_START_SEC_INTERNAL_VAR_POWER_ON_INIT_16) || (defined CRY_START_SEC_VAR_POWER_ON_INIT_16))
  #undef CRY_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef CRY_START_SEC_INTERNAL_VAR_POWER_ON_INIT_16
  #undef CRY_START_SEC_VAR_POWER_ON_INIT_16
  #undef MEMMAP_ERROR

#elif ((defined CRY_STOP_SEC_VAR_POWER_ON_INIT_16BIT) || (defined CRY_STOP_SEC_INTERNAL_VAR_POWER_ON_INIT_16) || (defined CRY_STOP_SEC_VAR_POWER_ON_INIT_16))
  #undef CRY_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef CRY_STOP_SEC_INTERNAL_VAR_POWER_ON_INIT_16
  #undef CRY_STOP_SEC_VAR_POWER_ON_INIT_16
  #undef MEMMAP_ERROR

#elif ((defined CRY_START_SEC_VAR_POWER_ON_INIT_32BIT) || (defined CRY_START_SEC_INTERNAL_VAR_POWER_ON_INIT_32) || (defined CRY_START_SEC_VAR_POWER_ON_INIT_32))
  #undef CRY_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef CRY_START_SEC_INTERNAL_VAR_POWER_ON_INIT_32
  #undef CRY_START_SEC_VAR_POWER_ON_INIT_32
  #undef MEMMAP_ERROR

#elif ((defined CRY_STOP_SEC_VAR_POWER_ON_INIT_32BIT) || (defined CRY_STOP_SEC_INTERNAL_VAR_POWER_ON_INIT_32) || (defined CRY_STOP_SEC_VAR_POWER_ON_INIT_32))
  #undef CRY_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef CRY_STOP_SEC_INTERNAL_VAR_POWER_ON_INIT_32
  #undef CRY_STOP_SEC_VAR_POWER_ON_INIT_32
  #undef MEMMAP_ERROR

#elif ((defined CRY_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED) || (defined CRY_START_SEC_INTERNAL_VAR_POWER_ON_INIT_UNSPECIFIED))
  #undef CRY_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef CRY_START_SEC_INTERNAL_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif ((defined CRY_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED) || (defined CRY_STOP_SEC_INTERNAL_VAR_POWER_ON_INIT_UNSPECIFIED))
  #undef CRY_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef CRY_STOP_SEC_INTERNAL_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif ((defined CRY_START_SEC_VAR_FAST_BOOLEAN) || (defined CRY_START_SEC_VAR_FAST_INIT_BOOLEAN))
  #undef CRY_START_SEC_VAR_FAST_BOOLEAN
  #undef CRY_START_SEC_VAR_FAST_INIT_BOOLEAN
  #undef MEMMAP_ERROR

#elif ((defined CRY_STOP_SEC_VAR_FAST_BOOLEAN) || (defined CRY_STOP_SEC_VAR_FAST_INIT_BOOLEAN))
  #undef CRY_STOP_SEC_VAR_FAST_BOOLEAN
  #undef CRY_STOP_SEC_VAR_FAST_INIT_BOOLEAN
  #undef MEMMAP_ERROR

#elif ((defined CRY_START_SEC_VAR_FAST_8BIT) || (defined CRY_START_SEC_VAR_FAST_INIT_8))
  #undef CRY_START_SEC_VAR_FAST_8BIT
  #undef CRY_START_SEC_VAR_FAST_INIT_8
  #undef MEMMAP_ERROR

#elif ((defined CRY_STOP_SEC_VAR_FAST_8BIT) || (defined CRY_STOP_SEC_VAR_FAST_INIT_8))
  #undef CRY_STOP_SEC_VAR_FAST_8BIT
  #undef CRY_STOP_SEC_VAR_FAST_INIT_8
  #undef MEMMAP_ERROR

#elif ((defined CRY_START_SEC_VAR_FAST_16BIT) || (defined CRY_START_SEC_VAR_FAST_INIT_16))
  #undef CRY_START_SEC_VAR_FAST_16BIT
  #undef CRY_START_SEC_VAR_FAST_INIT_16
  #undef MEMMAP_ERROR

#elif ((defined CRY_STOP_SEC_VAR_FAST_16BIT) || (defined CRY_STOP_SEC_VAR_FAST_INIT_16))
  #undef CRY_STOP_SEC_VAR_FAST_16BIT
  #undef CRY_STOP_SEC_VAR_FAST_INIT_16
  #undef MEMMAP_ERROR

#elif ((defined CRY_START_SEC_VAR_FAST_32BIT) || (defined CRY_START_SEC_VAR_FAST_INIT_32))
  #undef CRY_START_SEC_VAR_FAST_32BIT
  #undef CRY_START_SEC_VAR_FAST_INIT_32
  #undef MEMMAP_ERROR

#elif ((defined CRY_STOP_SEC_VAR_FAST_32BIT) || (defined CRY_STOP_SEC_VAR_FAST_INIT_32))
  #undef CRY_STOP_SEC_VAR_FAST_32BIT
  #undef CRY_STOP_SEC_VAR_FAST_INIT_32
  #undef MEMMAP_ERROR

#elif ((defined CRY_START_SEC_VAR_FAST_UNSPECIFIED) || (defined CRY_START_SEC_VAR_FAST_INIT_UNSPECIFIED))
  #undef CRY_START_SEC_VAR_FAST_UNSPECIFIED
  #undef CRY_START_SEC_VAR_FAST_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif ((defined CRY_STOP_SEC_VAR_FAST_UNSPECIFIED) || (defined CRY_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED))
  #undef CRY_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef CRY_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif ((defined CRY_START_SEC_INTERNAL_VAR_BOOLEAN) || (defined CRY_START_SEC_INTERNAL_VAR_INIT_BOOLEAN))
  #undef CRY_START_SEC_INTERNAL_VAR_BOOLEAN
  #undef CRY_START_SEC_INTERNAL_VAR_INIT_BOOLEAN
  #undef MEMMAP_ERROR

#elif ((defined CRY_STOP_SEC_INTERNAL_VAR_BOOLEAN) || (defined CRY_STOP_SEC_INTERNAL_VAR_INIT_BOOLEAN))
  #undef CRY_STOP_SEC_INTERNAL_VAR_BOOLEAN
  #undef CRY_STOP_SEC_INTERNAL_VAR_INIT_BOOLEAN
  #undef MEMMAP_ERROR

#elif ((defined CRY_START_SEC_INTERNAL_VAR_8BIT) || (defined CRY_START_SEC_INTERNAL_VAR_INIT_8))
  #undef CRY_START_SEC_INTERNAL_VAR_8BIT
  #undef CRY_START_SEC_INTERNAL_VAR_INIT_8
  #undef MEMMAP_ERROR

#elif ((defined CRY_STOP_SEC_INTERNAL_VAR_8BIT) || (defined CRY_STOP_SEC_INTERNAL_VAR_INIT_8))
  #undef CRY_STOP_SEC_INTERNAL_VAR_8BIT
  #undef CRY_STOP_SEC_INTERNAL_VAR_INIT_8
  #undef MEMMAP_ERROR

#elif ((defined CRY_START_SEC_INTERNAL_VAR_16BIT) || (defined CRY_START_SEC_INTERNAL_VAR_INIT_16))
  #undef CRY_START_SEC_INTERNAL_VAR_16BIT
  #undef CRY_START_SEC_INTERNAL_VAR_INIT_16
  #undef MEMMAP_ERROR

#elif ((defined CRY_STOP_SEC_INTERNAL_VAR_16BIT) || (defined CRY_STOP_SEC_INTERNAL_VAR_INIT_16))
  #undef CRY_STOP_SEC_INTERNAL_VAR_16BIT
  #undef CRY_STOP_SEC_INTERNAL_VAR_INIT_16
  #undef MEMMAP_ERROR

#elif ((defined CRY_START_SEC_INTERNAL_VAR_32BIT) || (defined CRY_START_SEC_INTERNAL_VAR_INIT_32))
  #undef CRY_START_SEC_INTERNAL_VAR_32BIT
  #undef CRY_START_SEC_INTERNAL_VAR_INIT_32
  #undef MEMMAP_ERROR

#elif ((defined CRY_STOP_SEC_INTERNAL_VAR_32BIT) || (defined CRY_STOP_SEC_INTERNAL_VAR_INIT_32))
  #undef CRY_STOP_SEC_INTERNAL_VAR_32BIT
  #undef CRY_STOP_SEC_INTERNAL_VAR_INIT_32
  #undef MEMMAP_ERROR

#elif ((defined CRY_START_SEC_INTERNAL_VAR_UNSPECIFIED) || (defined CRY_START_SEC_INTERNAL_VAR_INIT_UNSPECIFIED))
  #undef CRY_START_SEC_INTERNAL_VAR_UNSPECIFIED
  #undef CRY_START_SEC_INTERNAL_VAR_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif ((defined CRY_STOP_SEC_INTERNAL_VAR_UNSPECIFIED) || (defined CRY_STOP_SEC_INTERNAL_VAR_INIT_UNSPECIFIED))
  #undef CRY_STOP_SEC_INTERNAL_VAR_UNSPECIFIED
  #undef CRY_STOP_SEC_INTERNAL_VAR_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif ((defined CRY_START_SEC_VAR_BOOLEAN) || (defined CRY_START_SEC_VAR_INIT_BOOLEAN))
  #undef CRY_START_SEC_VAR_BOOLEAN
  #undef CRY_START_SEC_VAR_INIT_BOOLEAN
  #undef MEMMAP_ERROR

#elif ((defined CRY_STOP_SEC_VAR_BOOLEAN) || (defined CRY_STOP_SEC_VAR_INIT_BOOLEAN))
  #undef CRY_STOP_SEC_VAR_BOOLEAN
  #undef CRY_STOP_SEC_VAR_INIT_BOOLEAN
  #undef MEMMAP_ERROR

#elif ((defined CRY_START_SEC_VAR_8BIT) || (defined CRY_START_SEC_VAR_INIT_8))
  #undef CRY_START_SEC_VAR_8BIT
  #undef CRY_START_SEC_VAR_INIT_8
  #undef MEMMAP_ERROR

#elif ((defined CRY_STOP_SEC_VAR_8BIT) || (defined CRY_STOP_SEC_VAR_INIT_8))
  #undef CRY_STOP_SEC_VAR_8BIT
  #undef CRY_STOP_SEC_VAR_INIT_8
  #undef MEMMAP_ERROR

#elif ((defined CRY_START_SEC_VAR_16BIT) || (defined CRY_START_SEC_VAR_INIT_16))
  #undef CRY_START_SEC_VAR_16BIT
  #undef CRY_START_SEC_VAR_INIT_16
  #undef MEMMAP_ERROR

#elif ((defined CRY_STOP_SEC_VAR_16BIT) || (defined CRY_STOP_SEC_VAR_INIT_16))
  #undef CRY_STOP_SEC_VAR_16BIT
  #undef CRY_STOP_SEC_VAR_INIT_16
  #undef MEMMAP_ERROR

#elif ((defined CRY_START_SEC_VAR_32BIT) || (defined CRY_START_SEC_VAR_INIT_32))
  #undef CRY_START_SEC_VAR_32BIT
  #undef CRY_START_SEC_VAR_INIT_32
  #undef MEMMAP_ERROR

#elif ((defined CRY_STOP_SEC_VAR_32BIT) || (defined CRY_STOP_SEC_VAR_INIT_32))
  #undef CRY_STOP_SEC_VAR_32BIT
  #undef CRY_STOP_SEC_VAR_INIT_32
  #undef MEMMAP_ERROR

#elif ((defined CRY_START_SEC_VAR_UNSPECIFIED) || (defined CRY_START_SEC_VAR_INIT_UNSPECIFIED))
  #undef CRY_START_SEC_VAR_UNSPECIFIED
  #undef CRY_START_SEC_VAR_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif ((defined CRY_STOP_SEC_VAR_UNSPECIFIED) || (defined CRY_STOP_SEC_VAR_INIT_UNSPECIFIED))
  #undef CRY_STOP_SEC_VAR_UNSPECIFIED
  #undef CRY_STOP_SEC_VAR_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_CONST_BOOLEAN)
  #undef CRY_START_SEC_CONST_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_CONST_BOOLEAN)
  #undef CRY_STOP_SEC_CONST_BOOLEAN
  #undef MEMMAP_ERROR

#elif ((defined CRY_START_SEC_CONST_8BIT) || (defined CRY_START_SEC_CONST_8))
  #undef CRY_START_SEC_CONST_8BIT
  #undef CRY_START_SEC_CONST_8
  #undef MEMMAP_ERROR

#elif ((defined CRY_STOP_SEC_CONST_8BIT) || (defined CRY_STOP_SEC_CONST_8))
  #undef CRY_STOP_SEC_CONST_8BIT
  #undef CRY_STOP_SEC_CONST_8
  #undef MEMMAP_ERROR

#elif ((defined CRY_START_SEC_CONST_16BIT) || (defined CRY_START_SEC_CONST_16))
  #undef CRY_START_SEC_CONST_16BIT
  #undef CRY_START_SEC_CONST_16
  #undef MEMMAP_ERROR

#elif ((defined CRY_STOP_SEC_CONST_16BIT) || (defined CRY_STOP_SEC_CONST_16))
  #undef CRY_STOP_SEC_CONST_16BIT
  #undef CRY_STOP_SEC_CONST_16
  #undef MEMMAP_ERROR

#elif ((defined CRY_START_SEC_CONST_32BIT) || (defined CRY_START_SEC_CONST_32))
  #undef CRY_START_SEC_CONST_32BIT
  #undef CRY_START_SEC_CONST_32
  #undef MEMMAP_ERROR

#elif ((defined CRY_STOP_SEC_CONST_32BIT) || (defined CRY_STOP_SEC_CONST_32))
  #undef CRY_STOP_SEC_CONST_32BIT
  #undef CRY_STOP_SEC_CONST_32
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_CONST_UNSPECIFIED)
  #undef CRY_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_CONST_UNSPECIFIED)
  #undef CRY_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_CALIB_BOOLEAN)
  #undef CRY_START_SEC_CALIB_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_CALIB_BOOLEAN)
  #undef CRY_STOP_SEC_CALIB_BOOLEAN
  #undef MEMMAP_ERROR

#elif ((defined CRY_START_SEC_CALIB_8BIT) || (defined CRY_START_SEC_CALIB_8))
  #undef CRY_START_SEC_CALIB_8BIT
  #undef CRY_START_SEC_CALIB_8
  #undef MEMMAP_ERROR

#elif ((defined CRY_STOP_SEC_CALIB_8BIT) || (defined CRY_STOP_SEC_CALIB_8))
  #undef CRY_STOP_SEC_CALIB_8BIT
  #undef CRY_STOP_SEC_CALIB_8
  #undef MEMMAP_ERROR

#elif ((defined CRY_START_SEC_CALIB_16BIT) || (defined CRY_START_SEC_CALIB_16))
  #undef CRY_START_SEC_CALIB_16BIT
  #undef CRY_START_SEC_CALIB_16
  #undef MEMMAP_ERROR

#elif ((defined CRY_STOP_SEC_CALIB_16BIT) || (defined CRY_STOP_SEC_CALIB_16))
  #undef CRY_STOP_SEC_CALIB_16BIT
  #undef CRY_STOP_SEC_CALIB_16
  #undef MEMMAP_ERROR

#elif ((defined CRY_START_SEC_CALIB_32BIT) || (defined CRY_START_SEC_CALIB_32))
  #undef CRY_START_SEC_CALIB_32BIT
  #undef CRY_START_SEC_CALIB_32
  #undef MEMMAP_ERROR

#elif ((defined CRY_STOP_SEC_CALIB_32BIT) || (defined CRY_STOP_SEC_CALIB_32))
  #undef CRY_STOP_SEC_CALIB_32BIT
  #undef CRY_STOP_SEC_CALIB_32
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_CALIB_UNSPECIFIED)
  #undef CRY_START_SEC_CALIB_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_CALIB_UNSPECIFIED)
  #undef CRY_STOP_SEC_CALIB_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_CARTO_BOOLEAN)
  #undef CRY_START_SEC_CARTO_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_CARTO_BOOLEAN)
  #undef CRY_STOP_SEC_CARTO_BOOLEAN
  #undef MEMMAP_ERROR

#elif ((defined CRY_START_SEC_CARTO_8BIT) || (defined CRY_START_SEC_CARTO_8))
  #undef CRY_START_SEC_CARTO_8BIT
  #undef CRY_START_SEC_CARTO_8
  #undef MEMMAP_ERROR

#elif ((defined CRY_STOP_SEC_CARTO_8BIT) || (defined CRY_STOP_SEC_CARTO_8))
  #undef CRY_STOP_SEC_CARTO_8BIT
  #undef CRY_STOP_SEC_CARTO_8
  #undef MEMMAP_ERROR

#elif ((defined CRY_START_SEC_CARTO_16BIT) || (defined CRY_START_SEC_CARTO_16))
  #undef CRY_START_SEC_CARTO_16BIT
  #undef CRY_START_SEC_CARTO_16
  #undef MEMMAP_ERROR

#elif ((defined CRY_STOP_SEC_CARTO_16BIT) || (defined CRY_STOP_SEC_CARTO_16))
  #undef CRY_STOP_SEC_CARTO_16BIT
  #undef CRY_STOP_SEC_CARTO_16
  #undef MEMMAP_ERROR

#elif ((defined CRY_START_SEC_CARTO_32BIT) || (defined CRY_START_SEC_CARTO_32))
  #undef CRY_START_SEC_CARTO_32BIT
  #undef CRY_START_SEC_CARTO_32
  #undef MEMMAP_ERROR

#elif ((defined CRY_STOP_SEC_CARTO_32BIT) || (defined CRY_STOP_SEC_CARTO_32))
  #undef CRY_STOP_SEC_CARTO_32BIT
  #undef CRY_STOP_SEC_CARTO_32
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_CARTO_UNSPECIFIED)
  #undef CRY_START_SEC_CARTO_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_CARTO_UNSPECIFIED)
  #undef CRY_STOP_SEC_CARTO_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif ((defined CRY_START_CONFIG_DATA_BOOLEAN) || (defined CRY_START_SEC_CONFIG_DATA_BOOLEAN))
  #undef CRY_START_CONFIG_DATA_BOOLEAN
  #undef CRY_START_SEC_CONFIG_DATA_BOOLEAN
  #undef MEMMAP_ERROR

#elif ((defined CRY_STOP_CONFIG_DATA_BOOLEAN) || (defined CRY_STOP_SEC_CONFIG_DATA_BOOLEAN))
  #undef CRY_STOP_CONFIG_DATA_BOOLEAN
  #undef CRY_STOP_SEC_CONFIG_DATA_BOOLEAN
  #undef MEMMAP_ERROR

#elif ((defined CRY_START_CONFIG_DATA_8BIT) || (defined CRY_START_SEC_CONFIG_DATA_8))
  #undef CRY_START_CONFIG_DATA_8BIT
  #undef CRY_START_SEC_CONFIG_DATA_8
  #undef MEMMAP_ERROR

#elif ((defined CRY_STOP_CONFIG_DATA_8BIT) || (defined CRY_STOP_SEC_CONFIG_DATA_8))
  #undef CRY_STOP_CONFIG_DATA_8BIT
  #undef CRY_STOP_SEC_CONFIG_DATA_8
  #undef MEMMAP_ERROR

#elif ((defined CRY_START_CONFIG_DATA_16BIT) || (defined CRY_START_SEC_CONFIG_DATA_16))
  #undef CRY_START_CONFIG_DATA_16BIT
  #undef CRY_START_SEC_CONFIG_DATA_16
  #undef MEMMAP_ERROR

#elif ((defined CRY_STOP_CONFIG_DATA_16BIT) || (defined CRY_STOP_SEC_CONFIG_DATA_16))
  #undef CRY_STOP_CONFIG_DATA_16BIT
  #undef CRY_STOP_SEC_CONFIG_DATA_16
  #undef MEMMAP_ERROR

#elif ((defined CRY_START_CONFIG_DATA_32BIT) || (defined CRY_START_SEC_CONFIG_DATA_32))
  #undef CRY_START_CONFIG_DATA_32BIT
  #undef CRY_START_SEC_CONFIG_DATA_32
  #undef MEMMAP_ERROR

#elif ((defined CRY_STOP_CONFIG_DATA_32BIT) || (defined CRY_STOP_SEC_CONFIG_DATA_32))
  #undef CRY_STOP_CONFIG_DATA_32BIT
  #undef CRY_STOP_SEC_CONFIG_DATA_32
  #undef MEMMAP_ERROR

#elif ((defined CRY_START_CONFIG_DATA_UNSPECIFIED) || (defined CRY_START_SEC_CONFIG_DATA_UNSPECIFIED))
  #undef CRY_START_CONFIG_DATA_UNSPECIFIED
  #undef CRY_START_SEC_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif ((defined CRY_STOP_CONFIG_DATA_UNSPECIFIED) || (defined CRY_STOP_SEC_CONFIG_DATA_UNSPECIFIED))
  #undef CRY_STOP_CONFIG_DATA_UNSPECIFIED
  #undef CRY_STOP_SEC_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_CLEARED_BOOLEAN)
  #undef CRY_START_SEC_VAR_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_CLEARED_BOOLEAN)
  #undef CRY_STOP_SEC_VAR_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_CLEARED_8)
  #undef CRY_START_SEC_VAR_CLEARED_8
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_CLEARED_8)
  #undef CRY_STOP_SEC_VAR_CLEARED_8
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_CLEARED_16)
  #undef CRY_START_SEC_VAR_CLEARED_16
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_CLEARED_16)
  #undef CRY_STOP_SEC_VAR_CLEARED_16
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_CLEARED_32)
  #undef CRY_START_SEC_VAR_CLEARED_32
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_CLEARED_32)
  #undef CRY_STOP_SEC_VAR_CLEARED_32
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_CLEARED_UNSPECIFIED)
  #undef CRY_START_SEC_VAR_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_CLEARED_UNSPECIFIED)
  #undef CRY_STOP_SEC_VAR_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_POWER_ON_CLEARED_BOOLEAN)
  #undef CRY_START_SEC_VAR_POWER_ON_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_POWER_ON_CLEARED_BOOLEAN)
  #undef CRY_STOP_SEC_VAR_POWER_ON_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_POWER_ON_CLEARED_8)
  #undef CRY_START_SEC_VAR_POWER_ON_CLEARED_8
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_POWER_ON_CLEARED_8)
  #undef CRY_STOP_SEC_VAR_POWER_ON_CLEARED_8
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_POWER_ON_CLEARED_16)
  #undef CRY_START_SEC_VAR_POWER_ON_CLEARED_16
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_POWER_ON_CLEARED_16)
  #undef CRY_STOP_SEC_VAR_POWER_ON_CLEARED_16
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_POWER_ON_CLEARED_32)
  #undef CRY_START_SEC_VAR_POWER_ON_CLEARED_32
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_POWER_ON_CLEARED_32)
  #undef CRY_STOP_SEC_VAR_POWER_ON_CLEARED_32
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_POWER_ON_CLEARED_UNSPECIFIED)
  #undef CRY_START_SEC_VAR_POWER_ON_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_POWER_ON_CLEARED_UNSPECIFIED)
  #undef CRY_STOP_SEC_VAR_POWER_ON_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_FAST_NO_INIT_BOOLEAN)
  #undef CRY_START_SEC_VAR_FAST_NO_INIT_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_FAST_NO_INIT_BOOLEAN)
  #undef CRY_STOP_SEC_VAR_FAST_NO_INIT_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_FAST_NO_INIT_8)
  #undef CRY_START_SEC_VAR_FAST_NO_INIT_8
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_FAST_NO_INIT_8)
  #undef CRY_STOP_SEC_VAR_FAST_NO_INIT_8
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_FAST_NO_INIT_16)
  #undef CRY_START_SEC_VAR_FAST_NO_INIT_16
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_FAST_NO_INIT_16)
  #undef CRY_STOP_SEC_VAR_FAST_NO_INIT_16
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_FAST_NO_INIT_32)
  #undef CRY_START_SEC_VAR_FAST_NO_INIT_32
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_FAST_NO_INIT_32)
  #undef CRY_STOP_SEC_VAR_FAST_NO_INIT_32
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_FAST_NO_INIT_UNSPECIFIED)
  #undef CRY_START_SEC_VAR_FAST_NO_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_FAST_NO_INIT_UNSPECIFIED)
  #undef CRY_STOP_SEC_VAR_FAST_NO_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_FAST_CLEARED_BOOLEAN)
  #undef CRY_START_SEC_VAR_FAST_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_FAST_CLEARED_BOOLEAN)
  #undef CRY_STOP_SEC_VAR_FAST_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_FAST_CLEARED_8)
  #undef CRY_START_SEC_VAR_FAST_CLEARED_8
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_FAST_CLEARED_8)
  #undef CRY_STOP_SEC_VAR_FAST_CLEARED_8
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_FAST_CLEARED_16)
  #undef CRY_START_SEC_VAR_FAST_CLEARED_16
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_FAST_CLEARED_16)
  #undef CRY_STOP_SEC_VAR_FAST_CLEARED_16
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_FAST_CLEARED_32)
  #undef CRY_START_SEC_VAR_FAST_CLEARED_32
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_FAST_CLEARED_32)
  #undef CRY_STOP_SEC_VAR_FAST_CLEARED_32
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_FAST_CLEARED_UNSPECIFIED)
  #undef CRY_START_SEC_VAR_FAST_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_FAST_CLEARED_UNSPECIFIED)
  #undef CRY_STOP_SEC_VAR_FAST_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_FAST_POWER_ON_CLEARED_BOOLEAN)
  #undef CRY_START_SEC_VAR_FAST_POWER_ON_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_FAST_POWER_ON_CLEARED_BOOLEAN)
  #undef CRY_STOP_SEC_VAR_FAST_POWER_ON_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_FAST_POWER_ON_CLEARED_8)
  #undef CRY_START_SEC_VAR_FAST_POWER_ON_CLEARED_8
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_FAST_POWER_ON_CLEARED_8)
  #undef CRY_STOP_SEC_VAR_FAST_POWER_ON_CLEARED_8
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_FAST_POWER_ON_CLEARED_16)
  #undef CRY_START_SEC_VAR_FAST_POWER_ON_CLEARED_16
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_FAST_POWER_ON_CLEARED_16)
  #undef CRY_STOP_SEC_VAR_FAST_POWER_ON_CLEARED_16
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_FAST_POWER_ON_CLEARED_32)
  #undef CRY_START_SEC_VAR_FAST_POWER_ON_CLEARED_32
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_FAST_POWER_ON_CLEARED_32)
  #undef CRY_STOP_SEC_VAR_FAST_POWER_ON_CLEARED_32
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_FAST_POWER_ON_CLEARED_UNSPECIFIED)
  #undef CRY_START_SEC_VAR_FAST_POWER_ON_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_FAST_POWER_ON_CLEARED_UNSPECIFIED)
  #undef CRY_STOP_SEC_VAR_FAST_POWER_ON_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_FAST_POWER_ON_INIT_BOOLEAN)
  #undef CRY_START_SEC_VAR_FAST_POWER_ON_INIT_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_FAST_POWER_ON_INIT_BOOLEAN)
  #undef CRY_STOP_SEC_VAR_FAST_POWER_ON_INIT_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_FAST_POWER_ON_INIT_8)
  #undef CRY_START_SEC_VAR_FAST_POWER_ON_INIT_8
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_FAST_POWER_ON_INIT_8)
  #undef CRY_STOP_SEC_VAR_FAST_POWER_ON_INIT_8
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_FAST_POWER_ON_INIT_16)
  #undef CRY_START_SEC_VAR_FAST_POWER_ON_INIT_16
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_FAST_POWER_ON_INIT_16)
  #undef CRY_STOP_SEC_VAR_FAST_POWER_ON_INIT_16
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_FAST_POWER_ON_INIT_32)
  #undef CRY_START_SEC_VAR_FAST_POWER_ON_INIT_32
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_FAST_POWER_ON_INIT_32)
  #undef CRY_STOP_SEC_VAR_FAST_POWER_ON_INIT_32
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_FAST_POWER_ON_INIT_UNSPECIFIED)
  #undef CRY_START_SEC_VAR_FAST_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_FAST_POWER_ON_INIT_UNSPECIFIED)
  #undef CRY_STOP_SEC_VAR_FAST_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_SLOW_NO_INIT_BOOLEAN)
  #undef CRY_START_SEC_VAR_SLOW_NO_INIT_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_SLOW_NO_INIT_BOOLEAN)
  #undef CRY_STOP_SEC_VAR_SLOW_NO_INIT_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_SLOW_NO_INIT_8)
  #undef CRY_START_SEC_VAR_SLOW_NO_INIT_8
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_SLOW_NO_INIT_8)
  #undef CRY_STOP_SEC_VAR_SLOW_NO_INIT_8
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_SLOW_NO_INIT_16)
  #undef CRY_START_SEC_VAR_SLOW_NO_INIT_16
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_SLOW_NO_INIT_16)
  #undef CRY_STOP_SEC_VAR_SLOW_NO_INIT_16
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_SLOW_NO_INIT_32)
  #undef CRY_START_SEC_VAR_SLOW_NO_INIT_32
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_SLOW_NO_INIT_32)
  #undef CRY_STOP_SEC_VAR_SLOW_NO_INIT_32
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_SLOW_NO_INIT_UNSPECIFIED)
  #undef CRY_START_SEC_VAR_SLOW_NO_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_SLOW_NO_INIT_UNSPECIFIED)
  #undef CRY_STOP_SEC_VAR_SLOW_NO_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_SLOW_CLEARED_BOOLEAN)
  #undef CRY_START_SEC_VAR_SLOW_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_SLOW_CLEARED_BOOLEAN)
  #undef CRY_STOP_SEC_VAR_SLOW_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_SLOW_CLEARED_8)
  #undef CRY_START_SEC_VAR_SLOW_CLEARED_8
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_SLOW_CLEARED_8)
  #undef CRY_STOP_SEC_VAR_SLOW_CLEARED_8
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_SLOW_CLEARED_16)
  #undef CRY_START_SEC_VAR_SLOW_CLEARED_16
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_SLOW_CLEARED_16)
  #undef CRY_STOP_SEC_VAR_SLOW_CLEARED_16
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_SLOW_CLEARED_32)
  #undef CRY_START_SEC_VAR_SLOW_CLEARED_32
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_SLOW_CLEARED_32)
  #undef CRY_STOP_SEC_VAR_SLOW_CLEARED_32
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_SLOW_CLEARED_UNSPECIFIED)
  #undef CRY_START_SEC_VAR_SLOW_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_SLOW_CLEARED_UNSPECIFIED)
  #undef CRY_STOP_SEC_VAR_SLOW_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_SLOW_POWER_ON_CLEARED_BOOLEAN)
  #undef CRY_START_SEC_VAR_SLOW_POWER_ON_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_SLOW_POWER_ON_CLEARED_BOOLEAN)
  #undef CRY_STOP_SEC_VAR_SLOW_POWER_ON_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_SLOW_POWER_ON_CLEARED_8)
  #undef CRY_START_SEC_VAR_SLOW_POWER_ON_CLEARED_8
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_SLOW_POWER_ON_CLEARED_8)
  #undef CRY_STOP_SEC_VAR_SLOW_POWER_ON_CLEARED_8
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_SLOW_POWER_ON_CLEARED_16)
  #undef CRY_START_SEC_VAR_SLOW_POWER_ON_CLEARED_16
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_SLOW_POWER_ON_CLEARED_16)
  #undef CRY_STOP_SEC_VAR_SLOW_POWER_ON_CLEARED_16
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_SLOW_POWER_ON_CLEARED_32)
  #undef CRY_START_SEC_VAR_SLOW_POWER_ON_CLEARED_32
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_SLOW_POWER_ON_CLEARED_32)
  #undef CRY_STOP_SEC_VAR_SLOW_POWER_ON_CLEARED_32
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_SLOW_POWER_ON_CLEARED_UNSPECIFIED)
  #undef CRY_START_SEC_VAR_SLOW_POWER_ON_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_SLOW_POWER_ON_CLEARED_UNSPECIFIED)
  #undef CRY_STOP_SEC_VAR_SLOW_POWER_ON_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_SLOW_INIT_BOOLEAN)
  #undef CRY_START_SEC_VAR_SLOW_INIT_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_SLOW_INIT_BOOLEAN)
  #undef CRY_STOP_SEC_VAR_SLOW_INIT_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_SLOW_INIT_8)
  #undef CRY_START_SEC_VAR_SLOW_INIT_8
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_SLOW_INIT_8)
  #undef CRY_STOP_SEC_VAR_SLOW_INIT_8
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_SLOW_INIT_16)
  #undef CRY_START_SEC_VAR_SLOW_INIT_16
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_SLOW_INIT_16)
  #undef CRY_STOP_SEC_VAR_SLOW_INIT_16
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_SLOW_INIT_32)
  #undef CRY_START_SEC_VAR_SLOW_INIT_32
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_SLOW_INIT_32)
  #undef CRY_STOP_SEC_VAR_SLOW_INIT_32
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_SLOW_INIT_UNSPECIFIED)
  #undef CRY_START_SEC_VAR_SLOW_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_SLOW_INIT_UNSPECIFIED)
  #undef CRY_STOP_SEC_VAR_SLOW_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_SLOW_POWER_ON_INIT_BOOLEAN)
  #undef CRY_START_SEC_VAR_SLOW_POWER_ON_INIT_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_SLOW_POWER_ON_INIT_BOOLEAN)
  #undef CRY_STOP_SEC_VAR_SLOW_POWER_ON_INIT_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_SLOW_POWER_ON_INIT_8)
  #undef CRY_START_SEC_VAR_SLOW_POWER_ON_INIT_8
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_SLOW_POWER_ON_INIT_8)
  #undef CRY_STOP_SEC_VAR_SLOW_POWER_ON_INIT_8
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_SLOW_POWER_ON_INIT_16)
  #undef CRY_START_SEC_VAR_SLOW_POWER_ON_INIT_16
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_SLOW_POWER_ON_INIT_16)
  #undef CRY_STOP_SEC_VAR_SLOW_POWER_ON_INIT_16
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_SLOW_POWER_ON_INIT_32)
  #undef CRY_START_SEC_VAR_SLOW_POWER_ON_INIT_32
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_SLOW_POWER_ON_INIT_32)
  #undef CRY_STOP_SEC_VAR_SLOW_POWER_ON_INIT_32
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_VAR_SLOW_POWER_ON_INIT_UNSPECIFIED)
  #undef CRY_START_SEC_VAR_SLOW_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_VAR_SLOW_POWER_ON_INIT_UNSPECIFIED)
  #undef CRY_STOP_SEC_VAR_SLOW_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_INTERNAL_VAR_NO_INIT_BOOLEAN)
  #undef CRY_START_SEC_INTERNAL_VAR_NO_INIT_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_INTERNAL_VAR_NO_INIT_BOOLEAN)
  #undef CRY_STOP_SEC_INTERNAL_VAR_NO_INIT_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_INTERNAL_VAR_NO_INIT_8)
  #undef CRY_START_SEC_INTERNAL_VAR_NO_INIT_8
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_INTERNAL_VAR_NO_INIT_8)
  #undef CRY_STOP_SEC_INTERNAL_VAR_NO_INIT_8
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_INTERNAL_VAR_NO_INIT_16)
  #undef CRY_START_SEC_INTERNAL_VAR_NO_INIT_16
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_INTERNAL_VAR_NO_INIT_16)
  #undef CRY_STOP_SEC_INTERNAL_VAR_NO_INIT_16
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_INTERNAL_VAR_NO_INIT_32)
  #undef CRY_START_SEC_INTERNAL_VAR_NO_INIT_32
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_INTERNAL_VAR_NO_INIT_32)
  #undef CRY_STOP_SEC_INTERNAL_VAR_NO_INIT_32
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_INTERNAL_VAR_NO_INIT_UNSPECIFIED)
  #undef CRY_START_SEC_INTERNAL_VAR_NO_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_INTERNAL_VAR_NO_INIT_UNSPECIFIED)
  #undef CRY_STOP_SEC_INTERNAL_VAR_NO_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_INTERNAL_VAR_CLEARED_BOOLEAN)
  #undef CRY_START_SEC_INTERNAL_VAR_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_INTERNAL_VAR_CLEARED_BOOLEAN)
  #undef CRY_STOP_SEC_INTERNAL_VAR_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_INTERNAL_VAR_CLEARED_8)
  #undef CRY_START_SEC_INTERNAL_VAR_CLEARED_8
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_INTERNAL_VAR_CLEARED_8)
  #undef CRY_STOP_SEC_INTERNAL_VAR_CLEARED_8
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_INTERNAL_VAR_CLEARED_16)
  #undef CRY_START_SEC_INTERNAL_VAR_CLEARED_16
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_INTERNAL_VAR_CLEARED_16)
  #undef CRY_STOP_SEC_INTERNAL_VAR_CLEARED_16
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_INTERNAL_VAR_CLEARED_32)
  #undef CRY_START_SEC_INTERNAL_VAR_CLEARED_32
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_INTERNAL_VAR_CLEARED_32)
  #undef CRY_STOP_SEC_INTERNAL_VAR_CLEARED_32
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_INTERNAL_VAR_CLEARED_UNSPECIFIED)
  #undef CRY_START_SEC_INTERNAL_VAR_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_INTERNAL_VAR_CLEARED_UNSPECIFIED)
  #undef CRY_STOP_SEC_INTERNAL_VAR_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_INTERNAL_VAR_POWER_ON_CLEARED_BOOLEAN)
  #undef CRY_START_SEC_INTERNAL_VAR_POWER_ON_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_INTERNAL_VAR_POWER_ON_CLEARED_BOOLEAN)
  #undef CRY_STOP_SEC_INTERNAL_VAR_POWER_ON_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_INTERNAL_VAR_POWER_ON_CLEARED_8)
  #undef CRY_START_SEC_INTERNAL_VAR_POWER_ON_CLEARED_8
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_INTERNAL_VAR_POWER_ON_CLEARED_8)
  #undef CRY_STOP_SEC_INTERNAL_VAR_POWER_ON_CLEARED_8
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_INTERNAL_VAR_POWER_ON_CLEARED_16)
  #undef CRY_START_SEC_INTERNAL_VAR_POWER_ON_CLEARED_16
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_INTERNAL_VAR_POWER_ON_CLEARED_16)
  #undef CRY_STOP_SEC_INTERNAL_VAR_POWER_ON_CLEARED_16
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_INTERNAL_VAR_POWER_ON_CLEARED_32)
  #undef CRY_START_SEC_INTERNAL_VAR_POWER_ON_CLEARED_32
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_INTERNAL_VAR_POWER_ON_CLEARED_32)
  #undef CRY_STOP_SEC_INTERNAL_VAR_POWER_ON_CLEARED_32
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_INTERNAL_VAR_POWER_ON_CLEARED_UNSPECIFIED)
  #undef CRY_START_SEC_INTERNAL_VAR_POWER_ON_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_INTERNAL_VAR_POWER_ON_CLEARED_UNSPECIFIED)
  #undef CRY_STOP_SEC_INTERNAL_VAR_POWER_ON_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_CODE_FAST)
  #undef CRY_START_SEC_CODE_FAST
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_CODE_FAST)
  #undef CRY_STOP_SEC_CODE_FAST
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_CODE_SLOW)
  #undef CRY_START_SEC_CODE_SLOW
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_CODE_SLOW)
  #undef CRY_STOP_SEC_CODE_SLOW
  #undef MEMMAP_ERROR

#elif (defined CRY_START_SEC_CODE_LIB)
  #undef CRY_START_SEC_CODE_LIB
  #undef MEMMAP_ERROR

#elif (defined CRY_STOP_SEC_CODE_LIB)
  #undef CRY_STOP_SEC_CODE_LIB
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined CSM_START_SEC_CODE)
  #undef CSM_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_CODE)
  #undef CSM_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif ((defined CSM_START_SEC_APPL_CODE) || (defined CSM_START_SEC_CALLOUT_CODE))
  #undef CSM_START_SEC_APPL_CODE
  #undef CSM_START_SEC_CALLOUT_CODE
  #undef MEMMAP_ERROR

#elif ((defined CSM_STOP_SEC_APPL_CODE) || (defined CSM_STOP_SEC_CALLOUT_CODE))
  #undef CSM_STOP_SEC_APPL_CODE
  #undef CSM_STOP_SEC_CALLOUT_CODE
  #undef MEMMAP_ERROR

#elif ((defined CSM_START_SEC_VAR_NOINIT_BOOLEAN) || (defined CSM_START_SEC_VAR_NO_INIT_BOOLEAN))
  #undef CSM_START_SEC_VAR_NOINIT_BOOLEAN
  #undef CSM_START_SEC_VAR_NO_INIT_BOOLEAN
  #undef MEMMAP_ERROR

#elif ((defined CSM_STOP_SEC_VAR_NOINIT_BOOLEAN) || (defined CSM_STOP_SEC_VAR_NO_INIT_BOOLEAN))
  #undef CSM_STOP_SEC_VAR_NOINIT_BOOLEAN
  #undef CSM_STOP_SEC_VAR_NO_INIT_BOOLEAN
  #undef MEMMAP_ERROR

#elif ((defined CSM_START_SEC_VAR_NOINIT_8BIT) || (defined CSM_START_SEC_VAR_NO_INIT_8))
  #undef CSM_START_SEC_VAR_NOINIT_8BIT
  #undef CSM_START_SEC_VAR_NO_INIT_8
  #undef MEMMAP_ERROR

#elif ((defined CSM_STOP_SEC_VAR_NOINIT_8BIT) || (defined CSM_STOP_SEC_VAR_NO_INIT_8))
  #undef CSM_STOP_SEC_VAR_NOINIT_8BIT
  #undef CSM_STOP_SEC_VAR_NO_INIT_8
  #undef MEMMAP_ERROR

#elif ((defined CSM_START_SEC_VAR_NOINIT_16BIT) || (defined CSM_START_SEC_VAR_NO_INIT_16))
  #undef CSM_START_SEC_VAR_NOINIT_16BIT
  #undef CSM_START_SEC_VAR_NO_INIT_16
  #undef MEMMAP_ERROR

#elif ((defined CSM_STOP_SEC_VAR_NOINIT_16BIT) || (defined CSM_STOP_SEC_VAR_NO_INIT_16))
  #undef CSM_STOP_SEC_VAR_NOINIT_16BIT
  #undef CSM_STOP_SEC_VAR_NO_INIT_16
  #undef MEMMAP_ERROR

#elif ((defined CSM_START_SEC_VAR_NOINIT_32BIT) || (defined CSM_START_SEC_VAR_NO_INIT_32))
  #undef CSM_START_SEC_VAR_NOINIT_32BIT
  #undef CSM_START_SEC_VAR_NO_INIT_32
  #undef MEMMAP_ERROR

#elif ((defined CSM_STOP_SEC_VAR_NOINIT_32BIT) || (defined CSM_STOP_SEC_VAR_NO_INIT_32))
  #undef CSM_STOP_SEC_VAR_NOINIT_32BIT
  #undef CSM_STOP_SEC_VAR_NO_INIT_32
  #undef MEMMAP_ERROR

#elif ((defined CSM_START_SEC_VAR_NOINIT_UNSPECIFIED) || (defined CSM_START_SEC_VAR_NO_INIT_UNSPECIFIED))
  #undef CSM_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef CSM_START_SEC_VAR_NO_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif ((defined CSM_STOP_SEC_VAR_NOINIT_UNSPECIFIED) || (defined CSM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED))
  #undef CSM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef CSM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif ((defined CSM_START_SEC_VAR_POWER_ON_INIT_BOOLEAN) || (defined CSM_START_SEC_INTERNAL_VAR_POWER_ON_INIT_BOOLEAN))
  #undef CSM_START_SEC_VAR_POWER_ON_INIT_BOOLEAN
  #undef CSM_START_SEC_INTERNAL_VAR_POWER_ON_INIT_BOOLEAN
  #undef MEMMAP_ERROR

#elif ((defined CSM_STOP_SEC_VAR_POWER_ON_INIT_BOOLEAN) || (defined CSM_STOP_SEC_INTERNAL_VAR_POWER_ON_INIT_BOOLEAN))
  #undef CSM_STOP_SEC_VAR_POWER_ON_INIT_BOOLEAN
  #undef CSM_STOP_SEC_INTERNAL_VAR_POWER_ON_INIT_BOOLEAN
  #undef MEMMAP_ERROR

#elif ((defined CSM_START_SEC_VAR_POWER_ON_INIT_8BIT) || (defined CSM_START_SEC_INTERNAL_VAR_POWER_ON_INIT_8) || (defined CSM_START_SEC_VAR_POWER_ON_INIT_8))
  #undef CSM_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef CSM_START_SEC_INTERNAL_VAR_POWER_ON_INIT_8
  #undef CSM_START_SEC_VAR_POWER_ON_INIT_8
  #undef MEMMAP_ERROR

#elif ((defined CSM_STOP_SEC_VAR_POWER_ON_INIT_8BIT) || (defined CSM_STOP_SEC_INTERNAL_VAR_POWER_ON_INIT_8) || (defined CSM_STOP_SEC_VAR_POWER_ON_INIT_8))
  #undef CSM_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef CSM_STOP_SEC_INTERNAL_VAR_POWER_ON_INIT_8
  #undef CSM_STOP_SEC_VAR_POWER_ON_INIT_8
  #undef MEMMAP_ERROR

#elif ((defined CSM_START_SEC_VAR_POWER_ON_INIT_16BIT) || (defined CSM_START_SEC_INTERNAL_VAR_POWER_ON_INIT_16) || (defined CSM_START_SEC_VAR_POWER_ON_INIT_16))
  #undef CSM_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef CSM_START_SEC_INTERNAL_VAR_POWER_ON_INIT_16
  #undef CSM_START_SEC_VAR_POWER_ON_INIT_16
  #undef MEMMAP_ERROR

#elif ((defined CSM_STOP_SEC_VAR_POWER_ON_INIT_16BIT) || (defined CSM_STOP_SEC_INTERNAL_VAR_POWER_ON_INIT_16) || (defined CSM_STOP_SEC_VAR_POWER_ON_INIT_16))
  #undef CSM_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef CSM_STOP_SEC_INTERNAL_VAR_POWER_ON_INIT_16
  #undef CSM_STOP_SEC_VAR_POWER_ON_INIT_16
  #undef MEMMAP_ERROR

#elif ((defined CSM_START_SEC_VAR_POWER_ON_INIT_32BIT) || (defined CSM_START_SEC_INTERNAL_VAR_POWER_ON_INIT_32) || (defined CSM_START_SEC_VAR_POWER_ON_INIT_32))
  #undef CSM_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef CSM_START_SEC_INTERNAL_VAR_POWER_ON_INIT_32
  #undef CSM_START_SEC_VAR_POWER_ON_INIT_32
  #undef MEMMAP_ERROR

#elif ((defined CSM_STOP_SEC_VAR_POWER_ON_INIT_32BIT) || (defined CSM_STOP_SEC_INTERNAL_VAR_POWER_ON_INIT_32) || (defined CSM_STOP_SEC_VAR_POWER_ON_INIT_32))
  #undef CSM_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef CSM_STOP_SEC_INTERNAL_VAR_POWER_ON_INIT_32
  #undef CSM_STOP_SEC_VAR_POWER_ON_INIT_32
  #undef MEMMAP_ERROR

#elif ((defined CSM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED) || (defined CSM_START_SEC_INTERNAL_VAR_POWER_ON_INIT_UNSPECIFIED))
  #undef CSM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef CSM_START_SEC_INTERNAL_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif ((defined CSM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED) || (defined CSM_STOP_SEC_INTERNAL_VAR_POWER_ON_INIT_UNSPECIFIED))
  #undef CSM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef CSM_STOP_SEC_INTERNAL_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif ((defined CSM_START_SEC_VAR_FAST_BOOLEAN) || (defined CSM_START_SEC_VAR_FAST_INIT_BOOLEAN))
  #undef CSM_START_SEC_VAR_FAST_BOOLEAN
  #undef CSM_START_SEC_VAR_FAST_INIT_BOOLEAN
  #undef MEMMAP_ERROR

#elif ((defined CSM_STOP_SEC_VAR_FAST_BOOLEAN) || (defined CSM_STOP_SEC_VAR_FAST_INIT_BOOLEAN))
  #undef CSM_STOP_SEC_VAR_FAST_BOOLEAN
  #undef CSM_STOP_SEC_VAR_FAST_INIT_BOOLEAN
  #undef MEMMAP_ERROR

#elif ((defined CSM_START_SEC_VAR_FAST_8BIT) || (defined CSM_START_SEC_VAR_FAST_INIT_8))
  #undef CSM_START_SEC_VAR_FAST_8BIT
  #undef CSM_START_SEC_VAR_FAST_INIT_8
  #undef MEMMAP_ERROR

#elif ((defined CSM_STOP_SEC_VAR_FAST_8BIT) || (defined CSM_STOP_SEC_VAR_FAST_INIT_8))
  #undef CSM_STOP_SEC_VAR_FAST_8BIT
  #undef CSM_STOP_SEC_VAR_FAST_INIT_8
  #undef MEMMAP_ERROR

#elif ((defined CSM_START_SEC_VAR_FAST_16BIT) || (defined CSM_START_SEC_VAR_FAST_INIT_16))
  #undef CSM_START_SEC_VAR_FAST_16BIT
  #undef CSM_START_SEC_VAR_FAST_INIT_16
  #undef MEMMAP_ERROR

#elif ((defined CSM_STOP_SEC_VAR_FAST_16BIT) || (defined CSM_STOP_SEC_VAR_FAST_INIT_16))
  #undef CSM_STOP_SEC_VAR_FAST_16BIT
  #undef CSM_STOP_SEC_VAR_FAST_INIT_16
  #undef MEMMAP_ERROR

#elif ((defined CSM_START_SEC_VAR_FAST_32BIT) || (defined CSM_START_SEC_VAR_FAST_INIT_32))
  #undef CSM_START_SEC_VAR_FAST_32BIT
  #undef CSM_START_SEC_VAR_FAST_INIT_32
  #undef MEMMAP_ERROR

#elif ((defined CSM_STOP_SEC_VAR_FAST_32BIT) || (defined CSM_STOP_SEC_VAR_FAST_INIT_32))
  #undef CSM_STOP_SEC_VAR_FAST_32BIT
  #undef CSM_STOP_SEC_VAR_FAST_INIT_32
  #undef MEMMAP_ERROR

#elif ((defined CSM_START_SEC_VAR_FAST_UNSPECIFIED) || (defined CSM_START_SEC_VAR_FAST_INIT_UNSPECIFIED))
  #undef CSM_START_SEC_VAR_FAST_UNSPECIFIED
  #undef CSM_START_SEC_VAR_FAST_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif ((defined CSM_STOP_SEC_VAR_FAST_UNSPECIFIED) || (defined CSM_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED))
  #undef CSM_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef CSM_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif ((defined CSM_START_SEC_INTERNAL_VAR_BOOLEAN) || (defined CSM_START_SEC_INTERNAL_VAR_INIT_BOOLEAN))
  #undef CSM_START_SEC_INTERNAL_VAR_BOOLEAN
  #undef CSM_START_SEC_INTERNAL_VAR_INIT_BOOLEAN
  #undef MEMMAP_ERROR

#elif ((defined CSM_STOP_SEC_INTERNAL_VAR_BOOLEAN) || (defined CSM_STOP_SEC_INTERNAL_VAR_INIT_BOOLEAN))
  #undef CSM_STOP_SEC_INTERNAL_VAR_BOOLEAN
  #undef CSM_STOP_SEC_INTERNAL_VAR_INIT_BOOLEAN
  #undef MEMMAP_ERROR

#elif ((defined CSM_START_SEC_INTERNAL_VAR_8BIT) || (defined CSM_START_SEC_INTERNAL_VAR_INIT_8))
  #undef CSM_START_SEC_INTERNAL_VAR_8BIT
  #undef CSM_START_SEC_INTERNAL_VAR_INIT_8
  #undef MEMMAP_ERROR

#elif ((defined CSM_STOP_SEC_INTERNAL_VAR_8BIT) || (defined CSM_STOP_SEC_INTERNAL_VAR_INIT_8))
  #undef CSM_STOP_SEC_INTERNAL_VAR_8BIT
  #undef CSM_STOP_SEC_INTERNAL_VAR_INIT_8
  #undef MEMMAP_ERROR

#elif ((defined CSM_START_SEC_INTERNAL_VAR_16BIT) || (defined CSM_START_SEC_INTERNAL_VAR_INIT_16))
  #undef CSM_START_SEC_INTERNAL_VAR_16BIT
  #undef CSM_START_SEC_INTERNAL_VAR_INIT_16
  #undef MEMMAP_ERROR

#elif ((defined CSM_STOP_SEC_INTERNAL_VAR_16BIT) || (defined CSM_STOP_SEC_INTERNAL_VAR_INIT_16))
  #undef CSM_STOP_SEC_INTERNAL_VAR_16BIT
  #undef CSM_STOP_SEC_INTERNAL_VAR_INIT_16
  #undef MEMMAP_ERROR

#elif ((defined CSM_START_SEC_INTERNAL_VAR_32BIT) || (defined CSM_START_SEC_INTERNAL_VAR_INIT_32))
  #undef CSM_START_SEC_INTERNAL_VAR_32BIT
  #undef CSM_START_SEC_INTERNAL_VAR_INIT_32
  #undef MEMMAP_ERROR

#elif ((defined CSM_STOP_SEC_INTERNAL_VAR_32BIT) || (defined CSM_STOP_SEC_INTERNAL_VAR_INIT_32))
  #undef CSM_STOP_SEC_INTERNAL_VAR_32BIT
  #undef CSM_STOP_SEC_INTERNAL_VAR_INIT_32
  #undef MEMMAP_ERROR

#elif ((defined CSM_START_SEC_INTERNAL_VAR_UNSPECIFIED) || (defined CSM_START_SEC_INTERNAL_VAR_INIT_UNSPECIFIED))
  #undef CSM_START_SEC_INTERNAL_VAR_UNSPECIFIED
  #undef CSM_START_SEC_INTERNAL_VAR_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif ((defined CSM_STOP_SEC_INTERNAL_VAR_UNSPECIFIED) || (defined CSM_STOP_SEC_INTERNAL_VAR_INIT_UNSPECIFIED))
  #undef CSM_STOP_SEC_INTERNAL_VAR_UNSPECIFIED
  #undef CSM_STOP_SEC_INTERNAL_VAR_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif ((defined CSM_START_SEC_VAR_BOOLEAN) || (defined CSM_START_SEC_VAR_INIT_BOOLEAN))
  #undef CSM_START_SEC_VAR_BOOLEAN
  #undef CSM_START_SEC_VAR_INIT_BOOLEAN
  #undef MEMMAP_ERROR

#elif ((defined CSM_STOP_SEC_VAR_BOOLEAN) || (defined CSM_STOP_SEC_VAR_INIT_BOOLEAN))
  #undef CSM_STOP_SEC_VAR_BOOLEAN
  #undef CSM_STOP_SEC_VAR_INIT_BOOLEAN
  #undef MEMMAP_ERROR

#elif ((defined CSM_START_SEC_VAR_8BIT) || (defined CSM_START_SEC_VAR_INIT_8))
  #undef CSM_START_SEC_VAR_8BIT
  #undef CSM_START_SEC_VAR_INIT_8
  #undef MEMMAP_ERROR

#elif ((defined CSM_STOP_SEC_VAR_8BIT) || (defined CSM_STOP_SEC_VAR_INIT_8))
  #undef CSM_STOP_SEC_VAR_8BIT
  #undef CSM_STOP_SEC_VAR_INIT_8
  #undef MEMMAP_ERROR

#elif ((defined CSM_START_SEC_VAR_16BIT) || (defined CSM_START_SEC_VAR_INIT_16))
  #undef CSM_START_SEC_VAR_16BIT
  #undef CSM_START_SEC_VAR_INIT_16
  #undef MEMMAP_ERROR

#elif ((defined CSM_STOP_SEC_VAR_16BIT) || (defined CSM_STOP_SEC_VAR_INIT_16))
  #undef CSM_STOP_SEC_VAR_16BIT
  #undef CSM_STOP_SEC_VAR_INIT_16
  #undef MEMMAP_ERROR

#elif ((defined CSM_START_SEC_VAR_32BIT) || (defined CSM_START_SEC_VAR_INIT_32))
  #undef CSM_START_SEC_VAR_32BIT
  #undef CSM_START_SEC_VAR_INIT_32
  #undef MEMMAP_ERROR

#elif ((defined CSM_STOP_SEC_VAR_32BIT) || (defined CSM_STOP_SEC_VAR_INIT_32))
  #undef CSM_STOP_SEC_VAR_32BIT
  #undef CSM_STOP_SEC_VAR_INIT_32
  #undef MEMMAP_ERROR

#elif ((defined CSM_START_SEC_VAR_UNSPECIFIED) || (defined CSM_START_SEC_VAR_INIT_UNSPECIFIED))
  #undef CSM_START_SEC_VAR_UNSPECIFIED
  #undef CSM_START_SEC_VAR_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif ((defined CSM_STOP_SEC_VAR_UNSPECIFIED) || (defined CSM_STOP_SEC_VAR_INIT_UNSPECIFIED))
  #undef CSM_STOP_SEC_VAR_UNSPECIFIED
  #undef CSM_STOP_SEC_VAR_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_CONST_BOOLEAN)
  #undef CSM_START_SEC_CONST_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_CONST_BOOLEAN)
  #undef CSM_STOP_SEC_CONST_BOOLEAN
  #undef MEMMAP_ERROR

#elif ((defined CSM_START_SEC_CONST_8BIT) || (defined CSM_START_SEC_CONST_8))
  #undef CSM_START_SEC_CONST_8BIT
  #undef CSM_START_SEC_CONST_8
  #undef MEMMAP_ERROR

#elif ((defined CSM_STOP_SEC_CONST_8BIT) || (defined CSM_STOP_SEC_CONST_8))
  #undef CSM_STOP_SEC_CONST_8BIT
  #undef CSM_STOP_SEC_CONST_8
  #undef MEMMAP_ERROR

#elif ((defined CSM_START_SEC_CONST_16BIT) || (defined CSM_START_SEC_CONST_16))
  #undef CSM_START_SEC_CONST_16BIT
  #undef CSM_START_SEC_CONST_16
  #undef MEMMAP_ERROR

#elif ((defined CSM_STOP_SEC_CONST_16BIT) || (defined CSM_STOP_SEC_CONST_16))
  #undef CSM_STOP_SEC_CONST_16BIT
  #undef CSM_STOP_SEC_CONST_16
  #undef MEMMAP_ERROR

#elif ((defined CSM_START_SEC_CONST_32BIT) || (defined CSM_START_SEC_CONST_32))
  #undef CSM_START_SEC_CONST_32BIT
  #undef CSM_START_SEC_CONST_32
  #undef MEMMAP_ERROR

#elif ((defined CSM_STOP_SEC_CONST_32BIT) || (defined CSM_STOP_SEC_CONST_32))
  #undef CSM_STOP_SEC_CONST_32BIT
  #undef CSM_STOP_SEC_CONST_32
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_CONST_UNSPECIFIED)
  #undef CSM_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_CONST_UNSPECIFIED)
  #undef CSM_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_CALIB_BOOLEAN)
  #undef CSM_START_SEC_CALIB_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_CALIB_BOOLEAN)
  #undef CSM_STOP_SEC_CALIB_BOOLEAN
  #undef MEMMAP_ERROR

#elif ((defined CSM_START_SEC_CALIB_8BIT) || (defined CSM_START_SEC_CALIB_8))
  #undef CSM_START_SEC_CALIB_8BIT
  #undef CSM_START_SEC_CALIB_8
  #undef MEMMAP_ERROR

#elif ((defined CSM_STOP_SEC_CALIB_8BIT) || (defined CSM_STOP_SEC_CALIB_8))
  #undef CSM_STOP_SEC_CALIB_8BIT
  #undef CSM_STOP_SEC_CALIB_8
  #undef MEMMAP_ERROR

#elif ((defined CSM_START_SEC_CALIB_16BIT) || (defined CSM_START_SEC_CALIB_16))
  #undef CSM_START_SEC_CALIB_16BIT
  #undef CSM_START_SEC_CALIB_16
  #undef MEMMAP_ERROR

#elif ((defined CSM_STOP_SEC_CALIB_16BIT) || (defined CSM_STOP_SEC_CALIB_16))
  #undef CSM_STOP_SEC_CALIB_16BIT
  #undef CSM_STOP_SEC_CALIB_16
  #undef MEMMAP_ERROR

#elif ((defined CSM_START_SEC_CALIB_32BIT) || (defined CSM_START_SEC_CALIB_32))
  #undef CSM_START_SEC_CALIB_32BIT
  #undef CSM_START_SEC_CALIB_32
  #undef MEMMAP_ERROR

#elif ((defined CSM_STOP_SEC_CALIB_32BIT) || (defined CSM_STOP_SEC_CALIB_32))
  #undef CSM_STOP_SEC_CALIB_32BIT
  #undef CSM_STOP_SEC_CALIB_32
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_CALIB_UNSPECIFIED)
  #undef CSM_START_SEC_CALIB_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_CALIB_UNSPECIFIED)
  #undef CSM_STOP_SEC_CALIB_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_CARTO_BOOLEAN)
  #undef CSM_START_SEC_CARTO_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_CARTO_BOOLEAN)
  #undef CSM_STOP_SEC_CARTO_BOOLEAN
  #undef MEMMAP_ERROR

#elif ((defined CSM_START_SEC_CARTO_8BIT) || (defined CSM_START_SEC_CARTO_8))
  #undef CSM_START_SEC_CARTO_8BIT
  #undef CSM_START_SEC_CARTO_8
  #undef MEMMAP_ERROR

#elif ((defined CSM_STOP_SEC_CARTO_8BIT) || (defined CSM_STOP_SEC_CARTO_8))
  #undef CSM_STOP_SEC_CARTO_8BIT
  #undef CSM_STOP_SEC_CARTO_8
  #undef MEMMAP_ERROR

#elif ((defined CSM_START_SEC_CARTO_16BIT) || (defined CSM_START_SEC_CARTO_16))
  #undef CSM_START_SEC_CARTO_16BIT
  #undef CSM_START_SEC_CARTO_16
  #undef MEMMAP_ERROR

#elif ((defined CSM_STOP_SEC_CARTO_16BIT) || (defined CSM_STOP_SEC_CARTO_16))
  #undef CSM_STOP_SEC_CARTO_16BIT
  #undef CSM_STOP_SEC_CARTO_16
  #undef MEMMAP_ERROR

#elif ((defined CSM_START_SEC_CARTO_32BIT) || (defined CSM_START_SEC_CARTO_32))
  #undef CSM_START_SEC_CARTO_32BIT
  #undef CSM_START_SEC_CARTO_32
  #undef MEMMAP_ERROR

#elif ((defined CSM_STOP_SEC_CARTO_32BIT) || (defined CSM_STOP_SEC_CARTO_32))
  #undef CSM_STOP_SEC_CARTO_32BIT
  #undef CSM_STOP_SEC_CARTO_32
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_CARTO_UNSPECIFIED)
  #undef CSM_START_SEC_CARTO_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_CARTO_UNSPECIFIED)
  #undef CSM_STOP_SEC_CARTO_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif ((defined CSM_START_CONFIG_DATA_BOOLEAN) || (defined CSM_START_SEC_CONFIG_DATA_BOOLEAN))
  #undef CSM_START_CONFIG_DATA_BOOLEAN
  #undef CSM_START_SEC_CONFIG_DATA_BOOLEAN
  #undef MEMMAP_ERROR

#elif ((defined CSM_STOP_CONFIG_DATA_BOOLEAN) || (defined CSM_STOP_SEC_CONFIG_DATA_BOOLEAN))
  #undef CSM_STOP_CONFIG_DATA_BOOLEAN
  #undef CSM_STOP_SEC_CONFIG_DATA_BOOLEAN
  #undef MEMMAP_ERROR

#elif ((defined CSM_START_CONFIG_DATA_8BIT) || (defined CSM_START_SEC_CONFIG_DATA_8))
  #undef CSM_START_CONFIG_DATA_8BIT
  #undef CSM_START_SEC_CONFIG_DATA_8
  #undef MEMMAP_ERROR

#elif ((defined CSM_STOP_CONFIG_DATA_8BIT) || (defined CSM_STOP_SEC_CONFIG_DATA_8))
  #undef CSM_STOP_CONFIG_DATA_8BIT
  #undef CSM_STOP_SEC_CONFIG_DATA_8
  #undef MEMMAP_ERROR

#elif ((defined CSM_START_CONFIG_DATA_16BIT) || (defined CSM_START_SEC_CONFIG_DATA_16))
  #undef CSM_START_CONFIG_DATA_16BIT
  #undef CSM_START_SEC_CONFIG_DATA_16
  #undef MEMMAP_ERROR

#elif ((defined CSM_STOP_CONFIG_DATA_16BIT) || (defined CSM_STOP_SEC_CONFIG_DATA_16))
  #undef CSM_STOP_CONFIG_DATA_16BIT
  #undef CSM_STOP_SEC_CONFIG_DATA_16
  #undef MEMMAP_ERROR

#elif ((defined CSM_START_CONFIG_DATA_32BIT) || (defined CSM_START_SEC_CONFIG_DATA_32))
  #undef CSM_START_CONFIG_DATA_32BIT
  #undef CSM_START_SEC_CONFIG_DATA_32
  #undef MEMMAP_ERROR

#elif ((defined CSM_STOP_CONFIG_DATA_32BIT) || (defined CSM_STOP_SEC_CONFIG_DATA_32))
  #undef CSM_STOP_CONFIG_DATA_32BIT
  #undef CSM_STOP_SEC_CONFIG_DATA_32
  #undef MEMMAP_ERROR

#elif ((defined CSM_START_CONFIG_DATA_UNSPECIFIED) || (defined CSM_START_SEC_CONFIG_DATA_UNSPECIFIED))
  #undef CSM_START_CONFIG_DATA_UNSPECIFIED
  #undef CSM_START_SEC_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif ((defined CSM_STOP_CONFIG_DATA_UNSPECIFIED) || (defined CSM_STOP_SEC_CONFIG_DATA_UNSPECIFIED))
  #undef CSM_STOP_CONFIG_DATA_UNSPECIFIED
  #undef CSM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_CLEARED_BOOLEAN)
  #undef CSM_START_SEC_VAR_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_CLEARED_BOOLEAN)
  #undef CSM_STOP_SEC_VAR_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_CLEARED_8)
  #undef CSM_START_SEC_VAR_CLEARED_8
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_CLEARED_8)
  #undef CSM_STOP_SEC_VAR_CLEARED_8
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_CLEARED_16)
  #undef CSM_START_SEC_VAR_CLEARED_16
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_CLEARED_16)
  #undef CSM_STOP_SEC_VAR_CLEARED_16
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_CLEARED_32)
  #undef CSM_START_SEC_VAR_CLEARED_32
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_CLEARED_32)
  #undef CSM_STOP_SEC_VAR_CLEARED_32
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_CLEARED_UNSPECIFIED)
  #undef CSM_START_SEC_VAR_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_CLEARED_UNSPECIFIED)
  #undef CSM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_POWER_ON_CLEARED_BOOLEAN)
  #undef CSM_START_SEC_VAR_POWER_ON_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_POWER_ON_CLEARED_BOOLEAN)
  #undef CSM_STOP_SEC_VAR_POWER_ON_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_POWER_ON_CLEARED_8)
  #undef CSM_START_SEC_VAR_POWER_ON_CLEARED_8
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_POWER_ON_CLEARED_8)
  #undef CSM_STOP_SEC_VAR_POWER_ON_CLEARED_8
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_POWER_ON_CLEARED_16)
  #undef CSM_START_SEC_VAR_POWER_ON_CLEARED_16
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_POWER_ON_CLEARED_16)
  #undef CSM_STOP_SEC_VAR_POWER_ON_CLEARED_16
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_POWER_ON_CLEARED_32)
  #undef CSM_START_SEC_VAR_POWER_ON_CLEARED_32
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_POWER_ON_CLEARED_32)
  #undef CSM_STOP_SEC_VAR_POWER_ON_CLEARED_32
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_POWER_ON_CLEARED_UNSPECIFIED)
  #undef CSM_START_SEC_VAR_POWER_ON_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_POWER_ON_CLEARED_UNSPECIFIED)
  #undef CSM_STOP_SEC_VAR_POWER_ON_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_FAST_NO_INIT_BOOLEAN)
  #undef CSM_START_SEC_VAR_FAST_NO_INIT_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_FAST_NO_INIT_BOOLEAN)
  #undef CSM_STOP_SEC_VAR_FAST_NO_INIT_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_FAST_NO_INIT_8)
  #undef CSM_START_SEC_VAR_FAST_NO_INIT_8
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_FAST_NO_INIT_8)
  #undef CSM_STOP_SEC_VAR_FAST_NO_INIT_8
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_FAST_NO_INIT_16)
  #undef CSM_START_SEC_VAR_FAST_NO_INIT_16
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_FAST_NO_INIT_16)
  #undef CSM_STOP_SEC_VAR_FAST_NO_INIT_16
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_FAST_NO_INIT_32)
  #undef CSM_START_SEC_VAR_FAST_NO_INIT_32
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_FAST_NO_INIT_32)
  #undef CSM_STOP_SEC_VAR_FAST_NO_INIT_32
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_FAST_NO_INIT_UNSPECIFIED)
  #undef CSM_START_SEC_VAR_FAST_NO_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_FAST_NO_INIT_UNSPECIFIED)
  #undef CSM_STOP_SEC_VAR_FAST_NO_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_FAST_CLEARED_BOOLEAN)
  #undef CSM_START_SEC_VAR_FAST_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_FAST_CLEARED_BOOLEAN)
  #undef CSM_STOP_SEC_VAR_FAST_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_FAST_CLEARED_8)
  #undef CSM_START_SEC_VAR_FAST_CLEARED_8
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_FAST_CLEARED_8)
  #undef CSM_STOP_SEC_VAR_FAST_CLEARED_8
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_FAST_CLEARED_16)
  #undef CSM_START_SEC_VAR_FAST_CLEARED_16
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_FAST_CLEARED_16)
  #undef CSM_STOP_SEC_VAR_FAST_CLEARED_16
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_FAST_CLEARED_32)
  #undef CSM_START_SEC_VAR_FAST_CLEARED_32
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_FAST_CLEARED_32)
  #undef CSM_STOP_SEC_VAR_FAST_CLEARED_32
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_FAST_CLEARED_UNSPECIFIED)
  #undef CSM_START_SEC_VAR_FAST_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_FAST_CLEARED_UNSPECIFIED)
  #undef CSM_STOP_SEC_VAR_FAST_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_FAST_POWER_ON_CLEARED_BOOLEAN)
  #undef CSM_START_SEC_VAR_FAST_POWER_ON_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_FAST_POWER_ON_CLEARED_BOOLEAN)
  #undef CSM_STOP_SEC_VAR_FAST_POWER_ON_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_FAST_POWER_ON_CLEARED_8)
  #undef CSM_START_SEC_VAR_FAST_POWER_ON_CLEARED_8
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_FAST_POWER_ON_CLEARED_8)
  #undef CSM_STOP_SEC_VAR_FAST_POWER_ON_CLEARED_8
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_FAST_POWER_ON_CLEARED_16)
  #undef CSM_START_SEC_VAR_FAST_POWER_ON_CLEARED_16
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_FAST_POWER_ON_CLEARED_16)
  #undef CSM_STOP_SEC_VAR_FAST_POWER_ON_CLEARED_16
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_FAST_POWER_ON_CLEARED_32)
  #undef CSM_START_SEC_VAR_FAST_POWER_ON_CLEARED_32
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_FAST_POWER_ON_CLEARED_32)
  #undef CSM_STOP_SEC_VAR_FAST_POWER_ON_CLEARED_32
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_FAST_POWER_ON_CLEARED_UNSPECIFIED)
  #undef CSM_START_SEC_VAR_FAST_POWER_ON_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_FAST_POWER_ON_CLEARED_UNSPECIFIED)
  #undef CSM_STOP_SEC_VAR_FAST_POWER_ON_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_FAST_POWER_ON_INIT_BOOLEAN)
  #undef CSM_START_SEC_VAR_FAST_POWER_ON_INIT_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_FAST_POWER_ON_INIT_BOOLEAN)
  #undef CSM_STOP_SEC_VAR_FAST_POWER_ON_INIT_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_FAST_POWER_ON_INIT_8)
  #undef CSM_START_SEC_VAR_FAST_POWER_ON_INIT_8
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_FAST_POWER_ON_INIT_8)
  #undef CSM_STOP_SEC_VAR_FAST_POWER_ON_INIT_8
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_FAST_POWER_ON_INIT_16)
  #undef CSM_START_SEC_VAR_FAST_POWER_ON_INIT_16
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_FAST_POWER_ON_INIT_16)
  #undef CSM_STOP_SEC_VAR_FAST_POWER_ON_INIT_16
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_FAST_POWER_ON_INIT_32)
  #undef CSM_START_SEC_VAR_FAST_POWER_ON_INIT_32
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_FAST_POWER_ON_INIT_32)
  #undef CSM_STOP_SEC_VAR_FAST_POWER_ON_INIT_32
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_FAST_POWER_ON_INIT_UNSPECIFIED)
  #undef CSM_START_SEC_VAR_FAST_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_FAST_POWER_ON_INIT_UNSPECIFIED)
  #undef CSM_STOP_SEC_VAR_FAST_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_SLOW_NO_INIT_BOOLEAN)
  #undef CSM_START_SEC_VAR_SLOW_NO_INIT_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_SLOW_NO_INIT_BOOLEAN)
  #undef CSM_STOP_SEC_VAR_SLOW_NO_INIT_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_SLOW_NO_INIT_8)
  #undef CSM_START_SEC_VAR_SLOW_NO_INIT_8
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_SLOW_NO_INIT_8)
  #undef CSM_STOP_SEC_VAR_SLOW_NO_INIT_8
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_SLOW_NO_INIT_16)
  #undef CSM_START_SEC_VAR_SLOW_NO_INIT_16
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_SLOW_NO_INIT_16)
  #undef CSM_STOP_SEC_VAR_SLOW_NO_INIT_16
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_SLOW_NO_INIT_32)
  #undef CSM_START_SEC_VAR_SLOW_NO_INIT_32
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_SLOW_NO_INIT_32)
  #undef CSM_STOP_SEC_VAR_SLOW_NO_INIT_32
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_SLOW_NO_INIT_UNSPECIFIED)
  #undef CSM_START_SEC_VAR_SLOW_NO_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_SLOW_NO_INIT_UNSPECIFIED)
  #undef CSM_STOP_SEC_VAR_SLOW_NO_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_SLOW_CLEARED_BOOLEAN)
  #undef CSM_START_SEC_VAR_SLOW_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_SLOW_CLEARED_BOOLEAN)
  #undef CSM_STOP_SEC_VAR_SLOW_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_SLOW_CLEARED_8)
  #undef CSM_START_SEC_VAR_SLOW_CLEARED_8
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_SLOW_CLEARED_8)
  #undef CSM_STOP_SEC_VAR_SLOW_CLEARED_8
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_SLOW_CLEARED_16)
  #undef CSM_START_SEC_VAR_SLOW_CLEARED_16
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_SLOW_CLEARED_16)
  #undef CSM_STOP_SEC_VAR_SLOW_CLEARED_16
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_SLOW_CLEARED_32)
  #undef CSM_START_SEC_VAR_SLOW_CLEARED_32
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_SLOW_CLEARED_32)
  #undef CSM_STOP_SEC_VAR_SLOW_CLEARED_32
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_SLOW_CLEARED_UNSPECIFIED)
  #undef CSM_START_SEC_VAR_SLOW_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_SLOW_CLEARED_UNSPECIFIED)
  #undef CSM_STOP_SEC_VAR_SLOW_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_SLOW_POWER_ON_CLEARED_BOOLEAN)
  #undef CSM_START_SEC_VAR_SLOW_POWER_ON_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_SLOW_POWER_ON_CLEARED_BOOLEAN)
  #undef CSM_STOP_SEC_VAR_SLOW_POWER_ON_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_SLOW_POWER_ON_CLEARED_8)
  #undef CSM_START_SEC_VAR_SLOW_POWER_ON_CLEARED_8
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_SLOW_POWER_ON_CLEARED_8)
  #undef CSM_STOP_SEC_VAR_SLOW_POWER_ON_CLEARED_8
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_SLOW_POWER_ON_CLEARED_16)
  #undef CSM_START_SEC_VAR_SLOW_POWER_ON_CLEARED_16
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_SLOW_POWER_ON_CLEARED_16)
  #undef CSM_STOP_SEC_VAR_SLOW_POWER_ON_CLEARED_16
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_SLOW_POWER_ON_CLEARED_32)
  #undef CSM_START_SEC_VAR_SLOW_POWER_ON_CLEARED_32
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_SLOW_POWER_ON_CLEARED_32)
  #undef CSM_STOP_SEC_VAR_SLOW_POWER_ON_CLEARED_32
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_SLOW_POWER_ON_CLEARED_UNSPECIFIED)
  #undef CSM_START_SEC_VAR_SLOW_POWER_ON_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_SLOW_POWER_ON_CLEARED_UNSPECIFIED)
  #undef CSM_STOP_SEC_VAR_SLOW_POWER_ON_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_SLOW_INIT_BOOLEAN)
  #undef CSM_START_SEC_VAR_SLOW_INIT_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_SLOW_INIT_BOOLEAN)
  #undef CSM_STOP_SEC_VAR_SLOW_INIT_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_SLOW_INIT_8)
  #undef CSM_START_SEC_VAR_SLOW_INIT_8
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_SLOW_INIT_8)
  #undef CSM_STOP_SEC_VAR_SLOW_INIT_8
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_SLOW_INIT_16)
  #undef CSM_START_SEC_VAR_SLOW_INIT_16
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_SLOW_INIT_16)
  #undef CSM_STOP_SEC_VAR_SLOW_INIT_16
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_SLOW_INIT_32)
  #undef CSM_START_SEC_VAR_SLOW_INIT_32
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_SLOW_INIT_32)
  #undef CSM_STOP_SEC_VAR_SLOW_INIT_32
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_SLOW_INIT_UNSPECIFIED)
  #undef CSM_START_SEC_VAR_SLOW_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_SLOW_INIT_UNSPECIFIED)
  #undef CSM_STOP_SEC_VAR_SLOW_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_SLOW_POWER_ON_INIT_BOOLEAN)
  #undef CSM_START_SEC_VAR_SLOW_POWER_ON_INIT_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_SLOW_POWER_ON_INIT_BOOLEAN)
  #undef CSM_STOP_SEC_VAR_SLOW_POWER_ON_INIT_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_SLOW_POWER_ON_INIT_8)
  #undef CSM_START_SEC_VAR_SLOW_POWER_ON_INIT_8
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_SLOW_POWER_ON_INIT_8)
  #undef CSM_STOP_SEC_VAR_SLOW_POWER_ON_INIT_8
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_SLOW_POWER_ON_INIT_16)
  #undef CSM_START_SEC_VAR_SLOW_POWER_ON_INIT_16
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_SLOW_POWER_ON_INIT_16)
  #undef CSM_STOP_SEC_VAR_SLOW_POWER_ON_INIT_16
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_SLOW_POWER_ON_INIT_32)
  #undef CSM_START_SEC_VAR_SLOW_POWER_ON_INIT_32
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_SLOW_POWER_ON_INIT_32)
  #undef CSM_STOP_SEC_VAR_SLOW_POWER_ON_INIT_32
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_VAR_SLOW_POWER_ON_INIT_UNSPECIFIED)
  #undef CSM_START_SEC_VAR_SLOW_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_VAR_SLOW_POWER_ON_INIT_UNSPECIFIED)
  #undef CSM_STOP_SEC_VAR_SLOW_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_INTERNAL_VAR_NO_INIT_BOOLEAN)
  #undef CSM_START_SEC_INTERNAL_VAR_NO_INIT_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_INTERNAL_VAR_NO_INIT_BOOLEAN)
  #undef CSM_STOP_SEC_INTERNAL_VAR_NO_INIT_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_INTERNAL_VAR_NO_INIT_8)
  #undef CSM_START_SEC_INTERNAL_VAR_NO_INIT_8
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_INTERNAL_VAR_NO_INIT_8)
  #undef CSM_STOP_SEC_INTERNAL_VAR_NO_INIT_8
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_INTERNAL_VAR_NO_INIT_16)
  #undef CSM_START_SEC_INTERNAL_VAR_NO_INIT_16
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_INTERNAL_VAR_NO_INIT_16)
  #undef CSM_STOP_SEC_INTERNAL_VAR_NO_INIT_16
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_INTERNAL_VAR_NO_INIT_32)
  #undef CSM_START_SEC_INTERNAL_VAR_NO_INIT_32
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_INTERNAL_VAR_NO_INIT_32)
  #undef CSM_STOP_SEC_INTERNAL_VAR_NO_INIT_32
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_INTERNAL_VAR_NO_INIT_UNSPECIFIED)
  #undef CSM_START_SEC_INTERNAL_VAR_NO_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_INTERNAL_VAR_NO_INIT_UNSPECIFIED)
  #undef CSM_STOP_SEC_INTERNAL_VAR_NO_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_INTERNAL_VAR_CLEARED_BOOLEAN)
  #undef CSM_START_SEC_INTERNAL_VAR_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_INTERNAL_VAR_CLEARED_BOOLEAN)
  #undef CSM_STOP_SEC_INTERNAL_VAR_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_INTERNAL_VAR_CLEARED_8)
  #undef CSM_START_SEC_INTERNAL_VAR_CLEARED_8
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_INTERNAL_VAR_CLEARED_8)
  #undef CSM_STOP_SEC_INTERNAL_VAR_CLEARED_8
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_INTERNAL_VAR_CLEARED_16)
  #undef CSM_START_SEC_INTERNAL_VAR_CLEARED_16
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_INTERNAL_VAR_CLEARED_16)
  #undef CSM_STOP_SEC_INTERNAL_VAR_CLEARED_16
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_INTERNAL_VAR_CLEARED_32)
  #undef CSM_START_SEC_INTERNAL_VAR_CLEARED_32
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_INTERNAL_VAR_CLEARED_32)
  #undef CSM_STOP_SEC_INTERNAL_VAR_CLEARED_32
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_INTERNAL_VAR_CLEARED_UNSPECIFIED)
  #undef CSM_START_SEC_INTERNAL_VAR_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_INTERNAL_VAR_CLEARED_UNSPECIFIED)
  #undef CSM_STOP_SEC_INTERNAL_VAR_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_INTERNAL_VAR_POWER_ON_CLEARED_BOOLEAN)
  #undef CSM_START_SEC_INTERNAL_VAR_POWER_ON_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_INTERNAL_VAR_POWER_ON_CLEARED_BOOLEAN)
  #undef CSM_STOP_SEC_INTERNAL_VAR_POWER_ON_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_INTERNAL_VAR_POWER_ON_CLEARED_8)
  #undef CSM_START_SEC_INTERNAL_VAR_POWER_ON_CLEARED_8
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_INTERNAL_VAR_POWER_ON_CLEARED_8)
  #undef CSM_STOP_SEC_INTERNAL_VAR_POWER_ON_CLEARED_8
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_INTERNAL_VAR_POWER_ON_CLEARED_16)
  #undef CSM_START_SEC_INTERNAL_VAR_POWER_ON_CLEARED_16
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_INTERNAL_VAR_POWER_ON_CLEARED_16)
  #undef CSM_STOP_SEC_INTERNAL_VAR_POWER_ON_CLEARED_16
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_INTERNAL_VAR_POWER_ON_CLEARED_32)
  #undef CSM_START_SEC_INTERNAL_VAR_POWER_ON_CLEARED_32
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_INTERNAL_VAR_POWER_ON_CLEARED_32)
  #undef CSM_STOP_SEC_INTERNAL_VAR_POWER_ON_CLEARED_32
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_INTERNAL_VAR_POWER_ON_CLEARED_UNSPECIFIED)
  #undef CSM_START_SEC_INTERNAL_VAR_POWER_ON_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_INTERNAL_VAR_POWER_ON_CLEARED_UNSPECIFIED)
  #undef CSM_STOP_SEC_INTERNAL_VAR_POWER_ON_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_CODE_FAST)
  #undef CSM_START_SEC_CODE_FAST
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_CODE_FAST)
  #undef CSM_STOP_SEC_CODE_FAST
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_CODE_SLOW)
  #undef CSM_START_SEC_CODE_SLOW
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_CODE_SLOW)
  #undef CSM_STOP_SEC_CODE_SLOW
  #undef MEMMAP_ERROR

#elif (defined CSM_START_SEC_CODE_LIB)
  #undef CSM_START_SEC_CODE_LIB
  #undef MEMMAP_ERROR

#elif (defined CSM_STOP_SEC_CODE_LIB)
  #undef CSM_STOP_SEC_CODE_LIB
  #undef MEMMAP_ERROR

/*------------------[Start of a module]--------------------------------------*/

#elif (defined DBG_START_SEC_CODE)
  #undef DBG_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined DBG_STOP_SEC_CODE)
  #undef DBG_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined DBG_START_SEC_APPL_CODE)
  #undef DBG_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined DBG_STOP_SEC_APPL_CODE)
  #undef DBG_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined DBG_START_SEC_VAR_NOINIT_8BIT)
  #undef DBG_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined DBG_STOP_SEC_VAR_NOINIT_8BIT)
  #undef DBG_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined DBG_START_SEC_VAR_NOINIT_16BIT)
  #undef DBG_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined DBG_STOP_SEC_VAR_NOINIT_16BIT)
  #undef DBG_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined DBG_START_SEC_VAR_NOINIT_32BIT)
  #undef DBG_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined DBG_STOP_SEC_VAR_NOINIT_32BIT)
  #undef DBG_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined DBG_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef DBG_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DBG_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef DBG_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DBG_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef DBG_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined DBG_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef DBG_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined DBG_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef DBG_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined DBG_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef DBG_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined DBG_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef DBG_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined DBG_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef DBG_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined DBG_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef DBG_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DBG_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef DBG_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DBG_START_SEC_VAR_FAST_8BIT)
  #undef DBG_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined DBG_STOP_SEC_VAR_FAST_8BIT)
  #undef DBG_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined DBG_START_SEC_VAR_FAST_16BIT)
  #undef DBG_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined DBG_STOP_SEC_VAR_FAST_16BIT)
  #undef DBG_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined DBG_START_SEC_VAR_FAST_32BIT)
  #undef DBG_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined DBG_STOP_SEC_VAR_FAST_32BIT)
  #undef DBG_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined DBG_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef DBG_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DBG_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef DBG_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DBG_START_SEC_VAR_8BIT)
  #undef DBG_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined DBG_STOP_SEC_VAR_8BIT)
  #undef DBG_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined DBG_START_SEC_VAR_16BIT)
  #undef DBG_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined DBG_STOP_SEC_VAR_16BIT)
  #undef DBG_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined DBG_START_SEC_VAR_32BIT)
  #undef DBG_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined DBG_STOP_SEC_VAR_32BIT)
  #undef DBG_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined DBG_START_SEC_VAR_UNSPECIFIED)
  #undef DBG_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DBG_STOP_SEC_VAR_UNSPECIFIED)
  #undef DBG_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DBG_START_SEC_CONST_8BIT)
  #undef DBG_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined DBG_STOP_SEC_CONST_8BIT)
  #undef DBG_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined DBG_START_SEC_CONST_16BIT)
  #undef DBG_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined DBG_STOP_SEC_CONST_16BIT)
  #undef DBG_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined DBG_START_SEC_CONST_32BIT)
  #undef DBG_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined DBG_STOP_SEC_CONST_32BIT)
  #undef DBG_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined DBG_START_SEC_CONST_UNSPECIFIED)
  #undef DBG_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DBG_STOP_SEC_CONST_UNSPECIFIED)
  #undef DBG_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DBG_START_CONFIG_DATA_8BIT)
  #undef DBG_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined DBG_STOP_CONFIG_DATA_8BIT)
  #undef DBG_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined DBG_START_CONFIG_DATA_16BIT)
  #undef DBG_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined DBG_STOP_CONFIG_DATA_16BIT)
  #undef DBG_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined DBG_START_CONFIG_DATA_32BIT)
  #undef DBG_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined DBG_STOP_CONFIG_DATA_32BIT)
  #undef DBG_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined DBG_START_CONFIG_DATA_UNSPECIFIED)
  #undef DBG_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DBG_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef DBG_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined DCM_START_SEC_CODE)
  #undef DCM_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined DCM_STOP_SEC_CODE)
  #undef DCM_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined DCM_START_SEC_APPL_CODE)
  #undef DCM_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined DCM_STOP_SEC_APPL_CODE)
  #undef DCM_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined DCM_START_SEC_VAR_NOINIT_8BIT)
  #undef DCM_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined DCM_STOP_SEC_VAR_NOINIT_8BIT)
  #undef DCM_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined DCM_START_SEC_VAR_NOINIT_16BIT)
  #undef DCM_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined DCM_STOP_SEC_VAR_NOINIT_16BIT)
  #undef DCM_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined DCM_START_SEC_VAR_NOINIT_32BIT)
  #undef DCM_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined DCM_STOP_SEC_VAR_NOINIT_32BIT)
  #undef DCM_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined DCM_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef DCM_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DCM_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef DCM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DCM_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef DCM_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined DCM_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef DCM_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined DCM_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef DCM_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined DCM_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef DCM_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined DCM_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef DCM_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined DCM_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef DCM_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined DCM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef DCM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DCM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef DCM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DCM_START_SEC_VAR_FAST_8BIT)
  #undef DCM_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined DCM_STOP_SEC_VAR_FAST_8BIT)
  #undef DCM_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined DCM_START_SEC_VAR_FAST_16BIT)
  #undef DCM_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined DCM_STOP_SEC_VAR_FAST_16BIT)
  #undef DCM_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined DCM_START_SEC_VAR_FAST_32BIT)
  #undef DCM_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined DCM_STOP_SEC_VAR_FAST_32BIT)
  #undef DCM_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined DCM_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef DCM_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DCM_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef DCM_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DCM_START_SEC_VAR_8BIT)
  #undef DCM_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined DCM_STOP_SEC_VAR_8BIT)
  #undef DCM_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined DCM_START_SEC_VAR_16BIT)
  #undef DCM_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined DCM_STOP_SEC_VAR_16BIT)
  #undef DCM_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined DCM_START_SEC_VAR_32BIT)
  #undef DCM_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined DCM_STOP_SEC_VAR_32BIT)
  #undef DCM_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined DCM_START_SEC_VAR_UNSPECIFIED)
  #undef DCM_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DCM_STOP_SEC_VAR_UNSPECIFIED)
  #undef DCM_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DCM_START_SEC_CONST_8BIT)
  #undef DCM_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined DCM_STOP_SEC_CONST_8BIT)
  #undef DCM_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined DCM_START_SEC_CONST_16BIT)
  #undef DCM_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined DCM_STOP_SEC_CONST_16BIT)
  #undef DCM_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined DCM_START_SEC_CONST_32BIT)
  #undef DCM_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined DCM_STOP_SEC_CONST_32BIT)
  #undef DCM_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined DCM_START_SEC_CONST_UNSPECIFIED)
  #undef DCM_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DCM_STOP_SEC_CONST_UNSPECIFIED)
  #undef DCM_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DCM_START_CONFIG_DATA_8BIT)
  #undef DCM_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined DCM_STOP_CONFIG_DATA_8BIT)
  #undef DCM_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined DCM_START_CONFIG_DATA_16BIT)
  #undef DCM_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined DCM_STOP_CONFIG_DATA_16BIT)
  #undef DCM_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined DCM_START_CONFIG_DATA_32BIT)
  #undef DCM_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined DCM_STOP_CONFIG_DATA_32BIT)
  #undef DCM_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined DCM_START_CONFIG_DATA_UNSPECIFIED)
  #undef DCM_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DCM_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef DCM_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined DEM_START_SEC_CODE)
  #undef DEM_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined DEM_STOP_SEC_CODE)
  #undef DEM_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined DEM_START_SEC_APPL_CODE)
  #undef DEM_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined DEM_STOP_SEC_APPL_CODE)
  #undef DEM_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined DEM_START_SEC_VAR_NOINIT_8BIT)
  #undef DEM_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined DEM_STOP_SEC_VAR_NOINIT_8BIT)
  #undef DEM_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined DEM_START_SEC_VAR_NOINIT_16BIT)
  #undef DEM_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined DEM_STOP_SEC_VAR_NOINIT_16BIT)
  #undef DEM_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined DEM_START_SEC_VAR_NOINIT_32BIT)
  #undef DEM_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined DEM_STOP_SEC_VAR_NOINIT_32BIT)
  #undef DEM_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined DEM_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef DEM_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DEM_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef DEM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DEM_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef DEM_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined DEM_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef DEM_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined DEM_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef DEM_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined DEM_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef DEM_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined DEM_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef DEM_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined DEM_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef DEM_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined DEM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef DEM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DEM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef DEM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DEM_START_SEC_VAR_FAST_8BIT)
  #undef DEM_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined DEM_STOP_SEC_VAR_FAST_8BIT)
  #undef DEM_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined DEM_START_SEC_VAR_FAST_16BIT)
  #undef DEM_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined DEM_STOP_SEC_VAR_FAST_16BIT)
  #undef DEM_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined DEM_START_SEC_VAR_FAST_32BIT)
  #undef DEM_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined DEM_STOP_SEC_VAR_FAST_32BIT)
  #undef DEM_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined DEM_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef DEM_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DEM_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef DEM_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DEM_START_SEC_VAR_8BIT)
  #undef DEM_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined DEM_STOP_SEC_VAR_8BIT)
  #undef DEM_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined DEM_START_SEC_VAR_16BIT)
  #undef DEM_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined DEM_STOP_SEC_VAR_16BIT)
  #undef DEM_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined DEM_START_SEC_VAR_32BIT)
  #undef DEM_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined DEM_STOP_SEC_VAR_32BIT)
  #undef DEM_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined DEM_START_SEC_VAR_UNSPECIFIED)
  #undef DEM_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DEM_STOP_SEC_VAR_UNSPECIFIED)
  #undef DEM_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DEM_START_SEC_CONST_8BIT)
  #undef DEM_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined DEM_STOP_SEC_CONST_8BIT)
  #undef DEM_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined DEM_START_SEC_CONST_16BIT)
  #undef DEM_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined DEM_STOP_SEC_CONST_16BIT)
  #undef DEM_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined DEM_START_SEC_CONST_32BIT)
  #undef DEM_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined DEM_STOP_SEC_CONST_32BIT)
  #undef DEM_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined DEM_START_SEC_CONST_UNSPECIFIED)
  #undef DEM_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DEM_STOP_SEC_CONST_UNSPECIFIED)
  #undef DEM_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DEM_START_CONFIG_DATA_8BIT)
  #undef DEM_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined DEM_STOP_CONFIG_DATA_8BIT)
  #undef DEM_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined DEM_START_CONFIG_DATA_16BIT)
  #undef DEM_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined DEM_STOP_CONFIG_DATA_16BIT)
  #undef DEM_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined DEM_START_CONFIG_DATA_32BIT)
  #undef DEM_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined DEM_STOP_CONFIG_DATA_32BIT)
  #undef DEM_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined DEM_START_CONFIG_DATA_UNSPECIFIED)
  #undef DEM_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DEM_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef DEM_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined DET_START_SEC_CODE)
  #undef DET_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined DET_STOP_SEC_CODE)
  #undef DET_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined DET_START_SEC_APPL_CODE)
  #undef DET_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined DET_STOP_SEC_APPL_CODE)
  #undef DET_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined DET_START_SEC_VAR_NOINIT_8BIT)
  #undef DET_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined DET_STOP_SEC_VAR_NOINIT_8BIT)
  #undef DET_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined DET_START_SEC_VAR_NOINIT_16BIT)
  #undef DET_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined DET_STOP_SEC_VAR_NOINIT_16BIT)
  #undef DET_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined DET_START_SEC_VAR_NOINIT_32BIT)
  #undef DET_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined DET_STOP_SEC_VAR_NOINIT_32BIT)
  #undef DET_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined DET_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef DET_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DET_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef DET_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DET_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef DET_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined DET_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef DET_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined DET_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef DET_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined DET_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef DET_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined DET_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef DET_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined DET_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef DET_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined DET_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef DET_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DET_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef DET_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DET_START_SEC_VAR_FAST_8BIT)
  #undef DET_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined DET_STOP_SEC_VAR_FAST_8BIT)
  #undef DET_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined DET_START_SEC_VAR_FAST_16BIT)
  #undef DET_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined DET_STOP_SEC_VAR_FAST_16BIT)
  #undef DET_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined DET_START_SEC_VAR_FAST_32BIT)
  #undef DET_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined DET_STOP_SEC_VAR_FAST_32BIT)
  #undef DET_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined DET_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef DET_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DET_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef DET_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DET_START_SEC_VAR_8BIT)
  #undef DET_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined DET_STOP_SEC_VAR_8BIT)
  #undef DET_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined DET_START_SEC_VAR_16BIT)
  #undef DET_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined DET_STOP_SEC_VAR_16BIT)
  #undef DET_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined DET_START_SEC_VAR_32BIT)
  #undef DET_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined DET_STOP_SEC_VAR_32BIT)
  #undef DET_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined DET_START_SEC_VAR_UNSPECIFIED)
  #undef DET_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DET_STOP_SEC_VAR_UNSPECIFIED)
  #undef DET_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DET_START_SEC_CONST_8BIT)
  #undef DET_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined DET_STOP_SEC_CONST_8BIT)
  #undef DET_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined DET_START_SEC_CONST_16BIT)
  #undef DET_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined DET_STOP_SEC_CONST_16BIT)
  #undef DET_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined DET_START_SEC_CONST_32BIT)
  #undef DET_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined DET_STOP_SEC_CONST_32BIT)
  #undef DET_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined DET_START_SEC_CONST_UNSPECIFIED)
  #undef DET_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DET_STOP_SEC_CONST_UNSPECIFIED)
  #undef DET_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DET_START_CONFIG_DATA_8BIT)
  #undef DET_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined DET_STOP_CONFIG_DATA_8BIT)
  #undef DET_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined DET_START_CONFIG_DATA_16BIT)
  #undef DET_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined DET_STOP_CONFIG_DATA_16BIT)
  #undef DET_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined DET_START_CONFIG_DATA_32BIT)
  #undef DET_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined DET_STOP_CONFIG_DATA_32BIT)
  #undef DET_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined DET_START_CONFIG_DATA_UNSPECIFIED)
  #undef DET_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DET_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef DET_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined DIO_START_SEC_CODE)
  #undef DIO_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined DIO_STOP_SEC_CODE)
  #undef DIO_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined DIO_START_SEC_APPL_CODE)
  #undef DIO_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined DIO_STOP_SEC_APPL_CODE)
  #undef DIO_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined DIO_START_SEC_VAR_NOINIT_8BIT)
  #undef DIO_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined DIO_STOP_SEC_VAR_NOINIT_8BIT)
  #undef DIO_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined DIO_START_SEC_VAR_NOINIT_16BIT)
  #undef DIO_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined DIO_STOP_SEC_VAR_NOINIT_16BIT)
  #undef DIO_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined DIO_START_SEC_VAR_NOINIT_32BIT)
  #undef DIO_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined DIO_STOP_SEC_VAR_NOINIT_32BIT)
  #undef DIO_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined DIO_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef DIO_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DIO_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef DIO_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DIO_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef DIO_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined DIO_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef DIO_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined DIO_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef DIO_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined DIO_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef DIO_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined DIO_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef DIO_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined DIO_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef DIO_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined DIO_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef DIO_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DIO_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef DIO_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DIO_START_SEC_VAR_FAST_8BIT)
  #undef DIO_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined DIO_STOP_SEC_VAR_FAST_8BIT)
  #undef DIO_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined DIO_START_SEC_VAR_FAST_16BIT)
  #undef DIO_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined DIO_STOP_SEC_VAR_FAST_16BIT)
  #undef DIO_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined DIO_START_SEC_VAR_FAST_32BIT)
  #undef DIO_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined DIO_STOP_SEC_VAR_FAST_32BIT)
  #undef DIO_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined DIO_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef DIO_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DIO_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef DIO_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DIO_START_SEC_VAR_8BIT)
  #undef DIO_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined DIO_STOP_SEC_VAR_8BIT)
  #undef DIO_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined DIO_START_SEC_VAR_16BIT)
  #undef DIO_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined DIO_STOP_SEC_VAR_16BIT)
  #undef DIO_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined DIO_START_SEC_VAR_32BIT)
  #undef DIO_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined DIO_STOP_SEC_VAR_32BIT)
  #undef DIO_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined DIO_START_SEC_VAR_UNSPECIFIED)
  #undef DIO_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DIO_STOP_SEC_VAR_UNSPECIFIED)
  #undef DIO_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DIO_START_SEC_CONST_8BIT)
  #undef DIO_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined DIO_STOP_SEC_CONST_8BIT)
  #undef DIO_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined DIO_START_SEC_CONST_16BIT)
  #undef DIO_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined DIO_STOP_SEC_CONST_16BIT)
  #undef DIO_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined DIO_START_SEC_CONST_32BIT)
  #undef DIO_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined DIO_STOP_SEC_CONST_32BIT)
  #undef DIO_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined DIO_START_SEC_CONST_UNSPECIFIED)
  #undef DIO_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DIO_STOP_SEC_CONST_UNSPECIFIED)
  #undef DIO_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DIO_START_CONFIG_DATA_8BIT)
  #undef DIO_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined DIO_STOP_CONFIG_DATA_8BIT)
  #undef DIO_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined DIO_START_CONFIG_DATA_16BIT)
  #undef DIO_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined DIO_STOP_CONFIG_DATA_16BIT)
  #undef DIO_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined DIO_START_CONFIG_DATA_32BIT)
  #undef DIO_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined DIO_STOP_CONFIG_DATA_32BIT)
  #undef DIO_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined DIO_START_CONFIG_DATA_UNSPECIFIED)
  #undef DIO_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined DIO_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef DIO_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined E2E_START_SEC_CODE)
  #undef E2E_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined E2E_STOP_SEC_CODE)
  #undef E2E_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined E2E_START_SEC_APPL_CODE)
  #undef E2E_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined E2E_STOP_SEC_APPL_CODE)
  #undef E2E_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined E2E_START_SEC_VAR_NOINIT_8BIT)
  #undef E2E_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined E2E_STOP_SEC_VAR_NOINIT_8BIT)
  #undef E2E_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined E2E_START_SEC_VAR_NOINIT_16BIT)
  #undef E2E_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined E2E_STOP_SEC_VAR_NOINIT_16BIT)
  #undef E2E_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined E2E_START_SEC_VAR_NOINIT_32BIT)
  #undef E2E_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined E2E_STOP_SEC_VAR_NOINIT_32BIT)
  #undef E2E_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined E2E_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef E2E_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined E2E_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef E2E_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined E2E_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef E2E_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined E2E_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef E2E_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined E2E_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef E2E_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined E2E_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef E2E_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined E2E_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef E2E_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined E2E_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef E2E_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined E2E_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef E2E_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined E2E_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef E2E_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined E2E_START_SEC_VAR_FAST_8BIT)
  #undef E2E_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined E2E_STOP_SEC_VAR_FAST_8BIT)
  #undef E2E_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined E2E_START_SEC_VAR_FAST_16BIT)
  #undef E2E_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined E2E_STOP_SEC_VAR_FAST_16BIT)
  #undef E2E_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined E2E_START_SEC_VAR_FAST_32BIT)
  #undef E2E_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined E2E_STOP_SEC_VAR_FAST_32BIT)
  #undef E2E_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined E2E_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef E2E_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined E2E_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef E2E_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined E2E_START_SEC_VAR_8BIT)
  #undef E2E_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined E2E_STOP_SEC_VAR_8BIT)
  #undef E2E_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined E2E_START_SEC_VAR_16BIT)
  #undef E2E_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined E2E_STOP_SEC_VAR_16BIT)
  #undef E2E_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined E2E_START_SEC_VAR_32BIT)
  #undef E2E_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined E2E_STOP_SEC_VAR_32BIT)
  #undef E2E_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined E2E_START_SEC_VAR_UNSPECIFIED)
  #undef E2E_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined E2E_STOP_SEC_VAR_UNSPECIFIED)
  #undef E2E_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined E2E_START_SEC_CONST_8BIT)
  #undef E2E_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined E2E_STOP_SEC_CONST_8BIT)
  #undef E2E_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined E2E_START_SEC_CONST_16BIT)
  #undef E2E_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined E2E_STOP_SEC_CONST_16BIT)
  #undef E2E_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined E2E_START_SEC_CONST_32BIT)
  #undef E2E_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined E2E_STOP_SEC_CONST_32BIT)
  #undef E2E_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined E2E_START_SEC_CONST_UNSPECIFIED)
  #undef E2E_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined E2E_STOP_SEC_CONST_UNSPECIFIED)
  #undef E2E_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined E2E_START_CONFIG_DATA_8BIT)
  #undef E2E_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined E2E_STOP_CONFIG_DATA_8BIT)
  #undef E2E_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined E2E_START_CONFIG_DATA_16BIT)
  #undef E2E_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined E2E_STOP_CONFIG_DATA_16BIT)
  #undef E2E_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined E2E_START_CONFIG_DATA_32BIT)
  #undef E2E_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined E2E_STOP_CONFIG_DATA_32BIT)
  #undef E2E_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined E2E_START_CONFIG_DATA_UNSPECIFIED)
  #undef E2E_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined E2E_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef E2E_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined E2EPW_START_SEC_CODE)
  #undef E2EPW_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined E2EPW_STOP_SEC_CODE)
  #undef E2EPW_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined E2EPW_START_SEC_APPL_CODE)
  #undef E2EPW_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined E2EPW_STOP_SEC_APPL_CODE)
  #undef E2EPW_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined E2EPW_START_SEC_VAR_NOINIT_8BIT)
  #undef E2EPW_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined E2EPW_STOP_SEC_VAR_NOINIT_8BIT)
  #undef E2EPW_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined E2EPW_START_SEC_VAR_NOINIT_16BIT)
  #undef E2EPW_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined E2EPW_STOP_SEC_VAR_NOINIT_16BIT)
  #undef E2EPW_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined E2EPW_START_SEC_VAR_NOINIT_32BIT)
  #undef E2EPW_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined E2EPW_STOP_SEC_VAR_NOINIT_32BIT)
  #undef E2EPW_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined E2EPW_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef E2EPW_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined E2EPW_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef E2EPW_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined E2EPW_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef E2EPW_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined E2EPW_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef E2EPW_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined E2EPW_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef E2EPW_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined E2EPW_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef E2EPW_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined E2EPW_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef E2EPW_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined E2EPW_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef E2EPW_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined E2EPW_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef E2EPW_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined E2EPW_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef E2EPW_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined E2EPW_START_SEC_VAR_FAST_8BIT)
  #undef E2EPW_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined E2EPW_STOP_SEC_VAR_FAST_8BIT)
  #undef E2EPW_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined E2EPW_START_SEC_VAR_FAST_16BIT)
  #undef E2EPW_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined E2EPW_STOP_SEC_VAR_FAST_16BIT)
  #undef E2EPW_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined E2EPW_START_SEC_VAR_FAST_32BIT)
  #undef E2EPW_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined E2EPW_STOP_SEC_VAR_FAST_32BIT)
  #undef E2EPW_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined E2EPW_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef E2EPW_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined E2EPW_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef E2EPW_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined E2EPW_START_SEC_VAR_8BIT)
  #undef E2EPW_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined E2EPW_STOP_SEC_VAR_8BIT)
  #undef E2EPW_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined E2EPW_START_SEC_VAR_16BIT)
  #undef E2EPW_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined E2EPW_STOP_SEC_VAR_16BIT)
  #undef E2EPW_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined E2EPW_START_SEC_VAR_32BIT)
  #undef E2EPW_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined E2EPW_STOP_SEC_VAR_32BIT)
  #undef E2EPW_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined E2EPW_START_SEC_VAR_UNSPECIFIED)
  #undef E2EPW_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined E2EPW_STOP_SEC_VAR_UNSPECIFIED)
  #undef E2EPW_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined E2EPW_START_SEC_CONST_8BIT)
  #undef E2EPW_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined E2EPW_STOP_SEC_CONST_8BIT)
  #undef E2EPW_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined E2EPW_START_SEC_CONST_16BIT)
  #undef E2EPW_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined E2EPW_STOP_SEC_CONST_16BIT)
  #undef E2EPW_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined E2EPW_START_SEC_CONST_32BIT)
  #undef E2EPW_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined E2EPW_STOP_SEC_CONST_32BIT)
  #undef E2EPW_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined E2EPW_START_SEC_CONST_UNSPECIFIED)
  #undef E2EPW_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined E2EPW_STOP_SEC_CONST_UNSPECIFIED)
  #undef E2EPW_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined E2EPW_START_CONFIG_DATA_8BIT)
  #undef E2EPW_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined E2EPW_STOP_CONFIG_DATA_8BIT)
  #undef E2EPW_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined E2EPW_START_CONFIG_DATA_16BIT)
  #undef E2EPW_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined E2EPW_STOP_CONFIG_DATA_16BIT)
  #undef E2EPW_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined E2EPW_START_CONFIG_DATA_32BIT)
  #undef E2EPW_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined E2EPW_STOP_CONFIG_DATA_32BIT)
  #undef E2EPW_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined E2EPW_START_CONFIG_DATA_UNSPECIFIED)
  #undef E2EPW_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined E2EPW_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef E2EPW_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined EA_START_SEC_CODE)
  #undef EA_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined EA_STOP_SEC_CODE)
  #undef EA_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined EA_START_SEC_APPL_CODE)
  #undef EA_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined EA_STOP_SEC_APPL_CODE)
  #undef EA_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined EA_START_SEC_VAR_NOINIT_8BIT)
  #undef EA_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined EA_STOP_SEC_VAR_NOINIT_8BIT)
  #undef EA_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined EA_START_SEC_VAR_NOINIT_16BIT)
  #undef EA_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined EA_STOP_SEC_VAR_NOINIT_16BIT)
  #undef EA_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined EA_START_SEC_VAR_NOINIT_32BIT)
  #undef EA_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined EA_STOP_SEC_VAR_NOINIT_32BIT)
  #undef EA_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined EA_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef EA_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EA_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef EA_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EA_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef EA_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined EA_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef EA_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined EA_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef EA_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined EA_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef EA_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined EA_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef EA_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined EA_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef EA_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined EA_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef EA_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EA_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef EA_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EA_START_SEC_VAR_FAST_8BIT)
  #undef EA_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined EA_STOP_SEC_VAR_FAST_8BIT)
  #undef EA_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined EA_START_SEC_VAR_FAST_16BIT)
  #undef EA_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined EA_STOP_SEC_VAR_FAST_16BIT)
  #undef EA_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined EA_START_SEC_VAR_FAST_32BIT)
  #undef EA_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined EA_STOP_SEC_VAR_FAST_32BIT)
  #undef EA_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined EA_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef EA_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EA_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef EA_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EA_START_SEC_VAR_8BIT)
  #undef EA_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined EA_STOP_SEC_VAR_8BIT)
  #undef EA_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined EA_START_SEC_VAR_16BIT)
  #undef EA_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined EA_STOP_SEC_VAR_16BIT)
  #undef EA_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined EA_START_SEC_VAR_32BIT)
  #undef EA_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined EA_STOP_SEC_VAR_32BIT)
  #undef EA_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined EA_START_SEC_VAR_UNSPECIFIED)
  #undef EA_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EA_STOP_SEC_VAR_UNSPECIFIED)
  #undef EA_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EA_START_SEC_CONST_8BIT)
  #undef EA_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined EA_STOP_SEC_CONST_8BIT)
  #undef EA_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined EA_START_SEC_CONST_16BIT)
  #undef EA_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined EA_STOP_SEC_CONST_16BIT)
  #undef EA_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined EA_START_SEC_CONST_32BIT)
  #undef EA_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined EA_STOP_SEC_CONST_32BIT)
  #undef EA_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined EA_START_SEC_CONST_UNSPECIFIED)
  #undef EA_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EA_STOP_SEC_CONST_UNSPECIFIED)
  #undef EA_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EA_START_CONFIG_DATA_8BIT)
  #undef EA_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined EA_STOP_CONFIG_DATA_8BIT)
  #undef EA_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined EA_START_CONFIG_DATA_16BIT)
  #undef EA_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined EA_STOP_CONFIG_DATA_16BIT)
  #undef EA_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined EA_START_CONFIG_DATA_32BIT)
  #undef EA_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined EA_STOP_CONFIG_DATA_32BIT)
  #undef EA_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined EA_START_CONFIG_DATA_UNSPECIFIED)
  #undef EA_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EA_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef EA_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined EBSTUBS_START_SEC_CODE)
  #undef EBSTUBS_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined EBSTUBS_STOP_SEC_CODE)
  #undef EBSTUBS_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined EBSTUBS_START_SEC_APPL_CODE)
  #undef EBSTUBS_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined EBSTUBS_STOP_SEC_APPL_CODE)
  #undef EBSTUBS_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined EBSTUBS_START_SEC_VAR_NOINIT_8BIT)
  #undef EBSTUBS_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined EBSTUBS_STOP_SEC_VAR_NOINIT_8BIT)
  #undef EBSTUBS_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined EBSTUBS_START_SEC_VAR_NOINIT_16BIT)
  #undef EBSTUBS_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined EBSTUBS_STOP_SEC_VAR_NOINIT_16BIT)
  #undef EBSTUBS_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined EBSTUBS_START_SEC_VAR_NOINIT_32BIT)
  #undef EBSTUBS_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined EBSTUBS_STOP_SEC_VAR_NOINIT_32BIT)
  #undef EBSTUBS_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined EBSTUBS_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef EBSTUBS_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EBSTUBS_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef EBSTUBS_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EBSTUBS_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef EBSTUBS_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined EBSTUBS_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef EBSTUBS_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined EBSTUBS_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef EBSTUBS_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined EBSTUBS_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef EBSTUBS_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined EBSTUBS_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef EBSTUBS_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined EBSTUBS_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef EBSTUBS_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined EBSTUBS_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef EBSTUBS_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EBSTUBS_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef EBSTUBS_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EBSTUBS_START_SEC_VAR_FAST_8BIT)
  #undef EBSTUBS_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined EBSTUBS_STOP_SEC_VAR_FAST_8BIT)
  #undef EBSTUBS_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined EBSTUBS_START_SEC_VAR_FAST_16BIT)
  #undef EBSTUBS_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined EBSTUBS_STOP_SEC_VAR_FAST_16BIT)
  #undef EBSTUBS_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined EBSTUBS_START_SEC_VAR_FAST_32BIT)
  #undef EBSTUBS_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined EBSTUBS_STOP_SEC_VAR_FAST_32BIT)
  #undef EBSTUBS_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined EBSTUBS_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef EBSTUBS_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EBSTUBS_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef EBSTUBS_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EBSTUBS_START_SEC_VAR_8BIT)
  #undef EBSTUBS_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined EBSTUBS_STOP_SEC_VAR_8BIT)
  #undef EBSTUBS_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined EBSTUBS_START_SEC_VAR_16BIT)
  #undef EBSTUBS_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined EBSTUBS_STOP_SEC_VAR_16BIT)
  #undef EBSTUBS_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined EBSTUBS_START_SEC_VAR_32BIT)
  #undef EBSTUBS_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined EBSTUBS_STOP_SEC_VAR_32BIT)
  #undef EBSTUBS_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined EBSTUBS_START_SEC_VAR_UNSPECIFIED)
  #undef EBSTUBS_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EBSTUBS_STOP_SEC_VAR_UNSPECIFIED)
  #undef EBSTUBS_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EBSTUBS_START_SEC_CONST_8BIT)
  #undef EBSTUBS_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined EBSTUBS_STOP_SEC_CONST_8BIT)
  #undef EBSTUBS_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined EBSTUBS_START_SEC_CONST_16BIT)
  #undef EBSTUBS_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined EBSTUBS_STOP_SEC_CONST_16BIT)
  #undef EBSTUBS_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined EBSTUBS_START_SEC_CONST_32BIT)
  #undef EBSTUBS_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined EBSTUBS_STOP_SEC_CONST_32BIT)
  #undef EBSTUBS_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined EBSTUBS_START_SEC_CONST_UNSPECIFIED)
  #undef EBSTUBS_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EBSTUBS_STOP_SEC_CONST_UNSPECIFIED)
  #undef EBSTUBS_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EBSTUBS_START_CONFIG_DATA_8BIT)
  #undef EBSTUBS_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined EBSTUBS_STOP_CONFIG_DATA_8BIT)
  #undef EBSTUBS_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined EBSTUBS_START_CONFIG_DATA_16BIT)
  #undef EBSTUBS_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined EBSTUBS_STOP_CONFIG_DATA_16BIT)
  #undef EBSTUBS_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined EBSTUBS_START_CONFIG_DATA_32BIT)
  #undef EBSTUBS_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined EBSTUBS_STOP_CONFIG_DATA_32BIT)
  #undef EBSTUBS_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined EBSTUBS_START_CONFIG_DATA_UNSPECIFIED)
  #undef EBSTUBS_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EBSTUBS_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef EBSTUBS_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined EBTEST_START_SEC_CODE)
  #undef EBTEST_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined EBTEST_STOP_SEC_CODE)
  #undef EBTEST_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined EBTEST_START_SEC_APPL_CODE)
  #undef EBTEST_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined EBTEST_STOP_SEC_APPL_CODE)
  #undef EBTEST_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined EBTEST_START_SEC_VAR_NOINIT_8BIT)
  #undef EBTEST_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined EBTEST_STOP_SEC_VAR_NOINIT_8BIT)
  #undef EBTEST_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined EBTEST_START_SEC_VAR_NO_INIT_BOOLEAN)
  #undef EBTEST_START_SEC_VAR_NO_INIT_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined EBTEST_STOP_SEC_VAR_NO_INIT_BOOLEAN)
  #undef EBTEST_STOP_SEC_VAR_NO_INIT_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined EBTEST_START_SEC_VAR_NO_INIT_8)
  #undef EBTEST_START_SEC_VAR_NO_INIT_8
  #undef MEMMAP_ERROR

#elif (defined EBTEST_STOP_SEC_VAR_NO_INIT_8)
  #undef EBTEST_STOP_SEC_VAR_NO_INIT_8
  #undef MEMMAP_ERROR

#elif (defined EBTEST_START_SEC_VAR_NOINIT_16BIT)
  #undef EBTEST_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined EBTEST_STOP_SEC_VAR_NOINIT_16BIT)
  #undef EBTEST_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined EBTEST_START_SEC_VAR_NO_INIT_16)
  #undef EBTEST_START_SEC_VAR_NO_INIT_16
  #undef MEMMAP_ERROR

#elif (defined EBTEST_STOP_SEC_VAR_NO_INIT_16)
  #undef EBTEST_STOP_SEC_VAR_NO_INIT_16
  #undef MEMMAP_ERROR

#elif (defined EBTEST_START_SEC_VAR_NOINIT_32BIT)
  #undef EBTEST_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined EBTEST_STOP_SEC_VAR_NOINIT_32BIT)
  #undef EBTEST_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined EBTEST_START_SEC_VAR_NO_INIT_32)
  #undef EBTEST_START_SEC_VAR_NO_INIT_32
  #undef MEMMAP_ERROR

#elif (defined EBTEST_STOP_SEC_VAR_NO_INIT_32)
  #undef EBTEST_STOP_SEC_VAR_NO_INIT_32
  #undef MEMMAP_ERROR

#elif (defined EBTEST_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef EBTEST_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EBTEST_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef EBTEST_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EBTEST_START_SEC_VAR_NO_INIT_UNSPECIFIED)
  #undef EBTEST_START_SEC_VAR_NO_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EBTEST_STOP_SEC_VAR_NO_INIT_UNSPECIFIED)
  #undef EBTEST_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EBTEST_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef EBTEST_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined EBTEST_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef EBTEST_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined EBTEST_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef EBTEST_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined EBTEST_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef EBTEST_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined EBTEST_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef EBTEST_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined EBTEST_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef EBTEST_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined EBTEST_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef EBTEST_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EBTEST_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef EBTEST_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EBTEST_START_SEC_VAR_INIT_8)
  #undef EBTEST_START_SEC_VAR_INIT_8
  #undef MEMMAP_ERROR

#elif (defined EBTEST_STOP_SEC_VAR_INIT_8)
  #undef EBTEST_STOP_SEC_VAR_INIT_8
  #undef MEMMAP_ERROR

#elif (defined EBTEST_START_SEC_VAR_INIT_16)
  #undef EBTEST_START_SEC_VAR_INIT_16
  #undef MEMMAP_ERROR

#elif (defined EBTEST_STOP_SEC_VAR_INIT_16)
  #undef EBTEST_STOP_SEC_VAR_INIT_16
  #undef MEMMAP_ERROR

#elif (defined EBTEST_START_SEC_VAR_INIT_32)
  #undef EBTEST_START_SEC_VAR_INIT_32
  #undef MEMMAP_ERROR

#elif (defined EBTEST_STOP_SEC_VAR_INIT_32)
  #undef EBTEST_STOP_SEC_VAR_INIT_32
  #undef MEMMAP_ERROR

#elif (defined EBTEST_START_SEC_VAR_FAST_8BIT)
  #undef EBTEST_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined EBTEST_STOP_SEC_VAR_FAST_8BIT)
  #undef EBTEST_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined EBTEST_START_SEC_VAR_FAST_16BIT)
  #undef EBTEST_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined EBTEST_STOP_SEC_VAR_FAST_16BIT)
  #undef EBTEST_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined EBTEST_START_SEC_VAR_FAST_32BIT)
  #undef EBTEST_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined EBTEST_STOP_SEC_VAR_FAST_32BIT)
  #undef EBTEST_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined EBTEST_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef EBTEST_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EBTEST_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef EBTEST_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EBTEST_START_SEC_VAR_8BIT)
  #undef EBTEST_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined EBTEST_STOP_SEC_VAR_8BIT)
  #undef EBTEST_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined EBTEST_START_SEC_VAR_16BIT)
  #undef EBTEST_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined EBTEST_STOP_SEC_VAR_16BIT)
  #undef EBTEST_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined EBTEST_START_SEC_VAR_32BIT)
  #undef EBTEST_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined EBTEST_STOP_SEC_VAR_32BIT)
  #undef EBTEST_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined EBTEST_START_SEC_VAR_UNSPECIFIED)
  #undef EBTEST_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EBTEST_STOP_SEC_VAR_UNSPECIFIED)
  #undef EBTEST_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EBTEST_START_SEC_CONST_8BIT)
  #undef EBTEST_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined EBTEST_STOP_SEC_CONST_8BIT)
  #undef EBTEST_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined EBTEST_START_SEC_CONST_8)
  #undef EBTEST_START_SEC_CONST_8
  #undef MEMMAP_ERROR

#elif (defined EBTEST_STOP_SEC_CONST_8)
  #undef EBTEST_STOP_SEC_CONST_8
  #undef MEMMAP_ERROR

#elif (defined EBTEST_START_SEC_CONST_16BIT)
  #undef EBTEST_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined EBTEST_STOP_SEC_CONST_16BIT)
  #undef EBTEST_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined EBTEST_START_SEC_CONST_16)
  #undef EBTEST_START_SEC_CONST_16
  #undef MEMMAP_ERROR

#elif (defined EBTEST_STOP_SEC_CONST_16)
  #undef EBTEST_STOP_SEC_CONST_16
  #undef MEMMAP_ERROR

#elif (defined EBTEST_START_SEC_CONST_32BIT)
  #undef EBTEST_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined EBTEST_STOP_SEC_CONST_32BIT)
  #undef EBTEST_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined EBTEST_START_SEC_CONST_32)
  #undef EBTEST_START_SEC_CONST_32
  #undef MEMMAP_ERROR

#elif (defined EBTEST_STOP_SEC_CONST_32)
  #undef EBTEST_STOP_SEC_CONST_32
  #undef MEMMAP_ERROR

#elif (defined EBTEST_START_SEC_CONST_UNSPECIFIED)
  #undef EBTEST_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EBTEST_STOP_SEC_CONST_UNSPECIFIED)
  #undef EBTEST_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EBTEST_START_CONFIG_DATA_8BIT)
  #undef EBTEST_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined EBTEST_STOP_CONFIG_DATA_8BIT)
  #undef EBTEST_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined EBTEST_START_CONFIG_DATA_16BIT)
  #undef EBTEST_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined EBTEST_STOP_CONFIG_DATA_16BIT)
  #undef EBTEST_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined EBTEST_START_CONFIG_DATA_32BIT)
  #undef EBTEST_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined EBTEST_STOP_CONFIG_DATA_32BIT)
  #undef EBTEST_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined EBTEST_START_CONFIG_DATA_UNSPECIFIED)
  #undef EBTEST_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EBTEST_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef EBTEST_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined ECUM_START_SEC_CODE)
  #undef ECUM_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined ECUM_STOP_SEC_CODE)
  #undef ECUM_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined ECUM_START_SEC_APPL_CODE)
  #undef ECUM_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined ECUM_STOP_SEC_APPL_CODE)
  #undef ECUM_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined ECUM_START_SEC_VAR_NOINIT_8BIT)
  #undef ECUM_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined ECUM_STOP_SEC_VAR_NOINIT_8BIT)
  #undef ECUM_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined ECUM_START_SEC_VAR_NOINIT_16BIT)
  #undef ECUM_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined ECUM_STOP_SEC_VAR_NOINIT_16BIT)
  #undef ECUM_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined ECUM_START_SEC_VAR_NOINIT_32BIT)
  #undef ECUM_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined ECUM_STOP_SEC_VAR_NOINIT_32BIT)
  #undef ECUM_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined ECUM_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef ECUM_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined ECUM_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef ECUM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined ECUM_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef ECUM_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined ECUM_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef ECUM_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined ECUM_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef ECUM_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined ECUM_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef ECUM_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined ECUM_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef ECUM_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined ECUM_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef ECUM_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined ECUM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef ECUM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined ECUM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef ECUM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined ECUM_START_SEC_VAR_FAST_8BIT)
  #undef ECUM_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined ECUM_STOP_SEC_VAR_FAST_8BIT)
  #undef ECUM_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined ECUM_START_SEC_VAR_FAST_16BIT)
  #undef ECUM_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined ECUM_STOP_SEC_VAR_FAST_16BIT)
  #undef ECUM_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined ECUM_START_SEC_VAR_FAST_32BIT)
  #undef ECUM_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined ECUM_STOP_SEC_VAR_FAST_32BIT)
  #undef ECUM_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined ECUM_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef ECUM_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined ECUM_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef ECUM_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined ECUM_START_SEC_VAR_8BIT)
  #undef ECUM_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined ECUM_STOP_SEC_VAR_8BIT)
  #undef ECUM_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined ECUM_START_SEC_VAR_16BIT)
  #undef ECUM_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined ECUM_STOP_SEC_VAR_16BIT)
  #undef ECUM_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined ECUM_START_SEC_VAR_32BIT)
  #undef ECUM_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined ECUM_STOP_SEC_VAR_32BIT)
  #undef ECUM_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined ECUM_START_SEC_VAR_UNSPECIFIED)
  #undef ECUM_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined ECUM_STOP_SEC_VAR_UNSPECIFIED)
  #undef ECUM_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined ECUM_START_SEC_CONST_8BIT)
  #undef ECUM_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined ECUM_STOP_SEC_CONST_8BIT)
  #undef ECUM_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined ECUM_START_SEC_CONST_16BIT)
  #undef ECUM_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined ECUM_STOP_SEC_CONST_16BIT)
  #undef ECUM_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined ECUM_START_SEC_CONST_32BIT)
  #undef ECUM_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined ECUM_STOP_SEC_CONST_32BIT)
  #undef ECUM_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined ECUM_START_SEC_CONST_UNSPECIFIED)
  #undef ECUM_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined ECUM_STOP_SEC_CONST_UNSPECIFIED)
  #undef ECUM_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined ECUM_START_CONFIG_DATA_8BIT)
  #undef ECUM_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined ECUM_STOP_CONFIG_DATA_8BIT)
  #undef ECUM_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined ECUM_START_CONFIG_DATA_16BIT)
  #undef ECUM_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined ECUM_STOP_CONFIG_DATA_16BIT)
  #undef ECUM_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined ECUM_START_CONFIG_DATA_32BIT)
  #undef ECUM_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined ECUM_STOP_CONFIG_DATA_32BIT)
  #undef ECUM_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined ECUM_START_CONFIG_DATA_UNSPECIFIED)
  #undef ECUM_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined ECUM_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef ECUM_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined EEP_START_SEC_CODE)
  #undef EEP_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined EEP_STOP_SEC_CODE)
  #undef EEP_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined EEP_START_SEC_APPL_CODE)
  #undef EEP_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined EEP_STOP_SEC_APPL_CODE)
  #undef EEP_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined EEP_START_SEC_VAR_NOINIT_8BIT)
  #undef EEP_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined EEP_STOP_SEC_VAR_NOINIT_8BIT)
  #undef EEP_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined EEP_START_SEC_VAR_NOINIT_16BIT)
  #undef EEP_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined EEP_STOP_SEC_VAR_NOINIT_16BIT)
  #undef EEP_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined EEP_START_SEC_VAR_NOINIT_32BIT)
  #undef EEP_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined EEP_STOP_SEC_VAR_NOINIT_32BIT)
  #undef EEP_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined EEP_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef EEP_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EEP_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef EEP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EEP_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef EEP_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined EEP_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef EEP_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined EEP_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef EEP_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined EEP_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef EEP_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined EEP_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef EEP_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined EEP_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef EEP_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined EEP_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef EEP_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EEP_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef EEP_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EEP_START_SEC_VAR_FAST_8BIT)
  #undef EEP_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined EEP_STOP_SEC_VAR_FAST_8BIT)
  #undef EEP_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined EEP_START_SEC_VAR_FAST_16BIT)
  #undef EEP_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined EEP_STOP_SEC_VAR_FAST_16BIT)
  #undef EEP_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined EEP_START_SEC_VAR_FAST_32BIT)
  #undef EEP_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined EEP_STOP_SEC_VAR_FAST_32BIT)
  #undef EEP_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined EEP_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef EEP_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EEP_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef EEP_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EEP_START_SEC_VAR_8BIT)
  #undef EEP_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined EEP_STOP_SEC_VAR_8BIT)
  #undef EEP_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined EEP_START_SEC_VAR_16BIT)
  #undef EEP_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined EEP_STOP_SEC_VAR_16BIT)
  #undef EEP_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined EEP_START_SEC_VAR_32BIT)
  #undef EEP_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined EEP_STOP_SEC_VAR_32BIT)
  #undef EEP_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined EEP_START_SEC_VAR_UNSPECIFIED)
  #undef EEP_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EEP_STOP_SEC_VAR_UNSPECIFIED)
  #undef EEP_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EEP_START_SEC_CONST_8BIT)
  #undef EEP_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined EEP_STOP_SEC_CONST_8BIT)
  #undef EEP_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined EEP_START_SEC_CONST_16BIT)
  #undef EEP_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined EEP_STOP_SEC_CONST_16BIT)
  #undef EEP_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined EEP_START_SEC_CONST_32BIT)
  #undef EEP_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined EEP_STOP_SEC_CONST_32BIT)
  #undef EEP_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined EEP_START_SEC_CONST_UNSPECIFIED)
  #undef EEP_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EEP_STOP_SEC_CONST_UNSPECIFIED)
  #undef EEP_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EEP_START_CONFIG_DATA_8BIT)
  #undef EEP_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined EEP_STOP_CONFIG_DATA_8BIT)
  #undef EEP_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined EEP_START_CONFIG_DATA_16BIT)
  #undef EEP_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined EEP_STOP_CONFIG_DATA_16BIT)
  #undef EEP_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined EEP_START_CONFIG_DATA_32BIT)
  #undef EEP_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined EEP_STOP_CONFIG_DATA_32BIT)
  #undef EEP_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined EEP_START_CONFIG_DATA_UNSPECIFIED)
  #undef EEP_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EEP_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef EEP_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined EEPIF_START_SEC_CODE)
  #undef EEPIF_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined EEPIF_STOP_SEC_CODE)
  #undef EEPIF_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined EEPIF_START_SEC_APPL_CODE)
  #undef EEPIF_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined EEPIF_STOP_SEC_APPL_CODE)
  #undef EEPIF_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined EEPIF_START_SEC_VAR_NOINIT_8BIT)
  #undef EEPIF_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined EEPIF_STOP_SEC_VAR_NOINIT_8BIT)
  #undef EEPIF_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined EEPIF_START_SEC_VAR_NOINIT_16BIT)
  #undef EEPIF_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined EEPIF_STOP_SEC_VAR_NOINIT_16BIT)
  #undef EEPIF_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined EEPIF_START_SEC_VAR_NOINIT_32BIT)
  #undef EEPIF_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined EEPIF_STOP_SEC_VAR_NOINIT_32BIT)
  #undef EEPIF_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined EEPIF_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef EEPIF_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EEPIF_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef EEPIF_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EEPIF_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef EEPIF_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined EEPIF_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef EEPIF_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined EEPIF_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef EEPIF_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined EEPIF_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef EEPIF_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined EEPIF_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef EEPIF_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined EEPIF_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef EEPIF_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined EEPIF_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef EEPIF_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EEPIF_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef EEPIF_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EEPIF_START_SEC_VAR_FAST_8BIT)
  #undef EEPIF_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined EEPIF_STOP_SEC_VAR_FAST_8BIT)
  #undef EEPIF_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined EEPIF_START_SEC_VAR_FAST_16BIT)
  #undef EEPIF_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined EEPIF_STOP_SEC_VAR_FAST_16BIT)
  #undef EEPIF_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined EEPIF_START_SEC_VAR_FAST_32BIT)
  #undef EEPIF_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined EEPIF_STOP_SEC_VAR_FAST_32BIT)
  #undef EEPIF_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined EEPIF_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef EEPIF_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EEPIF_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef EEPIF_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EEPIF_START_SEC_VAR_8BIT)
  #undef EEPIF_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined EEPIF_STOP_SEC_VAR_8BIT)
  #undef EEPIF_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined EEPIF_START_SEC_VAR_16BIT)
  #undef EEPIF_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined EEPIF_STOP_SEC_VAR_16BIT)
  #undef EEPIF_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined EEPIF_START_SEC_VAR_32BIT)
  #undef EEPIF_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined EEPIF_STOP_SEC_VAR_32BIT)
  #undef EEPIF_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined EEPIF_START_SEC_VAR_UNSPECIFIED)
  #undef EEPIF_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EEPIF_STOP_SEC_VAR_UNSPECIFIED)
  #undef EEPIF_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EEPIF_START_SEC_CONST_8BIT)
  #undef EEPIF_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined EEPIF_STOP_SEC_CONST_8BIT)
  #undef EEPIF_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined EEPIF_START_SEC_CONST_16BIT)
  #undef EEPIF_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined EEPIF_STOP_SEC_CONST_16BIT)
  #undef EEPIF_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined EEPIF_START_SEC_CONST_32BIT)
  #undef EEPIF_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined EEPIF_STOP_SEC_CONST_32BIT)
  #undef EEPIF_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined EEPIF_START_SEC_CONST_UNSPECIFIED)
  #undef EEPIF_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EEPIF_STOP_SEC_CONST_UNSPECIFIED)
  #undef EEPIF_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EEPIF_START_CONFIG_DATA_8BIT)
  #undef EEPIF_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined EEPIF_STOP_CONFIG_DATA_8BIT)
  #undef EEPIF_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined EEPIF_START_CONFIG_DATA_16BIT)
  #undef EEPIF_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined EEPIF_STOP_CONFIG_DATA_16BIT)
  #undef EEPIF_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined EEPIF_START_CONFIG_DATA_32BIT)
  #undef EEPIF_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined EEPIF_STOP_CONFIG_DATA_32BIT)
  #undef EEPIF_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined EEPIF_START_CONFIG_DATA_UNSPECIFIED)
  #undef EEPIF_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined EEPIF_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef EEPIF_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined FEE_START_SEC_CODE)
  #undef FEE_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined FEE_STOP_SEC_CODE)
  #undef FEE_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined FEE_START_SEC_APPL_CODE)
  #undef FEE_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined FEE_STOP_SEC_APPL_CODE)
  #undef FEE_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined FEE_START_SEC_VAR_NOINIT_8BIT)
  #undef FEE_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FEE_STOP_SEC_VAR_NOINIT_8BIT)
  #undef FEE_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FEE_START_SEC_VAR_NOINIT_16BIT)
  #undef FEE_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FEE_STOP_SEC_VAR_NOINIT_16BIT)
  #undef FEE_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FEE_START_SEC_VAR_NOINIT_32BIT)
  #undef FEE_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FEE_STOP_SEC_VAR_NOINIT_32BIT)
  #undef FEE_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FEE_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef FEE_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FEE_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef FEE_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FEE_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef FEE_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FEE_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef FEE_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FEE_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef FEE_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FEE_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef FEE_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FEE_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef FEE_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FEE_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef FEE_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FEE_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef FEE_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FEE_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef FEE_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FEE_START_SEC_VAR_FAST_8BIT)
  #undef FEE_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FEE_STOP_SEC_VAR_FAST_8BIT)
  #undef FEE_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FEE_START_SEC_VAR_FAST_16BIT)
  #undef FEE_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FEE_STOP_SEC_VAR_FAST_16BIT)
  #undef FEE_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FEE_START_SEC_VAR_FAST_32BIT)
  #undef FEE_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FEE_STOP_SEC_VAR_FAST_32BIT)
  #undef FEE_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FEE_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef FEE_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FEE_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef FEE_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FEE_START_SEC_VAR_8BIT)
  #undef FEE_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined FEE_STOP_SEC_VAR_8BIT)
  #undef FEE_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined FEE_START_SEC_VAR_16BIT)
  #undef FEE_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined FEE_STOP_SEC_VAR_16BIT)
  #undef FEE_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined FEE_START_SEC_VAR_32BIT)
  #undef FEE_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined FEE_STOP_SEC_VAR_32BIT)
  #undef FEE_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined FEE_START_SEC_VAR_UNSPECIFIED)
  #undef FEE_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FEE_STOP_SEC_VAR_UNSPECIFIED)
  #undef FEE_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FEE_START_SEC_CONST_8BIT)
  #undef FEE_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FEE_STOP_SEC_CONST_8BIT)
  #undef FEE_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FEE_START_SEC_CONST_16BIT)
  #undef FEE_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FEE_STOP_SEC_CONST_16BIT)
  #undef FEE_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FEE_START_SEC_CONST_32BIT)
  #undef FEE_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FEE_STOP_SEC_CONST_32BIT)
  #undef FEE_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FEE_START_SEC_CONST_UNSPECIFIED)
  #undef FEE_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FEE_STOP_SEC_CONST_UNSPECIFIED)
  #undef FEE_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FEE_START_CONFIG_DATA_8BIT)
  #undef FEE_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined FEE_STOP_CONFIG_DATA_8BIT)
  #undef FEE_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined FEE_START_CONFIG_DATA_16BIT)
  #undef FEE_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined FEE_STOP_CONFIG_DATA_16BIT)
  #undef FEE_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined FEE_START_CONFIG_DATA_32BIT)
  #undef FEE_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined FEE_STOP_CONFIG_DATA_32BIT)
  #undef FEE_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined FEE_START_CONFIG_DATA_UNSPECIFIED)
  #undef FEE_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FEE_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef FEE_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined FIM_START_SEC_CODE)
  #undef FIM_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined FIM_STOP_SEC_CODE)
  #undef FIM_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined FIM_START_SEC_APPL_CODE)
  #undef FIM_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined FIM_STOP_SEC_APPL_CODE)
  #undef FIM_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined FIM_START_SEC_VAR_NOINIT_8BIT)
  #undef FIM_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FIM_STOP_SEC_VAR_NOINIT_8BIT)
  #undef FIM_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FIM_START_SEC_VAR_NOINIT_16BIT)
  #undef FIM_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FIM_STOP_SEC_VAR_NOINIT_16BIT)
  #undef FIM_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FIM_START_SEC_VAR_NOINIT_32BIT)
  #undef FIM_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FIM_STOP_SEC_VAR_NOINIT_32BIT)
  #undef FIM_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FIM_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef FIM_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FIM_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef FIM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FIM_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef FIM_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FIM_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef FIM_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FIM_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef FIM_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FIM_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef FIM_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FIM_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef FIM_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FIM_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef FIM_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FIM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef FIM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FIM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef FIM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FIM_START_SEC_VAR_FAST_8BIT)
  #undef FIM_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FIM_STOP_SEC_VAR_FAST_8BIT)
  #undef FIM_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FIM_START_SEC_VAR_FAST_16BIT)
  #undef FIM_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FIM_STOP_SEC_VAR_FAST_16BIT)
  #undef FIM_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FIM_START_SEC_VAR_FAST_32BIT)
  #undef FIM_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FIM_STOP_SEC_VAR_FAST_32BIT)
  #undef FIM_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FIM_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef FIM_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FIM_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef FIM_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FIM_START_SEC_VAR_8BIT)
  #undef FIM_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined FIM_STOP_SEC_VAR_8BIT)
  #undef FIM_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined FIM_START_SEC_VAR_16BIT)
  #undef FIM_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined FIM_STOP_SEC_VAR_16BIT)
  #undef FIM_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined FIM_START_SEC_VAR_32BIT)
  #undef FIM_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined FIM_STOP_SEC_VAR_32BIT)
  #undef FIM_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined FIM_START_SEC_VAR_UNSPECIFIED)
  #undef FIM_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FIM_STOP_SEC_VAR_UNSPECIFIED)
  #undef FIM_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FIM_START_SEC_CONST_8BIT)
  #undef FIM_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FIM_STOP_SEC_CONST_8BIT)
  #undef FIM_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FIM_START_SEC_CONST_16BIT)
  #undef FIM_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FIM_STOP_SEC_CONST_16BIT)
  #undef FIM_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FIM_START_SEC_CONST_32BIT)
  #undef FIM_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FIM_STOP_SEC_CONST_32BIT)
  #undef FIM_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FIM_START_SEC_CONST_UNSPECIFIED)
  #undef FIM_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FIM_STOP_SEC_CONST_UNSPECIFIED)
  #undef FIM_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FIM_START_CONFIG_DATA_8BIT)
  #undef FIM_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined FIM_STOP_CONFIG_DATA_8BIT)
  #undef FIM_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined FIM_START_CONFIG_DATA_16BIT)
  #undef FIM_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined FIM_STOP_CONFIG_DATA_16BIT)
  #undef FIM_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined FIM_START_CONFIG_DATA_32BIT)
  #undef FIM_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined FIM_STOP_CONFIG_DATA_32BIT)
  #undef FIM_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined FIM_START_CONFIG_DATA_UNSPECIFIED)
  #undef FIM_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FIM_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef FIM_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined FLS_START_SEC_CODE)
  #undef FLS_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined FLS_STOP_SEC_CODE)
  #undef FLS_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined FLS_START_SEC_PUBLIC_CODE)
  #undef FLS_START_SEC_PUBLIC_CODE
  #undef MEMMAP_ERROR

#elif (defined FLS_STOP_SEC_PUBLIC_CODE)
  #undef FLS_STOP_SEC_PUBLIC_CODE
  #undef MEMMAP_ERROR

#elif (defined FLS_START_SEC_PRIVATE_CODE)
  #undef FLS_START_SEC_PRIVATE_CODE
  #undef MEMMAP_ERROR

#elif (defined FLS_STOP_SEC_PRIVATE_CODE)
  #undef FLS_STOP_SEC_PRIVATE_CODE
  #undef MEMMAP_ERROR

#elif (defined FLS_START_SEC_CODE_FAST)
  #undef FLS_START_SEC_CODE_FAST
  #undef MEMMAP_ERROR

#elif (defined FLS_STOP_SEC_CODE_FAST)
  #undef FLS_STOP_SEC_CODE_FAST
  #undef MEMMAP_ERROR

#elif (defined FLS_START_SEC_BUFFER_CODE)
  #undef FLS_START_SEC_BUFFER_CODE
  #undef MEMMAP_ERROR

#elif (defined FLS_STOP_SEC_BUFFER_CODE)
  #undef FLS_STOP_SEC_BUFFER_CODE
  #undef MEMMAP_ERROR

#elif (defined FLS_START_SEC_APPL_CODE)
  #undef FLS_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined FLS_STOP_SEC_APPL_CODE)
  #undef FLS_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined FLS_START_SEC_VAR_NOINIT_8BIT)
  #undef FLS_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FLS_STOP_SEC_VAR_NOINIT_8BIT)
  #undef FLS_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FLS_START_SEC_VAR_NOINIT_16BIT)
  #undef FLS_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FLS_STOP_SEC_VAR_NOINIT_16BIT)
  #undef FLS_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FLS_START_SEC_VAR_NOINIT_32BIT)
  #undef FLS_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FLS_STOP_SEC_VAR_NOINIT_32BIT)
  #undef FLS_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FLS_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef FLS_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FLS_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef FLS_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FLS_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef FLS_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FLS_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef FLS_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FLS_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef FLS_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FLS_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef FLS_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FLS_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef FLS_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FLS_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef FLS_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FLS_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef FLS_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FLS_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef FLS_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FLS_START_SEC_VAR_FAST_8BIT)
  #undef FLS_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FLS_STOP_SEC_VAR_FAST_8BIT)
  #undef FLS_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FLS_START_SEC_VAR_FAST_16BIT)
  #undef FLS_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FLS_STOP_SEC_VAR_FAST_16BIT)
  #undef FLS_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FLS_START_SEC_VAR_FAST_32BIT)
  #undef FLS_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FLS_STOP_SEC_VAR_FAST_32BIT)
  #undef FLS_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FLS_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef FLS_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FLS_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef FLS_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FLS_START_SEC_VAR_8BIT)
  #undef FLS_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined FLS_STOP_SEC_VAR_8BIT)
  #undef FLS_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined FLS_START_SEC_VAR_16BIT)
  #undef FLS_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined FLS_STOP_SEC_VAR_16BIT)
  #undef FLS_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined FLS_START_SEC_VAR_32BIT)
  #undef FLS_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined FLS_STOP_SEC_VAR_32BIT)
  #undef FLS_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined FLS_START_SEC_VAR_UNSPECIFIED)
  #undef FLS_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FLS_STOP_SEC_VAR_UNSPECIFIED)
  #undef FLS_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FLS_START_SEC_CONST_8BIT)
  #undef FLS_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FLS_STOP_SEC_CONST_8BIT)
  #undef FLS_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FLS_START_SEC_CONST_16BIT)
  #undef FLS_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FLS_STOP_SEC_CONST_16BIT)
  #undef FLS_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif ((defined FLS_START_SEC_CONST_32BIT) || (defined FLS_START_SEC_CONST_32))
  #undef FLS_START_SEC_CONST_32BIT
  #undef FLS_START_SEC_CONST_32
  #undef MEMMAP_ERROR

#elif ((defined FLS_STOP_SEC_CONST_32BIT) || (defined FLS_STOP_SEC_CONST_32))
  #undef FLS_STOP_SEC_CONST_32BIT
  #undef FLS_STOP_SEC_CONST_32
  #undef MEMMAP_ERROR

#elif (defined FLS_START_SEC_CONST_UNSPECIFIED)
  #undef FLS_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FLS_STOP_SEC_CONST_UNSPECIFIED)
  #undef FLS_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif ((defined FLS_START_CONFIG_DATA_8BIT) || (defined FLS_START_SEC_CONFIG_DATA_8))
  #undef FLS_START_CONFIG_DATA_8BIT
  #undef FLS_START_SEC_CONFIG_DATA_8
  #undef MEMMAP_ERROR

#elif ((defined FLS_STOP_CONFIG_DATA_8BIT) || (defined FLS_STOP_SEC_CONFIG_DATA_8))
  #undef FLS_STOP_CONFIG_DATA_8BIT
  #undef FLS_STOP_SEC_CONFIG_DATA_8
  #undef MEMMAP_ERROR

#elif ((defined FLS_START_CONFIG_DATA_16BIT) || (defined FLS_START_SEC_CONFIG_DATA_16))
  #undef FLS_START_CONFIG_DATA_16BIT
  #undef FLS_START_SEC_CONFIG_DATA_16
  #undef MEMMAP_ERROR

#elif ((defined FLS_STOP_CONFIG_DATA_16BIT) || (defined FLS_STOP_SEC_CONFIG_DATA_16))
  #undef FLS_STOP_CONFIG_DATA_16BIT
  #undef FLS_STOP_SEC_CONFIG_DATA_16
  #undef MEMMAP_ERROR

#elif ((defined FLS_START_CONFIG_DATA_32BIT) || (defined FLS_START_SEC_CONFIG_DATA_32))
  #undef FLS_START_CONFIG_DATA_32BIT
  #undef FLS_START_SEC_CONFIG_DATA_32
  #undef MEMMAP_ERROR

#elif ((defined FLS_STOP_CONFIG_DATA_32BIT) || (defined FLS_STOP_SEC_CONFIG_DATA_32))
  #undef FLS_STOP_CONFIG_DATA_32BIT
  #undef FLS_STOP_SEC_CONFIG_DATA_32
  #undef MEMMAP_ERROR

#elif ((defined FLS_START_CONFIG_DATA_UNSPECIFIED) || (defined FLS_START_SEC_CONFIG_DATA_UNSPECIFIED))
  #undef FLS_START_CONFIG_DATA_UNSPECIFIED
  #undef FLS_START_SEC_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif ((defined FLS_STOP_CONFIG_DATA_UNSPECIFIED) || (defined FLS_STOP_SEC_CONFIG_DATA_UNSPECIFIED))
  #undef FLS_STOP_CONFIG_DATA_UNSPECIFIED
  #undef FLS_STOP_SEC_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FLS_START_SEC_CODE_AC)
  #undef FLS_START_SEC_CODE_AC
  #undef MEMMAP_ERROR
  /* use code relative addressing mode to ensure Position-independent Code */
  #pragma ghs section text=".acfls_code"
#elif (defined FLS_STOP_SEC_CODE_AC)
  #undef FLS_STOP_SEC_CODE_AC
  #undef MEMMAP_ERROR
  #pragma ghs section text=default
#elif (defined FLS_START_SEC_VAR_INIT_UNSPECIFIED)
  #undef FLS_START_SEC_VAR_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FLS_STOP_SEC_VAR_INIT_UNSPECIFIED)
  #undef FLS_STOP_SEC_VAR_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FLS_START_SEC_DBTOC_DATA_UNSPECIFIED)
  #undef FLS_START_SEC_DBTOC_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FLS_STOP_SEC_DBTOC_DATA_UNSPECIFIED)
  #undef FLS_STOP_SEC_DBTOC_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR
#elif (defined FLS_START_SEC_VAR_INIT_BOOLEAN)
  #undef FLS_START_SEC_VAR_INIT_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined FLS_STOP_SEC_VAR_INIT_BOOLEAN)
  #undef FLS_STOP_SEC_VAR_INIT_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined FLS_START_SEC_VAR_INIT_8)
  #undef FLS_START_SEC_VAR_INIT_8
  #undef MEMMAP_ERROR

#elif (defined FLS_STOP_SEC_VAR_INIT_8)
  #undef FLS_STOP_SEC_VAR_INIT_8
  #undef MEMMAP_ERROR

#elif (defined FLS_START_SEC_VAR_INIT_16)
  #undef FLS_START_SEC_VAR_INIT_16
  #undef MEMMAP_ERROR

#elif (defined FLS_STOP_SEC_VAR_INIT_16)
  #undef FLS_STOP_SEC_VAR_INIT_16
  #undef MEMMAP_ERROR

#elif (defined FLS_START_SEC_VAR_INIT_32)
  #undef FLS_START_SEC_VAR_INIT_32
  #undef MEMMAP_ERROR

#elif (defined FLS_STOP_SEC_VAR_INIT_32)
  #undef FLS_STOP_SEC_VAR_INIT_32
  #undef MEMMAP_ERROR

#elif (defined FLS_START_SEC_RAMCODE)
  #undef FLS_START_SEC_RAMCODE
  #undef MEMMAP_ERROR
  #pragma ghs section text=".ramcode"
#elif (defined FLS_STOP_SEC_RAMCODE)
  #undef FLS_STOP_SEC_RAMCODE
  #undef MEMMAP_ERROR
  #pragma ghs section text=default
/*------------------[Start of a module]--------------------------------------*/

#elif (defined FLSIF_START_SEC_CODE)
  #undef FLSIF_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined FLSIF_STOP_SEC_CODE)
  #undef FLSIF_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined FLSIF_START_SEC_APPL_CODE)
  #undef FLSIF_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined FLSIF_STOP_SEC_APPL_CODE)
  #undef FLSIF_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined FLSIF_START_SEC_VAR_NOINIT_8BIT)
  #undef FLSIF_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FLSIF_STOP_SEC_VAR_NOINIT_8BIT)
  #undef FLSIF_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FLSIF_START_SEC_VAR_NOINIT_16BIT)
  #undef FLSIF_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FLSIF_STOP_SEC_VAR_NOINIT_16BIT)
  #undef FLSIF_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FLSIF_START_SEC_VAR_NOINIT_32BIT)
  #undef FLSIF_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FLSIF_STOP_SEC_VAR_NOINIT_32BIT)
  #undef FLSIF_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FLSIF_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef FLSIF_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FLSIF_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef FLSIF_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FLSIF_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef FLSIF_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FLSIF_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef FLSIF_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FLSIF_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef FLSIF_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FLSIF_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef FLSIF_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FLSIF_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef FLSIF_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FLSIF_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef FLSIF_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FLSIF_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef FLSIF_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FLSIF_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef FLSIF_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FLSIF_START_SEC_VAR_FAST_8BIT)
  #undef FLSIF_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FLSIF_STOP_SEC_VAR_FAST_8BIT)
  #undef FLSIF_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FLSIF_START_SEC_VAR_FAST_16BIT)
  #undef FLSIF_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FLSIF_STOP_SEC_VAR_FAST_16BIT)
  #undef FLSIF_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FLSIF_START_SEC_VAR_FAST_32BIT)
  #undef FLSIF_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FLSIF_STOP_SEC_VAR_FAST_32BIT)
  #undef FLSIF_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FLSIF_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef FLSIF_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FLSIF_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef FLSIF_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FLSIF_START_SEC_VAR_8BIT)
  #undef FLSIF_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined FLSIF_STOP_SEC_VAR_8BIT)
  #undef FLSIF_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined FLSIF_START_SEC_VAR_16BIT)
  #undef FLSIF_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined FLSIF_STOP_SEC_VAR_16BIT)
  #undef FLSIF_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined FLSIF_START_SEC_VAR_32BIT)
  #undef FLSIF_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined FLSIF_STOP_SEC_VAR_32BIT)
  #undef FLSIF_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined FLSIF_START_SEC_VAR_UNSPECIFIED)
  #undef FLSIF_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FLSIF_STOP_SEC_VAR_UNSPECIFIED)
  #undef FLSIF_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FLSIF_START_SEC_CONST_8BIT)
  #undef FLSIF_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FLSIF_STOP_SEC_CONST_8BIT)
  #undef FLSIF_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FLSIF_START_SEC_CONST_16BIT)
  #undef FLSIF_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FLSIF_STOP_SEC_CONST_16BIT)
  #undef FLSIF_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FLSIF_START_SEC_CONST_32BIT)
  #undef FLSIF_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FLSIF_STOP_SEC_CONST_32BIT)
  #undef FLSIF_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FLSIF_START_SEC_CONST_UNSPECIFIED)
  #undef FLSIF_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FLSIF_STOP_SEC_CONST_UNSPECIFIED)
  #undef FLSIF_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FLSIF_START_CONFIG_DATA_8BIT)
  #undef FLSIF_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined FLSIF_STOP_CONFIG_DATA_8BIT)
  #undef FLSIF_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined FLSIF_START_CONFIG_DATA_16BIT)
  #undef FLSIF_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined FLSIF_STOP_CONFIG_DATA_16BIT)
  #undef FLSIF_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined FLSIF_START_CONFIG_DATA_32BIT)
  #undef FLSIF_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined FLSIF_STOP_CONFIG_DATA_32BIT)
  #undef FLSIF_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined FLSIF_START_CONFIG_DATA_UNSPECIFIED)
  #undef FLSIF_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FLSIF_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef FLSIF_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined FR_START_SEC_CODE)
  #undef FR_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined FR_STOP_SEC_CODE)
  #undef FR_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined FR_START_SEC_APPL_CODE)
  #undef FR_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined FR_STOP_SEC_APPL_CODE)
  #undef FR_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined FR_START_SEC_VAR_NOINIT_8BIT)
  #undef FR_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FR_STOP_SEC_VAR_NOINIT_8BIT)
  #undef FR_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FR_START_SEC_VAR_NOINIT_16BIT)
  #undef FR_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FR_STOP_SEC_VAR_NOINIT_16BIT)
  #undef FR_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FR_START_SEC_VAR_NOINIT_32BIT)
  #undef FR_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FR_STOP_SEC_VAR_NOINIT_32BIT)
  #undef FR_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FR_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef FR_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FR_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef FR_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FR_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef FR_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FR_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef FR_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FR_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef FR_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FR_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef FR_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FR_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef FR_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FR_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef FR_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FR_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef FR_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FR_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef FR_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FR_START_SEC_VAR_FAST_8BIT)
  #undef FR_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FR_STOP_SEC_VAR_FAST_8BIT)
  #undef FR_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FR_START_SEC_VAR_FAST_16BIT)
  #undef FR_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FR_STOP_SEC_VAR_FAST_16BIT)
  #undef FR_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FR_START_SEC_VAR_FAST_32BIT)
  #undef FR_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FR_STOP_SEC_VAR_FAST_32BIT)
  #undef FR_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FR_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef FR_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FR_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef FR_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FR_START_SEC_VAR_8BIT)
  #undef FR_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined FR_STOP_SEC_VAR_8BIT)
  #undef FR_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined FR_START_SEC_VAR_16BIT)
  #undef FR_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined FR_STOP_SEC_VAR_16BIT)
  #undef FR_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined FR_START_SEC_VAR_32BIT)
  #undef FR_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined FR_STOP_SEC_VAR_32BIT)
  #undef FR_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined FR_START_SEC_VAR_UNSPECIFIED)
  #undef FR_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FR_STOP_SEC_VAR_UNSPECIFIED)
  #undef FR_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FR_START_SEC_CONST_8BIT)
  #undef FR_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FR_STOP_SEC_CONST_8BIT)
  #undef FR_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FR_START_SEC_CONST_16BIT)
  #undef FR_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FR_STOP_SEC_CONST_16BIT)
  #undef FR_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FR_START_SEC_CONST_32BIT)
  #undef FR_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FR_STOP_SEC_CONST_32BIT)
  #undef FR_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FR_START_SEC_CONST_UNSPECIFIED)
  #undef FR_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FR_STOP_SEC_CONST_UNSPECIFIED)
  #undef FR_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FR_START_CONFIG_DATA_8BIT)
  #undef FR_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined FR_STOP_CONFIG_DATA_8BIT)
  #undef FR_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined FR_START_CONFIG_DATA_16BIT)
  #undef FR_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined FR_STOP_CONFIG_DATA_16BIT)
  #undef FR_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined FR_START_CONFIG_DATA_32BIT)
  #undef FR_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined FR_STOP_CONFIG_DATA_32BIT)
  #undef FR_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined FR_START_CONFIG_DATA_UNSPECIFIED)
  #undef FR_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FR_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef FR_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined FRIF_START_SEC_CODE)
  #undef FRIF_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined FRIF_STOP_SEC_CODE)
  #undef FRIF_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined FRIF_START_SEC_APPL_CODE)
  #undef FRIF_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined FRIF_STOP_SEC_APPL_CODE)
  #undef FRIF_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined FRIF_START_SEC_VAR_NOINIT_8BIT)
  #undef FRIF_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRIF_STOP_SEC_VAR_NOINIT_8BIT)
  #undef FRIF_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRIF_START_SEC_VAR_NOINIT_16BIT)
  #undef FRIF_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRIF_STOP_SEC_VAR_NOINIT_16BIT)
  #undef FRIF_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRIF_START_SEC_VAR_NOINIT_32BIT)
  #undef FRIF_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRIF_STOP_SEC_VAR_NOINIT_32BIT)
  #undef FRIF_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRIF_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef FRIF_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRIF_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef FRIF_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRIF_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef FRIF_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRIF_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef FRIF_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRIF_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef FRIF_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRIF_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef FRIF_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRIF_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef FRIF_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRIF_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef FRIF_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRIF_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef FRIF_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRIF_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef FRIF_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRIF_START_SEC_VAR_FAST_8BIT)
  #undef FRIF_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRIF_STOP_SEC_VAR_FAST_8BIT)
  #undef FRIF_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRIF_START_SEC_VAR_FAST_16BIT)
  #undef FRIF_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRIF_STOP_SEC_VAR_FAST_16BIT)
  #undef FRIF_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRIF_START_SEC_VAR_FAST_32BIT)
  #undef FRIF_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRIF_STOP_SEC_VAR_FAST_32BIT)
  #undef FRIF_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRIF_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef FRIF_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRIF_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef FRIF_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRIF_START_SEC_VAR_8BIT)
  #undef FRIF_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRIF_STOP_SEC_VAR_8BIT)
  #undef FRIF_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRIF_START_SEC_VAR_16BIT)
  #undef FRIF_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRIF_STOP_SEC_VAR_16BIT)
  #undef FRIF_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRIF_START_SEC_VAR_32BIT)
  #undef FRIF_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRIF_STOP_SEC_VAR_32BIT)
  #undef FRIF_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRIF_START_SEC_VAR_UNSPECIFIED)
  #undef FRIF_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRIF_STOP_SEC_VAR_UNSPECIFIED)
  #undef FRIF_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRIF_START_SEC_CONST_8BIT)
  #undef FRIF_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRIF_STOP_SEC_CONST_8BIT)
  #undef FRIF_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRIF_START_SEC_CONST_16BIT)
  #undef FRIF_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRIF_STOP_SEC_CONST_16BIT)
  #undef FRIF_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRIF_START_SEC_CONST_32BIT)
  #undef FRIF_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRIF_STOP_SEC_CONST_32BIT)
  #undef FRIF_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRIF_START_SEC_CONST_32)
  #undef FRIF_START_SEC_CONST_32
  #undef MEMMAP_ERROR

#elif (defined FRIF_STOP_SEC_CONST_32)
  #undef FRIF_STOP_SEC_CONST_32
  #undef MEMMAP_ERROR

#elif (defined FRIF_START_SEC_CONST_UNSPECIFIED)
  #undef FRIF_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRIF_STOP_SEC_CONST_UNSPECIFIED)
  #undef FRIF_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRIF_START_CONFIG_DATA_8BIT)
  #undef FRIF_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRIF_STOP_CONFIG_DATA_8BIT)
  #undef FRIF_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRIF_START_CONFIG_DATA_16BIT)
  #undef FRIF_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRIF_STOP_CONFIG_DATA_16BIT)
  #undef FRIF_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRIF_START_CONFIG_DATA_32BIT)
  #undef FRIF_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRIF_STOP_CONFIG_DATA_32BIT)
  #undef FRIF_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRIF_START_CONFIG_DATA_UNSPECIFIED)
  #undef FRIF_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRIF_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef FRIF_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRIF_START_SEC_CODE)
    #undef FRIF_START_SEC_CODE
    #undef MEMMAP_ERROR
#elif (defined FRIF_STOP_SEC_CODE)
    #undef FRIF_STOP_SEC_CODE
    #undef MEMMAP_ERROR
#elif (defined FRIF_START_SEC_CONST_32)
    #undef FRIF_START_SEC_CONST_32
    #undef MEMMAP_ERROR
#elif (defined FRIF_STOP_SEC_CONST_32)
    #undef FRIF_STOP_SEC_CONST_32
    #undef MEMMAP_ERROR
#elif (defined FRIF_START_SEC_VAR_NO_INIT_UNSPECIFIED)
    #undef FRIF_START_SEC_VAR_NO_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined FRIF_STOP_SEC_VAR_NO_INIT_UNSPECIFIED)
    #undef FRIF_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined FRIF_START_SEC_VAR_FAST_INIT_UNSPECIFIED)
    #undef FRIF_START_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined FRIF_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED)
    #undef FRIF_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined FRIF_START_SEC_CONFIG_DATA_UNSPECIFIED)
    #undef FRIF_START_SEC_CONFIG_DATA_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined FRIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED)
    #undef FRIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined FRIF_START_CONFIG_DATA_UNSPECIFIED)
    #undef FRIF_START_CONFIG_DATA_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined FRIF_STOP_CONFIG_DATA_UNSPECIFIED)
    #undef FRIF_STOP_CONFIG_DATA_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined FRIF_START_SEC_CONST_UNSPECIFIED)
    #undef FRIF_START_SEC_CONST_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined FRIF_STOP_SEC_CONST_UNSPECIFIED)
    #undef FRIF_STOP_SEC_CONST_UNSPECIFIED
    #undef MEMMAP_ERROR

/*------------------[Start of a module]--------------------------------------*/

#elif (defined FRNM_START_SEC_CODE)
  #undef FRNM_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined FRNM_STOP_SEC_CODE)
  #undef FRNM_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined FRNM_START_SEC_APPL_CODE)
  #undef FRNM_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined FRNM_STOP_SEC_APPL_CODE)
  #undef FRNM_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined FRNM_START_SEC_VAR_NOINIT_8BIT)
  #undef FRNM_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRNM_STOP_SEC_VAR_NOINIT_8BIT)
  #undef FRNM_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRNM_START_SEC_VAR_NOINIT_16BIT)
  #undef FRNM_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRNM_STOP_SEC_VAR_NOINIT_16BIT)
  #undef FRNM_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRNM_START_SEC_VAR_NOINIT_32BIT)
  #undef FRNM_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRNM_STOP_SEC_VAR_NOINIT_32BIT)
  #undef FRNM_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRNM_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef FRNM_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRNM_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef FRNM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRNM_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef FRNM_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRNM_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef FRNM_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRNM_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef FRNM_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRNM_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef FRNM_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRNM_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef FRNM_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRNM_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef FRNM_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRNM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef FRNM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRNM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef FRNM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRNM_START_SEC_VAR_FAST_8BIT)
  #undef FRNM_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRNM_STOP_SEC_VAR_FAST_8BIT)
  #undef FRNM_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRNM_START_SEC_VAR_FAST_16BIT)
  #undef FRNM_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRNM_STOP_SEC_VAR_FAST_16BIT)
  #undef FRNM_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRNM_START_SEC_VAR_FAST_32BIT)
  #undef FRNM_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRNM_STOP_SEC_VAR_FAST_32BIT)
  #undef FRNM_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRNM_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef FRNM_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRNM_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef FRNM_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRNM_START_SEC_VAR_8BIT)
  #undef FRNM_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRNM_STOP_SEC_VAR_8BIT)
  #undef FRNM_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRNM_START_SEC_VAR_16BIT)
  #undef FRNM_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRNM_STOP_SEC_VAR_16BIT)
  #undef FRNM_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRNM_START_SEC_VAR_32BIT)
  #undef FRNM_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRNM_STOP_SEC_VAR_32BIT)
  #undef FRNM_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRNM_START_SEC_VAR_UNSPECIFIED)
  #undef FRNM_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRNM_STOP_SEC_VAR_UNSPECIFIED)
  #undef FRNM_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRNM_START_SEC_CONST_8BIT)
  #undef FRNM_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRNM_STOP_SEC_CONST_8BIT)
  #undef FRNM_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRNM_START_SEC_CONST_16BIT)
  #undef FRNM_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRNM_STOP_SEC_CONST_16BIT)
  #undef FRNM_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRNM_START_SEC_CONST_32BIT)
  #undef FRNM_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRNM_STOP_SEC_CONST_32BIT)
  #undef FRNM_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRNM_START_SEC_CONST_UNSPECIFIED)
  #undef FRNM_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRNM_STOP_SEC_CONST_UNSPECIFIED)
  #undef FRNM_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRNM_START_CONFIG_DATA_8BIT)
  #undef FRNM_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRNM_STOP_CONFIG_DATA_8BIT)
  #undef FRNM_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRNM_START_CONFIG_DATA_16BIT)
  #undef FRNM_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRNM_STOP_CONFIG_DATA_16BIT)
  #undef FRNM_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRNM_START_CONFIG_DATA_32BIT)
  #undef FRNM_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRNM_STOP_CONFIG_DATA_32BIT)
  #undef FRNM_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRNM_START_CONFIG_DATA_UNSPECIFIED)
  #undef FRNM_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRNM_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef FRNM_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined FRSM_START_SEC_CODE)
  #undef FRSM_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined FRSM_STOP_SEC_CODE)
  #undef FRSM_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined FRSM_START_SEC_APPL_CODE)
  #undef FRSM_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined FRSM_STOP_SEC_APPL_CODE)
  #undef FRSM_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined FRSM_START_SEC_VAR_NOINIT_8BIT)
  #undef FRSM_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRSM_STOP_SEC_VAR_NOINIT_8BIT)
  #undef FRSM_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRSM_START_SEC_VAR_NOINIT_16BIT)
  #undef FRSM_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRSM_STOP_SEC_VAR_NOINIT_16BIT)
  #undef FRSM_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRSM_START_SEC_VAR_NOINIT_32BIT)
  #undef FRSM_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRSM_STOP_SEC_VAR_NOINIT_32BIT)
  #undef FRSM_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRSM_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef FRSM_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRSM_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef FRSM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRSM_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef FRSM_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRSM_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef FRSM_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRSM_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef FRSM_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRSM_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef FRSM_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRSM_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef FRSM_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRSM_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef FRSM_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRSM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef FRSM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRSM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef FRSM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRSM_START_SEC_VAR_FAST_8BIT)
  #undef FRSM_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRSM_STOP_SEC_VAR_FAST_8BIT)
  #undef FRSM_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRSM_START_SEC_VAR_FAST_16BIT)
  #undef FRSM_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRSM_STOP_SEC_VAR_FAST_16BIT)
  #undef FRSM_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRSM_START_SEC_VAR_FAST_32BIT)
  #undef FRSM_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRSM_STOP_SEC_VAR_FAST_32BIT)
  #undef FRSM_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRSM_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef FRSM_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRSM_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef FRSM_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRSM_START_SEC_VAR_8BIT)
  #undef FRSM_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRSM_STOP_SEC_VAR_8BIT)
  #undef FRSM_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRSM_START_SEC_VAR_16BIT)
  #undef FRSM_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRSM_STOP_SEC_VAR_16BIT)
  #undef FRSM_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRSM_START_SEC_VAR_32BIT)
  #undef FRSM_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRSM_STOP_SEC_VAR_32BIT)
  #undef FRSM_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRSM_START_SEC_VAR_UNSPECIFIED)
  #undef FRSM_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRSM_STOP_SEC_VAR_UNSPECIFIED)
  #undef FRSM_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRSM_START_SEC_CONST_8BIT)
  #undef FRSM_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRSM_STOP_SEC_CONST_8BIT)
  #undef FRSM_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRSM_START_SEC_CONST_16BIT)
  #undef FRSM_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRSM_STOP_SEC_CONST_16BIT)
  #undef FRSM_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRSM_START_SEC_CONST_32BIT)
  #undef FRSM_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRSM_STOP_SEC_CONST_32BIT)
  #undef FRSM_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRSM_START_SEC_CONST_UNSPECIFIED)
  #undef FRSM_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRSM_STOP_SEC_CONST_UNSPECIFIED)
  #undef FRSM_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRSM_START_CONFIG_DATA_8BIT)
  #undef FRSM_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRSM_STOP_CONFIG_DATA_8BIT)
  #undef FRSM_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRSM_START_CONFIG_DATA_16BIT)
  #undef FRSM_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRSM_STOP_CONFIG_DATA_16BIT)
  #undef FRSM_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRSM_START_CONFIG_DATA_32BIT)
  #undef FRSM_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRSM_STOP_CONFIG_DATA_32BIT)
  #undef FRSM_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRSM_START_CONFIG_DATA_UNSPECIFIED)
  #undef FRSM_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRSM_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef FRSM_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRSM_START_SEC_CONFIG_DATA_UNSPECIFIED)
  #undef FRSM_START_SEC_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRSM_STOP_SEC_CONFIG_DATA_UNSPECIFIED)
  #undef FRSM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR
#elif (defined FRSM_START_SEC_VAR_FAST_INIT_UNSPECIFIED)
    #undef FRSM_START_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined FRSM_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED)
    #undef FRSM_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined FRSM_START_SEC_VAR_NO_INIT_8)
    #undef FRSM_START_SEC_VAR_NO_INIT_8
    #undef MEMMAP_ERROR
#elif (defined FRSM_STOP_SEC_VAR_NO_INIT_8)
    #undef FRSM_STOP_SEC_VAR_NO_INIT_8
    #undef MEMMAP_ERROR
#elif (defined FRSM_START_SEC_VAR_NO_INIT_UNSPECIFIED)
  #undef FRSM_START_SEC_VAR_NO_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRSM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED)
  #undef FRSM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

/*------------------[Start of a module]--------------------------------------*/
/* Memory section macros for Fr */
#elif (defined FR_1_ERAY_START_SEC_CODE)
  #undef FR_1_ERAY_START_SEC_CODE
  #undef MEMMAP_ERROR
#elif (defined FR_1_ERAY_STOP_SEC_CODE)
  #undef FR_1_ERAY_STOP_SEC_CODE
  #undef MEMMAP_ERROR
#elif (defined FR_1_ERAY_START_SEC_CONST_32)
  #undef FR_1_ERAY_START_SEC_CONST_32
  #undef MEMMAP_ERROR
#elif (defined FR_1_ERAY_STOP_SEC_CONST_32)
  #undef FR_1_ERAY_STOP_SEC_CONST_32
  #undef MEMMAP_ERROR
#elif (defined FR_1_ERAY_START_SEC_CONST_UNSPECIFIED)
  #undef FR_1_ERAY_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR
#elif (defined FR_1_ERAY_STOP_SEC_CONST_UNSPECIFIED)
  #undef FR_1_ERAY_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR
#elif (defined FR_1_ERAY_START_SEC_VAR_FAST_INIT_8)
  #undef FR_1_ERAY_START_SEC_VAR_FAST_INIT_8
  #undef MEMMAP_ERROR
#elif (defined FR_1_ERAY_STOP_SEC_VAR_FAST_INIT_8)
  #undef FR_1_ERAY_STOP_SEC_VAR_FAST_INIT_8
  #undef MEMMAP_ERROR
#elif (defined FR_1_ERAY_START_SEC_VAR_FAST_INIT_UNSPECIFIED)
  #undef FR_1_ERAY_START_SEC_VAR_FAST_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR
#elif (defined FR_1_ERAY_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED)
  #undef FR_1_ERAY_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR
#elif (defined FR_1_ERAY_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef FR_1_ERAY_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR
#elif (defined FR_1_ERAY_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef FR_1_ERAY_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR
#elif (defined FR_1_ERAY_START_SEC_VAR_INIT_8)
  #undef FR_1_ERAY_START_SEC_VAR_INIT_8
  #undef MEMMAP_ERROR
#elif (defined FR_1_ERAY_STOP_SEC_VAR_INIT_8)
  #undef FR_1_ERAY_STOP_SEC_VAR_INIT_8
  #undef MEMMAP_ERROR
#elif (defined FR_1_ERAY_START_SEC_CONFIG_DATA_UNSPECIFIED)
  #undef FR_1_ERAY_START_SEC_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR
#elif (defined FR_1_ERAY_STOP_SEC_CONFIG_DATA_UNSPECIFIED)
  #undef FR_1_ERAY_STOP_SEC_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR
#elif (defined FR_1_ERAY_START_CONFIG_DATA_UNSPECIFIED)
  #undef FR_1_ERAY_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR
#elif (defined FR_1_ERAY_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef FR_1_ERAY_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

/*------------------[Start of a module]--------------------------------------*/

#elif (defined FRTP_START_SEC_CODE)
  #undef FRTP_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined FRTP_STOP_SEC_CODE)
  #undef FRTP_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined FRTP_START_SEC_APPL_CODE)
  #undef FRTP_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined FRTP_STOP_SEC_APPL_CODE)
  #undef FRTP_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined FRTP_START_SEC_VAR_NOINIT_8BIT)
  #undef FRTP_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRTP_STOP_SEC_VAR_NOINIT_8BIT)
  #undef FRTP_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRTP_START_SEC_VAR_NOINIT_16BIT)
  #undef FRTP_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRTP_STOP_SEC_VAR_NOINIT_16BIT)
  #undef FRTP_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRTP_START_SEC_VAR_NOINIT_32BIT)
  #undef FRTP_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRTP_STOP_SEC_VAR_NOINIT_32BIT)
  #undef FRTP_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRTP_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef FRTP_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRTP_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef FRTP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRTP_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef FRTP_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRTP_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef FRTP_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRTP_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef FRTP_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRTP_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef FRTP_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRTP_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef FRTP_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRTP_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef FRTP_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRTP_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef FRTP_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRTP_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef FRTP_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRTP_START_SEC_VAR_FAST_8BIT)
  #undef FRTP_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRTP_STOP_SEC_VAR_FAST_8BIT)
  #undef FRTP_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRTP_START_SEC_VAR_FAST_16BIT)
  #undef FRTP_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRTP_STOP_SEC_VAR_FAST_16BIT)
  #undef FRTP_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRTP_START_SEC_VAR_FAST_32BIT)
  #undef FRTP_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRTP_STOP_SEC_VAR_FAST_32BIT)
  #undef FRTP_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRTP_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef FRTP_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRTP_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef FRTP_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRTP_START_SEC_VAR_8BIT)
  #undef FRTP_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRTP_STOP_SEC_VAR_8BIT)
  #undef FRTP_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRTP_START_SEC_VAR_16BIT)
  #undef FRTP_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRTP_STOP_SEC_VAR_16BIT)
  #undef FRTP_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRTP_START_SEC_VAR_32BIT)
  #undef FRTP_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRTP_STOP_SEC_VAR_32BIT)
  #undef FRTP_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRTP_START_SEC_VAR_UNSPECIFIED)
  #undef FRTP_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRTP_STOP_SEC_VAR_UNSPECIFIED)
  #undef FRTP_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRTP_START_SEC_CONST_8BIT)
  #undef FRTP_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRTP_STOP_SEC_CONST_8BIT)
  #undef FRTP_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRTP_START_SEC_CONST_16BIT)
  #undef FRTP_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRTP_STOP_SEC_CONST_16BIT)
  #undef FRTP_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRTP_START_SEC_CONST_32BIT)
  #undef FRTP_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRTP_STOP_SEC_CONST_32BIT)
  #undef FRTP_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRTP_START_SEC_CONST_32)
  #undef FRTP_START_SEC_CONST_32
  #undef MEMMAP_ERROR

#elif (defined FRTP_STOP_SEC_CONST_32)
  #undef FRTP_STOP_SEC_CONST_32
  #undef MEMMAP_ERROR

#elif (defined FRTP_START_SEC_CONST_UNSPECIFIED)
  #undef FRTP_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRTP_STOP_SEC_CONST_UNSPECIFIED)
  #undef FRTP_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRTP_START_CONFIG_DATA_8BIT)
  #undef FRTP_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRTP_STOP_CONFIG_DATA_8BIT)
  #undef FRTP_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRTP_START_CONFIG_DATA_16BIT)
  #undef FRTP_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRTP_STOP_CONFIG_DATA_16BIT)
  #undef FRTP_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRTP_START_CONFIG_DATA_32BIT)
  #undef FRTP_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRTP_STOP_CONFIG_DATA_32BIT)
  #undef FRTP_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRTP_START_CONFIG_DATA_UNSPECIFIED)
  #undef FRTP_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRTP_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef FRTP_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRTP_START_SEC_CODE)
    #undef FRTP_START_SEC_CODE
    #undef MEMMAP_ERROR

#elif (defined FRTP_STOP_SEC_CODE)
    #undef FRTP_STOP_SEC_CODE
    #undef MEMMAP_ERROR

#elif (defined FRTP_START_SEC_CONST_32)
    #undef FRTP_START_SEC_CONST_32
    #undef MEMMAP_ERROR

#elif (defined FRTP_STOP_SEC_CONST_32)
    #undef FRTP_STOP_SEC_CONST_32
    #undef MEMMAP_ERROR

#elif (defined FRTP_START_SEC_CONFIG_DATA_UNSPECIFIED)
  #undef FRTP_START_SEC_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRTP_STOP_SEC_CONFIG_DATA_UNSPECIFIED)
  #undef FRTP_STOP_SEC_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRTP_START_CONFIG_DATA_UNSPECIFIED)
    #undef FRTP_START_CONFIG_DATA_UNSPECIFIED
    #undef MEMMAP_ERROR

#elif (defined FRTP_STOP_CONFIG_DATA_UNSPECIFIED)
    #undef FRTP_STOP_CONFIG_DATA_UNSPECIFIED
    #undef MEMMAP_ERROR

#elif (defined FRTP_START_SEC_VAR_INIT_UNSPECIFIED)
    #undef FRTP_START_SEC_VAR_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR

#elif (defined FRTP_STOP_SEC_VAR_INIT_UNSPECIFIED)
    #undef FRTP_STOP_SEC_VAR_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR

#elif (defined FRTP_START_SEC_VAR_FAST_INIT_UNSPECIFIED)
    #undef FRTP_START_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR

#elif (defined FRTP_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED)
    #undef FRTP_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR

/*------------------[Start of a module]--------------------------------------*/

#elif (defined FRTRCV_START_SEC_CODE)
  #undef FRTRCV_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_STOP_SEC_CODE)
  #undef FRTRCV_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_START_SEC_APPL_CODE)
  #undef FRTRCV_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_STOP_SEC_APPL_CODE)
  #undef FRTRCV_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_START_SEC_VAR_NOINIT_8BIT)
  #undef FRTRCV_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_STOP_SEC_VAR_NOINIT_8BIT)
  #undef FRTRCV_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_START_SEC_VAR_NOINIT_16BIT)
  #undef FRTRCV_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_STOP_SEC_VAR_NOINIT_16BIT)
  #undef FRTRCV_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_START_SEC_VAR_NOINIT_32BIT)
  #undef FRTRCV_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_STOP_SEC_VAR_NOINIT_32BIT)
  #undef FRTRCV_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef FRTRCV_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef FRTRCV_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef FRTRCV_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef FRTRCV_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef FRTRCV_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef FRTRCV_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef FRTRCV_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef FRTRCV_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef FRTRCV_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef FRTRCV_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_START_SEC_VAR_FAST_8BIT)
  #undef FRTRCV_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_STOP_SEC_VAR_FAST_8BIT)
  #undef FRTRCV_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_START_SEC_VAR_FAST_16BIT)
  #undef FRTRCV_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_STOP_SEC_VAR_FAST_16BIT)
  #undef FRTRCV_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_START_SEC_VAR_FAST_32BIT)
  #undef FRTRCV_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_STOP_SEC_VAR_FAST_32BIT)
  #undef FRTRCV_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef FRTRCV_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef FRTRCV_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_START_SEC_VAR_8BIT)
  #undef FRTRCV_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_STOP_SEC_VAR_8BIT)
  #undef FRTRCV_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_START_SEC_VAR_16BIT)
  #undef FRTRCV_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_STOP_SEC_VAR_16BIT)
  #undef FRTRCV_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_START_SEC_VAR_32BIT)
  #undef FRTRCV_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_STOP_SEC_VAR_32BIT)
  #undef FRTRCV_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_START_SEC_VAR_UNSPECIFIED)
  #undef FRTRCV_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_STOP_SEC_VAR_UNSPECIFIED)
  #undef FRTRCV_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_START_SEC_CONST_8BIT)
  #undef FRTRCV_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_STOP_SEC_CONST_8BIT)
  #undef FRTRCV_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_START_SEC_CONST_16BIT)
  #undef FRTRCV_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_STOP_SEC_CONST_16BIT)
  #undef FRTRCV_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_START_SEC_CONST_32BIT)
  #undef FRTRCV_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_STOP_SEC_CONST_32BIT)
  #undef FRTRCV_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_START_SEC_CONST_UNSPECIFIED)
  #undef FRTRCV_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_STOP_SEC_CONST_UNSPECIFIED)
  #undef FRTRCV_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_START_CONFIG_DATA_8BIT)
  #undef FRTRCV_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_STOP_CONFIG_DATA_8BIT)
  #undef FRTRCV_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_START_CONFIG_DATA_16BIT)
  #undef FRTRCV_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_STOP_CONFIG_DATA_16BIT)
  #undef FRTRCV_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_START_CONFIG_DATA_32BIT)
  #undef FRTRCV_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_STOP_CONFIG_DATA_32BIT)
  #undef FRTRCV_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_START_CONFIG_DATA_UNSPECIFIED)
  #undef FRTRCV_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef FRTRCV_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined FRTRCV_1_TJA1080_START_SEC_CODE)
  #undef FRTRCV_1_TJA1080_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_1_TJA1080_STOP_SEC_CODE)
  #undef FRTRCV_1_TJA1080_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_1_TJA1080_START_SEC_APPL_CODE)
  #undef FRTRCV_1_TJA1080_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_1_TJA1080_STOP_SEC_APPL_CODE)
  #undef FRTRCV_1_TJA1080_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_1_TJA1080_START_SEC_VAR_NOINIT_8BIT)
  #undef FRTRCV_1_TJA1080_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_1_TJA1080_STOP_SEC_VAR_NOINIT_8BIT)
  #undef FRTRCV_1_TJA1080_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_1_TJA1080_START_SEC_VAR_NOINIT_16BIT)
  #undef FRTRCV_1_TJA1080_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_1_TJA1080_STOP_SEC_VAR_NOINIT_16BIT)
  #undef FRTRCV_1_TJA1080_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_1_TJA1080_START_SEC_VAR_NOINIT_32BIT)
  #undef FRTRCV_1_TJA1080_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_1_TJA1080_STOP_SEC_VAR_NOINIT_32BIT)
  #undef FRTRCV_1_TJA1080_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_1_TJA1080_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef FRTRCV_1_TJA1080_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_1_TJA1080_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef FRTRCV_1_TJA1080_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_1_TJA1080_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef FRTRCV_1_TJA1080_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_1_TJA1080_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef FRTRCV_1_TJA1080_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_1_TJA1080_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef FRTRCV_1_TJA1080_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_1_TJA1080_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef FRTRCV_1_TJA1080_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_1_TJA1080_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef FRTRCV_1_TJA1080_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_1_TJA1080_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef FRTRCV_1_TJA1080_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_1_TJA1080_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef FRTRCV_1_TJA1080_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_1_TJA1080_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef FRTRCV_1_TJA1080_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_1_TJA1080_START_SEC_VAR_FAST_8BIT)
  #undef FRTRCV_1_TJA1080_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_1_TJA1080_STOP_SEC_VAR_FAST_8BIT)
  #undef FRTRCV_1_TJA1080_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_1_TJA1080_START_SEC_VAR_FAST_16BIT)
  #undef FRTRCV_1_TJA1080_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_1_TJA1080_STOP_SEC_VAR_FAST_16BIT)
  #undef FRTRCV_1_TJA1080_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_1_TJA1080_START_SEC_VAR_FAST_32BIT)
  #undef FRTRCV_1_TJA1080_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_1_TJA1080_STOP_SEC_VAR_FAST_32BIT)
  #undef FRTRCV_1_TJA1080_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_1_TJA1080_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef FRTRCV_1_TJA1080_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_1_TJA1080_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef FRTRCV_1_TJA1080_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_1_TJA1080_START_SEC_VAR_8BIT)
  #undef FRTRCV_1_TJA1080_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_1_TJA1080_STOP_SEC_VAR_8BIT)
  #undef FRTRCV_1_TJA1080_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_1_TJA1080_START_SEC_VAR_16BIT)
  #undef FRTRCV_1_TJA1080_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_1_TJA1080_STOP_SEC_VAR_16BIT)
  #undef FRTRCV_1_TJA1080_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_1_TJA1080_START_SEC_VAR_32BIT)
  #undef FRTRCV_1_TJA1080_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_1_TJA1080_STOP_SEC_VAR_32BIT)
  #undef FRTRCV_1_TJA1080_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_1_TJA1080_START_SEC_VAR_UNSPECIFIED)
  #undef FRTRCV_1_TJA1080_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_1_TJA1080_STOP_SEC_VAR_UNSPECIFIED)
  #undef FRTRCV_1_TJA1080_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_1_TJA1080_START_SEC_CONST_8BIT)
  #undef FRTRCV_1_TJA1080_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_1_TJA1080_STOP_SEC_CONST_8BIT)
  #undef FRTRCV_1_TJA1080_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_1_TJA1080_START_SEC_CONST_16BIT)
  #undef FRTRCV_1_TJA1080_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_1_TJA1080_STOP_SEC_CONST_16BIT)
  #undef FRTRCV_1_TJA1080_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_1_TJA1080_START_SEC_CONST_32BIT)
  #undef FRTRCV_1_TJA1080_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_1_TJA1080_STOP_SEC_CONST_32BIT)
  #undef FRTRCV_1_TJA1080_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_1_TJA1080_START_SEC_CONST_UNSPECIFIED)
  #undef FRTRCV_1_TJA1080_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_1_TJA1080_STOP_SEC_CONST_UNSPECIFIED)
  #undef FRTRCV_1_TJA1080_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_1_TJA1080_START_CONFIG_DATA_8BIT)
  #undef FRTRCV_1_TJA1080_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_1_TJA1080_STOP_CONFIG_DATA_8BIT)
  #undef FRTRCV_1_TJA1080_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_1_TJA1080_START_CONFIG_DATA_16BIT)
  #undef FRTRCV_1_TJA1080_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_1_TJA1080_STOP_CONFIG_DATA_16BIT)
  #undef FRTRCV_1_TJA1080_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_1_TJA1080_START_CONFIG_DATA_32BIT)
  #undef FRTRCV_1_TJA1080_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_1_TJA1080_STOP_CONFIG_DATA_32BIT)
  #undef FRTRCV_1_TJA1080_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_1_TJA1080_START_CONFIG_DATA_UNSPECIFIED)
  #undef FRTRCV_1_TJA1080_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FRTRCV_1_TJA1080_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef FRTRCV_1_TJA1080_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined FR_1_ERAY_START_SEC_CODE)
  #undef FR_1_ERAY_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined FR_1_ERAY_STOP_SEC_CODE)
  #undef FR_1_ERAY_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined FR_1_ERAY_START_SEC_APPL_CODE)
  #undef FR_1_ERAY_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined FR_1_ERAY_STOP_SEC_APPL_CODE)
  #undef FR_1_ERAY_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined FR_1_ERAY_START_SEC_VAR_NOINIT_8BIT)
  #undef FR_1_ERAY_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_ERAY_STOP_SEC_VAR_NOINIT_8BIT)
  #undef FR_1_ERAY_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_ERAY_START_SEC_VAR_NOINIT_16BIT)
  #undef FR_1_ERAY_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_ERAY_STOP_SEC_VAR_NOINIT_16BIT)
  #undef FR_1_ERAY_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_ERAY_START_SEC_VAR_NOINIT_32BIT)
  #undef FR_1_ERAY_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_ERAY_STOP_SEC_VAR_NOINIT_32BIT)
  #undef FR_1_ERAY_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_ERAY_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef FR_1_ERAY_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FR_1_ERAY_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef FR_1_ERAY_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FR_1_ERAY_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef FR_1_ERAY_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_ERAY_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef FR_1_ERAY_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_ERAY_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef FR_1_ERAY_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_ERAY_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef FR_1_ERAY_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_ERAY_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef FR_1_ERAY_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_ERAY_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef FR_1_ERAY_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_ERAY_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef FR_1_ERAY_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FR_1_ERAY_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef FR_1_ERAY_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FR_1_ERAY_START_SEC_VAR_FAST_8BIT)
  #undef FR_1_ERAY_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_ERAY_STOP_SEC_VAR_FAST_8BIT)
  #undef FR_1_ERAY_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_ERAY_START_SEC_VAR_FAST_16BIT)
  #undef FR_1_ERAY_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_ERAY_STOP_SEC_VAR_FAST_16BIT)
  #undef FR_1_ERAY_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_ERAY_START_SEC_VAR_FAST_32BIT)
  #undef FR_1_ERAY_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_ERAY_STOP_SEC_VAR_FAST_32BIT)
  #undef FR_1_ERAY_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_ERAY_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef FR_1_ERAY_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FR_1_ERAY_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef FR_1_ERAY_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FR_1_ERAY_START_SEC_VAR_8BIT)
  #undef FR_1_ERAY_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_ERAY_STOP_SEC_VAR_8BIT)
  #undef FR_1_ERAY_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_ERAY_START_SEC_VAR_16BIT)
  #undef FR_1_ERAY_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_ERAY_STOP_SEC_VAR_16BIT)
  #undef FR_1_ERAY_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_ERAY_START_SEC_VAR_32BIT)
  #undef FR_1_ERAY_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_ERAY_STOP_SEC_VAR_32BIT)
  #undef FR_1_ERAY_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_ERAY_START_SEC_VAR_UNSPECIFIED)
  #undef FR_1_ERAY_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FR_1_ERAY_STOP_SEC_VAR_UNSPECIFIED)
  #undef FR_1_ERAY_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FR_1_ERAY_START_SEC_CONST_8BIT)
  #undef FR_1_ERAY_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_ERAY_STOP_SEC_CONST_8BIT)
  #undef FR_1_ERAY_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_ERAY_START_SEC_CONST_16BIT)
  #undef FR_1_ERAY_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_ERAY_STOP_SEC_CONST_16BIT)
  #undef FR_1_ERAY_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_ERAY_START_SEC_CONST_32BIT)
  #undef FR_1_ERAY_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_ERAY_STOP_SEC_CONST_32BIT)
  #undef FR_1_ERAY_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_ERAY_START_SEC_CONST_UNSPECIFIED)
  #undef FR_1_ERAY_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FR_1_ERAY_STOP_SEC_CONST_UNSPECIFIED)
  #undef FR_1_ERAY_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FR_1_ERAY_START_CONFIG_DATA_8BIT)
  #undef FR_1_ERAY_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_ERAY_STOP_CONFIG_DATA_8BIT)
  #undef FR_1_ERAY_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_ERAY_START_CONFIG_DATA_16BIT)
  #undef FR_1_ERAY_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_ERAY_STOP_CONFIG_DATA_16BIT)
  #undef FR_1_ERAY_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_ERAY_START_CONFIG_DATA_32BIT)
  #undef FR_1_ERAY_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_ERAY_STOP_CONFIG_DATA_32BIT)
  #undef FR_1_ERAY_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_ERAY_START_CONFIG_DATA_UNSPECIFIED)
  #undef FR_1_ERAY_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FR_1_ERAY_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef FR_1_ERAY_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined FR_1_MFR4300_START_SEC_CODE)
  #undef FR_1_MFR4300_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined FR_1_MFR4300_STOP_SEC_CODE)
  #undef FR_1_MFR4300_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined FR_1_MFR4300_START_SEC_APPL_CODE)
  #undef FR_1_MFR4300_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined FR_1_MFR4300_STOP_SEC_APPL_CODE)
  #undef FR_1_MFR4300_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined FR_1_MFR4300_START_SEC_VAR_NOINIT_8BIT)
  #undef FR_1_MFR4300_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_MFR4300_STOP_SEC_VAR_NOINIT_8BIT)
  #undef FR_1_MFR4300_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_MFR4300_START_SEC_VAR_NOINIT_16BIT)
  #undef FR_1_MFR4300_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_MFR4300_STOP_SEC_VAR_NOINIT_16BIT)
  #undef FR_1_MFR4300_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_MFR4300_START_SEC_VAR_NOINIT_32BIT)
  #undef FR_1_MFR4300_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_MFR4300_STOP_SEC_VAR_NOINIT_32BIT)
  #undef FR_1_MFR4300_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_MFR4300_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef FR_1_MFR4300_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FR_1_MFR4300_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef FR_1_MFR4300_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FR_1_MFR4300_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef FR_1_MFR4300_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_MFR4300_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef FR_1_MFR4300_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_MFR4300_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef FR_1_MFR4300_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_MFR4300_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef FR_1_MFR4300_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_MFR4300_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef FR_1_MFR4300_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_MFR4300_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef FR_1_MFR4300_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_MFR4300_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef FR_1_MFR4300_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FR_1_MFR4300_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef FR_1_MFR4300_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FR_1_MFR4300_START_SEC_VAR_FAST_8BIT)
  #undef FR_1_MFR4300_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_MFR4300_STOP_SEC_VAR_FAST_8BIT)
  #undef FR_1_MFR4300_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_MFR4300_START_SEC_VAR_FAST_16BIT)
  #undef FR_1_MFR4300_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_MFR4300_STOP_SEC_VAR_FAST_16BIT)
  #undef FR_1_MFR4300_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_MFR4300_START_SEC_VAR_FAST_32BIT)
  #undef FR_1_MFR4300_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_MFR4300_STOP_SEC_VAR_FAST_32BIT)
  #undef FR_1_MFR4300_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_MFR4300_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef FR_1_MFR4300_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FR_1_MFR4300_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef FR_1_MFR4300_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FR_1_MFR4300_START_SEC_VAR_8BIT)
  #undef FR_1_MFR4300_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_MFR4300_STOP_SEC_VAR_8BIT)
  #undef FR_1_MFR4300_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_MFR4300_START_SEC_VAR_16BIT)
  #undef FR_1_MFR4300_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_MFR4300_STOP_SEC_VAR_16BIT)
  #undef FR_1_MFR4300_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_MFR4300_START_SEC_VAR_32BIT)
  #undef FR_1_MFR4300_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_MFR4300_STOP_SEC_VAR_32BIT)
  #undef FR_1_MFR4300_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_MFR4300_START_SEC_VAR_UNSPECIFIED)
  #undef FR_1_MFR4300_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FR_1_MFR4300_STOP_SEC_VAR_UNSPECIFIED)
  #undef FR_1_MFR4300_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FR_1_MFR4300_START_SEC_CONST_8BIT)
  #undef FR_1_MFR4300_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_MFR4300_STOP_SEC_CONST_8BIT)
  #undef FR_1_MFR4300_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_MFR4300_START_SEC_CONST_16BIT)
  #undef FR_1_MFR4300_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_MFR4300_STOP_SEC_CONST_16BIT)
  #undef FR_1_MFR4300_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_MFR4300_START_SEC_CONST_32BIT)
  #undef FR_1_MFR4300_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_MFR4300_STOP_SEC_CONST_32BIT)
  #undef FR_1_MFR4300_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_MFR4300_START_SEC_CONST_UNSPECIFIED)
  #undef FR_1_MFR4300_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FR_1_MFR4300_STOP_SEC_CONST_UNSPECIFIED)
  #undef FR_1_MFR4300_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FR_1_MFR4300_START_CONFIG_DATA_8BIT)
  #undef FR_1_MFR4300_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_MFR4300_STOP_CONFIG_DATA_8BIT)
  #undef FR_1_MFR4300_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_MFR4300_START_CONFIG_DATA_16BIT)
  #undef FR_1_MFR4300_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_MFR4300_STOP_CONFIG_DATA_16BIT)
  #undef FR_1_MFR4300_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_MFR4300_START_CONFIG_DATA_32BIT)
  #undef FR_1_MFR4300_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_MFR4300_STOP_CONFIG_DATA_32BIT)
  #undef FR_1_MFR4300_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined FR_1_MFR4300_START_CONFIG_DATA_UNSPECIFIED)
  #undef FR_1_MFR4300_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FR_1_MFR4300_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef FR_1_MFR4300_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

/*------------------[Start of a module]--------------------------------------*/

#elif (defined FR_59_RENESAS_START_SEC_CODE)
  #undef FR_59_RENESAS_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_STOP_SEC_CODE)
  #undef FR_59_RENESAS_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_START_SEC_PUBLIC_CODE)
  #undef FR_59_RENESAS_START_SEC_PUBLIC_CODE
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_STOP_SEC_PUBLIC_CODE)
  #undef FR_59_RENESAS_STOP_SEC_PUBLIC_CODE
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_START_SEC_DBTOC_DATA_UNSPECIFIED)
  #undef FR_59_RENESAS_START_SEC_DBTOC_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_STOP_SEC_DBTOC_DATA_UNSPECIFIED)
  #undef FR_59_RENESAS_STOP_SEC_DBTOC_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_START_SEC_CONFIG_DATA_UNSPECIFIED)
  #undef FR_59_RENESAS_START_SEC_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_STOP_SEC_CONFIG_DATA_UNSPECIFIED)
  #undef FR_59_RENESAS_STOP_SEC_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_START_SEC_PRIVATE_CODE)
  #undef FR_59_RENESAS_START_SEC_PRIVATE_CODE
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_STOP_SEC_PRIVATE_CODE)
  #undef FR_59_RENESAS_STOP_SEC_PRIVATE_CODE
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_START_SEC_VAR_NOINIT_16)
  #undef FR_59_RENESAS_START_SEC_VAR_NOINIT_16
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_STOP_SEC_VAR_NOINIT_16)
  #undef FR_59_RENESAS_STOP_SEC_VAR_NOINIT_16
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_START_SEC_VAR_NOINIT_32)
  #undef FR_59_RENESAS_START_SEC_VAR_NOINIT_32
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_STOP_SEC_VAR_NOINIT_32)
  #undef FR_59_RENESAS_STOP_SEC_VAR_NOINIT_32
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_START_SEC_APPL_CODE)
  #undef FR_59_RENESAS_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_STOP_SEC_APPL_CODE)
  #undef FR_59_RENESAS_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_START_SEC_VAR_NOINIT_8BIT)
  #undef FR_59_RENESAS_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_STOP_SEC_VAR_NOINIT_8BIT)
  #undef FR_59_RENESAS_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_START_SEC_VAR_NOINIT_16BIT)
  #undef FR_59_RENESAS_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_STOP_SEC_VAR_NOINIT_16BIT)
  #undef FR_59_RENESAS_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_START_SEC_VAR_NOINIT_32BIT)
  #undef FR_59_RENESAS_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_STOP_SEC_VAR_NOINIT_32BIT)
  #undef FR_59_RENESAS_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef FR_59_RENESAS_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef FR_59_RENESAS_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef FR_59_RENESAS_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef FR_59_RENESAS_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef FR_59_RENESAS_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef FR_59_RENESAS_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef FR_59_RENESAS_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef FR_59_RENESAS_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef FR_59_RENESAS_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef FR_59_RENESAS_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_START_SEC_VAR_FAST_8BIT)
  #undef FR_59_RENESAS_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_STOP_SEC_VAR_FAST_8BIT)
  #undef FR_59_RENESAS_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_START_SEC_VAR_FAST_16BIT)
  #undef FR_59_RENESAS_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_STOP_SEC_VAR_FAST_16BIT)
  #undef FR_59_RENESAS_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_START_SEC_VAR_FAST_32BIT)
  #undef FR_59_RENESAS_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_STOP_SEC_VAR_FAST_32BIT)
  #undef FR_59_RENESAS_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef FR_59_RENESAS_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef FR_59_RENESAS_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_START_SEC_VAR_8BIT)
  #undef FR_59_RENESAS_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_STOP_SEC_VAR_8BIT)
  #undef FR_59_RENESAS_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_START_SEC_VAR_16BIT)
  #undef FR_59_RENESAS_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_STOP_SEC_VAR_16BIT)
  #undef FR_59_RENESAS_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_START_SEC_VAR_32BIT)
  #undef FR_59_RENESAS_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_STOP_SEC_VAR_32BIT)
  #undef FR_59_RENESAS_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_START_SEC_VAR_UNSPECIFIED)
  #undef FR_59_RENESAS_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_STOP_SEC_VAR_UNSPECIFIED)
  #undef FR_59_RENESAS_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_START_SEC_CONST_8BIT)
  #undef FR_59_RENESAS_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_STOP_SEC_CONST_8BIT)
  #undef FR_59_RENESAS_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_START_SEC_CONST_16BIT)
  #undef FR_59_RENESAS_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_STOP_SEC_CONST_16BIT)
  #undef FR_59_RENESAS_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_START_SEC_CONST_32BIT)
  #undef FR_59_RENESAS_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_STOP_SEC_CONST_32BIT)
  #undef FR_59_RENESAS_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_START_SEC_CONST_UNSPECIFIED)
  #undef FR_59_RENESAS_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_STOP_SEC_CONST_UNSPECIFIED)
  #undef FR_59_RENESAS_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_START_CONFIG_DATA_8BIT)
  #undef FR_59_RENESAS_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_STOP_CONFIG_DATA_8BIT)
  #undef FR_59_RENESAS_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_START_CONFIG_DATA_16BIT)
  #undef FR_59_RENESAS_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_STOP_CONFIG_DATA_16BIT)
  #undef FR_59_RENESAS_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_START_CONFIG_DATA_32BIT)
  #undef FR_59_RENESAS_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_STOP_CONFIG_DATA_32BIT)
  #undef FR_59_RENESAS_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_START_CONFIG_DATA_UNSPECIFIED)
  #undef FR_59_RENESAS_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef FR_59_RENESAS_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_START_SEC_VAR_BOOLEAN)
  #undef FR_59_RENESAS_START_SEC_VAR_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_STOP_SEC_VAR_BOOLEAN)
  #undef FR_59_RENESAS_STOP_SEC_VAR_BOOLEAN
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_START_SEC_VAR_NOINIT_32)
  #undef FR_59_RENESAS_START_SEC_VAR_NOINIT_32
  #undef MEMMAP_ERROR

#elif (defined FR_59_RENESAS_STOP_SEC_VAR_NOINIT_32)
  #undef FR_59_RENESAS_STOP_SEC_VAR_NOINIT_32
  #undef MEMMAP_ERROR

/* ---------------------------------------- Start of a Module ---------------------------*/
#elif (defined GPT_START_SEC_CODE)
  #undef GPT_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined GPT_STOP_SEC_CODE)
  #undef GPT_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined GPT_START_SEC_APPL_CODE)
  #undef GPT_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined GPT_STOP_SEC_APPL_CODE)
  #undef GPT_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined GPT_START_SEC_VAR_NOINIT_8BIT)
  #undef GPT_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined GPT_STOP_SEC_VAR_NOINIT_8BIT)
  #undef GPT_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined GPT_START_SEC_VAR_NOINIT_16BIT)
  #undef GPT_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined GPT_STOP_SEC_VAR_NOINIT_16BIT)
  #undef GPT_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined GPT_START_SEC_VAR_NOINIT_32BIT)
  #undef GPT_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined GPT_STOP_SEC_VAR_NOINIT_32BIT)
  #undef GPT_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined GPT_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef GPT_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined GPT_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef GPT_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined GPT_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef GPT_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined GPT_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef GPT_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined GPT_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef GPT_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined GPT_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef GPT_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined GPT_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef GPT_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined GPT_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef GPT_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined GPT_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef GPT_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined GPT_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef GPT_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined GPT_START_SEC_VAR_FAST_8BIT)
  #undef GPT_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined GPT_STOP_SEC_VAR_FAST_8BIT)
  #undef GPT_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined GPT_START_SEC_VAR_FAST_16BIT)
  #undef GPT_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined GPT_STOP_SEC_VAR_FAST_16BIT)
  #undef GPT_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined GPT_START_SEC_VAR_FAST_32BIT)
  #undef GPT_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined GPT_STOP_SEC_VAR_FAST_32BIT)
  #undef GPT_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined GPT_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef GPT_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined GPT_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef GPT_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined GPT_START_SEC_VAR_8BIT)
  #undef GPT_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined GPT_STOP_SEC_VAR_8BIT)
  #undef GPT_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined GPT_START_SEC_VAR_16BIT)
  #undef GPT_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined GPT_STOP_SEC_VAR_16BIT)
  #undef GPT_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined GPT_START_SEC_VAR_32BIT)
  #undef GPT_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined GPT_STOP_SEC_VAR_32BIT)
  #undef GPT_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined GPT_START_SEC_VAR_UNSPECIFIED)
  #undef GPT_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined GPT_STOP_SEC_VAR_UNSPECIFIED)
  #undef GPT_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined GPT_START_SEC_CONST_8BIT)
  #undef GPT_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined GPT_STOP_SEC_CONST_8BIT)
  #undef GPT_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined GPT_START_SEC_CONST_16BIT)
  #undef GPT_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined GPT_STOP_SEC_CONST_16BIT)
  #undef GPT_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined GPT_START_SEC_CONST_32BIT)
  #undef GPT_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined GPT_STOP_SEC_CONST_32BIT)
  #undef GPT_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined GPT_START_SEC_CONST_UNSPECIFIED)
  #undef GPT_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined GPT_STOP_SEC_CONST_UNSPECIFIED)
  #undef GPT_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined GPT_START_CONFIG_DATA_8BIT)
  #undef GPT_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined GPT_STOP_CONFIG_DATA_8BIT)
  #undef GPT_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined GPT_START_CONFIG_DATA_16BIT)
  #undef GPT_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined GPT_STOP_CONFIG_DATA_16BIT)
  #undef GPT_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined GPT_START_CONFIG_DATA_32BIT)
  #undef GPT_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined GPT_STOP_CONFIG_DATA_32BIT)
  #undef GPT_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined GPT_START_CONFIG_DATA_UNSPECIFIED)
  #undef GPT_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined GPT_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef GPT_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined GW_START_SEC_CODE)
  #undef GW_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined GW_STOP_SEC_CODE)
  #undef GW_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined GW_START_SEC_APPL_CODE)
  #undef GW_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined GW_STOP_SEC_APPL_CODE)
  #undef GW_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined GW_START_SEC_VAR_NOINIT_8BIT)
  #undef GW_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined GW_STOP_SEC_VAR_NOINIT_8BIT)
  #undef GW_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined GW_START_SEC_VAR_NOINIT_16BIT)
  #undef GW_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined GW_STOP_SEC_VAR_NOINIT_16BIT)
  #undef GW_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined GW_START_SEC_VAR_NOINIT_32BIT)
  #undef GW_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined GW_STOP_SEC_VAR_NOINIT_32BIT)
  #undef GW_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined GW_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef GW_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined GW_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef GW_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined GW_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef GW_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined GW_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef GW_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined GW_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef GW_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined GW_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef GW_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined GW_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef GW_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined GW_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef GW_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined GW_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef GW_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined GW_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef GW_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined GW_START_SEC_VAR_FAST_8BIT)
  #undef GW_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined GW_STOP_SEC_VAR_FAST_8BIT)
  #undef GW_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined GW_START_SEC_VAR_FAST_16BIT)
  #undef GW_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined GW_STOP_SEC_VAR_FAST_16BIT)
  #undef GW_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined GW_START_SEC_VAR_FAST_32BIT)
  #undef GW_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined GW_STOP_SEC_VAR_FAST_32BIT)
  #undef GW_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined GW_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef GW_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined GW_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef GW_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined GW_START_SEC_VAR_8BIT)
  #undef GW_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined GW_STOP_SEC_VAR_8BIT)
  #undef GW_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined GW_START_SEC_VAR_16BIT)
  #undef GW_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined GW_STOP_SEC_VAR_16BIT)
  #undef GW_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined GW_START_SEC_VAR_32BIT)
  #undef GW_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined GW_STOP_SEC_VAR_32BIT)
  #undef GW_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined GW_START_SEC_VAR_UNSPECIFIED)
  #undef GW_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined GW_STOP_SEC_VAR_UNSPECIFIED)
  #undef GW_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined GW_START_SEC_CONST_8BIT)
  #undef GW_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined GW_STOP_SEC_CONST_8BIT)
  #undef GW_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined GW_START_SEC_CONST_16BIT)
  #undef GW_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined GW_STOP_SEC_CONST_16BIT)
  #undef GW_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined GW_START_SEC_CONST_32BIT)
  #undef GW_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined GW_STOP_SEC_CONST_32BIT)
  #undef GW_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined GW_START_SEC_CONST_UNSPECIFIED)
  #undef GW_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined GW_STOP_SEC_CONST_UNSPECIFIED)
  #undef GW_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined GW_START_CONFIG_DATA_8BIT)
  #undef GW_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined GW_STOP_CONFIG_DATA_8BIT)
  #undef GW_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined GW_START_CONFIG_DATA_16BIT)
  #undef GW_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined GW_STOP_CONFIG_DATA_16BIT)
  #undef GW_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined GW_START_CONFIG_DATA_32BIT)
  #undef GW_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined GW_STOP_CONFIG_DATA_32BIT)
  #undef GW_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined GW_START_CONFIG_DATA_UNSPECIFIED)
  #undef GW_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined GW_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef GW_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined ICU_START_SEC_CODE)
  #undef ICU_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined ICU_STOP_SEC_CODE)
  #undef ICU_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined ICU_START_SEC_APPL_CODE)
  #undef ICU_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined ICU_STOP_SEC_APPL_CODE)
  #undef ICU_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined ICU_START_SEC_VAR_NOINIT_8BIT)
  #undef ICU_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined ICU_STOP_SEC_VAR_NOINIT_8BIT)
  #undef ICU_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined ICU_START_SEC_VAR_NOINIT_16BIT)
  #undef ICU_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined ICU_STOP_SEC_VAR_NOINIT_16BIT)
  #undef ICU_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined ICU_START_SEC_VAR_NOINIT_32BIT)
  #undef ICU_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined ICU_STOP_SEC_VAR_NOINIT_32BIT)
  #undef ICU_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined ICU_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef ICU_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined ICU_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef ICU_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined ICU_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef ICU_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined ICU_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef ICU_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined ICU_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef ICU_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined ICU_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef ICU_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined ICU_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef ICU_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined ICU_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef ICU_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined ICU_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef ICU_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined ICU_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef ICU_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined ICU_START_SEC_VAR_FAST_8BIT)
  #undef ICU_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined ICU_STOP_SEC_VAR_FAST_8BIT)
  #undef ICU_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined ICU_START_SEC_VAR_FAST_16BIT)
  #undef ICU_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined ICU_STOP_SEC_VAR_FAST_16BIT)
  #undef ICU_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined ICU_START_SEC_VAR_FAST_32BIT)
  #undef ICU_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined ICU_STOP_SEC_VAR_FAST_32BIT)
  #undef ICU_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined ICU_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef ICU_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined ICU_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef ICU_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined ICU_START_SEC_VAR_8BIT)
  #undef ICU_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined ICU_STOP_SEC_VAR_8BIT)
  #undef ICU_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined ICU_START_SEC_VAR_16BIT)
  #undef ICU_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined ICU_STOP_SEC_VAR_16BIT)
  #undef ICU_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined ICU_START_SEC_VAR_32BIT)
  #undef ICU_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined ICU_STOP_SEC_VAR_32BIT)
  #undef ICU_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined ICU_START_SEC_VAR_UNSPECIFIED)
  #undef ICU_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined ICU_STOP_SEC_VAR_UNSPECIFIED)
  #undef ICU_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined ICU_START_SEC_CONST_8BIT)
  #undef ICU_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined ICU_STOP_SEC_CONST_8BIT)
  #undef ICU_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined ICU_START_SEC_CONST_16BIT)
  #undef ICU_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined ICU_STOP_SEC_CONST_16BIT)
  #undef ICU_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined ICU_START_SEC_CONST_32BIT)
  #undef ICU_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined ICU_STOP_SEC_CONST_32BIT)
  #undef ICU_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined ICU_START_SEC_CONST_UNSPECIFIED)
  #undef ICU_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined ICU_STOP_SEC_CONST_UNSPECIFIED)
  #undef ICU_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined ICU_START_CONFIG_DATA_8BIT)
  #undef ICU_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined ICU_STOP_CONFIG_DATA_8BIT)
  #undef ICU_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined ICU_START_CONFIG_DATA_16BIT)
  #undef ICU_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined ICU_STOP_CONFIG_DATA_16BIT)
  #undef ICU_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined ICU_START_CONFIG_DATA_32BIT)
  #undef ICU_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined ICU_STOP_CONFIG_DATA_32BIT)
  #undef ICU_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined ICU_START_CONFIG_DATA_UNSPECIFIED)
  #undef ICU_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined ICU_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef ICU_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined IOHWAB_START_SEC_CODE)
  #undef IOHWAB_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined IOHWAB_STOP_SEC_CODE)
  #undef IOHWAB_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined IOHWAB_START_SEC_APPL_CODE)
  #undef IOHWAB_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined IOHWAB_STOP_SEC_APPL_CODE)
  #undef IOHWAB_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined IOHWAB_START_SEC_VAR_NOINIT_8BIT)
  #undef IOHWAB_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined IOHWAB_STOP_SEC_VAR_NOINIT_8BIT)
  #undef IOHWAB_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined IOHWAB_START_SEC_VAR_NOINIT_16BIT)
  #undef IOHWAB_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined IOHWAB_STOP_SEC_VAR_NOINIT_16BIT)
  #undef IOHWAB_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined IOHWAB_START_SEC_VAR_NOINIT_32BIT)
  #undef IOHWAB_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined IOHWAB_STOP_SEC_VAR_NOINIT_32BIT)
  #undef IOHWAB_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined IOHWAB_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef IOHWAB_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined IOHWAB_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef IOHWAB_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined IOHWAB_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef IOHWAB_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined IOHWAB_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef IOHWAB_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined IOHWAB_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef IOHWAB_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined IOHWAB_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef IOHWAB_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined IOHWAB_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef IOHWAB_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined IOHWAB_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef IOHWAB_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined IOHWAB_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef IOHWAB_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined IOHWAB_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef IOHWAB_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined IOHWAB_START_SEC_VAR_FAST_8BIT)
  #undef IOHWAB_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined IOHWAB_STOP_SEC_VAR_FAST_8BIT)
  #undef IOHWAB_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined IOHWAB_START_SEC_VAR_FAST_16BIT)
  #undef IOHWAB_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined IOHWAB_STOP_SEC_VAR_FAST_16BIT)
  #undef IOHWAB_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined IOHWAB_START_SEC_VAR_FAST_32BIT)
  #undef IOHWAB_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined IOHWAB_STOP_SEC_VAR_FAST_32BIT)
  #undef IOHWAB_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined IOHWAB_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef IOHWAB_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined IOHWAB_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef IOHWAB_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined IOHWAB_START_SEC_VAR_8BIT)
  #undef IOHWAB_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined IOHWAB_STOP_SEC_VAR_8BIT)
  #undef IOHWAB_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined IOHWAB_START_SEC_VAR_16BIT)
  #undef IOHWAB_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined IOHWAB_STOP_SEC_VAR_16BIT)
  #undef IOHWAB_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined IOHWAB_START_SEC_VAR_32BIT)
  #undef IOHWAB_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined IOHWAB_STOP_SEC_VAR_32BIT)
  #undef IOHWAB_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined IOHWAB_START_SEC_VAR_UNSPECIFIED)
  #undef IOHWAB_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined IOHWAB_STOP_SEC_VAR_UNSPECIFIED)
  #undef IOHWAB_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined IOHWAB_START_SEC_CONST_8BIT)
  #undef IOHWAB_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined IOHWAB_STOP_SEC_CONST_8BIT)
  #undef IOHWAB_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined IOHWAB_START_SEC_CONST_16BIT)
  #undef IOHWAB_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined IOHWAB_STOP_SEC_CONST_16BIT)
  #undef IOHWAB_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined IOHWAB_START_SEC_CONST_32BIT)
  #undef IOHWAB_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined IOHWAB_STOP_SEC_CONST_32BIT)
  #undef IOHWAB_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined IOHWAB_START_SEC_CONST_UNSPECIFIED)
  #undef IOHWAB_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined IOHWAB_STOP_SEC_CONST_UNSPECIFIED)
  #undef IOHWAB_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined IOHWAB_START_CONFIG_DATA_8BIT)
  #undef IOHWAB_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined IOHWAB_STOP_CONFIG_DATA_8BIT)
  #undef IOHWAB_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined IOHWAB_START_CONFIG_DATA_16BIT)
  #undef IOHWAB_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined IOHWAB_STOP_CONFIG_DATA_16BIT)
  #undef IOHWAB_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined IOHWAB_START_CONFIG_DATA_32BIT)
  #undef IOHWAB_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined IOHWAB_STOP_CONFIG_DATA_32BIT)
  #undef IOHWAB_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined IOHWAB_START_CONFIG_DATA_UNSPECIFIED)
  #undef IOHWAB_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined IOHWAB_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef IOHWAB_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined IPDUM_START_SEC_CODE)
  #undef IPDUM_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined IPDUM_STOP_SEC_CODE)
  #undef IPDUM_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined IPDUM_START_SEC_APPL_CODE)
  #undef IPDUM_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined IPDUM_STOP_SEC_APPL_CODE)
  #undef IPDUM_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined IPDUM_START_SEC_VAR_NOINIT_8BIT)
  #undef IPDUM_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined IPDUM_STOP_SEC_VAR_NOINIT_8BIT)
  #undef IPDUM_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined IPDUM_START_SEC_VAR_NOINIT_16BIT)
  #undef IPDUM_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined IPDUM_STOP_SEC_VAR_NOINIT_16BIT)
  #undef IPDUM_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined IPDUM_START_SEC_VAR_NOINIT_32BIT)
  #undef IPDUM_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined IPDUM_STOP_SEC_VAR_NOINIT_32BIT)
  #undef IPDUM_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined IPDUM_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef IPDUM_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined IPDUM_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef IPDUM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined IPDUM_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef IPDUM_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined IPDUM_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef IPDUM_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined IPDUM_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef IPDUM_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined IPDUM_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef IPDUM_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined IPDUM_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef IPDUM_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined IPDUM_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef IPDUM_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined IPDUM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef IPDUM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined IPDUM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef IPDUM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined IPDUM_START_SEC_VAR_FAST_8BIT)
  #undef IPDUM_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined IPDUM_STOP_SEC_VAR_FAST_8BIT)
  #undef IPDUM_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined IPDUM_START_SEC_VAR_FAST_16BIT)
  #undef IPDUM_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined IPDUM_STOP_SEC_VAR_FAST_16BIT)
  #undef IPDUM_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined IPDUM_START_SEC_VAR_FAST_32BIT)
  #undef IPDUM_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined IPDUM_STOP_SEC_VAR_FAST_32BIT)
  #undef IPDUM_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined IPDUM_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef IPDUM_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined IPDUM_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef IPDUM_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined IPDUM_START_SEC_VAR_8BIT)
  #undef IPDUM_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined IPDUM_STOP_SEC_VAR_8BIT)
  #undef IPDUM_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined IPDUM_START_SEC_VAR_16BIT)
  #undef IPDUM_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined IPDUM_STOP_SEC_VAR_16BIT)
  #undef IPDUM_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined IPDUM_START_SEC_VAR_32BIT)
  #undef IPDUM_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined IPDUM_STOP_SEC_VAR_32BIT)
  #undef IPDUM_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined IPDUM_START_SEC_VAR_UNSPECIFIED)
  #undef IPDUM_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined IPDUM_STOP_SEC_VAR_UNSPECIFIED)
  #undef IPDUM_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined IPDUM_START_SEC_CONST_8BIT)
  #undef IPDUM_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined IPDUM_STOP_SEC_CONST_8BIT)
  #undef IPDUM_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined IPDUM_START_SEC_CONST_16BIT)
  #undef IPDUM_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined IPDUM_STOP_SEC_CONST_16BIT)
  #undef IPDUM_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined IPDUM_START_SEC_CONST_32BIT)
  #undef IPDUM_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined IPDUM_STOP_SEC_CONST_32BIT)
  #undef IPDUM_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined IPDUM_START_SEC_CONST_UNSPECIFIED)
  #undef IPDUM_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined IPDUM_STOP_SEC_CONST_UNSPECIFIED)
  #undef IPDUM_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined IPDUM_START_CONFIG_DATA_8BIT)
  #undef IPDUM_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined IPDUM_STOP_CONFIG_DATA_8BIT)
  #undef IPDUM_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined IPDUM_START_CONFIG_DATA_16BIT)
  #undef IPDUM_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined IPDUM_STOP_CONFIG_DATA_16BIT)
  #undef IPDUM_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined IPDUM_START_CONFIG_DATA_32BIT)
  #undef IPDUM_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined IPDUM_STOP_CONFIG_DATA_32BIT)
  #undef IPDUM_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined IPDUM_START_CONFIG_DATA_UNSPECIFIED)
  #undef IPDUM_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined IPDUM_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef IPDUM_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined J1939DM_START_SEC_CODE)
  #undef J1939DM_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined J1939DM_STOP_SEC_CODE)
  #undef J1939DM_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined J1939DM_START_SEC_APPL_CODE)
  #undef J1939DM_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined J1939DM_STOP_SEC_APPL_CODE)
  #undef J1939DM_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined J1939DM_START_SEC_VAR_NOINIT_8BIT)
  #undef J1939DM_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined J1939DM_STOP_SEC_VAR_NOINIT_8BIT)
  #undef J1939DM_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined J1939DM_START_SEC_VAR_NOINIT_16BIT)
  #undef J1939DM_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined J1939DM_STOP_SEC_VAR_NOINIT_16BIT)
  #undef J1939DM_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined J1939DM_START_SEC_VAR_NOINIT_32BIT)
  #undef J1939DM_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined J1939DM_STOP_SEC_VAR_NOINIT_32BIT)
  #undef J1939DM_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined J1939DM_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef J1939DM_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined J1939DM_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef J1939DM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined J1939DM_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef J1939DM_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined J1939DM_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef J1939DM_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined J1939DM_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef J1939DM_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined J1939DM_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef J1939DM_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined J1939DM_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef J1939DM_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined J1939DM_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef J1939DM_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined J1939DM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef J1939DM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined J1939DM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef J1939DM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined J1939DM_START_SEC_VAR_FAST_8BIT)
  #undef J1939DM_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined J1939DM_STOP_SEC_VAR_FAST_8BIT)
  #undef J1939DM_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined J1939DM_START_SEC_VAR_FAST_16BIT)
  #undef J1939DM_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined J1939DM_STOP_SEC_VAR_FAST_16BIT)
  #undef J1939DM_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined J1939DM_START_SEC_VAR_FAST_32BIT)
  #undef J1939DM_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined J1939DM_STOP_SEC_VAR_FAST_32BIT)
  #undef J1939DM_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined J1939DM_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef J1939DM_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined J1939DM_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef J1939DM_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined J1939DM_START_SEC_VAR_8BIT)
  #undef J1939DM_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined J1939DM_STOP_SEC_VAR_8BIT)
  #undef J1939DM_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined J1939DM_START_SEC_VAR_16BIT)
  #undef J1939DM_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined J1939DM_STOP_SEC_VAR_16BIT)
  #undef J1939DM_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined J1939DM_START_SEC_VAR_32BIT)
  #undef J1939DM_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined J1939DM_STOP_SEC_VAR_32BIT)
  #undef J1939DM_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined J1939DM_START_SEC_VAR_UNSPECIFIED)
  #undef J1939DM_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined J1939DM_STOP_SEC_VAR_UNSPECIFIED)
  #undef J1939DM_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined J1939DM_START_SEC_CONST_8BIT)
  #undef J1939DM_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined J1939DM_STOP_SEC_CONST_8BIT)
  #undef J1939DM_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined J1939DM_START_SEC_CONST_16BIT)
  #undef J1939DM_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined J1939DM_STOP_SEC_CONST_16BIT)
  #undef J1939DM_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined J1939DM_START_SEC_CONST_32BIT)
  #undef J1939DM_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined J1939DM_STOP_SEC_CONST_32BIT)
  #undef J1939DM_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined J1939DM_START_SEC_CONST_UNSPECIFIED)
  #undef J1939DM_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined J1939DM_STOP_SEC_CONST_UNSPECIFIED)
  #undef J1939DM_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined J1939DM_START_CONFIG_DATA_8BIT)
  #undef J1939DM_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined J1939DM_STOP_CONFIG_DATA_8BIT)
  #undef J1939DM_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined J1939DM_START_CONFIG_DATA_16BIT)
  #undef J1939DM_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined J1939DM_STOP_CONFIG_DATA_16BIT)
  #undef J1939DM_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined J1939DM_START_CONFIG_DATA_32BIT)
  #undef J1939DM_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined J1939DM_STOP_CONFIG_DATA_32BIT)
  #undef J1939DM_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined J1939DM_START_CONFIG_DATA_UNSPECIFIED)
  #undef J1939DM_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined J1939DM_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef J1939DM_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined J1939RSM_START_SEC_CODE)
  #undef J1939RSM_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined J1939RSM_STOP_SEC_CODE)
  #undef J1939RSM_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined J1939RSM_START_SEC_APPL_CODE)
  #undef J1939RSM_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined J1939RSM_STOP_SEC_APPL_CODE)
  #undef J1939RSM_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined J1939RSM_START_SEC_VAR_NOINIT_8BIT)
  #undef J1939RSM_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined J1939RSM_STOP_SEC_VAR_NOINIT_8BIT)
  #undef J1939RSM_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined J1939RSM_START_SEC_VAR_NOINIT_16BIT)
  #undef J1939RSM_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined J1939RSM_STOP_SEC_VAR_NOINIT_16BIT)
  #undef J1939RSM_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined J1939RSM_START_SEC_VAR_NOINIT_32BIT)
  #undef J1939RSM_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined J1939RSM_STOP_SEC_VAR_NOINIT_32BIT)
  #undef J1939RSM_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined J1939RSM_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef J1939RSM_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined J1939RSM_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef J1939RSM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined J1939RSM_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef J1939RSM_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined J1939RSM_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef J1939RSM_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined J1939RSM_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef J1939RSM_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined J1939RSM_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef J1939RSM_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined J1939RSM_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef J1939RSM_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined J1939RSM_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef J1939RSM_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined J1939RSM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef J1939RSM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined J1939RSM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef J1939RSM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined J1939RSM_START_SEC_VAR_FAST_8BIT)
  #undef J1939RSM_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined J1939RSM_STOP_SEC_VAR_FAST_8BIT)
  #undef J1939RSM_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined J1939RSM_START_SEC_VAR_FAST_16BIT)
  #undef J1939RSM_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined J1939RSM_STOP_SEC_VAR_FAST_16BIT)
  #undef J1939RSM_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined J1939RSM_START_SEC_VAR_FAST_32BIT)
  #undef J1939RSM_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined J1939RSM_STOP_SEC_VAR_FAST_32BIT)
  #undef J1939RSM_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined J1939RSM_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef J1939RSM_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined J1939RSM_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef J1939RSM_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined J1939RSM_START_SEC_VAR_8BIT)
  #undef J1939RSM_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined J1939RSM_STOP_SEC_VAR_8BIT)
  #undef J1939RSM_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined J1939RSM_START_SEC_VAR_16BIT)
  #undef J1939RSM_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined J1939RSM_STOP_SEC_VAR_16BIT)
  #undef J1939RSM_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined J1939RSM_START_SEC_VAR_32BIT)
  #undef J1939RSM_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined J1939RSM_STOP_SEC_VAR_32BIT)
  #undef J1939RSM_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined J1939RSM_START_SEC_VAR_UNSPECIFIED)
  #undef J1939RSM_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined J1939RSM_STOP_SEC_VAR_UNSPECIFIED)
  #undef J1939RSM_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined J1939RSM_START_SEC_CONST_8BIT)
  #undef J1939RSM_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined J1939RSM_STOP_SEC_CONST_8BIT)
  #undef J1939RSM_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined J1939RSM_START_SEC_CONST_16BIT)
  #undef J1939RSM_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined J1939RSM_STOP_SEC_CONST_16BIT)
  #undef J1939RSM_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined J1939RSM_START_SEC_CONST_32BIT)
  #undef J1939RSM_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined J1939RSM_STOP_SEC_CONST_32BIT)
  #undef J1939RSM_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined J1939RSM_START_SEC_CONST_UNSPECIFIED)
  #undef J1939RSM_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined J1939RSM_STOP_SEC_CONST_UNSPECIFIED)
  #undef J1939RSM_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined J1939RSM_START_CONFIG_DATA_8BIT)
  #undef J1939RSM_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined J1939RSM_STOP_CONFIG_DATA_8BIT)
  #undef J1939RSM_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined J1939RSM_START_CONFIG_DATA_16BIT)
  #undef J1939RSM_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined J1939RSM_STOP_CONFIG_DATA_16BIT)
  #undef J1939RSM_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined J1939RSM_START_CONFIG_DATA_32BIT)
  #undef J1939RSM_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined J1939RSM_STOP_CONFIG_DATA_32BIT)
  #undef J1939RSM_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined J1939RSM_START_CONFIG_DATA_UNSPECIFIED)
  #undef J1939RSM_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined J1939RSM_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef J1939RSM_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined J1939TP_START_SEC_CODE)
  #undef J1939TP_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined J1939TP_STOP_SEC_CODE)
  #undef J1939TP_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined J1939TP_START_SEC_APPL_CODE)
  #undef J1939TP_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined J1939TP_STOP_SEC_APPL_CODE)
  #undef J1939TP_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined J1939TP_START_SEC_VAR_NOINIT_8BIT)
  #undef J1939TP_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined J1939TP_STOP_SEC_VAR_NOINIT_8BIT)
  #undef J1939TP_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined J1939TP_START_SEC_VAR_NOINIT_16BIT)
  #undef J1939TP_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined J1939TP_STOP_SEC_VAR_NOINIT_16BIT)
  #undef J1939TP_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined J1939TP_START_SEC_VAR_NOINIT_32BIT)
  #undef J1939TP_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined J1939TP_STOP_SEC_VAR_NOINIT_32BIT)
  #undef J1939TP_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined J1939TP_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef J1939TP_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined J1939TP_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef J1939TP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined J1939TP_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef J1939TP_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined J1939TP_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef J1939TP_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined J1939TP_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef J1939TP_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined J1939TP_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef J1939TP_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined J1939TP_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef J1939TP_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined J1939TP_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef J1939TP_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined J1939TP_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef J1939TP_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined J1939TP_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef J1939TP_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined J1939TP_START_SEC_VAR_FAST_8BIT)
  #undef J1939TP_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined J1939TP_STOP_SEC_VAR_FAST_8BIT)
  #undef J1939TP_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined J1939TP_START_SEC_VAR_FAST_16BIT)
  #undef J1939TP_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined J1939TP_STOP_SEC_VAR_FAST_16BIT)
  #undef J1939TP_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined J1939TP_START_SEC_VAR_FAST_32BIT)
  #undef J1939TP_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined J1939TP_STOP_SEC_VAR_FAST_32BIT)
  #undef J1939TP_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined J1939TP_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef J1939TP_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined J1939TP_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef J1939TP_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined J1939TP_START_SEC_VAR_8BIT)
  #undef J1939TP_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined J1939TP_STOP_SEC_VAR_8BIT)
  #undef J1939TP_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined J1939TP_START_SEC_VAR_16BIT)
  #undef J1939TP_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined J1939TP_STOP_SEC_VAR_16BIT)
  #undef J1939TP_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined J1939TP_START_SEC_VAR_32BIT)
  #undef J1939TP_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined J1939TP_STOP_SEC_VAR_32BIT)
  #undef J1939TP_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined J1939TP_START_SEC_VAR_UNSPECIFIED)
  #undef J1939TP_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined J1939TP_STOP_SEC_VAR_UNSPECIFIED)
  #undef J1939TP_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined J1939TP_START_SEC_CONST_8BIT)
  #undef J1939TP_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined J1939TP_STOP_SEC_CONST_8BIT)
  #undef J1939TP_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined J1939TP_START_SEC_CONST_16BIT)
  #undef J1939TP_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined J1939TP_STOP_SEC_CONST_16BIT)
  #undef J1939TP_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined J1939TP_START_SEC_CONST_32BIT)
  #undef J1939TP_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined J1939TP_STOP_SEC_CONST_32BIT)
  #undef J1939TP_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined J1939TP_START_SEC_CONST_UNSPECIFIED)
  #undef J1939TP_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined J1939TP_STOP_SEC_CONST_UNSPECIFIED)
  #undef J1939TP_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined J1939TP_START_CONFIG_DATA_8BIT)
  #undef J1939TP_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined J1939TP_STOP_CONFIG_DATA_8BIT)
  #undef J1939TP_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined J1939TP_START_CONFIG_DATA_16BIT)
  #undef J1939TP_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined J1939TP_STOP_CONFIG_DATA_16BIT)
  #undef J1939TP_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined J1939TP_START_CONFIG_DATA_32BIT)
  #undef J1939TP_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined J1939TP_STOP_CONFIG_DATA_32BIT)
  #undef J1939TP_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined J1939TP_START_CONFIG_DATA_UNSPECIFIED)
  #undef J1939TP_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined J1939TP_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef J1939TP_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined KS_START_SEC_CODE)
  #undef KS_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined KS_STOP_SEC_CODE)
  #undef KS_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined KS_START_SEC_APPL_CODE)
  #undef KS_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined KS_STOP_SEC_APPL_CODE)
  #undef KS_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined KS_START_SEC_VAR_NOINIT_8BIT)
  #undef KS_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined KS_STOP_SEC_VAR_NOINIT_8BIT)
  #undef KS_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined KS_START_SEC_VAR_NOINIT_16BIT)
  #undef KS_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined KS_STOP_SEC_VAR_NOINIT_16BIT)
  #undef KS_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined KS_START_SEC_VAR_NOINIT_32BIT)
  #undef KS_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined KS_STOP_SEC_VAR_NOINIT_32BIT)
  #undef KS_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined KS_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef KS_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined KS_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef KS_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined KS_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef KS_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined KS_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef KS_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined KS_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef KS_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined KS_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef KS_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined KS_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef KS_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined KS_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef KS_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined KS_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef KS_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined KS_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef KS_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined KS_START_SEC_VAR_FAST_8BIT)
  #undef KS_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined KS_STOP_SEC_VAR_FAST_8BIT)
  #undef KS_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined KS_START_SEC_VAR_FAST_16BIT)
  #undef KS_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined KS_STOP_SEC_VAR_FAST_16BIT)
  #undef KS_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined KS_START_SEC_VAR_FAST_32BIT)
  #undef KS_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined KS_STOP_SEC_VAR_FAST_32BIT)
  #undef KS_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined KS_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef KS_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined KS_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef KS_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined KS_START_SEC_VAR_8BIT)
  #undef KS_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined KS_STOP_SEC_VAR_8BIT)
  #undef KS_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined KS_START_SEC_VAR_16BIT)
  #undef KS_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined KS_STOP_SEC_VAR_16BIT)
  #undef KS_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined KS_START_SEC_VAR_32BIT)
  #undef KS_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined KS_STOP_SEC_VAR_32BIT)
  #undef KS_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined KS_START_SEC_VAR_UNSPECIFIED)
  #undef KS_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined KS_STOP_SEC_VAR_UNSPECIFIED)
  #undef KS_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined KS_START_SEC_CONST_8BIT)
  #undef KS_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined KS_STOP_SEC_CONST_8BIT)
  #undef KS_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined KS_START_SEC_CONST_16BIT)
  #undef KS_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined KS_STOP_SEC_CONST_16BIT)
  #undef KS_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined KS_START_SEC_CONST_32BIT)
  #undef KS_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined KS_STOP_SEC_CONST_32BIT)
  #undef KS_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined KS_START_SEC_CONST_UNSPECIFIED)
  #undef KS_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined KS_STOP_SEC_CONST_UNSPECIFIED)
  #undef KS_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined KS_START_CONFIG_DATA_8BIT)
  #undef KS_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined KS_STOP_CONFIG_DATA_8BIT)
  #undef KS_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined KS_START_CONFIG_DATA_16BIT)
  #undef KS_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined KS_STOP_CONFIG_DATA_16BIT)
  #undef KS_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined KS_START_CONFIG_DATA_32BIT)
  #undef KS_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined KS_STOP_CONFIG_DATA_32BIT)
  #undef KS_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined KS_START_CONFIG_DATA_UNSPECIFIED)
  #undef KS_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined KS_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef KS_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined LIN_START_SEC_CODE)
  #undef LIN_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined LIN_STOP_SEC_CODE)
  #undef LIN_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined LIN_START_SEC_APPL_CODE)
  #undef LIN_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined LIN_STOP_SEC_APPL_CODE)
  #undef LIN_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined LIN_START_SEC_VAR_NOINIT_8BIT)
  #undef LIN_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined LIN_STOP_SEC_VAR_NOINIT_8BIT)
  #undef LIN_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined LIN_START_SEC_VAR_NOINIT_16BIT)
  #undef LIN_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined LIN_STOP_SEC_VAR_NOINIT_16BIT)
  #undef LIN_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined LIN_START_SEC_VAR_NOINIT_32BIT)
  #undef LIN_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined LIN_STOP_SEC_VAR_NOINIT_32BIT)
  #undef LIN_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined LIN_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef LIN_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined LIN_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef LIN_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined LIN_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef LIN_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined LIN_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef LIN_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined LIN_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef LIN_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined LIN_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef LIN_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined LIN_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef LIN_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined LIN_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef LIN_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined LIN_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef LIN_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined LIN_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef LIN_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined LIN_START_SEC_VAR_FAST_8BIT)
  #undef LIN_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined LIN_STOP_SEC_VAR_FAST_8BIT)
  #undef LIN_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined LIN_START_SEC_VAR_FAST_16BIT)
  #undef LIN_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined LIN_STOP_SEC_VAR_FAST_16BIT)
  #undef LIN_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined LIN_START_SEC_VAR_FAST_32BIT)
  #undef LIN_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined LIN_STOP_SEC_VAR_FAST_32BIT)
  #undef LIN_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined LIN_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef LIN_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined LIN_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef LIN_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined LIN_START_SEC_VAR_8BIT)
  #undef LIN_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined LIN_STOP_SEC_VAR_8BIT)
  #undef LIN_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined LIN_START_SEC_VAR_16BIT)
  #undef LIN_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined LIN_STOP_SEC_VAR_16BIT)
  #undef LIN_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined LIN_START_SEC_VAR_32BIT)
  #undef LIN_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined LIN_STOP_SEC_VAR_32BIT)
  #undef LIN_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined LIN_START_SEC_VAR_UNSPECIFIED)
  #undef LIN_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined LIN_STOP_SEC_VAR_UNSPECIFIED)
  #undef LIN_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined LIN_START_SEC_CONST_8BIT)
  #undef LIN_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined LIN_STOP_SEC_CONST_8BIT)
  #undef LIN_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined LIN_START_SEC_CONST_16BIT)
  #undef LIN_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined LIN_STOP_SEC_CONST_16BIT)
  #undef LIN_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined LIN_START_SEC_CONST_32BIT)
  #undef LIN_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined LIN_STOP_SEC_CONST_32BIT)
  #undef LIN_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined LIN_START_SEC_CONST_UNSPECIFIED)
  #undef LIN_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined LIN_STOP_SEC_CONST_UNSPECIFIED)
  #undef LIN_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined LIN_START_CONFIG_DATA_8BIT)
  #undef LIN_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined LIN_STOP_CONFIG_DATA_8BIT)
  #undef LIN_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined LIN_START_CONFIG_DATA_16BIT)
  #undef LIN_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined LIN_STOP_CONFIG_DATA_16BIT)
  #undef LIN_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined LIN_START_CONFIG_DATA_32BIT)
  #undef LIN_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined LIN_STOP_CONFIG_DATA_32BIT)
  #undef LIN_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined LIN_START_CONFIG_DATA_UNSPECIFIED)
  #undef LIN_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined LIN_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef LIN_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined LINIF_START_SEC_CODE)
  #undef LINIF_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined LINIF_STOP_SEC_CODE)
  #undef LINIF_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined LINIF_START_SEC_APPL_CODE)
  #undef LINIF_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined LINIF_STOP_SEC_APPL_CODE)
  #undef LINIF_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined LINIF_START_SEC_VAR_NOINIT_8BIT)
  #undef LINIF_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined LINIF_STOP_SEC_VAR_NOINIT_8BIT)
  #undef LINIF_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined LINIF_START_SEC_VAR_NOINIT_16BIT)
  #undef LINIF_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined LINIF_STOP_SEC_VAR_NOINIT_16BIT)
  #undef LINIF_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined LINIF_START_SEC_VAR_NOINIT_32BIT)
  #undef LINIF_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined LINIF_STOP_SEC_VAR_NOINIT_32BIT)
  #undef LINIF_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined LINIF_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef LINIF_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined LINIF_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef LINIF_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined LINIF_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef LINIF_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined LINIF_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef LINIF_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined LINIF_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef LINIF_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined LINIF_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef LINIF_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined LINIF_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef LINIF_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined LINIF_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef LINIF_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined LINIF_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef LINIF_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined LINIF_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef LINIF_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined LINIF_START_SEC_VAR_FAST_8BIT)
  #undef LINIF_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined LINIF_STOP_SEC_VAR_FAST_8BIT)
  #undef LINIF_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined LINIF_START_SEC_VAR_FAST_16BIT)
  #undef LINIF_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined LINIF_STOP_SEC_VAR_FAST_16BIT)
  #undef LINIF_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined LINIF_START_SEC_VAR_FAST_32BIT)
  #undef LINIF_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined LINIF_STOP_SEC_VAR_FAST_32BIT)
  #undef LINIF_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined LINIF_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef LINIF_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined LINIF_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef LINIF_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined LINIF_START_SEC_VAR_8BIT)
  #undef LINIF_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined LINIF_STOP_SEC_VAR_8BIT)
  #undef LINIF_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined LINIF_START_SEC_VAR_16BIT)
  #undef LINIF_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined LINIF_STOP_SEC_VAR_16BIT)
  #undef LINIF_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined LINIF_START_SEC_VAR_32BIT)
  #undef LINIF_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined LINIF_STOP_SEC_VAR_32BIT)
  #undef LINIF_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined LINIF_START_SEC_VAR_UNSPECIFIED)
  #undef LINIF_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined LINIF_STOP_SEC_VAR_UNSPECIFIED)
  #undef LINIF_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined LINIF_START_SEC_CONST_8BIT)
  #undef LINIF_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined LINIF_STOP_SEC_CONST_8BIT)
  #undef LINIF_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined LINIF_START_SEC_CONST_16BIT)
  #undef LINIF_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined LINIF_STOP_SEC_CONST_16BIT)
  #undef LINIF_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined LINIF_START_SEC_CONST_32BIT)
  #undef LINIF_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined LINIF_STOP_SEC_CONST_32BIT)
  #undef LINIF_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined LINIF_START_SEC_CONST_UNSPECIFIED)
  #undef LINIF_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined LINIF_STOP_SEC_CONST_UNSPECIFIED)
  #undef LINIF_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined LINIF_START_CONFIG_DATA_8BIT)
  #undef LINIF_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined LINIF_STOP_CONFIG_DATA_8BIT)
  #undef LINIF_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined LINIF_START_CONFIG_DATA_16BIT)
  #undef LINIF_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined LINIF_STOP_CONFIG_DATA_16BIT)
  #undef LINIF_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined LINIF_START_CONFIG_DATA_32BIT)
  #undef LINIF_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined LINIF_STOP_CONFIG_DATA_32BIT)
  #undef LINIF_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined LINIF_START_CONFIG_DATA_UNSPECIFIED)
  #undef LINIF_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined LINIF_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef LINIF_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined LINSM_START_SEC_CODE)
  #undef LINSM_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined LINSM_STOP_SEC_CODE)
  #undef LINSM_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined LINSM_START_SEC_APPL_CODE)
  #undef LINSM_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined LINSM_STOP_SEC_APPL_CODE)
  #undef LINSM_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined LINSM_START_SEC_VAR_NOINIT_8BIT)
  #undef LINSM_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined LINSM_STOP_SEC_VAR_NOINIT_8BIT)
  #undef LINSM_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined LINSM_START_SEC_VAR_NOINIT_16BIT)
  #undef LINSM_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined LINSM_STOP_SEC_VAR_NOINIT_16BIT)
  #undef LINSM_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined LINSM_START_SEC_VAR_NOINIT_32BIT)
  #undef LINSM_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined LINSM_STOP_SEC_VAR_NOINIT_32BIT)
  #undef LINSM_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined LINSM_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef LINSM_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined LINSM_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef LINSM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined LINSM_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef LINSM_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined LINSM_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef LINSM_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined LINSM_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef LINSM_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined LINSM_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef LINSM_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined LINSM_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef LINSM_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined LINSM_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef LINSM_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined LINSM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef LINSM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined LINSM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef LINSM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined LINSM_START_SEC_VAR_FAST_8BIT)
  #undef LINSM_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined LINSM_STOP_SEC_VAR_FAST_8BIT)
  #undef LINSM_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined LINSM_START_SEC_VAR_FAST_16BIT)
  #undef LINSM_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined LINSM_STOP_SEC_VAR_FAST_16BIT)
  #undef LINSM_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined LINSM_START_SEC_VAR_FAST_32BIT)
  #undef LINSM_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined LINSM_STOP_SEC_VAR_FAST_32BIT)
  #undef LINSM_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined LINSM_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef LINSM_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined LINSM_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef LINSM_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined LINSM_START_SEC_VAR_8BIT)
  #undef LINSM_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined LINSM_STOP_SEC_VAR_8BIT)
  #undef LINSM_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined LINSM_START_SEC_VAR_16BIT)
  #undef LINSM_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined LINSM_STOP_SEC_VAR_16BIT)
  #undef LINSM_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined LINSM_START_SEC_VAR_32BIT)
  #undef LINSM_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined LINSM_STOP_SEC_VAR_32BIT)
  #undef LINSM_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined LINSM_START_SEC_VAR_UNSPECIFIED)
  #undef LINSM_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined LINSM_STOP_SEC_VAR_UNSPECIFIED)
  #undef LINSM_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined LINSM_START_SEC_CONST_8BIT)
  #undef LINSM_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined LINSM_STOP_SEC_CONST_8BIT)
  #undef LINSM_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined LINSM_START_SEC_CONST_16BIT)
  #undef LINSM_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined LINSM_STOP_SEC_CONST_16BIT)
  #undef LINSM_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined LINSM_START_SEC_CONST_32BIT)
  #undef LINSM_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined LINSM_STOP_SEC_CONST_32BIT)
  #undef LINSM_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined LINSM_START_SEC_CONST_UNSPECIFIED)
  #undef LINSM_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined LINSM_STOP_SEC_CONST_UNSPECIFIED)
  #undef LINSM_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined LINSM_START_CONFIG_DATA_8BIT)
  #undef LINSM_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined LINSM_STOP_CONFIG_DATA_8BIT)
  #undef LINSM_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined LINSM_START_CONFIG_DATA_16BIT)
  #undef LINSM_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined LINSM_STOP_CONFIG_DATA_16BIT)
  #undef LINSM_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined LINSM_START_CONFIG_DATA_32BIT)
  #undef LINSM_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined LINSM_STOP_CONFIG_DATA_32BIT)
  #undef LINSM_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined LINSM_START_CONFIG_DATA_UNSPECIFIED)
  #undef LINSM_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined LINSM_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef LINSM_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined MCU_START_SEC_CODE)
  #undef MCU_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined MCU_STOP_SEC_CODE)
  #undef MCU_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined MCU_START_SEC_APPL_CODE)
  #undef MCU_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined MCU_STOP_SEC_APPL_CODE)
  #undef MCU_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined MCU_START_SEC_VAR_NOINIT_8BIT)
  #undef MCU_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined MCU_STOP_SEC_VAR_NOINIT_8BIT)
  #undef MCU_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined MCU_START_SEC_VAR_NOINIT_16BIT)
  #undef MCU_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined MCU_STOP_SEC_VAR_NOINIT_16BIT)
  #undef MCU_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined MCU_START_SEC_VAR_NOINIT_32BIT)
  #undef MCU_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined MCU_STOP_SEC_VAR_NOINIT_32BIT)
  #undef MCU_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined MCU_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef MCU_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined MCU_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef MCU_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined MCU_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef MCU_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined MCU_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef MCU_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined MCU_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef MCU_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined MCU_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef MCU_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined MCU_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef MCU_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined MCU_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef MCU_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined MCU_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef MCU_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined MCU_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef MCU_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined MCU_START_SEC_VAR_FAST_8BIT)
  #undef MCU_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined MCU_STOP_SEC_VAR_FAST_8BIT)
  #undef MCU_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined MCU_START_SEC_VAR_FAST_16BIT)
  #undef MCU_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined MCU_STOP_SEC_VAR_FAST_16BIT)
  #undef MCU_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined MCU_START_SEC_VAR_FAST_32BIT)
  #undef MCU_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined MCU_STOP_SEC_VAR_FAST_32BIT)
  #undef MCU_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined MCU_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef MCU_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined MCU_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef MCU_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined MCU_START_SEC_VAR_8BIT)
  #undef MCU_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined MCU_STOP_SEC_VAR_8BIT)
  #undef MCU_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined MCU_START_SEC_VAR_16BIT)
  #undef MCU_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined MCU_STOP_SEC_VAR_16BIT)
  #undef MCU_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined MCU_START_SEC_VAR_32BIT)
  #undef MCU_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined MCU_STOP_SEC_VAR_32BIT)
  #undef MCU_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined MCU_START_SEC_VAR_UNSPECIFIED)
  #undef MCU_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined MCU_STOP_SEC_VAR_UNSPECIFIED)
  #undef MCU_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined MCU_START_SEC_CONST_8BIT)
  #undef MCU_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined MCU_STOP_SEC_CONST_8BIT)
  #undef MCU_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined MCU_START_SEC_CONST_16BIT)
  #undef MCU_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined MCU_STOP_SEC_CONST_16BIT)
  #undef MCU_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined MCU_START_SEC_CONST_32BIT)
  #undef MCU_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined MCU_STOP_SEC_CONST_32BIT)
  #undef MCU_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined MCU_START_SEC_CONST_UNSPECIFIED)
  #undef MCU_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined MCU_STOP_SEC_CONST_UNSPECIFIED)
  #undef MCU_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined MCU_START_CONFIG_DATA_8BIT)
  #undef MCU_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined MCU_STOP_CONFIG_DATA_8BIT)
  #undef MCU_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined MCU_START_CONFIG_DATA_16BIT)
  #undef MCU_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined MCU_STOP_CONFIG_DATA_16BIT)
  #undef MCU_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined MCU_START_CONFIG_DATA_32BIT)
  #undef MCU_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined MCU_STOP_CONFIG_DATA_32BIT)
  #undef MCU_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined MCU_START_CONFIG_DATA_UNSPECIFIED)
  #undef MCU_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined MCU_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef MCU_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined MEMIF_START_SEC_CODE)
  #undef MEMIF_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined MEMIF_STOP_SEC_CODE)
  #undef MEMIF_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined MEMIF_START_SEC_APPL_CODE)
  #undef MEMIF_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined MEMIF_STOP_SEC_APPL_CODE)
  #undef MEMIF_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined MEMIF_START_SEC_VAR_NOINIT_8BIT)
  #undef MEMIF_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined MEMIF_STOP_SEC_VAR_NOINIT_8BIT)
  #undef MEMIF_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined MEMIF_START_SEC_VAR_NOINIT_16BIT)
  #undef MEMIF_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined MEMIF_STOP_SEC_VAR_NOINIT_16BIT)
  #undef MEMIF_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined MEMIF_START_SEC_VAR_NOINIT_32BIT)
  #undef MEMIF_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined MEMIF_STOP_SEC_VAR_NOINIT_32BIT)
  #undef MEMIF_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined MEMIF_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef MEMIF_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined MEMIF_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef MEMIF_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined MEMIF_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef MEMIF_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined MEMIF_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef MEMIF_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined MEMIF_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef MEMIF_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined MEMIF_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef MEMIF_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined MEMIF_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef MEMIF_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined MEMIF_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef MEMIF_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined MEMIF_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef MEMIF_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined MEMIF_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef MEMIF_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined MEMIF_START_SEC_VAR_FAST_8BIT)
  #undef MEMIF_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined MEMIF_STOP_SEC_VAR_FAST_8BIT)
  #undef MEMIF_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined MEMIF_START_SEC_VAR_FAST_16BIT)
  #undef MEMIF_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined MEMIF_STOP_SEC_VAR_FAST_16BIT)
  #undef MEMIF_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined MEMIF_START_SEC_VAR_FAST_32BIT)
  #undef MEMIF_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined MEMIF_STOP_SEC_VAR_FAST_32BIT)
  #undef MEMIF_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined MEMIF_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef MEMIF_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined MEMIF_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef MEMIF_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined MEMIF_START_SEC_VAR_8BIT)
  #undef MEMIF_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined MEMIF_STOP_SEC_VAR_8BIT)
  #undef MEMIF_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined MEMIF_START_SEC_VAR_16BIT)
  #undef MEMIF_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined MEMIF_STOP_SEC_VAR_16BIT)
  #undef MEMIF_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined MEMIF_START_SEC_VAR_32BIT)
  #undef MEMIF_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined MEMIF_STOP_SEC_VAR_32BIT)
  #undef MEMIF_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined MEMIF_START_SEC_VAR_UNSPECIFIED)
  #undef MEMIF_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined MEMIF_STOP_SEC_VAR_UNSPECIFIED)
  #undef MEMIF_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined MEMIF_START_SEC_CONST_8BIT)
  #undef MEMIF_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined MEMIF_STOP_SEC_CONST_8BIT)
  #undef MEMIF_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined MEMIF_START_SEC_CONST_16BIT)
  #undef MEMIF_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined MEMIF_STOP_SEC_CONST_16BIT)
  #undef MEMIF_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined MEMIF_START_SEC_CONST_32BIT)
  #undef MEMIF_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined MEMIF_STOP_SEC_CONST_32BIT)
  #undef MEMIF_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined MEMIF_START_SEC_CONST_UNSPECIFIED)
  #undef MEMIF_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined MEMIF_STOP_SEC_CONST_UNSPECIFIED)
  #undef MEMIF_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined MEMIF_START_CONFIG_DATA_8BIT)
  #undef MEMIF_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined MEMIF_STOP_CONFIG_DATA_8BIT)
  #undef MEMIF_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined MEMIF_START_CONFIG_DATA_16BIT)
  #undef MEMIF_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined MEMIF_STOP_CONFIG_DATA_16BIT)
  #undef MEMIF_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined MEMIF_START_CONFIG_DATA_32BIT)
  #undef MEMIF_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined MEMIF_STOP_CONFIG_DATA_32BIT)
  #undef MEMIF_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined MEMIF_START_CONFIG_DATA_UNSPECIFIED)
  #undef MEMIF_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined MEMIF_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef MEMIF_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined NM_START_SEC_CODE)
  #undef NM_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined NM_STOP_SEC_CODE)
  #undef NM_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined NM_START_SEC_APPL_CODE)
  #undef NM_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined NM_STOP_SEC_APPL_CODE)
  #undef NM_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined NM_START_SEC_VAR_NOINIT_8BIT)
  #undef NM_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined NM_STOP_SEC_VAR_NOINIT_8BIT)
  #undef NM_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined NM_START_SEC_VAR_NOINIT_16BIT)
  #undef NM_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined NM_STOP_SEC_VAR_NOINIT_16BIT)
  #undef NM_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined NM_START_SEC_VAR_NOINIT_32BIT)
  #undef NM_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined NM_STOP_SEC_VAR_NOINIT_32BIT)
  #undef NM_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined NM_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef NM_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined NM_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef NM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined NM_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef NM_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined NM_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef NM_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined NM_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef NM_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined NM_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef NM_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined NM_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef NM_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined NM_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef NM_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined NM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef NM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined NM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef NM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined NM_START_SEC_VAR_FAST_8BIT)
  #undef NM_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined NM_STOP_SEC_VAR_FAST_8BIT)
  #undef NM_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined NM_START_SEC_VAR_FAST_16BIT)
  #undef NM_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined NM_STOP_SEC_VAR_FAST_16BIT)
  #undef NM_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined NM_START_SEC_VAR_FAST_32BIT)
  #undef NM_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined NM_STOP_SEC_VAR_FAST_32BIT)
  #undef NM_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined NM_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef NM_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined NM_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef NM_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined NM_START_SEC_VAR_8BIT)
  #undef NM_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined NM_STOP_SEC_VAR_8BIT)
  #undef NM_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined NM_START_SEC_VAR_16BIT)
  #undef NM_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined NM_STOP_SEC_VAR_16BIT)
  #undef NM_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined NM_START_SEC_VAR_32BIT)
  #undef NM_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined NM_STOP_SEC_VAR_32BIT)
  #undef NM_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined NM_START_SEC_VAR_UNSPECIFIED)
  #undef NM_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined NM_STOP_SEC_VAR_UNSPECIFIED)
  #undef NM_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined NM_START_SEC_CONST_8BIT)
  #undef NM_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined NM_STOP_SEC_CONST_8BIT)
  #undef NM_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined NM_START_SEC_CONST_16BIT)
  #undef NM_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined NM_STOP_SEC_CONST_16BIT)
  #undef NM_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined NM_START_SEC_CONST_32BIT)
  #undef NM_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined NM_STOP_SEC_CONST_32BIT)
  #undef NM_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined NM_START_SEC_CONST_UNSPECIFIED)
  #undef NM_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined NM_STOP_SEC_CONST_UNSPECIFIED)
  #undef NM_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined NM_START_CONFIG_DATA_8BIT)
  #undef NM_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined NM_STOP_CONFIG_DATA_8BIT)
  #undef NM_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined NM_START_CONFIG_DATA_16BIT)
  #undef NM_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined NM_STOP_CONFIG_DATA_16BIT)
  #undef NM_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined NM_START_CONFIG_DATA_32BIT)
  #undef NM_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined NM_STOP_CONFIG_DATA_32BIT)
  #undef NM_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined NM_START_CONFIG_DATA_UNSPECIFIED)
  #undef NM_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined NM_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef NM_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined NVM_START_SEC_CODE)
  #undef NVM_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined NVM_STOP_SEC_CODE)
  #undef NVM_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined NVM_START_SEC_APPL_CODE)
  #undef NVM_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined NVM_STOP_SEC_APPL_CODE)
  #undef NVM_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined NVM_START_SEC_VAR_NOINIT_8BIT)
  #undef NVM_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined NVM_STOP_SEC_VAR_NOINIT_8BIT)
  #undef NVM_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined NVM_START_SEC_VAR_NOINIT_16BIT)
  #undef NVM_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined NVM_STOP_SEC_VAR_NOINIT_16BIT)
  #undef NVM_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined NVM_START_SEC_VAR_NOINIT_32BIT)
  #undef NVM_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined NVM_STOP_SEC_VAR_NOINIT_32BIT)
  #undef NVM_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined NVM_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef NVM_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined NVM_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef NVM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined NVM_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef NVM_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined NVM_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef NVM_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined NVM_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef NVM_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined NVM_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef NVM_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined NVM_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef NVM_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined NVM_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef NVM_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined NVM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef NVM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined NVM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef NVM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined NVM_START_SEC_VAR_FAST_8BIT)
  #undef NVM_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined NVM_STOP_SEC_VAR_FAST_8BIT)
  #undef NVM_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined NVM_START_SEC_VAR_FAST_16BIT)
  #undef NVM_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined NVM_STOP_SEC_VAR_FAST_16BIT)
  #undef NVM_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined NVM_START_SEC_VAR_FAST_32BIT)
  #undef NVM_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined NVM_STOP_SEC_VAR_FAST_32BIT)
  #undef NVM_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined NVM_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef NVM_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined NVM_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef NVM_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined NVM_START_SEC_VAR_8BIT)
  #undef NVM_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined NVM_STOP_SEC_VAR_8BIT)
  #undef NVM_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined NVM_START_SEC_VAR_16BIT)
  #undef NVM_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined NVM_STOP_SEC_VAR_16BIT)
  #undef NVM_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined NVM_START_SEC_VAR_32BIT)
  #undef NVM_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined NVM_STOP_SEC_VAR_32BIT)
  #undef NVM_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined NVM_START_SEC_VAR_UNSPECIFIED)
  #undef NVM_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined NVM_STOP_SEC_VAR_UNSPECIFIED)
  #undef NVM_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined NVM_START_SEC_CONST_8BIT)
  #undef NVM_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined NVM_STOP_SEC_CONST_8BIT)
  #undef NVM_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined NVM_START_SEC_CONST_16BIT)
  #undef NVM_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined NVM_STOP_SEC_CONST_16BIT)
  #undef NVM_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined NVM_START_SEC_CONST_32BIT)
  #undef NVM_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined NVM_STOP_SEC_CONST_32BIT)
  #undef NVM_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined NVM_START_SEC_CONST_UNSPECIFIED)
  #undef NVM_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined NVM_STOP_SEC_CONST_UNSPECIFIED)
  #undef NVM_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined NVM_START_CONFIG_DATA_8BIT)
  #undef NVM_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined NVM_STOP_CONFIG_DATA_8BIT)
  #undef NVM_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined NVM_START_CONFIG_DATA_16BIT)
  #undef NVM_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined NVM_STOP_CONFIG_DATA_16BIT)
  #undef NVM_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined NVM_START_CONFIG_DATA_32BIT)
  #undef NVM_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined NVM_STOP_CONFIG_DATA_32BIT)
  #undef NVM_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined NVM_START_CONFIG_DATA_UNSPECIFIED)
  #undef NVM_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined NVM_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef NVM_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined NVM_START_SEC_VAR_APPL_DATA)
  #undef NVM_START_SEC_VAR_APPL_DATA
  #undef MEMMAP_ERROR

#elif (defined NVM_STOP_SEC_VAR_APPL_DATA)
  #undef NVM_STOP_SEC_VAR_APPL_DATA
  #undef MEMMAP_ERROR

#elif (defined NVM_START_CONFIG_DATA_APPL_DATA)
  #undef NVM_START_CONFIG_DATA_APPL_DATA
  #undef MEMMAP_ERROR

#elif (defined NVM_STOP_CONFIG_DATA_APPL_DATA)
  #undef NVM_STOP_CONFIG_DATA_APPL_DATA
  #undef MEMMAP_ERROR

#elif (defined NVM_START_CONFIG_DATA_APPL_CONST)
  #undef NVM_START_CONFIG_DATA_APPL_CONST
  #undef MEMMAP_ERROR

#elif (defined NVM_STOP_CONFIG_DATA_APPL_CONST)
  #undef NVM_STOP_CONFIG_DATA_APPL_CONST
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined OS_START_SEC_CODE)
  #undef OS_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined OS_STOP_SEC_CODE)
  #undef OS_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined OS_START_SEC_APPL_CODE)
  #undef OS_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined OS_STOP_SEC_APPL_CODE)
  #undef OS_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined OS_START_SEC_VAR_NOINIT_8BIT)
  #undef OS_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined OS_STOP_SEC_VAR_NOINIT_8BIT)
  #undef OS_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined OS_START_SEC_VAR_NOINIT_16BIT)
  #undef OS_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined OS_STOP_SEC_VAR_NOINIT_16BIT)
  #undef OS_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined OS_START_SEC_VAR_NOINIT_32BIT)
  #undef OS_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined OS_STOP_SEC_VAR_NOINIT_32BIT)
  #undef OS_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined OS_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef OS_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined OS_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef OS_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined OS_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef OS_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined OS_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef OS_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined OS_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef OS_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined OS_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef OS_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined OS_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef OS_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined OS_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef OS_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined OS_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef OS_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined OS_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef OS_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined OS_START_SEC_VAR_FAST_8BIT)
  #undef OS_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined OS_STOP_SEC_VAR_FAST_8BIT)
  #undef OS_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined OS_START_SEC_VAR_FAST_16BIT)
  #undef OS_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined OS_STOP_SEC_VAR_FAST_16BIT)
  #undef OS_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined OS_START_SEC_VAR_FAST_32BIT)
  #undef OS_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined OS_STOP_SEC_VAR_FAST_32BIT)
  #undef OS_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined OS_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef OS_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined OS_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef OS_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined OS_START_SEC_VAR_8BIT)
  #undef OS_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined OS_STOP_SEC_VAR_8BIT)
  #undef OS_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined OS_START_SEC_VAR_16BIT)
  #undef OS_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined OS_STOP_SEC_VAR_16BIT)
  #undef OS_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined OS_START_SEC_VAR_32BIT)
  #undef OS_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined OS_STOP_SEC_VAR_32BIT)
  #undef OS_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined OS_START_SEC_VAR_UNSPECIFIED)
  #undef OS_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined OS_STOP_SEC_VAR_UNSPECIFIED)
  #undef OS_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined OS_START_SEC_CONST_8BIT)
  #undef OS_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined OS_STOP_SEC_CONST_8BIT)
  #undef OS_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined OS_START_SEC_CONST_16BIT)
  #undef OS_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined OS_STOP_SEC_CONST_16BIT)
  #undef OS_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined OS_START_SEC_CONST_32BIT)
  #undef OS_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined OS_STOP_SEC_CONST_32BIT)
  #undef OS_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined OS_START_SEC_CONST_UNSPECIFIED)
  #undef OS_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined OS_STOP_SEC_CONST_UNSPECIFIED)
  #undef OS_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined OS_START_CONFIG_DATA_8BIT)
  #undef OS_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined OS_STOP_CONFIG_DATA_8BIT)
  #undef OS_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined OS_START_CONFIG_DATA_16BIT)
  #undef OS_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined OS_STOP_CONFIG_DATA_16BIT)
  #undef OS_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined OS_START_CONFIG_DATA_32BIT)
  #undef OS_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined OS_STOP_CONFIG_DATA_32BIT)
  #undef OS_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined OS_START_CONFIG_DATA_UNSPECIFIED)
  #undef OS_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined OS_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef OS_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined PDUR_START_SEC_CODE)
  #undef PDUR_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined PDUR_STOP_SEC_CODE)
  #undef PDUR_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined PDUR_START_SEC_APPL_CODE)
  #undef PDUR_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined PDUR_STOP_SEC_APPL_CODE)
  #undef PDUR_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined PDUR_START_SEC_VAR_NOINIT_8BIT)
  #undef PDUR_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined PDUR_STOP_SEC_VAR_NOINIT_8BIT)
  #undef PDUR_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined PDUR_START_SEC_VAR_NOINIT_16BIT)
  #undef PDUR_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined PDUR_STOP_SEC_VAR_NOINIT_16BIT)
  #undef PDUR_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined PDUR_START_SEC_VAR_NOINIT_32BIT)
  #undef PDUR_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined PDUR_STOP_SEC_VAR_NOINIT_32BIT)
  #undef PDUR_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined PDUR_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef PDUR_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined PDUR_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef PDUR_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined PDUR_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef PDUR_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined PDUR_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef PDUR_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined PDUR_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef PDUR_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined PDUR_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef PDUR_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined PDUR_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef PDUR_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined PDUR_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef PDUR_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined PDUR_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef PDUR_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined PDUR_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef PDUR_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined PDUR_START_SEC_VAR_FAST_8BIT)
  #undef PDUR_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined PDUR_STOP_SEC_VAR_FAST_8BIT)
  #undef PDUR_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined PDUR_START_SEC_VAR_FAST_16BIT)
  #undef PDUR_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined PDUR_STOP_SEC_VAR_FAST_16BIT)
  #undef PDUR_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined PDUR_START_SEC_VAR_FAST_32BIT)
  #undef PDUR_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined PDUR_STOP_SEC_VAR_FAST_32BIT)
  #undef PDUR_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined PDUR_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef PDUR_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined PDUR_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef PDUR_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined PDUR_START_SEC_VAR_8BIT)
  #undef PDUR_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined PDUR_STOP_SEC_VAR_8BIT)
  #undef PDUR_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined PDUR_START_SEC_VAR_16BIT)
  #undef PDUR_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined PDUR_STOP_SEC_VAR_16BIT)
  #undef PDUR_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined PDUR_START_SEC_VAR_32BIT)
  #undef PDUR_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined PDUR_STOP_SEC_VAR_32BIT)
  #undef PDUR_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined PDUR_START_SEC_VAR_UNSPECIFIED)
  #undef PDUR_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined PDUR_STOP_SEC_VAR_UNSPECIFIED)
  #undef PDUR_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined PDUR_START_SEC_CONST_8BIT)
  #undef PDUR_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined PDUR_STOP_SEC_CONST_8BIT)
  #undef PDUR_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined PDUR_START_SEC_CONST_16BIT)
  #undef PDUR_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined PDUR_STOP_SEC_CONST_16BIT)
  #undef PDUR_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined PDUR_START_SEC_CONST_32BIT)
  #undef PDUR_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined PDUR_STOP_SEC_CONST_32BIT)
  #undef PDUR_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined PDUR_START_SEC_CONST_UNSPECIFIED)
  #undef PDUR_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined PDUR_STOP_SEC_CONST_UNSPECIFIED)
  #undef PDUR_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined PDUR_START_CONFIG_DATA_8BIT)
  #undef PDUR_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined PDUR_STOP_CONFIG_DATA_8BIT)
  #undef PDUR_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined PDUR_START_CONFIG_DATA_16BIT)
  #undef PDUR_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined PDUR_STOP_CONFIG_DATA_16BIT)
  #undef PDUR_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined PDUR_START_CONFIG_DATA_32BIT)
  #undef PDUR_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined PDUR_STOP_CONFIG_DATA_32BIT)
  #undef PDUR_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined PDUR_START_CONFIG_DATA_UNSPECIFIED)
  #undef PDUR_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined PDUR_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef PDUR_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined PORT_START_SEC_CODE)
  #undef PORT_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined PORT_STOP_SEC_CODE)
  #undef PORT_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined PORT_START_SEC_APPL_CODE)
  #undef PORT_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined PORT_STOP_SEC_APPL_CODE)
  #undef PORT_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined PORT_START_SEC_VAR_NOINIT_8BIT)
  #undef PORT_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined PORT_STOP_SEC_VAR_NOINIT_8BIT)
  #undef PORT_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined PORT_START_SEC_VAR_NOINIT_16BIT)
  #undef PORT_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined PORT_STOP_SEC_VAR_NOINIT_16BIT)
  #undef PORT_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined PORT_START_SEC_VAR_NOINIT_32BIT)
  #undef PORT_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined PORT_STOP_SEC_VAR_NOINIT_32BIT)
  #undef PORT_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined PORT_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef PORT_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined PORT_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef PORT_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined PORT_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef PORT_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined PORT_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef PORT_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined PORT_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef PORT_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined PORT_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef PORT_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined PORT_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef PORT_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined PORT_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef PORT_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined PORT_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef PORT_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined PORT_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef PORT_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined PORT_START_SEC_VAR_FAST_8BIT)
  #undef PORT_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined PORT_STOP_SEC_VAR_FAST_8BIT)
  #undef PORT_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined PORT_START_SEC_VAR_FAST_16BIT)
  #undef PORT_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined PORT_STOP_SEC_VAR_FAST_16BIT)
  #undef PORT_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined PORT_START_SEC_VAR_FAST_32BIT)
  #undef PORT_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined PORT_STOP_SEC_VAR_FAST_32BIT)
  #undef PORT_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined PORT_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef PORT_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined PORT_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef PORT_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined PORT_START_SEC_VAR_8BIT)
  #undef PORT_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined PORT_STOP_SEC_VAR_8BIT)
  #undef PORT_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined PORT_START_SEC_VAR_16BIT)
  #undef PORT_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined PORT_STOP_SEC_VAR_16BIT)
  #undef PORT_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined PORT_START_SEC_VAR_32BIT)
  #undef PORT_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined PORT_STOP_SEC_VAR_32BIT)
  #undef PORT_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined PORT_START_SEC_VAR_UNSPECIFIED)
  #undef PORT_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined PORT_STOP_SEC_VAR_UNSPECIFIED)
  #undef PORT_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined PORT_START_SEC_CONST_8BIT)
  #undef PORT_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined PORT_STOP_SEC_CONST_8BIT)
  #undef PORT_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined PORT_START_SEC_CONST_16BIT)
  #undef PORT_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined PORT_STOP_SEC_CONST_16BIT)
  #undef PORT_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined PORT_START_SEC_CONST_32BIT)
  #undef PORT_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined PORT_STOP_SEC_CONST_32BIT)
  #undef PORT_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined PORT_START_SEC_CONST_UNSPECIFIED)
  #undef PORT_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined PORT_STOP_SEC_CONST_UNSPECIFIED)
  #undef PORT_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined PORT_START_CONFIG_DATA_8BIT)
  #undef PORT_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined PORT_STOP_CONFIG_DATA_8BIT)
  #undef PORT_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined PORT_START_CONFIG_DATA_16BIT)
  #undef PORT_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined PORT_STOP_CONFIG_DATA_16BIT)
  #undef PORT_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined PORT_START_CONFIG_DATA_32BIT)
  #undef PORT_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined PORT_STOP_CONFIG_DATA_32BIT)
  #undef PORT_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined PORT_START_CONFIG_DATA_UNSPECIFIED)
  #undef PORT_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined PORT_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef PORT_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined PORT_START_SEC_POSTBUILDCFG)
  #undef PORT_START_SEC_POSTBUILDCFG
  #undef MEMMAP_ERROR

#elif (defined PORT_STOP_SEC_POSTBUILDCFG)
  #undef PORT_STOP_SEC_POSTBUILDCFG
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined PWM_START_SEC_CODE)
  #undef PWM_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined PWM_STOP_SEC_CODE)
  #undef PWM_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined PWM_START_SEC_APPL_CODE)
  #undef PWM_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined PWM_STOP_SEC_APPL_CODE)
  #undef PWM_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined PWM_START_SEC_VAR_NOINIT_8BIT)
  #undef PWM_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined PWM_STOP_SEC_VAR_NOINIT_8BIT)
  #undef PWM_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined PWM_START_SEC_VAR_NOINIT_16BIT)
  #undef PWM_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined PWM_STOP_SEC_VAR_NOINIT_16BIT)
  #undef PWM_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined PWM_START_SEC_VAR_NOINIT_32BIT)
  #undef PWM_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined PWM_STOP_SEC_VAR_NOINIT_32BIT)
  #undef PWM_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined PWM_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef PWM_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined PWM_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef PWM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined PWM_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef PWM_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined PWM_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef PWM_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined PWM_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef PWM_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined PWM_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef PWM_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined PWM_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef PWM_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined PWM_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef PWM_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined PWM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef PWM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined PWM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef PWM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined PWM_START_SEC_VAR_FAST_8BIT)
  #undef PWM_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined PWM_STOP_SEC_VAR_FAST_8BIT)
  #undef PWM_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined PWM_START_SEC_VAR_FAST_16BIT)
  #undef PWM_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined PWM_STOP_SEC_VAR_FAST_16BIT)
  #undef PWM_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined PWM_START_SEC_VAR_FAST_32BIT)
  #undef PWM_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined PWM_STOP_SEC_VAR_FAST_32BIT)
  #undef PWM_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined PWM_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef PWM_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined PWM_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef PWM_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined PWM_START_SEC_VAR_8BIT)
  #undef PWM_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined PWM_STOP_SEC_VAR_8BIT)
  #undef PWM_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined PWM_START_SEC_VAR_16BIT)
  #undef PWM_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined PWM_STOP_SEC_VAR_16BIT)
  #undef PWM_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined PWM_START_SEC_VAR_32BIT)
  #undef PWM_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined PWM_STOP_SEC_VAR_32BIT)
  #undef PWM_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined PWM_START_SEC_VAR_UNSPECIFIED)
  #undef PWM_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined PWM_STOP_SEC_VAR_UNSPECIFIED)
  #undef PWM_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined PWM_START_SEC_CONST_8BIT)
  #undef PWM_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined PWM_STOP_SEC_CONST_8BIT)
  #undef PWM_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined PWM_START_SEC_CONST_16BIT)
  #undef PWM_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined PWM_STOP_SEC_CONST_16BIT)
  #undef PWM_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined PWM_START_SEC_CONST_32BIT)
  #undef PWM_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined PWM_STOP_SEC_CONST_32BIT)
  #undef PWM_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined PWM_START_SEC_CONST_UNSPECIFIED)
  #undef PWM_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined PWM_STOP_SEC_CONST_UNSPECIFIED)
  #undef PWM_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined PWM_START_CONFIG_DATA_8BIT)
  #undef PWM_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined PWM_STOP_CONFIG_DATA_8BIT)
  #undef PWM_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined PWM_START_CONFIG_DATA_16BIT)
  #undef PWM_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined PWM_STOP_CONFIG_DATA_16BIT)
  #undef PWM_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined PWM_START_CONFIG_DATA_32BIT)
  #undef PWM_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined PWM_STOP_CONFIG_DATA_32BIT)
  #undef PWM_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined PWM_START_CONFIG_DATA_UNSPECIFIED)
  #undef PWM_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined PWM_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef PWM_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined RAMTST_START_SEC_CODE)
  #undef RAMTST_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined RAMTST_STOP_SEC_CODE)
  #undef RAMTST_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined RAMTST_START_SEC_APPL_CODE)
  #undef RAMTST_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined RAMTST_STOP_SEC_APPL_CODE)
  #undef RAMTST_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined RAMTST_START_SEC_VAR_NOINIT_8BIT)
  #undef RAMTST_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined RAMTST_STOP_SEC_VAR_NOINIT_8BIT)
  #undef RAMTST_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined RAMTST_START_SEC_VAR_NOINIT_16BIT)
  #undef RAMTST_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined RAMTST_STOP_SEC_VAR_NOINIT_16BIT)
  #undef RAMTST_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined RAMTST_START_SEC_VAR_NOINIT_32BIT)
  #undef RAMTST_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined RAMTST_STOP_SEC_VAR_NOINIT_32BIT)
  #undef RAMTST_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined RAMTST_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef RAMTST_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined RAMTST_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef RAMTST_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined RAMTST_START_SEC_VAR_NO_INIT_UNSPECIFIED)
  #undef RAMTST_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined RAMTST_STOP_SEC_VAR_NO_INIT_UNSPECIFIED)
  #undef RAMTST_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined RAMTST_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef RAMTST_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined RAMTST_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef RAMTST_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined RAMTST_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef RAMTST_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined RAMTST_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef RAMTST_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined RAMTST_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef RAMTST_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined RAMTST_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef RAMTST_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined RAMTST_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef RAMTST_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined RAMTST_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef RAMTST_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined RAMTST_START_SEC_VAR_FAST_8BIT)
  #undef RAMTST_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined RAMTST_STOP_SEC_VAR_FAST_8BIT)
  #undef RAMTST_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined RAMTST_START_SEC_VAR_FAST_16BIT)
  #undef RAMTST_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined RAMTST_STOP_SEC_VAR_FAST_16BIT)
  #undef RAMTST_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined RAMTST_START_SEC_VAR_FAST_32BIT)
  #undef RAMTST_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined RAMTST_STOP_SEC_VAR_FAST_32BIT)
  #undef RAMTST_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined RAMTST_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef RAMTST_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined RAMTST_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef RAMTST_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined RAMTST_START_SEC_VAR_8BIT)
  #undef RAMTST_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined RAMTST_STOP_SEC_VAR_8BIT)
  #undef RAMTST_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined RAMTST_START_SEC_VAR_16BIT)
  #undef RAMTST_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined RAMTST_STOP_SEC_VAR_16BIT)
  #undef RAMTST_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined RAMTST_START_SEC_VAR_32BIT)
  #undef RAMTST_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined RAMTST_STOP_SEC_VAR_32BIT)
  #undef RAMTST_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined RAMTST_START_SEC_VAR_UNSPECIFIED)
  #undef RAMTST_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined RAMTST_STOP_SEC_VAR_UNSPECIFIED)
  #undef RAMTST_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined RAMTST_START_SEC_CONST_8BIT)
  #undef RAMTST_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined RAMTST_STOP_SEC_CONST_8BIT)
  #undef RAMTST_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined RAMTST_START_SEC_CONST_16BIT)
  #undef RAMTST_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined RAMTST_STOP_SEC_CONST_16BIT)
  #undef RAMTST_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined RAMTST_START_SEC_CONST_32BIT)
  #undef RAMTST_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined RAMTST_STOP_SEC_CONST_32BIT)
  #undef RAMTST_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined RAMTST_START_SEC_CONST_UNSPECIFIED)
  #undef RAMTST_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined RAMTST_STOP_SEC_CONST_UNSPECIFIED)
  #undef RAMTST_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined RAMTST_START_CONFIG_DATA_8BIT)
  #undef RAMTST_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined RAMTST_STOP_CONFIG_DATA_8BIT)
  #undef RAMTST_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined RAMTST_START_CONFIG_DATA_16BIT)
  #undef RAMTST_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined RAMTST_STOP_CONFIG_DATA_16BIT)
  #undef RAMTST_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined RAMTST_START_CONFIG_DATA_32BIT)
  #undef RAMTST_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined RAMTST_STOP_CONFIG_DATA_32BIT)
  #undef RAMTST_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined RAMTST_START_CONFIG_DATA_UNSPECIFIED)
  #undef RAMTST_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined RAMTST_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef RAMTST_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined RTE_START_SEC_CODE)
  #undef RTE_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined RTE_STOP_SEC_CODE)
  #undef RTE_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined RTE_START_SEC_APPL_CODE)
  #undef RTE_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined RTE_STOP_SEC_APPL_CODE)
  #undef RTE_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined RTE_START_SEC_VAR_NOINIT_8BIT)
  #undef RTE_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined RTE_STOP_SEC_VAR_NOINIT_8BIT)
  #undef RTE_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined RTE_START_SEC_VAR_NOINIT_16BIT)
  #undef RTE_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined RTE_STOP_SEC_VAR_NOINIT_16BIT)
  #undef RTE_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined RTE_START_SEC_VAR_NOINIT_32BIT)
  #undef RTE_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined RTE_STOP_SEC_VAR_NOINIT_32BIT)
  #undef RTE_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined RTE_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef RTE_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined RTE_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef RTE_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined RTE_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef RTE_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined RTE_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef RTE_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined RTE_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef RTE_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined RTE_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef RTE_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined RTE_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef RTE_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined RTE_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef RTE_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined RTE_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef RTE_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined RTE_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef RTE_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined RTE_START_SEC_VAR_FAST_8BIT)
  #undef RTE_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined RTE_STOP_SEC_VAR_FAST_8BIT)
  #undef RTE_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined RTE_START_SEC_VAR_FAST_16BIT)
  #undef RTE_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined RTE_STOP_SEC_VAR_FAST_16BIT)
  #undef RTE_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined RTE_START_SEC_VAR_FAST_32BIT)
  #undef RTE_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined RTE_STOP_SEC_VAR_FAST_32BIT)
  #undef RTE_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined RTE_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef RTE_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined RTE_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef RTE_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined RTE_START_SEC_VAR_8BIT)
  #undef RTE_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined RTE_STOP_SEC_VAR_8BIT)
  #undef RTE_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined RTE_START_SEC_VAR_16BIT)
  #undef RTE_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined RTE_STOP_SEC_VAR_16BIT)
  #undef RTE_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined RTE_START_SEC_VAR_32BIT)
  #undef RTE_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined RTE_STOP_SEC_VAR_32BIT)
  #undef RTE_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined RTE_START_SEC_VAR_UNSPECIFIED)
  #undef RTE_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined RTE_STOP_SEC_VAR_UNSPECIFIED)
  #undef RTE_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined RTE_START_SEC_CONST_8BIT)
  #undef RTE_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined RTE_STOP_SEC_CONST_8BIT)
  #undef RTE_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined RTE_START_SEC_CONST_16BIT)
  #undef RTE_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined RTE_STOP_SEC_CONST_16BIT)
  #undef RTE_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined RTE_START_SEC_CONST_32BIT)
  #undef RTE_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined RTE_STOP_SEC_CONST_32BIT)
  #undef RTE_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined RTE_START_SEC_CONST_UNSPECIFIED)
  #undef RTE_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined RTE_STOP_SEC_CONST_UNSPECIFIED)
  #undef RTE_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined RTE_START_CONFIG_DATA_8BIT)
  #undef RTE_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined RTE_STOP_CONFIG_DATA_8BIT)
  #undef RTE_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined RTE_START_CONFIG_DATA_16BIT)
  #undef RTE_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined RTE_STOP_CONFIG_DATA_16BIT)
  #undef RTE_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined RTE_START_CONFIG_DATA_32BIT)
  #undef RTE_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined RTE_STOP_CONFIG_DATA_32BIT)
  #undef RTE_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined RTE_START_CONFIG_DATA_UNSPECIFIED)
  #undef RTE_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined RTE_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef RTE_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined SCHM_START_SEC_CODE)
  #undef SCHM_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined SCHM_STOP_SEC_CODE)
  #undef SCHM_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined SCHM_START_SEC_APPL_CODE)
  #undef SCHM_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined SCHM_STOP_SEC_APPL_CODE)
  #undef SCHM_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined SCHM_START_SEC_VAR_NOINIT_8BIT)
  #undef SCHM_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined SCHM_STOP_SEC_VAR_NOINIT_8BIT)
  #undef SCHM_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined SCHM_START_SEC_VAR_NOINIT_16BIT)
  #undef SCHM_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined SCHM_STOP_SEC_VAR_NOINIT_16BIT)
  #undef SCHM_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined SCHM_START_SEC_VAR_NOINIT_32BIT)
  #undef SCHM_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined SCHM_STOP_SEC_VAR_NOINIT_32BIT)
  #undef SCHM_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined SCHM_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef SCHM_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined SCHM_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef SCHM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined SCHM_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef SCHM_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined SCHM_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef SCHM_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined SCHM_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef SCHM_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined SCHM_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef SCHM_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined SCHM_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef SCHM_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined SCHM_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef SCHM_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined SCHM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef SCHM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined SCHM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef SCHM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined SCHM_START_SEC_VAR_FAST_8BIT)
  #undef SCHM_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined SCHM_STOP_SEC_VAR_FAST_8BIT)
  #undef SCHM_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined SCHM_START_SEC_VAR_FAST_16BIT)
  #undef SCHM_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined SCHM_STOP_SEC_VAR_FAST_16BIT)
  #undef SCHM_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined SCHM_START_SEC_VAR_FAST_32BIT)
  #undef SCHM_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined SCHM_STOP_SEC_VAR_FAST_32BIT)
  #undef SCHM_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined SCHM_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef SCHM_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined SCHM_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef SCHM_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined SCHM_START_SEC_VAR_8BIT)
  #undef SCHM_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined SCHM_STOP_SEC_VAR_8BIT)
  #undef SCHM_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined SCHM_START_SEC_VAR_16BIT)
  #undef SCHM_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined SCHM_STOP_SEC_VAR_16BIT)
  #undef SCHM_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined SCHM_START_SEC_VAR_32BIT)
  #undef SCHM_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined SCHM_STOP_SEC_VAR_32BIT)
  #undef SCHM_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined SCHM_START_SEC_VAR_UNSPECIFIED)
  #undef SCHM_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined SCHM_STOP_SEC_VAR_UNSPECIFIED)
  #undef SCHM_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined SCHM_START_SEC_CONST_8BIT)
  #undef SCHM_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined SCHM_STOP_SEC_CONST_8BIT)
  #undef SCHM_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined SCHM_START_SEC_CONST_16BIT)
  #undef SCHM_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined SCHM_STOP_SEC_CONST_16BIT)
  #undef SCHM_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined SCHM_START_SEC_CONST_32BIT)
  #undef SCHM_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined SCHM_STOP_SEC_CONST_32BIT)
  #undef SCHM_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined SCHM_START_SEC_CONST_UNSPECIFIED)
  #undef SCHM_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined SCHM_STOP_SEC_CONST_UNSPECIFIED)
  #undef SCHM_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined SCHM_START_CONFIG_DATA_8BIT)
  #undef SCHM_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined SCHM_STOP_CONFIG_DATA_8BIT)
  #undef SCHM_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined SCHM_START_CONFIG_DATA_16BIT)
  #undef SCHM_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined SCHM_STOP_CONFIG_DATA_16BIT)
  #undef SCHM_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined SCHM_START_CONFIG_DATA_32BIT)
  #undef SCHM_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined SCHM_STOP_CONFIG_DATA_32BIT)
  #undef SCHM_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined SCHM_START_CONFIG_DATA_UNSPECIFIED)
  #undef SCHM_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined SCHM_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef SCHM_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined SCRC_START_SEC_CODE)
  #undef SCRC_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined SCRC_STOP_SEC_CODE)
  #undef SCRC_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined SCRC_START_SEC_APPL_CODE)
  #undef SCRC_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined SCRC_STOP_SEC_APPL_CODE)
  #undef SCRC_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined SCRC_START_SEC_VAR_NOINIT_8BIT)
  #undef SCRC_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined SCRC_STOP_SEC_VAR_NOINIT_8BIT)
  #undef SCRC_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined SCRC_START_SEC_VAR_NOINIT_16BIT)
  #undef SCRC_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined SCRC_STOP_SEC_VAR_NOINIT_16BIT)
  #undef SCRC_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined SCRC_START_SEC_VAR_NOINIT_32BIT)
  #undef SCRC_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined SCRC_STOP_SEC_VAR_NOINIT_32BIT)
  #undef SCRC_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined SCRC_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef SCRC_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined SCRC_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef SCRC_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined SCRC_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef SCRC_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined SCRC_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef SCRC_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined SCRC_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef SCRC_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined SCRC_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef SCRC_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined SCRC_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef SCRC_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined SCRC_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef SCRC_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined SCRC_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef SCRC_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined SCRC_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef SCRC_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined SCRC_START_SEC_VAR_FAST_8BIT)
  #undef SCRC_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined SCRC_STOP_SEC_VAR_FAST_8BIT)
  #undef SCRC_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined SCRC_START_SEC_VAR_FAST_16BIT)
  #undef SCRC_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined SCRC_STOP_SEC_VAR_FAST_16BIT)
  #undef SCRC_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined SCRC_START_SEC_VAR_FAST_32BIT)
  #undef SCRC_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined SCRC_STOP_SEC_VAR_FAST_32BIT)
  #undef SCRC_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined SCRC_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef SCRC_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined SCRC_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef SCRC_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined SCRC_START_SEC_VAR_8BIT)
  #undef SCRC_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined SCRC_STOP_SEC_VAR_8BIT)
  #undef SCRC_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined SCRC_START_SEC_VAR_16BIT)
  #undef SCRC_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined SCRC_STOP_SEC_VAR_16BIT)
  #undef SCRC_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined SCRC_START_SEC_VAR_32BIT)
  #undef SCRC_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined SCRC_STOP_SEC_VAR_32BIT)
  #undef SCRC_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined SCRC_START_SEC_VAR_UNSPECIFIED)
  #undef SCRC_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined SCRC_STOP_SEC_VAR_UNSPECIFIED)
  #undef SCRC_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined SCRC_START_SEC_CONST_8BIT)
  #undef SCRC_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined SCRC_STOP_SEC_CONST_8BIT)
  #undef SCRC_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined SCRC_START_SEC_CONST_16BIT)
  #undef SCRC_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined SCRC_STOP_SEC_CONST_16BIT)
  #undef SCRC_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined SCRC_START_SEC_CONST_32BIT)
  #undef SCRC_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined SCRC_STOP_SEC_CONST_32BIT)
  #undef SCRC_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined SCRC_START_SEC_CONST_UNSPECIFIED)
  #undef SCRC_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined SCRC_STOP_SEC_CONST_UNSPECIFIED)
  #undef SCRC_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined SCRC_START_CONFIG_DATA_8BIT)
  #undef SCRC_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined SCRC_STOP_CONFIG_DATA_8BIT)
  #undef SCRC_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined SCRC_START_CONFIG_DATA_16BIT)
  #undef SCRC_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined SCRC_STOP_CONFIG_DATA_16BIT)
  #undef SCRC_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined SCRC_START_CONFIG_DATA_32BIT)
  #undef SCRC_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined SCRC_STOP_CONFIG_DATA_32BIT)
  #undef SCRC_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined SCRC_START_CONFIG_DATA_UNSPECIFIED)
  #undef SCRC_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined SCRC_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef SCRC_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined SPI_START_SEC_CODE)
  #undef SPI_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined SPI_STOP_SEC_CODE)
  #undef SPI_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined SPI_START_SEC_APPL_CODE)
  #undef SPI_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined SPI_STOP_SEC_APPL_CODE)
  #undef SPI_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined SPI_START_SEC_VAR_NOINIT_8BIT)
  #undef SPI_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined SPI_STOP_SEC_VAR_NOINIT_8BIT)
  #undef SPI_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined SPI_START_SEC_VAR_NOINIT_16BIT)
  #undef SPI_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined SPI_STOP_SEC_VAR_NOINIT_16BIT)
  #undef SPI_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined SPI_START_SEC_VAR_NOINIT_32BIT)
  #undef SPI_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined SPI_STOP_SEC_VAR_NOINIT_32BIT)
  #undef SPI_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined SPI_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef SPI_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined SPI_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef SPI_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined SPI_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef SPI_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined SPI_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef SPI_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined SPI_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef SPI_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined SPI_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef SPI_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined SPI_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef SPI_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined SPI_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef SPI_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined SPI_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef SPI_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined SPI_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef SPI_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined SPI_START_SEC_VAR_FAST_8BIT)
  #undef SPI_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined SPI_STOP_SEC_VAR_FAST_8BIT)
  #undef SPI_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined SPI_START_SEC_VAR_FAST_16BIT)
  #undef SPI_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined SPI_STOP_SEC_VAR_FAST_16BIT)
  #undef SPI_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined SPI_START_SEC_VAR_FAST_32BIT)
  #undef SPI_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined SPI_STOP_SEC_VAR_FAST_32BIT)
  #undef SPI_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined SPI_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef SPI_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined SPI_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef SPI_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined SPI_START_SEC_VAR_8BIT)
  #undef SPI_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined SPI_STOP_SEC_VAR_8BIT)
  #undef SPI_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined SPI_START_SEC_VAR_16BIT)
  #undef SPI_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined SPI_STOP_SEC_VAR_16BIT)
  #undef SPI_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined SPI_START_SEC_VAR_32BIT)
  #undef SPI_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined SPI_STOP_SEC_VAR_32BIT)
  #undef SPI_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined SPI_START_SEC_VAR_UNSPECIFIED)
  #undef SPI_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined SPI_STOP_SEC_VAR_UNSPECIFIED)
  #undef SPI_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined SPI_START_SEC_CONST_8BIT)
  #undef SPI_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined SPI_STOP_SEC_CONST_8BIT)
  #undef SPI_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined SPI_START_SEC_CONST_16BIT)
  #undef SPI_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined SPI_STOP_SEC_CONST_16BIT)
  #undef SPI_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined SPI_START_SEC_CONST_32BIT)
  #undef SPI_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined SPI_STOP_SEC_CONST_32BIT)
  #undef SPI_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined SPI_START_SEC_CONST_UNSPECIFIED)
  #undef SPI_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined SPI_STOP_SEC_CONST_UNSPECIFIED)
  #undef SPI_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined SPI_START_CONFIG_DATA_8BIT)
  #undef SPI_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined SPI_STOP_CONFIG_DATA_8BIT)
  #undef SPI_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined SPI_START_CONFIG_DATA_16BIT)
  #undef SPI_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined SPI_STOP_CONFIG_DATA_16BIT)
  #undef SPI_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined SPI_START_CONFIG_DATA_32BIT)
  #undef SPI_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined SPI_STOP_CONFIG_DATA_32BIT)
  #undef SPI_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined SPI_START_CONFIG_DATA_UNSPECIFIED)
  #undef SPI_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined SPI_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef SPI_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined SWAPSF_START_SEC_CODE)
  #undef SWAPSF_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined SWAPSF_STOP_SEC_CODE)
  #undef SWAPSF_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined SWAPSF_START_SEC_APPL_CODE)
  #undef SWAPSF_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined SWAPSF_STOP_SEC_APPL_CODE)
  #undef SWAPSF_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined SWAPSF_START_SEC_VAR_NOINIT_8BIT)
  #undef SWAPSF_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined SWAPSF_STOP_SEC_VAR_NOINIT_8BIT)
  #undef SWAPSF_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined SWAPSF_START_SEC_VAR_NOINIT_16BIT)
  #undef SWAPSF_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined SWAPSF_STOP_SEC_VAR_NOINIT_16BIT)
  #undef SWAPSF_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined SWAPSF_START_SEC_VAR_NOINIT_32BIT)
  #undef SWAPSF_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined SWAPSF_STOP_SEC_VAR_NOINIT_32BIT)
  #undef SWAPSF_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined SWAPSF_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef SWAPSF_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined SWAPSF_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef SWAPSF_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined SWAPSF_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef SWAPSF_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined SWAPSF_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef SWAPSF_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined SWAPSF_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef SWAPSF_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined SWAPSF_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef SWAPSF_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined SWAPSF_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef SWAPSF_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined SWAPSF_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef SWAPSF_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined SWAPSF_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef SWAPSF_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined SWAPSF_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef SWAPSF_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined SWAPSF_START_SEC_VAR_FAST_8BIT)
  #undef SWAPSF_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined SWAPSF_STOP_SEC_VAR_FAST_8BIT)
  #undef SWAPSF_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined SWAPSF_START_SEC_VAR_FAST_16BIT)
  #undef SWAPSF_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined SWAPSF_STOP_SEC_VAR_FAST_16BIT)
  #undef SWAPSF_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined SWAPSF_START_SEC_VAR_FAST_32BIT)
  #undef SWAPSF_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined SWAPSF_STOP_SEC_VAR_FAST_32BIT)
  #undef SWAPSF_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined SWAPSF_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef SWAPSF_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined SWAPSF_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef SWAPSF_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined SWAPSF_START_SEC_VAR_8BIT)
  #undef SWAPSF_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined SWAPSF_STOP_SEC_VAR_8BIT)
  #undef SWAPSF_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined SWAPSF_START_SEC_VAR_16BIT)
  #undef SWAPSF_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined SWAPSF_STOP_SEC_VAR_16BIT)
  #undef SWAPSF_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined SWAPSF_START_SEC_VAR_32BIT)
  #undef SWAPSF_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined SWAPSF_STOP_SEC_VAR_32BIT)
  #undef SWAPSF_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined SWAPSF_START_SEC_VAR_UNSPECIFIED)
  #undef SWAPSF_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined SWAPSF_STOP_SEC_VAR_UNSPECIFIED)
  #undef SWAPSF_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined SWAPSF_START_SEC_CONST_8BIT)
  #undef SWAPSF_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined SWAPSF_STOP_SEC_CONST_8BIT)
  #undef SWAPSF_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined SWAPSF_START_SEC_CONST_16BIT)
  #undef SWAPSF_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined SWAPSF_STOP_SEC_CONST_16BIT)
  #undef SWAPSF_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined SWAPSF_START_SEC_CONST_32BIT)
  #undef SWAPSF_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined SWAPSF_STOP_SEC_CONST_32BIT)
  #undef SWAPSF_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined SWAPSF_START_SEC_CONST_UNSPECIFIED)
  #undef SWAPSF_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined SWAPSF_STOP_SEC_CONST_UNSPECIFIED)
  #undef SWAPSF_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined SWAPSF_START_CONFIG_DATA_8BIT)
  #undef SWAPSF_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined SWAPSF_STOP_CONFIG_DATA_8BIT)
  #undef SWAPSF_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined SWAPSF_START_CONFIG_DATA_16BIT)
  #undef SWAPSF_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined SWAPSF_STOP_CONFIG_DATA_16BIT)
  #undef SWAPSF_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined SWAPSF_START_CONFIG_DATA_32BIT)
  #undef SWAPSF_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined SWAPSF_STOP_CONFIG_DATA_32BIT)
  #undef SWAPSF_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined SWAPSF_START_CONFIG_DATA_UNSPECIFIED)
  #undef SWAPSF_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined SWAPSF_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef SWAPSF_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined TS5ATL_START_SEC_CODE)
  #undef TS5ATL_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_STOP_SEC_CODE)
  #undef TS5ATL_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_START_SEC_APPL_CODE)
  #undef TS5ATL_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_STOP_SEC_APPL_CODE)
  #undef TS5ATL_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_START_SEC_VAR_NOINIT_8BIT)
  #undef TS5ATL_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_STOP_SEC_VAR_NOINIT_8BIT)
  #undef TS5ATL_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_START_SEC_VAR_NOINIT_16BIT)
  #undef TS5ATL_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_STOP_SEC_VAR_NOINIT_16BIT)
  #undef TS5ATL_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_START_SEC_VAR_NOINIT_32BIT)
  #undef TS5ATL_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_STOP_SEC_VAR_NOINIT_32BIT)
  #undef TS5ATL_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_START_SEC_VAR_NO_INIT_8)
  #undef TS5ATL_START_SEC_VAR_NO_INIT_8
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_STOP_SEC_VAR_NO_INIT_8)
  #undef TS5ATL_STOP_SEC_VAR_NO_INIT_8
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_START_SEC_VAR_NO_INIT_16)
  #undef TS5ATL_START_SEC_VAR_NO_INIT_16
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_STOP_SEC_VAR_NO_INIT_16)
  #undef TS5ATL_STOP_SEC_VAR_NO_INIT_16
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_START_SEC_VAR_NO_INIT_32)
  #undef TS5ATL_START_SEC_VAR_NO_INIT_32
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_STOP_SEC_VAR_NO_INIT_32)
  #undef TS5ATL_STOP_SEC_VAR_NO_INIT_32
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef TS5ATL_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef TS5ATL_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef TS5ATL_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef TS5ATL_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef TS5ATL_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef TS5ATL_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef TS5ATL_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef TS5ATL_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef TS5ATL_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef TS5ATL_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_START_SEC_VAR_FAST_8BIT)
  #undef TS5ATL_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_STOP_SEC_VAR_FAST_8BIT)
  #undef TS5ATL_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_START_SEC_VAR_FAST_16BIT)
  #undef TS5ATL_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_STOP_SEC_VAR_FAST_16BIT)
  #undef TS5ATL_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_START_SEC_VAR_FAST_32BIT)
  #undef TS5ATL_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_STOP_SEC_VAR_FAST_32BIT)
  #undef TS5ATL_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef TS5ATL_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef TS5ATL_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_START_SEC_VAR_8BIT)
  #undef TS5ATL_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_STOP_SEC_VAR_8BIT)
  #undef TS5ATL_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_START_SEC_VAR_16BIT)
  #undef TS5ATL_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_STOP_SEC_VAR_16BIT)
  #undef TS5ATL_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_START_SEC_VAR_32BIT)
  #undef TS5ATL_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_STOP_SEC_VAR_32BIT)
  #undef TS5ATL_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_START_SEC_VAR_UNSPECIFIED)
  #undef TS5ATL_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_STOP_SEC_VAR_UNSPECIFIED)
  #undef TS5ATL_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_START_SEC_CONST_8BIT)
  #undef TS5ATL_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_STOP_SEC_CONST_8BIT)
  #undef TS5ATL_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_START_SEC_CONST_16BIT)
  #undef TS5ATL_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_STOP_SEC_CONST_16BIT)
  #undef TS5ATL_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_START_SEC_CONST_32BIT)
  #undef TS5ATL_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_STOP_SEC_CONST_32BIT)
  #undef TS5ATL_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_START_SEC_CONST_UNSPECIFIED)
  #undef TS5ATL_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_STOP_SEC_CONST_UNSPECIFIED)
  #undef TS5ATL_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_START_CONFIG_DATA_8BIT)
  #undef TS5ATL_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_STOP_CONFIG_DATA_8BIT)
  #undef TS5ATL_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_START_CONFIG_DATA_16BIT)
  #undef TS5ATL_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_STOP_CONFIG_DATA_16BIT)
  #undef TS5ATL_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_START_CONFIG_DATA_32BIT)
  #undef TS5ATL_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_STOP_CONFIG_DATA_32BIT)
  #undef TS5ATL_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_START_CONFIG_DATA_UNSPECIFIED)
  #undef TS5ATL_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef TS5ATL_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_START_SEC_VAR_INIT_8)
  #undef TS5ATL_START_SEC_VAR_INIT_8
  #undef MEMMAP_ERROR

#elif (defined TS5ATL_STOP_SEC_VAR_INIT_8)
  #undef TS5ATL_STOP_SEC_VAR_INIT_8
  #undef MEMMAP_ERROR

/*------------------[Start of a module]--------------------------------------*/

#elif (defined WDG_START_SEC_CODE)
  #undef WDG_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined WDG_STOP_SEC_CODE)
  #undef WDG_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined WDG_START_SEC_APPL_CODE)
  #undef WDG_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined WDG_STOP_SEC_APPL_CODE)
  #undef WDG_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined WDG_START_SEC_VAR_NOINIT_8BIT)
  #undef WDG_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined WDG_STOP_SEC_VAR_NOINIT_8BIT)
  #undef WDG_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined WDG_START_SEC_VAR_NOINIT_16BIT)
  #undef WDG_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined WDG_STOP_SEC_VAR_NOINIT_16BIT)
  #undef WDG_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined WDG_START_SEC_VAR_NOINIT_32BIT)
  #undef WDG_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined WDG_STOP_SEC_VAR_NOINIT_32BIT)
  #undef WDG_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined WDG_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef WDG_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined WDG_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef WDG_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined WDG_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef WDG_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined WDG_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef WDG_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined WDG_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef WDG_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined WDG_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef WDG_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined WDG_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef WDG_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined WDG_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef WDG_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined WDG_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef WDG_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined WDG_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef WDG_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined WDG_START_SEC_VAR_FAST_8BIT)
  #undef WDG_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined WDG_STOP_SEC_VAR_FAST_8BIT)
  #undef WDG_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined WDG_START_SEC_VAR_FAST_16BIT)
  #undef WDG_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined WDG_STOP_SEC_VAR_FAST_16BIT)
  #undef WDG_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined WDG_START_SEC_VAR_FAST_32BIT)
  #undef WDG_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined WDG_STOP_SEC_VAR_FAST_32BIT)
  #undef WDG_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined WDG_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef WDG_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined WDG_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef WDG_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined WDG_START_SEC_VAR_8BIT)
  #undef WDG_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined WDG_STOP_SEC_VAR_8BIT)
  #undef WDG_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined WDG_START_SEC_VAR_16BIT)
  #undef WDG_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined WDG_STOP_SEC_VAR_16BIT)
  #undef WDG_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined WDG_START_SEC_VAR_32BIT)
  #undef WDG_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined WDG_STOP_SEC_VAR_32BIT)
  #undef WDG_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined WDG_START_SEC_VAR_UNSPECIFIED)
  #undef WDG_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined WDG_STOP_SEC_VAR_UNSPECIFIED)
  #undef WDG_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined WDG_START_SEC_CONST_8BIT)
  #undef WDG_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined WDG_STOP_SEC_CONST_8BIT)
  #undef WDG_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined WDG_START_SEC_CONST_16BIT)
  #undef WDG_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined WDG_STOP_SEC_CONST_16BIT)
  #undef WDG_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined WDG_START_SEC_CONST_32BIT)
  #undef WDG_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined WDG_STOP_SEC_CONST_32BIT)
  #undef WDG_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined WDG_START_SEC_CONST_UNSPECIFIED)
  #undef WDG_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined WDG_STOP_SEC_CONST_UNSPECIFIED)
  #undef WDG_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined WDG_START_CONFIG_DATA_8BIT)
  #undef WDG_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined WDG_STOP_CONFIG_DATA_8BIT)
  #undef WDG_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined WDG_START_CONFIG_DATA_16BIT)
  #undef WDG_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined WDG_STOP_CONFIG_DATA_16BIT)
  #undef WDG_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined WDG_START_CONFIG_DATA_32BIT)
  #undef WDG_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined WDG_STOP_CONFIG_DATA_32BIT)
  #undef WDG_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined WDG_START_CONFIG_DATA_UNSPECIFIED)
  #undef WDG_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined WDG_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef WDG_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined WDGIF_START_SEC_CODE)
  #undef WDGIF_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined WDGIF_STOP_SEC_CODE)
  #undef WDGIF_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined WDGIF_START_SEC_APPL_CODE)
  #undef WDGIF_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined WDGIF_STOP_SEC_APPL_CODE)
  #undef WDGIF_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined WDGIF_START_SEC_VAR_NOINIT_8BIT)
  #undef WDGIF_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined WDGIF_STOP_SEC_VAR_NOINIT_8BIT)
  #undef WDGIF_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined WDGIF_START_SEC_VAR_NOINIT_16BIT)
  #undef WDGIF_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined WDGIF_STOP_SEC_VAR_NOINIT_16BIT)
  #undef WDGIF_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined WDGIF_START_SEC_VAR_NOINIT_32BIT)
  #undef WDGIF_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined WDGIF_STOP_SEC_VAR_NOINIT_32BIT)
  #undef WDGIF_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined WDGIF_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef WDGIF_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined WDGIF_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef WDGIF_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined WDGIF_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef WDGIF_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined WDGIF_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef WDGIF_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined WDGIF_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef WDGIF_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined WDGIF_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef WDGIF_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined WDGIF_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef WDGIF_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined WDGIF_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef WDGIF_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined WDGIF_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef WDGIF_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined WDGIF_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef WDGIF_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined WDGIF_START_SEC_VAR_FAST_8BIT)
  #undef WDGIF_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined WDGIF_STOP_SEC_VAR_FAST_8BIT)
  #undef WDGIF_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined WDGIF_START_SEC_VAR_FAST_16BIT)
  #undef WDGIF_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined WDGIF_STOP_SEC_VAR_FAST_16BIT)
  #undef WDGIF_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined WDGIF_START_SEC_VAR_FAST_32BIT)
  #undef WDGIF_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined WDGIF_STOP_SEC_VAR_FAST_32BIT)
  #undef WDGIF_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined WDGIF_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef WDGIF_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined WDGIF_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef WDGIF_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined WDGIF_START_SEC_VAR_8BIT)
  #undef WDGIF_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined WDGIF_STOP_SEC_VAR_8BIT)
  #undef WDGIF_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined WDGIF_START_SEC_VAR_16BIT)
  #undef WDGIF_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined WDGIF_STOP_SEC_VAR_16BIT)
  #undef WDGIF_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined WDGIF_START_SEC_VAR_32BIT)
  #undef WDGIF_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined WDGIF_STOP_SEC_VAR_32BIT)
  #undef WDGIF_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined WDGIF_START_SEC_VAR_UNSPECIFIED)
  #undef WDGIF_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined WDGIF_STOP_SEC_VAR_UNSPECIFIED)
  #undef WDGIF_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined WDGIF_START_SEC_CONST_8BIT)
  #undef WDGIF_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined WDGIF_STOP_SEC_CONST_8BIT)
  #undef WDGIF_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined WDGIF_START_SEC_CONST_16BIT)
  #undef WDGIF_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined WDGIF_STOP_SEC_CONST_16BIT)
  #undef WDGIF_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined WDGIF_START_SEC_CONST_32BIT)
  #undef WDGIF_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined WDGIF_STOP_SEC_CONST_32BIT)
  #undef WDGIF_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined WDGIF_START_SEC_CONST_UNSPECIFIED)
  #undef WDGIF_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined WDGIF_STOP_SEC_CONST_UNSPECIFIED)
  #undef WDGIF_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined WDGIF_START_CONFIG_DATA_8BIT)
  #undef WDGIF_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined WDGIF_STOP_CONFIG_DATA_8BIT)
  #undef WDGIF_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined WDGIF_START_CONFIG_DATA_16BIT)
  #undef WDGIF_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined WDGIF_STOP_CONFIG_DATA_16BIT)
  #undef WDGIF_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined WDGIF_START_CONFIG_DATA_32BIT)
  #undef WDGIF_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined WDGIF_STOP_CONFIG_DATA_32BIT)
  #undef WDGIF_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined WDGIF_START_CONFIG_DATA_UNSPECIFIED)
  #undef WDGIF_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined WDGIF_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef WDGIF_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined WDGM_START_SEC_CODE)
  #undef WDGM_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined WDGM_STOP_SEC_CODE)
  #undef WDGM_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined WDGM_START_SEC_APPL_CODE)
  #undef WDGM_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined WDGM_STOP_SEC_APPL_CODE)
  #undef WDGM_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined WDGM_START_SEC_VAR_NOINIT_8BIT)
  #undef WDGM_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined WDGM_STOP_SEC_VAR_NOINIT_8BIT)
  #undef WDGM_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined WDGM_START_SEC_VAR_NOINIT_16BIT)
  #undef WDGM_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined WDGM_STOP_SEC_VAR_NOINIT_16BIT)
  #undef WDGM_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined WDGM_START_SEC_VAR_NOINIT_32BIT)
  #undef WDGM_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined WDGM_STOP_SEC_VAR_NOINIT_32BIT)
  #undef WDGM_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined WDGM_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef WDGM_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined WDGM_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef WDGM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined WDGM_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef WDGM_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined WDGM_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef WDGM_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined WDGM_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef WDGM_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined WDGM_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef WDGM_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined WDGM_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef WDGM_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined WDGM_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef WDGM_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined WDGM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef WDGM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined WDGM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef WDGM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined WDGM_START_SEC_VAR_FAST_8BIT)
  #undef WDGM_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined WDGM_STOP_SEC_VAR_FAST_8BIT)
  #undef WDGM_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined WDGM_START_SEC_VAR_FAST_16BIT)
  #undef WDGM_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined WDGM_STOP_SEC_VAR_FAST_16BIT)
  #undef WDGM_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined WDGM_START_SEC_VAR_FAST_32BIT)
  #undef WDGM_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined WDGM_STOP_SEC_VAR_FAST_32BIT)
  #undef WDGM_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined WDGM_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef WDGM_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined WDGM_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef WDGM_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined WDGM_START_SEC_VAR_8BIT)
  #undef WDGM_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined WDGM_STOP_SEC_VAR_8BIT)
  #undef WDGM_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined WDGM_START_SEC_VAR_16BIT)
  #undef WDGM_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined WDGM_STOP_SEC_VAR_16BIT)
  #undef WDGM_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined WDGM_START_SEC_VAR_32BIT)
  #undef WDGM_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined WDGM_STOP_SEC_VAR_32BIT)
  #undef WDGM_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined WDGM_START_SEC_VAR_UNSPECIFIED)
  #undef WDGM_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined WDGM_STOP_SEC_VAR_UNSPECIFIED)
  #undef WDGM_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined WDGM_START_SEC_CONST_8BIT)
  #undef WDGM_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined WDGM_STOP_SEC_CONST_8BIT)
  #undef WDGM_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined WDGM_START_SEC_CONST_16BIT)
  #undef WDGM_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined WDGM_STOP_SEC_CONST_16BIT)
  #undef WDGM_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined WDGM_START_SEC_CONST_32BIT)
  #undef WDGM_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined WDGM_STOP_SEC_CONST_32BIT)
  #undef WDGM_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined WDGM_START_SEC_CONST_UNSPECIFIED)
  #undef WDGM_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined WDGM_STOP_SEC_CONST_UNSPECIFIED)
  #undef WDGM_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined WDGM_START_CONFIG_DATA_8BIT)
  #undef WDGM_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined WDGM_STOP_CONFIG_DATA_8BIT)
  #undef WDGM_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined WDGM_START_CONFIG_DATA_16BIT)
  #undef WDGM_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined WDGM_STOP_CONFIG_DATA_16BIT)
  #undef WDGM_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined WDGM_START_CONFIG_DATA_32BIT)
  #undef WDGM_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined WDGM_STOP_CONFIG_DATA_32BIT)
  #undef WDGM_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined WDGM_START_CONFIG_DATA_UNSPECIFIED)
  #undef WDGM_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined WDGM_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef WDGM_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR


/*------------------[Start of a module]--------------------------------------*/

#elif (defined XCP_START_SEC_CODE)
  #undef XCP_START_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined XCP_STOP_SEC_CODE)
  #undef XCP_STOP_SEC_CODE
  #undef MEMMAP_ERROR

#elif (defined XCP_START_SEC_APPL_CODE)
  #undef XCP_START_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined XCP_STOP_SEC_APPL_CODE)
  #undef XCP_STOP_SEC_APPL_CODE
  #undef MEMMAP_ERROR

#elif (defined XCP_START_SEC_VAR_NOINIT_8BIT)
  #undef XCP_START_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined XCP_STOP_SEC_VAR_NOINIT_8BIT)
  #undef XCP_STOP_SEC_VAR_NOINIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined XCP_START_SEC_VAR_NOINIT_16BIT)
  #undef XCP_START_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined XCP_STOP_SEC_VAR_NOINIT_16BIT)
  #undef XCP_STOP_SEC_VAR_NOINIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined XCP_START_SEC_VAR_NOINIT_32BIT)
  #undef XCP_START_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined XCP_STOP_SEC_VAR_NOINIT_32BIT)
  #undef XCP_STOP_SEC_VAR_NOINIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined XCP_START_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef XCP_START_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined XCP_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
  #undef XCP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined XCP_START_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef XCP_START_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined XCP_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
  #undef XCP_STOP_SEC_VAR_POWER_ON_INIT_8BIT
  #undef MEMMAP_ERROR

#elif (defined XCP_START_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef XCP_START_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined XCP_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
  #undef XCP_STOP_SEC_VAR_POWER_ON_INIT_16BIT
  #undef MEMMAP_ERROR

#elif (defined XCP_START_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef XCP_START_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined XCP_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
  #undef XCP_STOP_SEC_VAR_POWER_ON_INIT_32BIT
  #undef MEMMAP_ERROR

#elif (defined XCP_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef XCP_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined XCP_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
  #undef XCP_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined XCP_START_SEC_VAR_FAST_8BIT)
  #undef XCP_START_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined XCP_STOP_SEC_VAR_FAST_8BIT)
  #undef XCP_STOP_SEC_VAR_FAST_8BIT
  #undef MEMMAP_ERROR

#elif (defined XCP_START_SEC_VAR_FAST_16BIT)
  #undef XCP_START_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined XCP_STOP_SEC_VAR_FAST_16BIT)
  #undef XCP_STOP_SEC_VAR_FAST_16BIT
  #undef MEMMAP_ERROR

#elif (defined XCP_START_SEC_VAR_FAST_32BIT)
  #undef XCP_START_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined XCP_STOP_SEC_VAR_FAST_32BIT)
  #undef XCP_STOP_SEC_VAR_FAST_32BIT
  #undef MEMMAP_ERROR

#elif (defined XCP_START_SEC_VAR_FAST_UNSPECIFIED)
  #undef XCP_START_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined XCP_STOP_SEC_VAR_FAST_UNSPECIFIED)
  #undef XCP_STOP_SEC_VAR_FAST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined XCP_START_SEC_VAR_8BIT)
  #undef XCP_START_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined XCP_STOP_SEC_VAR_8BIT)
  #undef XCP_STOP_SEC_VAR_8BIT
  #undef MEMMAP_ERROR

#elif (defined XCP_START_SEC_VAR_16BIT)
  #undef XCP_START_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined XCP_STOP_SEC_VAR_16BIT)
  #undef XCP_STOP_SEC_VAR_16BIT
  #undef MEMMAP_ERROR

#elif (defined XCP_START_SEC_VAR_32BIT)
  #undef XCP_START_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined XCP_STOP_SEC_VAR_32BIT)
  #undef XCP_STOP_SEC_VAR_32BIT
  #undef MEMMAP_ERROR

#elif (defined XCP_START_SEC_VAR_UNSPECIFIED)
  #undef XCP_START_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined XCP_STOP_SEC_VAR_UNSPECIFIED)
  #undef XCP_STOP_SEC_VAR_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined XCP_START_SEC_CONST_8BIT)
  #undef XCP_START_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined XCP_STOP_SEC_CONST_8BIT)
  #undef XCP_STOP_SEC_CONST_8BIT
  #undef MEMMAP_ERROR

#elif (defined XCP_START_SEC_CONST_16BIT)
  #undef XCP_START_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined XCP_STOP_SEC_CONST_16BIT)
  #undef XCP_STOP_SEC_CONST_16BIT
  #undef MEMMAP_ERROR

#elif (defined XCP_START_SEC_CONST_32BIT)
  #undef XCP_START_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined XCP_STOP_SEC_CONST_32BIT)
  #undef XCP_STOP_SEC_CONST_32BIT
  #undef MEMMAP_ERROR

#elif (defined XCP_START_SEC_CONST_UNSPECIFIED)
  #undef XCP_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined XCP_STOP_SEC_CONST_UNSPECIFIED)
  #undef XCP_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined XCP_START_CONFIG_DATA_8BIT)
  #undef XCP_START_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined XCP_STOP_CONFIG_DATA_8BIT)
  #undef XCP_STOP_CONFIG_DATA_8BIT
  #undef MEMMAP_ERROR

#elif (defined XCP_START_CONFIG_DATA_16BIT)
  #undef XCP_START_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined XCP_STOP_CONFIG_DATA_16BIT)
  #undef XCP_STOP_CONFIG_DATA_16BIT
  #undef MEMMAP_ERROR

#elif (defined XCP_START_CONFIG_DATA_32BIT)
  #undef XCP_START_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined XCP_STOP_CONFIG_DATA_32BIT)
  #undef XCP_STOP_CONFIG_DATA_32BIT
  #undef MEMMAP_ERROR

#elif (defined XCP_START_CONFIG_DATA_UNSPECIFIED)
  #undef XCP_START_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

#elif (defined XCP_STOP_CONFIG_DATA_UNSPECIFIED)
  #undef XCP_STOP_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR

/* Memory section macros for CanTp */
#elif (defined CANTP_START_SEC_CONST_ENTRY_JUMP_TABLE)
    #undef CANTP_START_SEC_CONST_ENTRY_JUMP_TABLE
    #undef MEMMAP_ERROR
#elif (defined CANTP_STOP_SEC_CONST_ENTRY_JUMP_TABLE)
    #undef CANTP_STOP_SEC_CONST_ENTRY_JUMP_TABLE
    #undef MEMMAP_ERROR
#elif (defined CANTP_START_SEC_CONST_UNSPECIFIED)
    #undef CANTP_START_SEC_CONST_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined CANTP_STOP_SEC_CONST_UNSPECIFIED)
    #undef CANTP_STOP_SEC_CONST_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined CANTP_START_SEC_CONST_EXIT_JUMP_TABLE)
    #undef CANTP_START_SEC_CONST_EXIT_JUMP_TABLE
    #undef MEMMAP_ERROR
#elif (defined CANTP_STOP_SEC_CONST_EXIT_JUMP_TABLE)
    #undef CANTP_STOP_SEC_CONST_EXIT_JUMP_TABLE
    #undef MEMMAP_ERROR
#elif (defined CANTP_START_SEC_JUMP_TABLE_SHARED_VAR_INIT_UNSPECIFIED)
    #undef CANTP_START_SEC_JUMP_TABLE_SHARED_VAR_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined CANTP_STOP_SEC_JUMP_TABLE_SHARED_VAR_INIT_UNSPECIFIED)
    #undef CANTP_STOP_SEC_JUMP_TABLE_SHARED_VAR_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined CANTP_START_SEC_VAR_NO_INIT_8)
    #undef CANTP_START_SEC_VAR_NO_INIT_8
    #undef MEMMAP_ERROR
#elif (defined CANTP_STOP_SEC_VAR_NO_INIT_8)
    #undef CANTP_STOP_SEC_VAR_NO_INIT_8
    #undef MEMMAP_ERROR
#elif (defined CANTP_START_SEC_VAR_NO_INIT_UNSPECIFIED)
    #undef CANTP_START_SEC_VAR_NO_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined CANTP_STOP_SEC_VAR_NO_INIT_UNSPECIFIED)
    #undef CANTP_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined CANTP_START_SEC_CONFIG_DATA_UNSPECIFIED)
    #undef CANTP_START_SEC_CONFIG_DATA_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined CANTP_STOP_SEC_CONFIG_DATA_UNSPECIFIED)
    #undef CANTP_STOP_SEC_CONFIG_DATA_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined CANTP_START_CONFIG_DATA_UNSPECIFIED)
    #undef CANTP_START_CONFIG_DATA_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined CANTP_STOP_CONFIG_DATA_UNSPECIFIED)
    #undef CANTP_STOP_CONFIG_DATA_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined CANTP_START_SEC_VAR_INIT_UNSPECIFIED)
    #undef CANTP_START_SEC_VAR_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined CANTP_STOP_SEC_VAR_INIT_UNSPECIFIED)
    #undef CANTP_STOP_SEC_VAR_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined CANTP_START_SEC_CODE)
    #undef CANTP_START_SEC_CODE
    #undef MEMMAP_ERROR
#elif (defined CANTP_STOP_SEC_CODE)
    #undef CANTP_STOP_SEC_CODE
    #undef MEMMAP_ERROR
#elif (defined CANTP_START_SEC_CONST_32)
    #undef CANTP_START_SEC_CONST_32
    #undef MEMMAP_ERROR
#elif (defined CANTP_STOP_SEC_CONST_32)
    #undef CANTP_STOP_SEC_CONST_32
    #undef MEMMAP_ERROR

/* Memory section macros for CanIf */
#elif (defined CANIF_START_SEC_CONST_32)
    #undef CANIF_START_SEC_CONST_32
    #undef MEMMAP_ERROR
#elif (defined CANIF_STOP_SEC_CONST_32)
    #undef CANIF_STOP_SEC_CONST_32
    #undef MEMMAP_ERROR
#elif (defined CANIF_START_SEC_VAR_INIT_8)
    #undef CANIF_START_SEC_VAR_INIT_8
    #undef MEMMAP_ERROR
#elif (defined CANIF_STOP_SEC_VAR_INIT_8)
    #undef CANIF_STOP_SEC_VAR_INIT_8
    #undef MEMMAP_ERROR
#elif (defined CANIF_START_SEC_CONST_8)
    #undef CANIF_START_SEC_CONST_8
    #undef MEMMAP_ERROR
#elif (defined CANIF_STOP_SEC_CONST_8)
    #undef CANIF_STOP_SEC_CONST_8
    #undef MEMMAP_ERROR
#elif (defined CANIF_START_SEC_VAR_INIT_UNSPECIFIED)
    #undef CANIF_START_SEC_VAR_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined CANIF_STOP_SEC_VAR_INIT_UNSPECIFIED)
    #undef CANIF_STOP_SEC_VAR_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined CANIF_START_SEC_VAR_NO_INIT_8)
    #undef CANIF_START_SEC_VAR_NO_INIT_8
    #undef MEMMAP_ERROR
#elif (defined CANIF_STOP_SEC_VAR_NO_INIT_8)
    #undef CANIF_STOP_SEC_VAR_NO_INIT_8
    #undef MEMMAP_ERROR
#elif (defined CANIF_START_SEC_VAR_NO_INIT_16)
    #undef CANIF_START_SEC_VAR_NO_INIT_16
    #undef MEMMAP_ERROR
#elif (defined CANIF_STOP_SEC_VAR_NO_INIT_16)
    #undef CANIF_STOP_SEC_VAR_NO_INIT_16
    #undef MEMMAP_ERROR
#elif (defined CANIF_START_SEC_VAR_NO_INIT_32)
    #undef CANIF_START_SEC_VAR_NO_INIT_32
    #undef MEMMAP_ERROR
#elif (defined CANIF_STOP_SEC_VAR_NO_INIT_32)
    #undef CANIF_STOP_SEC_VAR_NO_INIT_32
    #undef MEMMAP_ERROR
#elif (defined CANIF_START_SEC_VAR_NO_INIT_UNSPECIFIED)
    #undef CANIF_START_SEC_VAR_NO_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined CANIF_STOP_SEC_VAR_NO_INIT_UNSPECIFIED)
    #undef CANIF_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined CANIF_START_SEC_CONST_UNSPECIFIED)
    #undef CANIF_START_SEC_CONST_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined CANIF_STOP_SEC_CONST_UNSPECIFIED)
    #undef CANIF_STOP_SEC_CONST_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined CANIF_START_SEC_CODE)
    #undef CANIF_START_SEC_CODE
    #undef MEMMAP_ERROR
#elif (defined CANIF_STOP_SEC_CODE)
    #undef CANIF_STOP_SEC_CODE
    #undef MEMMAP_ERROR
#elif (defined CANIF_START_SEC_CONFIG_DATA_UNSPECIFIED)
    #undef CANIF_START_SEC_CONFIG_DATA_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined CANIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED)
    #undef CANIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined CANIF_START_CONFIG_DATA_UNSPECIFIED)
    #undef CANIF_START_CONFIG_DATA_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined CANIF_STOP_CONFIG_DATA_UNSPECIFIED)
    #undef CANIF_STOP_CONFIG_DATA_UNSPECIFIED
    #undef MEMMAP_ERROR

/* Memory section macros for Can */
#elif (defined CAN_START_SEC_CODE)
    #undef CAN_START_SEC_CODE
    #undef MEMMAP_ERROR
#elif (defined CAN_STOP_SEC_CODE)
    #undef CAN_STOP_SEC_CODE
    #undef MEMMAP_ERROR
#elif (defined CAN_START_SEC_CONFIG_DATA_UNSPECIFIED)
    #undef CAN_START_SEC_CONFIG_DATA_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined CAN_STOP_SEC_CONFIG_DATA_UNSPECIFIED)
    #undef CAN_STOP_SEC_CONFIG_DATA_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined CAN_START_CONFIG_DATA_UNSPECIFIED)
    #undef CAN_START_CONFIG_DATA_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined CAN_STOP_CONFIG_DATA_UNSPECIFIED)
    #undef CAN_STOP_CONFIG_DATA_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined CAN_START_SEC_CONST_UNSPECIFIED)
    #undef CAN_START_SEC_CONST_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined CAN_STOP_SEC_CONST_UNSPECIFIED)
    #undef CAN_STOP_SEC_CONST_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined CAN_START_SEC_RAMCODE)
    #undef CAN_START_SEC_RAMCODE
    #undef MEMMAP_ERROR
#elif (defined CAN_STOP_SEC_RAMCODE)
    #undef CAN_STOP_SEC_RAMCODE
    #undef MEMMAP_ERROR
#elif (defined CAN_START_SEC_VAR_INIT_UNSPECIFIED)
    #undef CAN_START_SEC_VAR_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined CAN_STOP_SEC_VAR_INIT_UNSPECIFIED)
    #undef CAN_STOP_SEC_VAR_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined CAN_START_SEC_VAR_NO_INIT_32)
    #undef CAN_START_SEC_VAR_NO_INIT_32
    #undef MEMMAP_ERROR
#elif (defined CAN_STOP_SEC_VAR_NO_INIT_32)
    #undef CAN_STOP_SEC_VAR_NO_INIT_32
    #undef MEMMAP_ERROR
#elif (defined CAN_START_SEC_VAR_NO_INIT_UNSPECIFIED)
    #undef CAN_START_SEC_VAR_NO_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined CAN_STOP_SEC_VAR_NO_INIT_UNSPECIFIED)
    #undef CAN_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR

/* Memory section macros for PduR */
#elif (defined PDUR_START_SEC_VAR_NOINIT_UNSPECIFIED)
    #undef PDUR_START_SEC_VAR_NOINIT_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined PDUR_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
    #undef PDUR_STOP_SEC_VAR_NOINIT_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined PDUR_START_SEC_VAR_FAST_INIT_UNSPECIFIED)
    #undef PDUR_START_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined PDUR_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED)
    #undef PDUR_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined PDUR_START_SEC_VAR_INIT_UNSPECIFIED)
    #undef PDUR_START_SEC_VAR_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined PDUR_STOP_SEC_VAR_INIT_UNSPECIFIED)
    #undef PDUR_STOP_SEC_VAR_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined PDUR_START_SEC_CODE)
    #undef PDUR_START_SEC_CODE
    #undef MEMMAP_ERROR
#elif (defined PDUR_STOP_SEC_CODE)
    #undef PDUR_STOP_SEC_CODE
    #undef MEMMAP_ERROR
#elif (defined PDUR_START_SEC_CONFIG_DATA_UNSPECIFIED)
    #undef PDUR_START_SEC_CONFIG_DATA_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined PDUR_STOP_SEC_CONFIG_DATA_UNSPECIFIED)
    #undef PDUR_STOP_SEC_CONFIG_DATA_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined PDUR_START_CONFIG_DATA_UNSPECIFIED)
    #undef PDUR_START_CONFIG_DATA_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined PDUR_STOP_CONFIG_DATA_UNSPECIFIED)
    #undef PDUR_STOP_CONFIG_DATA_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined PDUR_START_SEC_CONST_8)
    #undef PDUR_START_SEC_CONST_8
    #undef MEMMAP_ERROR
#elif (defined PDUR_STOP_SEC_CONST_8)
    #undef PDUR_STOP_SEC_CONST_8
    #undef MEMMAP_ERROR
#elif (defined PDUR_START_SEC_CONST_32)
    #undef PDUR_START_SEC_CONST_32
    #undef MEMMAP_ERROR
#elif (defined PDUR_STOP_SEC_CONST_32)
    #undef PDUR_STOP_SEC_CONST_32
    #undef MEMMAP_ERROR
#elif (defined PDUR_START_SEC_CONST_UNSPECIFIED)
    #undef PDUR_START_SEC_CONST_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined PDUR_STOP_SEC_CONST_UNSPECIFIED)
    #undef PDUR_STOP_SEC_CONST_UNSPECIFIED
    #undef MEMMAP_ERROR

/* Memory section macros for CanSM */
#elif (defined CANSM_START_SEC_VAR_INIT_UNSPECIFIED)
    #undef CANSM_START_SEC_VAR_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined CANSM_STOP_SEC_VAR_INIT_UNSPECIFIED)
    #undef CANSM_STOP_SEC_VAR_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined CANSM_START_SEC_VAR_INIT_8)
    #undef CANSM_START_SEC_VAR_INIT_8
    #undef MEMMAP_ERROR
#elif (defined CANSM_STOP_SEC_VAR_INIT_8)
    #undef CANSM_STOP_SEC_VAR_INIT_8
    #undef MEMMAP_ERROR
#elif (defined CANSM_START_SEC_VAR_NO_INIT_8)
    #undef CANSM_START_SEC_VAR_NO_INIT_8
    #undef MEMMAP_ERROR
#elif (defined CANSM_STOP_SEC_VAR_NO_INIT_8)
    #undef CANSM_STOP_SEC_VAR_NO_INIT_8
    #undef MEMMAP_ERROR
#elif (defined CANSM_START_SEC_VAR_NO_INIT_UNSPECIFIED)
    #undef CANSM_START_SEC_VAR_NO_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined CANSM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED)
    #undef CANSM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined CANSM_START_SEC_CODE)
    #undef CANSM_START_SEC_CODE
    #undef MEMMAP_ERROR
#elif (defined CANSM_STOP_SEC_CODE)
    #undef CANSM_STOP_SEC_CODE
    #undef MEMMAP_ERROR
#elif (defined CANSM_START_SEC_CONFIG_DATA_8)
    #undef CANSM_START_SEC_CONFIG_DATA_8
    #undef MEMMAP_ERROR
#elif (defined CANSM_STOP_SEC_CONFIG_DATA_8)
    #undef CANSM_STOP_SEC_CONFIG_DATA_8
    #undef MEMMAP_ERROR
#elif (defined CANSM_START_CONFIG_DATA_8)
    #undef CANSM_START_CONFIG_DATA_8
    #undef MEMMAP_ERROR
#elif (defined CANSM_STOP_CONFIG_DATA_8)
    #undef CANSM_STOP_CONFIG_DATA_8
    #undef MEMMAP_ERROR
#elif (defined CANSM_START_SEC_CONFIG_DATA_UNSPECIFIED)
    #undef CANSM_START_SEC_CONFIG_DATA_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined CANSM_STOP_SEC_CONFIG_DATA_UNSPECIFIED)
    #undef CANSM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined CANSM_START_CONFIG_DATA_UNSPECIFIED)
    #undef CANSM_START_CONFIG_DATA_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined CANSM_STOP_CONFIG_DATA_UNSPECIFIED)
    #undef CANSM_STOP_CONFIG_DATA_UNSPECIFIED
    #undef MEMMAP_ERROR

/* Memory section macros for Det */
#elif (defined DET_START_SEC_VAR_NO_INIT_UNSPECIFIED)
    #undef DET_START_SEC_VAR_NO_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined DET_STOP_SEC_VAR_NO_INIT_UNSPECIFIED)
    #undef DET_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined DET_START_SEC_CODE)
    #undef DET_START_SEC_CODE
    #undef MEMMAP_ERROR
#elif (defined DET_STOP_SEC_CODE)
    #undef DET_STOP_SEC_CODE
    #undef MEMMAP_ERROR
#elif (defined DET_START_SEC_VAR_INIT_8)
    #undef DET_START_SEC_VAR_INIT_8
    #undef MEMMAP_ERROR
#elif (defined DET_STOP_SEC_VAR_INIT_8)
    #undef DET_STOP_SEC_VAR_INIT_8
    #undef MEMMAP_ERROR
#elif (defined DET_START_SEC_VAR_NO_INIT_16)
    #undef DET_START_SEC_VAR_NO_INIT_16
    #undef MEMMAP_ERROR
#elif (defined DET_STOP_SEC_VAR_NO_INIT_16)
    #undef DET_STOP_SEC_VAR_NO_INIT_16
    #undef MEMMAP_ERROR

/* Memory section macros for EBStubs Trace */
#elif (defined EBSTUBS_START_SEC_VAR_NO_INIT_UNSPECIFIED)
    #undef EBSTUBS_START_SEC_VAR_NO_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined EBSTUBS_STOP_SEC_VAR_NO_INIT_UNSPECIFIED)
    #undef EBSTUBS_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif (defined EBSTUBS_START_SEC_CODE)
    #undef EBSTUBS_START_SEC_CODE
    #undef MEMMAP_ERROR
#elif (defined EBSTUBS_STOP_SEC_CODE)
    #undef EBSTUBS_STOP_SEC_CODE
    #undef MEMMAP_ERROR
#elif (defined EBSTUBS_START_SEC_VAR_INIT_16)
    #undef EBSTUBS_START_SEC_VAR_INIT_16
    #undef MEMMAP_ERROR
#elif (defined EBSTUBS_STOP_SEC_VAR_INIT_16)
    #undef EBSTUBS_STOP_SEC_VAR_INIT_16
    #undef MEMMAP_ERROR

#endif

#if (defined __TenDRA__)
/* turn on "no #undef" warnings again */
#pragma TenDRA error "misra98_092" warning
#pragma TenDRA error "misra04_19_6" warning
#endif

/*------------------[MemMap error checking]----------------------------------*/

#if (defined MEMMAP_ERROR) /* to prevent double definition */
#error MEMMAP_ERROR defined, wrong MemMap.h usage
#endif /* if (!defined MEMMAP_ERROR) */

/* CHECK: PARSE */
/*==================[end of file]============================================*/
