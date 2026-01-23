/**
 * \file
 *
 * \blief Board startup files
 *
 * This file contains the implementation of the hardware
 * interrupt vector table
 *
 * \author Elektrobit France S.A.S
 *
 * Copyright 2005 - 2015 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH, 
 * unless expressly agreed to otherwise.
 */

/* file is nammed vector_vle_eb.s
 * !LINKSTO OsekCore.Platforms.Board.012,  1
 */

/*define to exclude C Code in header files*/
#define BOARD_ASM    1

/* Must be first */
#include <Platforms_asm_PA.h>
#include <board.h>


 #define BOARD_GEN_VECTOR(name ,  id ,  func, alignP2) \
  ISRVector name,   \
            id,     \
            func,    \
            alignP2

#define   BOARD_GEN_EXCEPTIONS(name ,  id ,  func, alignP2) \
  TrapVector name,  \
             id,    \
             func,   \
             alignP2

    Platforms_file(vector_vle_eb.s)
    
    Platforms_global IntcVectorTableHw

    Platforms_global BoardIsrHandler
    Platforms_global BoardInvalidException

    PLATFORMS_ASM_SECTION_INTCTAB
    PLATFORMS_ASM_VLE

/*
 * define symbols as global to allow easier debugging
 */
    Platforms_global IntcVectorTableHw


/*
 * intc vector table for HW vector mode at IVPR
 */
 /* All ISRs are mapped to BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2 to catch all unwanted exceptions
 * !LINKSTO OsekCore.Platforms.Board.014,  1
 */
IntcVectorTableHw:
#if BOARD_NB_VECTORS >= 0
  BOARD_GEN_VECTOR(Board_Prio_ISR_00, 0, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 00
#endif
#if BOARD_NB_VECTORS >= 1
  BOARD_GEN_VECTOR(Board_Prio_ISR_01, 1, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 1
#endif
#if BOARD_NB_VECTORS >= 2
  BOARD_GEN_VECTOR(Board_Prio_ISR_02, 2, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 2
#endif
#if BOARD_NB_VECTORS >= 3
  BOARD_GEN_VECTOR(Board_Prio_ISR_03, 3, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 3
#endif
#if BOARD_NB_VECTORS >= 4
  BOARD_GEN_VECTOR(Board_Prio_ISR_04, 4, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 4
#endif
#if BOARD_NB_VECTORS >= 5
  BOARD_GEN_VECTOR(Board_Prio_ISR_05, 5, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 5
#endif
#if BOARD_NB_VECTORS >= 6
  BOARD_GEN_VECTOR(Board_Prio_ISR_06, 6, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 6
#endif
#if BOARD_NB_VECTORS >= 7
  BOARD_GEN_VECTOR(Board_Prio_ISR_07, 7, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 7
#endif
#if BOARD_NB_VECTORS >= 8
  BOARD_GEN_VECTOR(Board_Prio_ISR_08, 8, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 8
#endif
#if BOARD_NB_VECTORS >= 9
  BOARD_GEN_VECTOR(Board_Prio_ISR_09, 9, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 9
#endif
#if BOARD_NB_VECTORS >= 10
  BOARD_GEN_VECTOR(Board_Prio_ISR_10, 10, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 10
#endif                            
#if BOARD_NB_VECTORS >= 11        
  BOARD_GEN_VECTOR(Board_Prio_ISR_11, 11, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 11
#endif                            
#if BOARD_NB_VECTORS >= 12        
  BOARD_GEN_VECTOR(Board_Prio_ISR_12, 12, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 12
#endif                            
#if BOARD_NB_VECTORS >= 13        
  BOARD_GEN_VECTOR(Board_Prio_ISR_13, 13, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 13
#endif                            
#if BOARD_NB_VECTORS >= 14        
  BOARD_GEN_VECTOR(Board_Prio_ISR_14, 14, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 14
#endif                            
#if BOARD_NB_VECTORS >= 15        
  BOARD_GEN_VECTOR(Board_Prio_ISR_15, 15, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 15
#endif                            
#if BOARD_NB_VECTORS >= 16        
  BOARD_GEN_VECTOR(Board_Prio_ISR_16, 16, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 16
#endif                            
#if BOARD_NB_VECTORS >= 17        
  BOARD_GEN_VECTOR(Board_Prio_ISR_17, 17, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 17
#endif                            
#if BOARD_NB_VECTORS >= 18        
  BOARD_GEN_VECTOR(Board_Prio_ISR_18, 18, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 18
#endif                            
#if BOARD_NB_VECTORS >= 19        
  BOARD_GEN_VECTOR(Board_Prio_ISR_19, 19, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 19
#endif
#if BOARD_NB_VECTORS >= 20
  BOARD_GEN_VECTOR(Board_Prio_ISR_20, 20, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 20
#endif                            
#if BOARD_NB_VECTORS >= 21        
  BOARD_GEN_VECTOR(Board_Prio_ISR_21, 21, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 21
#endif                            
#if BOARD_NB_VECTORS >= 22        
  BOARD_GEN_VECTOR(Board_Prio_ISR_22, 22, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 22
#endif                            
#if BOARD_NB_VECTORS >= 23        
  BOARD_GEN_VECTOR(Board_Prio_ISR_23, 23, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 23
#endif                            
#if BOARD_NB_VECTORS >= 24        
  BOARD_GEN_VECTOR(Board_Prio_ISR_24, 24, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 24
#endif                            
#if BOARD_NB_VECTORS >= 25        
  BOARD_GEN_VECTOR(Board_Prio_ISR_25, 25, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 25
#endif                            
#if BOARD_NB_VECTORS >= 26        
  BOARD_GEN_VECTOR(Board_Prio_ISR_26, 26, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 26
#endif                            
#if BOARD_NB_VECTORS >= 27        
  BOARD_GEN_VECTOR(Board_Prio_ISR_27, 27, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 27
#endif                            
#if BOARD_NB_VECTORS >= 28        
  BOARD_GEN_VECTOR(Board_Prio_ISR_28, 28, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 28
#endif                            
#if BOARD_NB_VECTORS >= 29        
  BOARD_GEN_VECTOR(Board_Prio_ISR_29, 29, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 29
#endif
#if BOARD_NB_VECTORS >= 30
  BOARD_GEN_VECTOR(Board_Prio_ISR_30, 30, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 30
#endif                            
#if BOARD_NB_VECTORS >= 31        
  BOARD_GEN_VECTOR(Board_Prio_ISR_31, 31, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 31
#endif                            
#if BOARD_NB_VECTORS >= 32        
  BOARD_GEN_VECTOR(Board_Prio_ISR_32, 32, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 32
#endif                            
#if BOARD_NB_VECTORS >= 33        
  BOARD_GEN_VECTOR(Board_Prio_ISR_33, 33, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 33
#endif                            
#if BOARD_NB_VECTORS >= 34        
  BOARD_GEN_VECTOR(Board_Prio_ISR_34, 34, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 34
#endif                            
#if BOARD_NB_VECTORS >= 35        
  BOARD_GEN_VECTOR(Board_Prio_ISR_35, 35, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 35
#endif                            
#if BOARD_NB_VECTORS >= 36        
  BOARD_GEN_VECTOR(Board_Prio_ISR_36, 36, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 36
#endif                            
#if BOARD_NB_VECTORS >= 37        
  BOARD_GEN_VECTOR(Board_Prio_ISR_37, 37, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 37
#endif                            
#if BOARD_NB_VECTORS >= 38        
  BOARD_GEN_VECTOR(Board_Prio_ISR_38, 38, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 38
#endif                            
#if BOARD_NB_VECTORS >= 39        
  BOARD_GEN_VECTOR(Board_Prio_ISR_39, 39, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 39
#endif
#if BOARD_NB_VECTORS >= 40
  BOARD_GEN_VECTOR(Board_Prio_ISR_40, 40, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 40
#endif                            
#if BOARD_NB_VECTORS >= 41        
  BOARD_GEN_VECTOR(Board_Prio_ISR_41, 41, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 41
#endif                            
#if BOARD_NB_VECTORS >= 42        
  BOARD_GEN_VECTOR(Board_Prio_ISR_42, 42, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 42
#endif                            
#if BOARD_NB_VECTORS >= 43        
  BOARD_GEN_VECTOR(Board_Prio_ISR_43, 43, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 43
#endif                            
#if BOARD_NB_VECTORS >= 44        
  BOARD_GEN_VECTOR(Board_Prio_ISR_44, 44, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 44
#endif                            
#if BOARD_NB_VECTORS >= 45        
  BOARD_GEN_VECTOR(Board_Prio_ISR_45, 45, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 45
#endif                            
#if BOARD_NB_VECTORS >= 46        
  BOARD_GEN_VECTOR(Board_Prio_ISR_46, 46, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 46
#endif                            
#if BOARD_NB_VECTORS >= 47        
  BOARD_GEN_VECTOR(Board_Prio_ISR_47, 47, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 47
#endif                            
#if BOARD_NB_VECTORS >= 48        
  BOARD_GEN_VECTOR(Board_Prio_ISR_48, 48, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 48
#endif                            
#if BOARD_NB_VECTORS >= 49        
  BOARD_GEN_VECTOR(Board_Prio_ISR_49, 49, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 49
#endif
#if BOARD_NB_VECTORS >= 50
  BOARD_GEN_VECTOR(Board_Prio_ISR_50, 50, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 50
#endif                            
#if BOARD_NB_VECTORS >= 51        
  BOARD_GEN_VECTOR(Board_Prio_ISR_51, 51, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 51
#endif                            
#if BOARD_NB_VECTORS >= 52        
  BOARD_GEN_VECTOR(Board_Prio_ISR_52, 52, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 52
#endif                            
#if BOARD_NB_VECTORS >= 53        
  BOARD_GEN_VECTOR(Board_Prio_ISR_53, 53, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 53
#endif                            
#if BOARD_NB_VECTORS >= 54        
  BOARD_GEN_VECTOR(Board_Prio_ISR_54, 54, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 54
#endif                            
#if BOARD_NB_VECTORS >= 55        
  BOARD_GEN_VECTOR(Board_Prio_ISR_55, 55, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 55
#endif                            
#if BOARD_NB_VECTORS >= 56        
  BOARD_GEN_VECTOR(Board_Prio_ISR_56, 56, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 56
#endif                            
#if BOARD_NB_VECTORS >= 57        
  BOARD_GEN_VECTOR(Board_Prio_ISR_57, 57, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 57
#endif                            
#if BOARD_NB_VECTORS >= 58        
  BOARD_GEN_VECTOR(Board_Prio_ISR_58, 58, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 58
#endif                            
#if BOARD_NB_VECTORS >= 59        
  BOARD_GEN_VECTOR(Board_Prio_ISR_59, 59, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 59
#endif
#if BOARD_NB_VECTORS >= 60
  BOARD_GEN_VECTOR(Board_Prio_ISR_60, 60, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 60
#endif                            
#if BOARD_NB_VECTORS >= 61        
  BOARD_GEN_VECTOR(Board_Prio_ISR_61, 61, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 61
#endif                            
#if BOARD_NB_VECTORS >= 62        
  BOARD_GEN_VECTOR(Board_Prio_ISR_62, 62, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 62
#endif                            
#if BOARD_NB_VECTORS >= 63        
  BOARD_GEN_VECTOR(Board_Prio_ISR_63, 63, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 63
#endif                            
#if BOARD_NB_VECTORS >= 64        
  BOARD_GEN_VECTOR(Board_Prio_ISR_64, 64, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 64
#endif                            
#if BOARD_NB_VECTORS >= 65        
  BOARD_GEN_VECTOR(Board_Prio_ISR_65, 65, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 65
#endif                            
#if BOARD_NB_VECTORS >= 66        
  BOARD_GEN_VECTOR(Board_Prio_ISR_66, 66, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 66
#endif                            
#if BOARD_NB_VECTORS >= 67        
  BOARD_GEN_VECTOR(Board_Prio_ISR_67, 67, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 67
#endif                            
#if BOARD_NB_VECTORS >= 68        
  BOARD_GEN_VECTOR(Board_Prio_ISR_68, 68, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 68
#endif                            
#if BOARD_NB_VECTORS >= 69        
  BOARD_GEN_VECTOR(Board_Prio_ISR_69, 69, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 69
#endif
#if BOARD_NB_VECTORS >= 70
  BOARD_GEN_VECTOR(Board_Prio_ISR_70, 70, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 70
#endif                            
#if BOARD_NB_VECTORS >= 71        
  BOARD_GEN_VECTOR(Board_Prio_ISR_71, 71, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 71
#endif                            
#if BOARD_NB_VECTORS >= 72        
  BOARD_GEN_VECTOR(Board_Prio_ISR_72, 72, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 72
#endif                            
#if BOARD_NB_VECTORS >= 73        
  BOARD_GEN_VECTOR(Board_Prio_ISR_73, 73, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 73
#endif                            
#if BOARD_NB_VECTORS >= 74        
  BOARD_GEN_VECTOR(Board_Prio_ISR_74, 74, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 74
#endif                            
#if BOARD_NB_VECTORS >= 75        
  BOARD_GEN_VECTOR(Board_Prio_ISR_75, 75, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 75
#endif                            
#if BOARD_NB_VECTORS >= 76        
  BOARD_GEN_VECTOR(Board_Prio_ISR_76, 76, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 76
#endif                            
#if BOARD_NB_VECTORS >= 77        
  BOARD_GEN_VECTOR(Board_Prio_ISR_77, 77, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 77
#endif                            
#if BOARD_NB_VECTORS >= 78        
  BOARD_GEN_VECTOR(Board_Prio_ISR_78, 78, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 78
#endif                            
#if BOARD_NB_VECTORS >= 79        
  BOARD_GEN_VECTOR(Board_Prio_ISR_79, 79, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 79
#endif
#if BOARD_NB_VECTORS >= 80
  BOARD_GEN_VECTOR(Board_Prio_ISR_80, 80, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 80
#endif                            
#if BOARD_NB_VECTORS >= 81        
  BOARD_GEN_VECTOR(Board_Prio_ISR_81, 81, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 81
#endif                            
#if BOARD_NB_VECTORS >= 82        
  BOARD_GEN_VECTOR(Board_Prio_ISR_82, 82, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 82
#endif                            
#if BOARD_NB_VECTORS >= 83        
  BOARD_GEN_VECTOR(Board_Prio_ISR_83, 83, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 83
#endif                            
#if BOARD_NB_VECTORS >= 84        
  BOARD_GEN_VECTOR(Board_Prio_ISR_84, 84, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 84
#endif                            
#if BOARD_NB_VECTORS >= 85        
  BOARD_GEN_VECTOR(Board_Prio_ISR_85, 85, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 85
#endif                            
#if BOARD_NB_VECTORS >= 86        
  BOARD_GEN_VECTOR(Board_Prio_ISR_86, 86, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 86
#endif                            
#if BOARD_NB_VECTORS >= 87        
  BOARD_GEN_VECTOR(Board_Prio_ISR_87, 87, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 87
#endif                            
#if BOARD_NB_VECTORS >= 88        
  BOARD_GEN_VECTOR(Board_Prio_ISR_88, 88, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 88
#endif                            
#if BOARD_NB_VECTORS >= 89        
  BOARD_GEN_VECTOR(Board_Prio_ISR_89, 89, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 89
#endif
#if BOARD_NB_VECTORS >= 90
  BOARD_GEN_VECTOR(Board_Prio_ISR_90, 90, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 90
#endif                            
#if BOARD_NB_VECTORS >= 91        
  BOARD_GEN_VECTOR(Board_Prio_ISR_91, 91, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 91
#endif                            
#if BOARD_NB_VECTORS >= 92        
  BOARD_GEN_VECTOR(Board_Prio_ISR_92, 92, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 92
#endif                            
#if BOARD_NB_VECTORS >= 93        
  BOARD_GEN_VECTOR(Board_Prio_ISR_93, 93, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 93
#endif                            
#if BOARD_NB_VECTORS >= 94        
  BOARD_GEN_VECTOR(Board_Prio_ISR_94, 94, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 94
#endif                            
#if BOARD_NB_VECTORS >= 95        
  BOARD_GEN_VECTOR(Board_Prio_ISR_95, 95, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 95
#endif                            
#if BOARD_NB_VECTORS >= 96        
  BOARD_GEN_VECTOR(Board_Prio_ISR_96, 96, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 96
#endif                            
#if BOARD_NB_VECTORS >= 97        
  BOARD_GEN_VECTOR(Board_Prio_ISR_97, 97, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 97
#endif                            
#if BOARD_NB_VECTORS >= 98        
  BOARD_GEN_VECTOR(Board_Prio_ISR_98, 98, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 98
#endif                            
#if BOARD_NB_VECTORS >= 99        
  BOARD_GEN_VECTOR(Board_Prio_ISR_99, 99, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 99
#endif
#if BOARD_NB_VECTORS >= 100
  BOARD_GEN_VECTOR(Board_Prio_ISR_100, 100, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 100
#endif                            
#if BOARD_NB_VECTORS >= 101       
  BOARD_GEN_VECTOR(Board_Prio_ISR_101, 101, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 101
#endif                            
#if BOARD_NB_VECTORS >= 102       
  BOARD_GEN_VECTOR(Board_Prio_ISR_102, 102, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 102
#endif                            
#if BOARD_NB_VECTORS >= 103       
  BOARD_GEN_VECTOR(Board_Prio_ISR_103, 103, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 103
#endif                            
#if BOARD_NB_VECTORS >= 104       
  BOARD_GEN_VECTOR(Board_Prio_ISR_104, 104, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 104
#endif                            
#if BOARD_NB_VECTORS >= 105       
  BOARD_GEN_VECTOR(Board_Prio_ISR_105, 105, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 105
#endif                            
#if BOARD_NB_VECTORS >= 106       
  BOARD_GEN_VECTOR(Board_Prio_ISR_106, 106, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 106
#endif                            
#if BOARD_NB_VECTORS >= 107       
  BOARD_GEN_VECTOR(Board_Prio_ISR_107, 107, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 107
#endif                            
#if BOARD_NB_VECTORS >= 108       
  BOARD_GEN_VECTOR(Board_Prio_ISR_108, 108, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 108
#endif                            
#if BOARD_NB_VECTORS >= 109       
  BOARD_GEN_VECTOR(Board_Prio_ISR_109, 109, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 109
#endif                            
#if BOARD_NB_VECTORS >= 110       
  BOARD_GEN_VECTOR(Board_Prio_ISR_110, 110, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 110
#endif                            
#if BOARD_NB_VECTORS >= 111       
  BOARD_GEN_VECTOR(Board_Prio_ISR_111, 111, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 111
#endif                            
#if BOARD_NB_VECTORS >= 112       
  BOARD_GEN_VECTOR(Board_Prio_ISR_112, 112, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 112
#endif                            
#if BOARD_NB_VECTORS >= 113       
  BOARD_GEN_VECTOR(Board_Prio_ISR_113, 113, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 113
#endif                            
#if BOARD_NB_VECTORS >= 114       
  BOARD_GEN_VECTOR(Board_Prio_ISR_114, 114, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 114
#endif                            
#if BOARD_NB_VECTORS >= 115       
  BOARD_GEN_VECTOR(Board_Prio_ISR_115, 115, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 115
#endif                            
#if BOARD_NB_VECTORS >= 116       
  BOARD_GEN_VECTOR(Board_Prio_ISR_116, 116, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 116
#endif                            
#if BOARD_NB_VECTORS >= 117       
  BOARD_GEN_VECTOR(Board_Prio_ISR_117, 117, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 117
#endif                            
#if BOARD_NB_VECTORS >= 118       
  BOARD_GEN_VECTOR(Board_Prio_ISR_118, 118, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 118
#endif                            
#if BOARD_NB_VECTORS >= 119       
  BOARD_GEN_VECTOR(Board_Prio_ISR_119, 119, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 119
#endif                            
#if BOARD_NB_VECTORS >= 120       
  BOARD_GEN_VECTOR(Board_Prio_ISR_120, 120, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 120
#endif                            
#if BOARD_NB_VECTORS >= 121       
  BOARD_GEN_VECTOR(Board_Prio_ISR_121, 121, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 121
#endif                            
#if BOARD_NB_VECTORS >= 122       
  BOARD_GEN_VECTOR(Board_Prio_ISR_122, 122, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 122
#endif                            
#if BOARD_NB_VECTORS >= 123       
  BOARD_GEN_VECTOR(Board_Prio_ISR_123, 123, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 123
#endif                            
#if BOARD_NB_VECTORS >= 124       
  BOARD_GEN_VECTOR(Board_Prio_ISR_124, 124, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 124
#endif                            
#if BOARD_NB_VECTORS >= 125       
  BOARD_GEN_VECTOR(Board_Prio_ISR_125, 125, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 125
#endif                            
#if BOARD_NB_VECTORS >= 126       
  BOARD_GEN_VECTOR(Board_Prio_ISR_126, 126, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 126
#endif                            
#if BOARD_NB_VECTORS >= 127       
  BOARD_GEN_VECTOR(Board_Prio_ISR_127, 127, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 127
#endif                            
#if BOARD_NB_VECTORS >= 128       
  BOARD_GEN_VECTOR(Board_Prio_ISR_128, 128, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 128
#endif                            
#if BOARD_NB_VECTORS >= 129       
  BOARD_GEN_VECTOR(Board_Prio_ISR_129, 129, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 129
#endif                            
#if BOARD_NB_VECTORS >= 130       
  BOARD_GEN_VECTOR(Board_Prio_ISR_130, 130, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 130
#endif                            
#if BOARD_NB_VECTORS >= 131       
  BOARD_GEN_VECTOR(Board_Prio_ISR_131, 131, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 131
#endif                            
#if BOARD_NB_VECTORS >= 132       
  BOARD_GEN_VECTOR(Board_Prio_ISR_132, 132, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 132
#endif                            
#if BOARD_NB_VECTORS >= 133       
  BOARD_GEN_VECTOR(Board_Prio_ISR_133, 133, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 133
#endif                            
#if BOARD_NB_VECTORS >= 134       
  BOARD_GEN_VECTOR(Board_Prio_ISR_134, 134, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 134
#endif                            
#if BOARD_NB_VECTORS >= 135       
  BOARD_GEN_VECTOR(Board_Prio_ISR_135, 135, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 135
#endif                            
#if BOARD_NB_VECTORS >= 136       
  BOARD_GEN_VECTOR(Board_Prio_ISR_136, 136, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 136
#endif                            
#if BOARD_NB_VECTORS >= 137       
  BOARD_GEN_VECTOR(Board_Prio_ISR_137, 137, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 137
#endif                            
#if BOARD_NB_VECTORS >= 138       
  BOARD_GEN_VECTOR(Board_Prio_ISR_138, 138, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 138
#endif                            
#if BOARD_NB_VECTORS >= 139       
  BOARD_GEN_VECTOR(Board_Prio_ISR_139, 139, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 139
#endif                            
#if BOARD_NB_VECTORS >= 140       
  BOARD_GEN_VECTOR(Board_Prio_ISR_140, 140, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 140
#endif                            
#if BOARD_NB_VECTORS >= 141       
  BOARD_GEN_VECTOR(Board_Prio_ISR_141, 141, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 141
#endif                            
#if BOARD_NB_VECTORS >= 142       
  BOARD_GEN_VECTOR(Board_Prio_ISR_142, 142, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 142
#endif                            
#if BOARD_NB_VECTORS >= 143       
  BOARD_GEN_VECTOR(Board_Prio_ISR_143, 143, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 143
#endif                            
#if BOARD_NB_VECTORS >= 144       
  BOARD_GEN_VECTOR(Board_Prio_ISR_144, 144, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 144
#endif                            
#if BOARD_NB_VECTORS >= 145       
  BOARD_GEN_VECTOR(Board_Prio_ISR_145, 145, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 145
#endif                            
#if BOARD_NB_VECTORS >= 146       
  BOARD_GEN_VECTOR(Board_Prio_ISR_146, 146, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 146
#endif                            
#if BOARD_NB_VECTORS >= 147       
  BOARD_GEN_VECTOR(Board_Prio_ISR_147, 147, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 147
#endif                            
#if BOARD_NB_VECTORS >= 148       
  BOARD_GEN_VECTOR(Board_Prio_ISR_148, 148, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 148
#endif                            
#if BOARD_NB_VECTORS >= 149       
  BOARD_GEN_VECTOR(Board_Prio_ISR_149, 149, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 149
#endif                            
#if BOARD_NB_VECTORS >= 150       
  BOARD_GEN_VECTOR(Board_Prio_ISR_150, 150, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 150
#endif                            
#if BOARD_NB_VECTORS >= 151       
  BOARD_GEN_VECTOR(Board_Prio_ISR_151, 151, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 151
#endif                            
#if BOARD_NB_VECTORS >= 152       
  BOARD_GEN_VECTOR(Board_Prio_ISR_152, 152, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 152
#endif                            
#if BOARD_NB_VECTORS >= 153       
  BOARD_GEN_VECTOR(Board_Prio_ISR_153, 153, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 153
#endif                            
#if BOARD_NB_VECTORS >= 154       
  BOARD_GEN_VECTOR(Board_Prio_ISR_154, 154, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 154
#endif                            
#if BOARD_NB_VECTORS >= 155       
  BOARD_GEN_VECTOR(Board_Prio_ISR_155, 155, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 155
#endif                            
#if BOARD_NB_VECTORS >= 156       
  BOARD_GEN_VECTOR(Board_Prio_ISR_156, 156, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 156
#endif                            
#if BOARD_NB_VECTORS >= 157       
  BOARD_GEN_VECTOR(Board_Prio_ISR_157, 157, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 157
#endif                            
#if BOARD_NB_VECTORS >= 158       
  BOARD_GEN_VECTOR(Board_Prio_ISR_158, 158, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 158
#endif                            
#if BOARD_NB_VECTORS >= 159       
  BOARD_GEN_VECTOR(Board_Prio_ISR_159, 159, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 159
#endif                            
#if BOARD_NB_VECTORS >= 160       
  BOARD_GEN_VECTOR(Board_Prio_ISR_160, 160, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 160
#endif                            
#if BOARD_NB_VECTORS >= 161       
  BOARD_GEN_VECTOR(Board_Prio_ISR_161, 161, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 161
#endif                            
#if BOARD_NB_VECTORS >= 162       
  BOARD_GEN_VECTOR(Board_Prio_ISR_162, 162, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 162
#endif                            
#if BOARD_NB_VECTORS >= 163       
  BOARD_GEN_VECTOR(Board_Prio_ISR_163, 163, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 163
#endif                            
#if BOARD_NB_VECTORS >= 164       
  BOARD_GEN_VECTOR(Board_Prio_ISR_164, 164, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 164
#endif                            
#if BOARD_NB_VECTORS >= 165       
  BOARD_GEN_VECTOR(Board_Prio_ISR_165, 165, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 165
#endif                            
#if BOARD_NB_VECTORS >= 166       
  BOARD_GEN_VECTOR(Board_Prio_ISR_166, 166, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 166
#endif                            
#if BOARD_NB_VECTORS >= 167       
  BOARD_GEN_VECTOR(Board_Prio_ISR_167, 167, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 167
#endif                            
#if BOARD_NB_VECTORS >= 168       
  BOARD_GEN_VECTOR(Board_Prio_ISR_168, 168, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 168
#endif                            
#if BOARD_NB_VECTORS >= 169       
  BOARD_GEN_VECTOR(Board_Prio_ISR_169, 169, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 169
#endif                            
#if BOARD_NB_VECTORS >= 170       
  BOARD_GEN_VECTOR(Board_Prio_ISR_170, 170, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 170
#endif                            
#if BOARD_NB_VECTORS >= 171       
  BOARD_GEN_VECTOR(Board_Prio_ISR_171, 171, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 171
#endif                            
#if BOARD_NB_VECTORS >= 172       
  BOARD_GEN_VECTOR(Board_Prio_ISR_172, 172, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 172
#endif                            
#if BOARD_NB_VECTORS >= 173       
  BOARD_GEN_VECTOR(Board_Prio_ISR_173, 173, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 173
#endif                            
#if BOARD_NB_VECTORS >= 174       
  BOARD_GEN_VECTOR(Board_Prio_ISR_174, 174, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 174
#endif                            
#if BOARD_NB_VECTORS >= 175       
  BOARD_GEN_VECTOR(Board_Prio_ISR_175, 175, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 175
#endif                            
#if BOARD_NB_VECTORS >= 176       
  BOARD_GEN_VECTOR(Board_Prio_ISR_176, 176, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 176
#endif                            
#if BOARD_NB_VECTORS >= 177       
  BOARD_GEN_VECTOR(Board_Prio_ISR_177, 177, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 177
#endif                            
#if BOARD_NB_VECTORS >= 178       
  BOARD_GEN_VECTOR(Board_Prio_ISR_178, 178, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 178
#endif                            
#if BOARD_NB_VECTORS >= 179       
  BOARD_GEN_VECTOR(Board_Prio_ISR_179, 179, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 179
#endif                            
#if BOARD_NB_VECTORS >= 180       
  BOARD_GEN_VECTOR(Board_Prio_ISR_180, 180, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 180
#endif                            
#if BOARD_NB_VECTORS >= 181       
  BOARD_GEN_VECTOR(Board_Prio_ISR_181, 181, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 181
#endif                            
#if BOARD_NB_VECTORS >= 182       
  BOARD_GEN_VECTOR(Board_Prio_ISR_182, 182, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 182
#endif                            
#if BOARD_NB_VECTORS >= 183       
  BOARD_GEN_VECTOR(Board_Prio_ISR_183, 183, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 183
#endif                            
#if BOARD_NB_VECTORS >= 184       
  BOARD_GEN_VECTOR(Board_Prio_ISR_184, 184, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 184
#endif                            
#if BOARD_NB_VECTORS >= 185       
  BOARD_GEN_VECTOR(Board_Prio_ISR_185, 185, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 185
#endif                            
#if BOARD_NB_VECTORS >= 186       
  BOARD_GEN_VECTOR(Board_Prio_ISR_186, 186, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 186
#endif                            
#if BOARD_NB_VECTORS >= 187       
  BOARD_GEN_VECTOR(Board_Prio_ISR_187, 187, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 187
#endif                            
#if BOARD_NB_VECTORS >= 188       
  BOARD_GEN_VECTOR(Board_Prio_ISR_188, 188, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 188
#endif                            
#if BOARD_NB_VECTORS >= 189       
  BOARD_GEN_VECTOR(Board_Prio_ISR_189, 189, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 189
#endif                            
#if BOARD_NB_VECTORS >= 190       
  BOARD_GEN_VECTOR(Board_Prio_ISR_190, 190, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 190
#endif                            
#if BOARD_NB_VECTORS >= 191       
  BOARD_GEN_VECTOR(Board_Prio_ISR_191, 191, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 191
#endif                            
#if BOARD_NB_VECTORS >= 192       
  BOARD_GEN_VECTOR(Board_Prio_ISR_192, 192, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 192
#endif                            
#if BOARD_NB_VECTORS >= 193       
  BOARD_GEN_VECTOR(Board_Prio_ISR_193, 193, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 193
#endif                            
#if BOARD_NB_VECTORS >= 194       
  BOARD_GEN_VECTOR(Board_Prio_ISR_194, 194, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 194
#endif                            
#if BOARD_NB_VECTORS >= 195       
  BOARD_GEN_VECTOR(Board_Prio_ISR_195, 195, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 195
#endif                            
#if BOARD_NB_VECTORS >= 196       
  BOARD_GEN_VECTOR(Board_Prio_ISR_196, 196, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 196
#endif                            
#if BOARD_NB_VECTORS >= 197       
  BOARD_GEN_VECTOR(Board_Prio_ISR_197, 197, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 197
#endif                            
#if BOARD_NB_VECTORS >= 198       
  BOARD_GEN_VECTOR(Board_Prio_ISR_198, 198, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 198
#endif                            
#if BOARD_NB_VECTORS >= 199       
  BOARD_GEN_VECTOR(Board_Prio_ISR_199, 199, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 199
#endif
#if BOARD_NB_VECTORS >= 200
  BOARD_GEN_VECTOR(Board_Prio_ISR_200, 200, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 200
#endif                            
#if BOARD_NB_VECTORS >= 201       
  BOARD_GEN_VECTOR(Board_Prio_ISR_201, 201, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 201
#endif                            
#if BOARD_NB_VECTORS >= 202       
  BOARD_GEN_VECTOR(Board_Prio_ISR_202, 202, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 202
#endif                            
#if BOARD_NB_VECTORS >= 203       
  BOARD_GEN_VECTOR(Board_Prio_ISR_203, 203, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 203
#endif                            
#if BOARD_NB_VECTORS >= 204       
  BOARD_GEN_VECTOR(Board_Prio_ISR_204, 204, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 204
#endif                            
#if BOARD_NB_VECTORS >= 205       
  BOARD_GEN_VECTOR(Board_Prio_ISR_205, 205, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 205
#endif                            
#if BOARD_NB_VECTORS >= 206       
  BOARD_GEN_VECTOR(Board_Prio_ISR_206, 206, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 206
#endif                            
#if BOARD_NB_VECTORS >= 207       
  BOARD_GEN_VECTOR(Board_Prio_ISR_207, 207, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 207
#endif                            
#if BOARD_NB_VECTORS >= 208       
  BOARD_GEN_VECTOR(Board_Prio_ISR_208, 208, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 208
#endif                            
#if BOARD_NB_VECTORS >= 209       
  BOARD_GEN_VECTOR(Board_Prio_ISR_209, 209, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 209
#endif                            
#if BOARD_NB_VECTORS >= 210       
  BOARD_GEN_VECTOR(Board_Prio_ISR_210, 210, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 210
#endif                            
#if BOARD_NB_VECTORS >= 211       
  BOARD_GEN_VECTOR(Board_Prio_ISR_211, 211, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 211
#endif                            
#if BOARD_NB_VECTORS >= 212       
  BOARD_GEN_VECTOR(Board_Prio_ISR_212, 212, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 212
#endif                            
#if BOARD_NB_VECTORS >= 213       
  BOARD_GEN_VECTOR(Board_Prio_ISR_213, 213, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 213
#endif                            
#if BOARD_NB_VECTORS >= 214       
  BOARD_GEN_VECTOR(Board_Prio_ISR_214, 214, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 214
#endif                            
#if BOARD_NB_VECTORS >= 215       
  BOARD_GEN_VECTOR(Board_Prio_ISR_215, 215, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 215
#endif                            
#if BOARD_NB_VECTORS >= 216       
  BOARD_GEN_VECTOR(Board_Prio_ISR_216, 216, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 216
#endif                            
#if BOARD_NB_VECTORS >= 217       
  BOARD_GEN_VECTOR(Board_Prio_ISR_217, 217, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 217
#endif                            
#if BOARD_NB_VECTORS >= 218       
  BOARD_GEN_VECTOR(Board_Prio_ISR_218, 218, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 218
#endif                            
#if BOARD_NB_VECTORS >= 219       
  BOARD_GEN_VECTOR(Board_Prio_ISR_219, 219, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 219
#endif                            
#if BOARD_NB_VECTORS >= 220       
  BOARD_GEN_VECTOR(Board_Prio_ISR_220, 220, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 220
#endif                            
#if BOARD_NB_VECTORS >= 221       
  BOARD_GEN_VECTOR(Board_Prio_ISR_221, 221, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 221
#endif                            
#if BOARD_NB_VECTORS >= 222       
  BOARD_GEN_VECTOR(Board_Prio_ISR_222, 222, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 222
#endif                            
#if BOARD_NB_VECTORS >= 223       
  BOARD_GEN_VECTOR(Board_Prio_ISR_223, 223, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 223
#endif                            
#if BOARD_NB_VECTORS >= 224       
  BOARD_GEN_VECTOR(Board_Prio_ISR_224, 224, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 224
#endif                            
#if BOARD_NB_VECTORS >= 225       
  BOARD_GEN_VECTOR(Board_Prio_ISR_225, 225, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 225
#endif                            
#if BOARD_NB_VECTORS >= 226       
  BOARD_GEN_VECTOR(Board_Prio_ISR_226, 226, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 226
#endif                            
#if BOARD_NB_VECTORS >= 227       
  BOARD_GEN_VECTOR(Board_Prio_ISR_227, 227, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 227
#endif                            
#if BOARD_NB_VECTORS >= 228       
  BOARD_GEN_VECTOR(Board_Prio_ISR_228, 228, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 228
#endif                            
#if BOARD_NB_VECTORS >= 229       
  BOARD_GEN_VECTOR(Board_Prio_ISR_229, 229, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 229
#endif                            
#if BOARD_NB_VECTORS >= 230       
  BOARD_GEN_VECTOR(Board_Prio_ISR_230, 230, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 230
#endif                            
#if BOARD_NB_VECTORS >= 231       
  BOARD_GEN_VECTOR(Board_Prio_ISR_231, 231, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 231
#endif                            
#if BOARD_NB_VECTORS >= 232       
  BOARD_GEN_VECTOR(Board_Prio_ISR_232, 232, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 232
#endif                            
#if BOARD_NB_VECTORS >= 233       
  BOARD_GEN_VECTOR(Board_Prio_ISR_233, 233, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 233
#endif                            
#if BOARD_NB_VECTORS >= 234       
  BOARD_GEN_VECTOR(Board_Prio_ISR_234, 234, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 234
#endif                            
#if BOARD_NB_VECTORS >= 235       
  BOARD_GEN_VECTOR(Board_Prio_ISR_235, 235, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 235
#endif                            
#if BOARD_NB_VECTORS >= 236       
  BOARD_GEN_VECTOR(Board_Prio_ISR_236, 236, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 236
#endif                            
#if BOARD_NB_VECTORS >= 237       
  BOARD_GEN_VECTOR(Board_Prio_ISR_237, 237, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 237
#endif                            
#if BOARD_NB_VECTORS >= 238       
  BOARD_GEN_VECTOR(Board_Prio_ISR_238, 238, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 238
#endif                            
#if BOARD_NB_VECTORS >= 239       
  BOARD_GEN_VECTOR(Board_Prio_ISR_239, 239, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 239
#endif                            
#if BOARD_NB_VECTORS >= 240       
  BOARD_GEN_VECTOR(Board_Prio_ISR_240, 240, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 240
#endif                            
#if BOARD_NB_VECTORS >= 241       
  BOARD_GEN_VECTOR(Board_Prio_ISR_241, 241, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 241
#endif                            
#if BOARD_NB_VECTORS >= 242       
  BOARD_GEN_VECTOR(Board_Prio_ISR_242, 242, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 242
#endif                            
#if BOARD_NB_VECTORS >= 243       
  BOARD_GEN_VECTOR(Board_Prio_ISR_243, 243, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 243
#endif                            
#if BOARD_NB_VECTORS >= 244       
  BOARD_GEN_VECTOR(Board_Prio_ISR_244, 244, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 244
#endif                            
#if BOARD_NB_VECTORS >= 245       
  BOARD_GEN_VECTOR(Board_Prio_ISR_245, 245, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 245
#endif                            
#if BOARD_NB_VECTORS >= 246       
  BOARD_GEN_VECTOR(Board_Prio_ISR_246, 246, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 246
#endif                            
#if BOARD_NB_VECTORS >= 247       
  BOARD_GEN_VECTOR(Board_Prio_ISR_247, 247, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 247
#endif                            
#if BOARD_NB_VECTORS >= 248       
  BOARD_GEN_VECTOR(Board_Prio_ISR_248, 248, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 248
#endif                            
#if BOARD_NB_VECTORS >= 249       
  BOARD_GEN_VECTOR(Board_Prio_ISR_249, 249, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 249
#endif                            
#if BOARD_NB_VECTORS >= 250       
  BOARD_GEN_VECTOR(Board_Prio_ISR_250, 250, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 250
#endif                            
#if BOARD_NB_VECTORS >= 251       
  BOARD_GEN_VECTOR(Board_Prio_ISR_251, 251, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 251
#endif                            
#if BOARD_NB_VECTORS >= 252       
  BOARD_GEN_VECTOR(Board_Prio_ISR_252, 252, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 252
#endif                            
#if BOARD_NB_VECTORS >= 253       
  BOARD_GEN_VECTOR(Board_Prio_ISR_253, 253, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 253
#endif                            
#if BOARD_NB_VECTORS >= 254       
  BOARD_GEN_VECTOR(Board_Prio_ISR_254, 254, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 254
#endif                            
#if BOARD_NB_VECTORS >= 255       
  BOARD_GEN_VECTOR(Board_Prio_ISR_255, 255, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 255
#endif                            
#if BOARD_NB_VECTORS >= 256       
  BOARD_GEN_VECTOR(Board_Prio_ISR_256, 256, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 256
#endif                            
#if BOARD_NB_VECTORS >= 257       
  BOARD_GEN_VECTOR(Board_Prio_ISR_257, 257, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 257
#endif                            
#if BOARD_NB_VECTORS >= 258       
  BOARD_GEN_VECTOR(Board_Prio_ISR_258, 258, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 258
#endif                            
#if BOARD_NB_VECTORS >= 259       
  BOARD_GEN_VECTOR(Board_Prio_ISR_259, 259, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 259
#endif                            
#if BOARD_NB_VECTORS >= 260       
  BOARD_GEN_VECTOR(Board_Prio_ISR_260, 260, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 260
#endif                            
#if BOARD_NB_VECTORS >= 261       
  BOARD_GEN_VECTOR(Board_Prio_ISR_261, 261, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 261
#endif                            
#if BOARD_NB_VECTORS >= 262       
  BOARD_GEN_VECTOR(Board_Prio_ISR_262, 262, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 262
#endif                            
#if BOARD_NB_VECTORS >= 263       
  BOARD_GEN_VECTOR(Board_Prio_ISR_263, 263, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 263
#endif                            
#if BOARD_NB_VECTORS >= 264       
  BOARD_GEN_VECTOR(Board_Prio_ISR_264, 264, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 264
#endif                            
#if BOARD_NB_VECTORS >= 265       
  BOARD_GEN_VECTOR(Board_Prio_ISR_265, 265, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 265
#endif                            
#if BOARD_NB_VECTORS >= 266       
  BOARD_GEN_VECTOR(Board_Prio_ISR_266, 266, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 266
#endif                            
#if BOARD_NB_VECTORS >= 267       
  BOARD_GEN_VECTOR(Board_Prio_ISR_267, 267, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 267
#endif                            
#if BOARD_NB_VECTORS >= 268       
  BOARD_GEN_VECTOR(Board_Prio_ISR_268, 268, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 268
#endif                            
#if BOARD_NB_VECTORS >= 269       
  BOARD_GEN_VECTOR(Board_Prio_ISR_269, 269, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 269
#endif                            
#if BOARD_NB_VECTORS >= 270       
  BOARD_GEN_VECTOR(Board_Prio_ISR_270, 270, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 270
#endif                            
#if BOARD_NB_VECTORS >= 271       
  BOARD_GEN_VECTOR(Board_Prio_ISR_271, 271, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 271
#endif                            
#if BOARD_NB_VECTORS >= 272       
  BOARD_GEN_VECTOR(Board_Prio_ISR_272, 272, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 272
#endif                            
#if BOARD_NB_VECTORS >= 273       
  BOARD_GEN_VECTOR(Board_Prio_ISR_273, 273, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 273
#endif                            
#if BOARD_NB_VECTORS >= 274       
  BOARD_GEN_VECTOR(Board_Prio_ISR_274, 274, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 274
#endif                            
#if BOARD_NB_VECTORS >= 275       
  BOARD_GEN_VECTOR(Board_Prio_ISR_275, 275, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 275
#endif                            
#if BOARD_NB_VECTORS >= 276       
  BOARD_GEN_VECTOR(Board_Prio_ISR_276, 276, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 276
#endif                            
#if BOARD_NB_VECTORS >= 277       
  BOARD_GEN_VECTOR(Board_Prio_ISR_277, 277, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 277
#endif                            
#if BOARD_NB_VECTORS >= 278       
  BOARD_GEN_VECTOR(Board_Prio_ISR_278, 278, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 278
#endif                            
#if BOARD_NB_VECTORS >= 279       
  BOARD_GEN_VECTOR(Board_Prio_ISR_279, 279, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 279
#endif                            
#if BOARD_NB_VECTORS >= 280       
  BOARD_GEN_VECTOR(Board_Prio_ISR_280, 280, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 280
#endif                            
#if BOARD_NB_VECTORS >= 281       
  BOARD_GEN_VECTOR(Board_Prio_ISR_281, 281, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 281
#endif                            
#if BOARD_NB_VECTORS >= 282       
  BOARD_GEN_VECTOR(Board_Prio_ISR_282, 282, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 282
#endif                            
#if BOARD_NB_VECTORS >= 283       
  BOARD_GEN_VECTOR(Board_Prio_ISR_283, 283, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 283
#endif                            
#if BOARD_NB_VECTORS >= 284       
  BOARD_GEN_VECTOR(Board_Prio_ISR_284, 284, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 284
#endif                            
#if BOARD_NB_VECTORS >= 285       
  BOARD_GEN_VECTOR(Board_Prio_ISR_285, 285, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 285
#endif                            
#if BOARD_NB_VECTORS >= 286       
  BOARD_GEN_VECTOR(Board_Prio_ISR_286, 286, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 286
#endif                            
#if BOARD_NB_VECTORS >= 287       
  BOARD_GEN_VECTOR(Board_Prio_ISR_287, 287, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 287
#endif                            
#if BOARD_NB_VECTORS >= 288       
  BOARD_GEN_VECTOR(Board_Prio_ISR_288, 288, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 288
#endif                            
#if BOARD_NB_VECTORS >= 289       
  BOARD_GEN_VECTOR(Board_Prio_ISR_289, 289, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 289
#endif                            
#if BOARD_NB_VECTORS >= 290       
  BOARD_GEN_VECTOR(Board_Prio_ISR_290, 290, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 290
#endif                            
#if BOARD_NB_VECTORS >= 291       
  BOARD_GEN_VECTOR(Board_Prio_ISR_291, 291, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 291
#endif                            
#if BOARD_NB_VECTORS >= 292       
  BOARD_GEN_VECTOR(Board_Prio_ISR_292, 292, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 292
#endif                            
#if BOARD_NB_VECTORS >= 293       
  BOARD_GEN_VECTOR(Board_Prio_ISR_293, 293, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 293
#endif                            
#if BOARD_NB_VECTORS >= 294       
  BOARD_GEN_VECTOR(Board_Prio_ISR_294, 294, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 294
#endif                            
#if BOARD_NB_VECTORS >= 295       
  BOARD_GEN_VECTOR(Board_Prio_ISR_295, 295, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 295
#endif                            
#if BOARD_NB_VECTORS >= 296       
  BOARD_GEN_VECTOR(Board_Prio_ISR_296, 296, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 296
#endif                            
#if BOARD_NB_VECTORS >= 297       
  BOARD_GEN_VECTOR(Board_Prio_ISR_297, 297, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 297
#endif                            
#if BOARD_NB_VECTORS >= 298       
  BOARD_GEN_VECTOR(Board_Prio_ISR_298, 298, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 298
#endif                            
#if BOARD_NB_VECTORS >= 299       
  BOARD_GEN_VECTOR(Board_Prio_ISR_299, 299, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 299
#endif
#if BOARD_NB_VECTORS >= 300
  BOARD_GEN_VECTOR(Board_Prio_ISR_300, 300, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 300
#endif                            
#if BOARD_NB_VECTORS >= 301       
  BOARD_GEN_VECTOR(Board_Prio_ISR_301, 301, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 301
#endif                            
#if BOARD_NB_VECTORS >= 302       
  BOARD_GEN_VECTOR(Board_Prio_ISR_302, 302, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 302
#endif                            
#if BOARD_NB_VECTORS >= 303       
  BOARD_GEN_VECTOR(Board_Prio_ISR_303, 303, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 303
#endif                            
#if BOARD_NB_VECTORS >= 304       
  BOARD_GEN_VECTOR(Board_Prio_ISR_304, 304, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 304
#endif                            
#if BOARD_NB_VECTORS >= 305       
  BOARD_GEN_VECTOR(Board_Prio_ISR_305, 305, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 305
#endif                            
#if BOARD_NB_VECTORS >= 306       
  BOARD_GEN_VECTOR(Board_Prio_ISR_306, 306, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 306
#endif                            
#if BOARD_NB_VECTORS >= 307       
  BOARD_GEN_VECTOR(Board_Prio_ISR_307, 307, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 307
#endif                            
#if BOARD_NB_VECTORS >= 308       
  BOARD_GEN_VECTOR(Board_Prio_ISR_308, 308, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 308
#endif                            
#if BOARD_NB_VECTORS >= 309       
  BOARD_GEN_VECTOR(Board_Prio_ISR_309, 309, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 309
#endif                            
#if BOARD_NB_VECTORS >= 310       
  BOARD_GEN_VECTOR(Board_Prio_ISR_310, 310, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 310
#endif                            
#if BOARD_NB_VECTORS >= 311       
  BOARD_GEN_VECTOR(Board_Prio_ISR_311, 311, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 311
#endif                            
#if BOARD_NB_VECTORS >= 312       
  BOARD_GEN_VECTOR(Board_Prio_ISR_312, 312, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 312
#endif                            
#if BOARD_NB_VECTORS >= 313       
  BOARD_GEN_VECTOR(Board_Prio_ISR_313, 313, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 313
#endif                            
#if BOARD_NB_VECTORS >= 314       
  BOARD_GEN_VECTOR(Board_Prio_ISR_314, 314, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 314
#endif                            
#if BOARD_NB_VECTORS >= 315       
  BOARD_GEN_VECTOR(Board_Prio_ISR_315, 315, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 315
#endif                            
#if BOARD_NB_VECTORS >= 316       
  BOARD_GEN_VECTOR(Board_Prio_ISR_316, 316, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 316
#endif                            
#if BOARD_NB_VECTORS >= 317       
  BOARD_GEN_VECTOR(Board_Prio_ISR_317, 317, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 317
#endif                            
#if BOARD_NB_VECTORS >= 318       
  BOARD_GEN_VECTOR(Board_Prio_ISR_318, 318, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 318
#endif                            
#if BOARD_NB_VECTORS >= 319       
  BOARD_GEN_VECTOR(Board_Prio_ISR_319, 319, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 319
#endif                            
#if BOARD_NB_VECTORS >= 320       
  BOARD_GEN_VECTOR(Board_Prio_ISR_320, 320, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 320
#endif                            
#if BOARD_NB_VECTORS >= 321       
  BOARD_GEN_VECTOR(Board_Prio_ISR_321, 321, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 321
#endif                            
#if BOARD_NB_VECTORS >= 322       
  BOARD_GEN_VECTOR(Board_Prio_ISR_322, 322, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 322
#endif                            
#if BOARD_NB_VECTORS >= 323       
  BOARD_GEN_VECTOR(Board_Prio_ISR_323, 323, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 323
#endif                            
#if BOARD_NB_VECTORS >= 324       
  BOARD_GEN_VECTOR(Board_Prio_ISR_324, 324, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 324
#endif                            
#if BOARD_NB_VECTORS >= 325       
  BOARD_GEN_VECTOR(Board_Prio_ISR_325, 325, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 325
#endif                            
#if BOARD_NB_VECTORS >= 326       
  BOARD_GEN_VECTOR(Board_Prio_ISR_326, 326, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 326
#endif                            
#if BOARD_NB_VECTORS >= 327       
  BOARD_GEN_VECTOR(Board_Prio_ISR_327, 327, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 327
#endif                            
#if BOARD_NB_VECTORS >= 328       
  BOARD_GEN_VECTOR(Board_Prio_ISR_328, 328, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 328
#endif                            
#if BOARD_NB_VECTORS >= 329       
  BOARD_GEN_VECTOR(Board_Prio_ISR_329, 329, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 329
#endif                            
#if BOARD_NB_VECTORS >= 330       
  BOARD_GEN_VECTOR(Board_Prio_ISR_330, 330, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 330
#endif                            
#if BOARD_NB_VECTORS >= 331       
  BOARD_GEN_VECTOR(Board_Prio_ISR_331, 331, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 331
#endif                            
#if BOARD_NB_VECTORS >= 332       
  BOARD_GEN_VECTOR(Board_Prio_ISR_332, 332, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 332
#endif                            
#if BOARD_NB_VECTORS >= 333       
  BOARD_GEN_VECTOR(Board_Prio_ISR_333, 333, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 333
#endif                            
#if BOARD_NB_VECTORS >= 334       
  BOARD_GEN_VECTOR(Board_Prio_ISR_334, 334, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 334
#endif                            
#if BOARD_NB_VECTORS >= 335       
  BOARD_GEN_VECTOR(Board_Prio_ISR_335, 335, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 335
#endif                            
#if BOARD_NB_VECTORS >= 336       
  BOARD_GEN_VECTOR(Board_Prio_ISR_336, 336, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 336
#endif                            
#if BOARD_NB_VECTORS >= 337       
  BOARD_GEN_VECTOR(Board_Prio_ISR_337, 337, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 337
#endif                            
#if BOARD_NB_VECTORS >= 338       
  BOARD_GEN_VECTOR(Board_Prio_ISR_338, 338, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 338
#endif                            
#if BOARD_NB_VECTORS >= 339       
  BOARD_GEN_VECTOR(Board_Prio_ISR_339, 339, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 339
#endif                            
#if BOARD_NB_VECTORS >= 340       
  BOARD_GEN_VECTOR(Board_Prio_ISR_340, 340, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 340
#endif                            
#if BOARD_NB_VECTORS >= 341       
  BOARD_GEN_VECTOR(Board_Prio_ISR_341, 341, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 341
#endif                            
#if BOARD_NB_VECTORS >= 342       
  BOARD_GEN_VECTOR(Board_Prio_ISR_342, 342, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 342
#endif                            
#if BOARD_NB_VECTORS >= 343       
  BOARD_GEN_VECTOR(Board_Prio_ISR_343, 343, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 343
#endif                            
#if BOARD_NB_VECTORS >= 344       
  BOARD_GEN_VECTOR(Board_Prio_ISR_344, 344, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 344
#endif                            
#if BOARD_NB_VECTORS >= 345       
  BOARD_GEN_VECTOR(Board_Prio_ISR_345, 345, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 345
#endif                            
#if BOARD_NB_VECTORS >= 346       
  BOARD_GEN_VECTOR(Board_Prio_ISR_346, 346, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 346
#endif                            
#if BOARD_NB_VECTORS >= 347       
  BOARD_GEN_VECTOR(Board_Prio_ISR_347, 347, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 347
#endif                            
#if BOARD_NB_VECTORS >= 348       
  BOARD_GEN_VECTOR(Board_Prio_ISR_348, 348, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 348
#endif                            
#if BOARD_NB_VECTORS >= 349       
  BOARD_GEN_VECTOR(Board_Prio_ISR_349, 349, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 349
#endif                            
#if BOARD_NB_VECTORS >= 350       
  BOARD_GEN_VECTOR(Board_Prio_ISR_350, 350, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 350
#endif                            
#if BOARD_NB_VECTORS >= 351       
  BOARD_GEN_VECTOR(Board_Prio_ISR_351, 351, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 351
#endif                            
#if BOARD_NB_VECTORS >= 352       
  BOARD_GEN_VECTOR(Board_Prio_ISR_352, 352, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 352
#endif                            
#if BOARD_NB_VECTORS >= 353       
  BOARD_GEN_VECTOR(Board_Prio_ISR_353, 353, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 353
#endif                            
#if BOARD_NB_VECTORS >= 354       
  BOARD_GEN_VECTOR(Board_Prio_ISR_354, 354, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 354
#endif                            
#if BOARD_NB_VECTORS >= 355       
  BOARD_GEN_VECTOR(Board_Prio_ISR_355, 355, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 355
#endif                            
#if BOARD_NB_VECTORS >= 356       
  BOARD_GEN_VECTOR(Board_Prio_ISR_356, 356, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 356
#endif                            
#if BOARD_NB_VECTORS >= 357       
  BOARD_GEN_VECTOR(Board_Prio_ISR_357, 357, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 357
#endif                            
#if BOARD_NB_VECTORS >= 358       
  BOARD_GEN_VECTOR(Board_Prio_ISR_358, 358, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 358
#endif                            
#if BOARD_NB_VECTORS >= 359       
  BOARD_GEN_VECTOR(Board_Prio_ISR_359, 359, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 359
#endif                            
#if BOARD_NB_VECTORS >= 360       
  BOARD_GEN_VECTOR(Board_Prio_ISR_360, 360, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 360
#endif                            
#if BOARD_NB_VECTORS >= 361       
  BOARD_GEN_VECTOR(Board_Prio_ISR_361, 361, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 361
#endif                            
#if BOARD_NB_VECTORS >= 362       
  BOARD_GEN_VECTOR(Board_Prio_ISR_362, 362, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 362
#endif                            
#if BOARD_NB_VECTORS >= 363       
  BOARD_GEN_VECTOR(Board_Prio_ISR_363, 363, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 363
#endif                            
#if BOARD_NB_VECTORS >= 364       
  BOARD_GEN_VECTOR(Board_Prio_ISR_364, 364, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 364
#endif                            
#if BOARD_NB_VECTORS >= 365       
  BOARD_GEN_VECTOR(Board_Prio_ISR_365, 365, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 365
#endif                            
#if BOARD_NB_VECTORS >= 366       
  BOARD_GEN_VECTOR(Board_Prio_ISR_366, 366, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 366
#endif                            
#if BOARD_NB_VECTORS >= 367       
  BOARD_GEN_VECTOR(Board_Prio_ISR_367, 367, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 367
#endif                            
#if BOARD_NB_VECTORS >= 368       
  BOARD_GEN_VECTOR(Board_Prio_ISR_368, 368, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 368
#endif                            
#if BOARD_NB_VECTORS >= 369       
  BOARD_GEN_VECTOR(Board_Prio_ISR_369, 369, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 369
#endif                            
#if BOARD_NB_VECTORS >= 370       
  BOARD_GEN_VECTOR(Board_Prio_ISR_370, 370, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 370
#endif                            
#if BOARD_NB_VECTORS >= 371       
  BOARD_GEN_VECTOR(Board_Prio_ISR_371, 371, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 371
#endif                            
#if BOARD_NB_VECTORS >= 372       
  BOARD_GEN_VECTOR(Board_Prio_ISR_372, 372, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 372
#endif                            
#if BOARD_NB_VECTORS >= 373       
  BOARD_GEN_VECTOR(Board_Prio_ISR_373, 373, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 373
#endif                            
#if BOARD_NB_VECTORS >= 374       
  BOARD_GEN_VECTOR(Board_Prio_ISR_374, 374, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 374
#endif                            
#if BOARD_NB_VECTORS >= 375       
  BOARD_GEN_VECTOR(Board_Prio_ISR_375, 375, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 375
#endif                            
#if BOARD_NB_VECTORS >= 376       
  BOARD_GEN_VECTOR(Board_Prio_ISR_376, 376, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 376
#endif                            
#if BOARD_NB_VECTORS >= 377       
  BOARD_GEN_VECTOR(Board_Prio_ISR_377, 377, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 377
#endif                            
#if BOARD_NB_VECTORS >= 378       
  BOARD_GEN_VECTOR(Board_Prio_ISR_378, 378, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 378
#endif                            
#if BOARD_NB_VECTORS >= 379       
  BOARD_GEN_VECTOR(Board_Prio_ISR_379, 379, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 379
#endif                            
#if BOARD_NB_VECTORS >= 380       
  BOARD_GEN_VECTOR(Board_Prio_ISR_380, 380, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 380
#endif                            
#if BOARD_NB_VECTORS >= 381       
  BOARD_GEN_VECTOR(Board_Prio_ISR_381, 381, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 381
#endif                            
#if BOARD_NB_VECTORS >= 382       
  BOARD_GEN_VECTOR(Board_Prio_ISR_382, 382, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 382
#endif                            
#if BOARD_NB_VECTORS >= 383       
  BOARD_GEN_VECTOR(Board_Prio_ISR_383, 383, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 383
#endif                            
#if BOARD_NB_VECTORS >= 384       
  BOARD_GEN_VECTOR(Board_Prio_ISR_384, 384, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 384
#endif                            
#if BOARD_NB_VECTORS >= 385       
  BOARD_GEN_VECTOR(Board_Prio_ISR_385, 385, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 385
#endif                            
#if BOARD_NB_VECTORS >= 386       
  BOARD_GEN_VECTOR(Board_Prio_ISR_386, 386, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 386
#endif                            
#if BOARD_NB_VECTORS >= 387       
  BOARD_GEN_VECTOR(Board_Prio_ISR_387, 387, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 387
#endif                            
#if BOARD_NB_VECTORS >= 388       
  BOARD_GEN_VECTOR(Board_Prio_ISR_388, 388, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 388
#endif                            
#if BOARD_NB_VECTORS >= 389       
  BOARD_GEN_VECTOR(Board_Prio_ISR_389, 389, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 389
#endif                            
#if BOARD_NB_VECTORS >= 390       
  BOARD_GEN_VECTOR(Board_Prio_ISR_390, 390, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 390
#endif                            
#if BOARD_NB_VECTORS >= 391       
  BOARD_GEN_VECTOR(Board_Prio_ISR_391, 391, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 391
#endif                            
#if BOARD_NB_VECTORS >= 392       
  BOARD_GEN_VECTOR(Board_Prio_ISR_392, 392, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 392
#endif                            
#if BOARD_NB_VECTORS >= 393       
  BOARD_GEN_VECTOR(Board_Prio_ISR_393, 393, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 393
#endif                            
#if BOARD_NB_VECTORS >= 394       
  BOARD_GEN_VECTOR(Board_Prio_ISR_394, 394, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 394
#endif                            
#if BOARD_NB_VECTORS >= 395       
  BOARD_GEN_VECTOR(Board_Prio_ISR_395, 395, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 395
#endif                            
#if BOARD_NB_VECTORS >= 396       
  BOARD_GEN_VECTOR(Board_Prio_ISR_396, 396, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 396
#endif                            
#if BOARD_NB_VECTORS >= 397       
  BOARD_GEN_VECTOR(Board_Prio_ISR_397, 397, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 397
#endif                            
#if BOARD_NB_VECTORS >= 398       
  BOARD_GEN_VECTOR(Board_Prio_ISR_398, 398, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 398
#endif                            
#if BOARD_NB_VECTORS >= 399       
  BOARD_GEN_VECTOR(Board_Prio_ISR_399, 399, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 399
#endif
#if BOARD_NB_VECTORS >= 400
  BOARD_GEN_VECTOR(Board_Prio_ISR_400, 400, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 400
#endif                            
#if BOARD_NB_VECTORS >= 401       
  BOARD_GEN_VECTOR(Board_Prio_ISR_401, 401, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 401
#endif                            
#if BOARD_NB_VECTORS >= 402       
  BOARD_GEN_VECTOR(Board_Prio_ISR_402, 402, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 402
#endif                            
#if BOARD_NB_VECTORS >= 403       
  BOARD_GEN_VECTOR(Board_Prio_ISR_403, 403, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 403
#endif                            
#if BOARD_NB_VECTORS >= 404       
  BOARD_GEN_VECTOR(Board_Prio_ISR_404, 404, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 404
#endif                            
#if BOARD_NB_VECTORS >= 405       
  BOARD_GEN_VECTOR(Board_Prio_ISR_405, 405, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 405
#endif                            
#if BOARD_NB_VECTORS >= 406       
  BOARD_GEN_VECTOR(Board_Prio_ISR_406, 406, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 406
#endif                            
#if BOARD_NB_VECTORS >= 407       
  BOARD_GEN_VECTOR(Board_Prio_ISR_407, 407, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 407
#endif                            
#if BOARD_NB_VECTORS >= 408       
  BOARD_GEN_VECTOR(Board_Prio_ISR_408, 408, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 408
#endif                            
#if BOARD_NB_VECTORS >= 409       
  BOARD_GEN_VECTOR(Board_Prio_ISR_409, 409, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 409
#endif                            
#if BOARD_NB_VECTORS >= 410       
  BOARD_GEN_VECTOR(Board_Prio_ISR_410, 410, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 410
#endif                            
#if BOARD_NB_VECTORS >= 411       
  BOARD_GEN_VECTOR(Board_Prio_ISR_411, 411, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 411
#endif                            
#if BOARD_NB_VECTORS >= 412       
  BOARD_GEN_VECTOR(Board_Prio_ISR_412, 412, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 412
#endif                            
#if BOARD_NB_VECTORS >= 413       
  BOARD_GEN_VECTOR(Board_Prio_ISR_413, 413, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 413
#endif                            
#if BOARD_NB_VECTORS >= 414       
  BOARD_GEN_VECTOR(Board_Prio_ISR_414, 414, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 414
#endif                            
#if BOARD_NB_VECTORS >= 415       
  BOARD_GEN_VECTOR(Board_Prio_ISR_415, 415, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 415
#endif                            
#if BOARD_NB_VECTORS >= 416       
  BOARD_GEN_VECTOR(Board_Prio_ISR_416, 416, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 416
#endif                            
#if BOARD_NB_VECTORS >= 417       
  BOARD_GEN_VECTOR(Board_Prio_ISR_417, 417, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 417
#endif                            
#if BOARD_NB_VECTORS >= 418       
  BOARD_GEN_VECTOR(Board_Prio_ISR_418, 418, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 418
#endif                            
#if BOARD_NB_VECTORS >= 419       
  BOARD_GEN_VECTOR(Board_Prio_ISR_419, 419, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 419
#endif                            
#if BOARD_NB_VECTORS >= 420       
  BOARD_GEN_VECTOR(Board_Prio_ISR_420, 420, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 420
#endif                            
#if BOARD_NB_VECTORS >= 421       
  BOARD_GEN_VECTOR(Board_Prio_ISR_421, 421, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 421
#endif                            
#if BOARD_NB_VECTORS >= 422       
  BOARD_GEN_VECTOR(Board_Prio_ISR_422, 422, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 422
#endif                            
#if BOARD_NB_VECTORS >= 423       
  BOARD_GEN_VECTOR(Board_Prio_ISR_423, 423, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 423
#endif                            
#if BOARD_NB_VECTORS >= 424       
  BOARD_GEN_VECTOR(Board_Prio_ISR_424, 424, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 424
#endif                            
#if BOARD_NB_VECTORS >= 425       
  BOARD_GEN_VECTOR(Board_Prio_ISR_425, 425, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 425
#endif                            
#if BOARD_NB_VECTORS >= 426       
  BOARD_GEN_VECTOR(Board_Prio_ISR_426, 426, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 426
#endif                            
#if BOARD_NB_VECTORS >= 427       
  BOARD_GEN_VECTOR(Board_Prio_ISR_427, 427, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 427
#endif                            
#if BOARD_NB_VECTORS >= 428       
  BOARD_GEN_VECTOR(Board_Prio_ISR_428, 428, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 428
#endif                            
#if BOARD_NB_VECTORS >= 429       
  BOARD_GEN_VECTOR(Board_Prio_ISR_429, 429, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 429
#endif                            
#if BOARD_NB_VECTORS >= 430       
  BOARD_GEN_VECTOR(Board_Prio_ISR_430, 430, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 430
#endif                            
#if BOARD_NB_VECTORS >= 431       
  BOARD_GEN_VECTOR(Board_Prio_ISR_431, 431, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 431
#endif                            
#if BOARD_NB_VECTORS >= 432       
  BOARD_GEN_VECTOR(Board_Prio_ISR_432, 432, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 432
#endif                            
#if BOARD_NB_VECTORS >= 433       
  BOARD_GEN_VECTOR(Board_Prio_ISR_433, 433, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 433
#endif                            
#if BOARD_NB_VECTORS >= 434       
  BOARD_GEN_VECTOR(Board_Prio_ISR_434, 434, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 434
#endif                            
#if BOARD_NB_VECTORS >= 435       
  BOARD_GEN_VECTOR(Board_Prio_ISR_435, 435, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 435
#endif                            
#if BOARD_NB_VECTORS >= 436       
  BOARD_GEN_VECTOR(Board_Prio_ISR_436, 436, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 436
#endif                            
#if BOARD_NB_VECTORS >= 437       
  BOARD_GEN_VECTOR(Board_Prio_ISR_437, 437, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 437
#endif                            
#if BOARD_NB_VECTORS >= 438       
  BOARD_GEN_VECTOR(Board_Prio_ISR_438, 438, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 438
#endif                            
#if BOARD_NB_VECTORS >= 439       
  BOARD_GEN_VECTOR(Board_Prio_ISR_439, 439, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 439
#endif                            
#if BOARD_NB_VECTORS >= 440       
  BOARD_GEN_VECTOR(Board_Prio_ISR_440, 440, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 440
#endif                            
#if BOARD_NB_VECTORS >= 441       
  BOARD_GEN_VECTOR(Board_Prio_ISR_441, 441, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 441
#endif                            
#if BOARD_NB_VECTORS >= 442       
  BOARD_GEN_VECTOR(Board_Prio_ISR_442, 442, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 442
#endif                            
#if BOARD_NB_VECTORS >= 443       
  BOARD_GEN_VECTOR(Board_Prio_ISR_443, 443, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 443
#endif                            
#if BOARD_NB_VECTORS >= 444       
  BOARD_GEN_VECTOR(Board_Prio_ISR_444, 444, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 444
#endif                            
#if BOARD_NB_VECTORS >= 445       
  BOARD_GEN_VECTOR(Board_Prio_ISR_445, 445, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 445
#endif                            
#if BOARD_NB_VECTORS >= 446       
  BOARD_GEN_VECTOR(Board_Prio_ISR_446, 446, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 446
#endif                            
#if BOARD_NB_VECTORS >= 447       
  BOARD_GEN_VECTOR(Board_Prio_ISR_447, 447, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 447
#endif                            
#if BOARD_NB_VECTORS >= 448       
  BOARD_GEN_VECTOR(Board_Prio_ISR_448, 448, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 448
#endif                            
#if BOARD_NB_VECTORS >= 449       
  BOARD_GEN_VECTOR(Board_Prio_ISR_449, 449, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 449
#endif                            
#if BOARD_NB_VECTORS >= 450       
  BOARD_GEN_VECTOR(Board_Prio_ISR_450, 450, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 450
#endif                            
#if BOARD_NB_VECTORS >= 451       
  BOARD_GEN_VECTOR(Board_Prio_ISR_451, 451, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 451
#endif                            
#if BOARD_NB_VECTORS >= 452       
  BOARD_GEN_VECTOR(Board_Prio_ISR_452, 452, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 452
#endif                            
#if BOARD_NB_VECTORS >= 453       
  BOARD_GEN_VECTOR(Board_Prio_ISR_453, 453, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 453
#endif                            
#if BOARD_NB_VECTORS >= 454       
  BOARD_GEN_VECTOR(Board_Prio_ISR_454, 454, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 454
#endif                            
#if BOARD_NB_VECTORS >= 455       
  BOARD_GEN_VECTOR(Board_Prio_ISR_455, 455, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 455
#endif                            
#if BOARD_NB_VECTORS >= 456       
  BOARD_GEN_VECTOR(Board_Prio_ISR_456, 456, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 456
#endif                            
#if BOARD_NB_VECTORS >= 457       
  BOARD_GEN_VECTOR(Board_Prio_ISR_457, 457, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 457
#endif                            
#if BOARD_NB_VECTORS >= 458       
  BOARD_GEN_VECTOR(Board_Prio_ISR_458, 458, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 458
#endif                            
#if BOARD_NB_VECTORS >= 459       
  BOARD_GEN_VECTOR(Board_Prio_ISR_459, 459, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 459
#endif                            
#if BOARD_NB_VECTORS >= 460       
  BOARD_GEN_VECTOR(Board_Prio_ISR_460, 460, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 460
#endif                            
#if BOARD_NB_VECTORS >= 461       
  BOARD_GEN_VECTOR(Board_Prio_ISR_461, 461, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 461
#endif                            
#if BOARD_NB_VECTORS >= 462       
  BOARD_GEN_VECTOR(Board_Prio_ISR_462, 462, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 462
#endif                            
#if BOARD_NB_VECTORS >= 463       
  BOARD_GEN_VECTOR(Board_Prio_ISR_463, 463, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 463
#endif                            
#if BOARD_NB_VECTORS >= 464       
  BOARD_GEN_VECTOR(Board_Prio_ISR_464, 464, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 464
#endif                            
#if BOARD_NB_VECTORS >= 465       
  BOARD_GEN_VECTOR(Board_Prio_ISR_465, 465, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 465
#endif                            
#if BOARD_NB_VECTORS >= 466       
  BOARD_GEN_VECTOR(Board_Prio_ISR_466, 466, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 466
#endif                            
#if BOARD_NB_VECTORS >= 467       
  BOARD_GEN_VECTOR(Board_Prio_ISR_467, 467, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 467
#endif                            
#if BOARD_NB_VECTORS >= 468       
  BOARD_GEN_VECTOR(Board_Prio_ISR_468, 468, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 468
#endif                            
#if BOARD_NB_VECTORS >= 469       
  BOARD_GEN_VECTOR(Board_Prio_ISR_469, 469, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 469
#endif                            
#if BOARD_NB_VECTORS >= 470       
  BOARD_GEN_VECTOR(Board_Prio_ISR_470, 470, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 470
#endif                            
#if BOARD_NB_VECTORS >= 471       
  BOARD_GEN_VECTOR(Board_Prio_ISR_471, 471, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 471
#endif                            
#if BOARD_NB_VECTORS >= 472       
  BOARD_GEN_VECTOR(Board_Prio_ISR_472, 472, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 472
#endif                            
#if BOARD_NB_VECTORS >= 473       
  BOARD_GEN_VECTOR(Board_Prio_ISR_473, 473, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 473
#endif                            
#if BOARD_NB_VECTORS >= 474       
  BOARD_GEN_VECTOR(Board_Prio_ISR_474, 474, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 474
#endif                            
#if BOARD_NB_VECTORS >= 475       
  BOARD_GEN_VECTOR(Board_Prio_ISR_475, 475, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 475
#endif                            
#if BOARD_NB_VECTORS >= 476       
  BOARD_GEN_VECTOR(Board_Prio_ISR_476, 476, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 476
#endif                            
#if BOARD_NB_VECTORS >= 477       
  BOARD_GEN_VECTOR(Board_Prio_ISR_477, 477, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 477
#endif                            
#if BOARD_NB_VECTORS >= 478       
  BOARD_GEN_VECTOR(Board_Prio_ISR_478, 478, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 478
#endif                            
#if BOARD_NB_VECTORS >= 479       
  BOARD_GEN_VECTOR(Board_Prio_ISR_479, 479, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 479
#endif                            
#if BOARD_NB_VECTORS >= 480       
  BOARD_GEN_VECTOR(Board_Prio_ISR_480, 480, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 480
#endif                            
#if BOARD_NB_VECTORS >= 481       
  BOARD_GEN_VECTOR(Board_Prio_ISR_481, 481, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 481
#endif                            
#if BOARD_NB_VECTORS >= 482       
  BOARD_GEN_VECTOR(Board_Prio_ISR_482, 482, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 482
#endif                            
#if BOARD_NB_VECTORS >= 483       
  BOARD_GEN_VECTOR(Board_Prio_ISR_483, 483, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 483
#endif                            
#if BOARD_NB_VECTORS >= 484       
  BOARD_GEN_VECTOR(Board_Prio_ISR_484, 484, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 484
#endif                            
#if BOARD_NB_VECTORS >= 485       
  BOARD_GEN_VECTOR(Board_Prio_ISR_485, 485, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 485
#endif                            
#if BOARD_NB_VECTORS >= 486       
  BOARD_GEN_VECTOR(Board_Prio_ISR_486, 486, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 486
#endif                            
#if BOARD_NB_VECTORS >= 487       
  BOARD_GEN_VECTOR(Board_Prio_ISR_487, 487, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 487
#endif                            
#if BOARD_NB_VECTORS >= 488       
  BOARD_GEN_VECTOR(Board_Prio_ISR_488, 488, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 488
#endif                            
#if BOARD_NB_VECTORS >= 489       
  BOARD_GEN_VECTOR(Board_Prio_ISR_489, 489, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 489
#endif                            
#if BOARD_NB_VECTORS >= 490       
  BOARD_GEN_VECTOR(Board_Prio_ISR_490, 490, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 490
#endif                            
#if BOARD_NB_VECTORS >= 491       
  BOARD_GEN_VECTOR(Board_Prio_ISR_491, 491, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 491
#endif                            
#if BOARD_NB_VECTORS >= 492       
  BOARD_GEN_VECTOR(Board_Prio_ISR_492, 492, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 492
#endif                            
#if BOARD_NB_VECTORS >= 493       
  BOARD_GEN_VECTOR(Board_Prio_ISR_493, 493, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 493
#endif                            
#if BOARD_NB_VECTORS >= 494       
  BOARD_GEN_VECTOR(Board_Prio_ISR_494, 494, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 494
#endif                            
#if BOARD_NB_VECTORS >= 495       
  BOARD_GEN_VECTOR(Board_Prio_ISR_495, 495, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 495
#endif                            
#if BOARD_NB_VECTORS >= 496       
  BOARD_GEN_VECTOR(Board_Prio_ISR_496, 496, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 496
#endif                            
#if BOARD_NB_VECTORS >= 497       
  BOARD_GEN_VECTOR(Board_Prio_ISR_497, 497, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 497
#endif                            
#if BOARD_NB_VECTORS >= 498       
  BOARD_GEN_VECTOR(Board_Prio_ISR_498, 498, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 498
#endif                            
#if BOARD_NB_VECTORS >= 499       
  BOARD_GEN_VECTOR(Board_Prio_ISR_499, 499, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 499
#endif
#if BOARD_NB_VECTORS >= 500
  BOARD_GEN_VECTOR(Board_Prio_ISR_500, 500, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 500
#endif                            
#if BOARD_NB_VECTORS >= 501       
  BOARD_GEN_VECTOR(Board_Prio_ISR_501, 501, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 501
#endif                            
#if BOARD_NB_VECTORS >= 502       
  BOARD_GEN_VECTOR(Board_Prio_ISR_502, 502, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 502
#endif                            
#if BOARD_NB_VECTORS >= 503       
  BOARD_GEN_VECTOR(Board_Prio_ISR_503, 503, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 503
#endif                            
#if BOARD_NB_VECTORS >= 504       
  BOARD_GEN_VECTOR(Board_Prio_ISR_504, 504, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 504
#endif                            
#if BOARD_NB_VECTORS >= 505       
  BOARD_GEN_VECTOR(Board_Prio_ISR_505, 505, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 505
#endif                            
#if BOARD_NB_VECTORS >= 506       
  BOARD_GEN_VECTOR(Board_Prio_ISR_506, 506, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 506
#endif                            
#if BOARD_NB_VECTORS >= 507       
  BOARD_GEN_VECTOR(Board_Prio_ISR_507, 507, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 507
#endif                            
#if BOARD_NB_VECTORS >= 508       
  BOARD_GEN_VECTOR(Board_Prio_ISR_508, 508, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 508
#endif                            
#if BOARD_NB_VECTORS >= 509       
  BOARD_GEN_VECTOR(Board_Prio_ISR_509, 509, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 509
#endif                            
#if BOARD_NB_VECTORS >= 510       
  BOARD_GEN_VECTOR(Board_Prio_ISR_510, 510, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 510
#endif                            
#if BOARD_NB_VECTORS >= 511       
  BOARD_GEN_VECTOR(Board_Prio_ISR_511, 511, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 511
#endif                            
#if BOARD_NB_VECTORS >= 512       
  BOARD_GEN_VECTOR(Board_Prio_ISR_512, 512, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 512
#endif                            
#if BOARD_NB_VECTORS >= 513       
  BOARD_GEN_VECTOR(Board_Prio_ISR_513, 513, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 513
#endif                            
#if BOARD_NB_VECTORS >= 514       
  BOARD_GEN_VECTOR(Board_Prio_ISR_514, 514, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 514
#endif                            
#if BOARD_NB_VECTORS >= 515       
  BOARD_GEN_VECTOR(Board_Prio_ISR_515, 515, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 515
#endif                            
#if BOARD_NB_VECTORS >= 516       
  BOARD_GEN_VECTOR(Board_Prio_ISR_516, 516, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 516
#endif                            
#if BOARD_NB_VECTORS >= 517       
  BOARD_GEN_VECTOR(Board_Prio_ISR_517, 517, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 517
#endif                            
#if BOARD_NB_VECTORS >= 518       
  BOARD_GEN_VECTOR(Board_Prio_ISR_518, 518, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 518
#endif                            
#if BOARD_NB_VECTORS >= 519       
  BOARD_GEN_VECTOR(Board_Prio_ISR_519, 519, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 519
#endif                            
#if BOARD_NB_VECTORS >= 520       
  BOARD_GEN_VECTOR(Board_Prio_ISR_520, 520, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 520
#endif                            
#if BOARD_NB_VECTORS >= 521       
  BOARD_GEN_VECTOR(Board_Prio_ISR_521, 521, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 521
#endif                            
#if BOARD_NB_VECTORS >= 522       
  BOARD_GEN_VECTOR(Board_Prio_ISR_522, 522, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 522
#endif                            
#if BOARD_NB_VECTORS >= 523       
  BOARD_GEN_VECTOR(Board_Prio_ISR_523, 523, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 523
#endif                            
#if BOARD_NB_VECTORS >= 524       
  BOARD_GEN_VECTOR(Board_Prio_ISR_524, 524, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 524
#endif                            
#if BOARD_NB_VECTORS >= 525       
  BOARD_GEN_VECTOR(Board_Prio_ISR_525, 525, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 525
#endif                            
#if BOARD_NB_VECTORS >= 526       
  BOARD_GEN_VECTOR(Board_Prio_ISR_526, 526, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 526
#endif                            
#if BOARD_NB_VECTORS >= 527       
  BOARD_GEN_VECTOR(Board_Prio_ISR_527, 527, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 527
#endif                            
#if BOARD_NB_VECTORS >= 528       
  BOARD_GEN_VECTOR(Board_Prio_ISR_528, 528, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 528
#endif                            
#if BOARD_NB_VECTORS >= 529       
  BOARD_GEN_VECTOR(Board_Prio_ISR_529, 529, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 529
#endif                            
#if BOARD_NB_VECTORS >= 530       
  BOARD_GEN_VECTOR(Board_Prio_ISR_530, 530, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 530
#endif                            
#if BOARD_NB_VECTORS >= 531       
  BOARD_GEN_VECTOR(Board_Prio_ISR_531, 531, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 531
#endif                            
#if BOARD_NB_VECTORS >= 532       
  BOARD_GEN_VECTOR(Board_Prio_ISR_532, 532, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 532
#endif                            
#if BOARD_NB_VECTORS >= 533       
  BOARD_GEN_VECTOR(Board_Prio_ISR_533, 533, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 533
#endif                            
#if BOARD_NB_VECTORS >= 534       
  BOARD_GEN_VECTOR(Board_Prio_ISR_534, 534, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 534
#endif                            
#if BOARD_NB_VECTORS >= 535       
  BOARD_GEN_VECTOR(Board_Prio_ISR_535, 535, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 535
#endif                            
#if BOARD_NB_VECTORS >= 536       
  BOARD_GEN_VECTOR(Board_Prio_ISR_536, 536, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 536
#endif                            
#if BOARD_NB_VECTORS >= 537       
  BOARD_GEN_VECTOR(Board_Prio_ISR_537, 537, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 537
#endif                            
#if BOARD_NB_VECTORS >= 538       
  BOARD_GEN_VECTOR(Board_Prio_ISR_538, 538, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 538
#endif                            
#if BOARD_NB_VECTORS >= 539       
  BOARD_GEN_VECTOR(Board_Prio_ISR_539, 539, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 539
#endif                            
#if BOARD_NB_VECTORS >= 540       
  BOARD_GEN_VECTOR(Board_Prio_ISR_540, 540, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 540
#endif                            
#if BOARD_NB_VECTORS >= 541       
  BOARD_GEN_VECTOR(Board_Prio_ISR_541, 541, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 541
#endif                            
#if BOARD_NB_VECTORS >= 542       
  BOARD_GEN_VECTOR(Board_Prio_ISR_542, 542, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 542
#endif                            
#if BOARD_NB_VECTORS >= 543       
  BOARD_GEN_VECTOR(Board_Prio_ISR_543, 543, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 543
#endif                            
#if BOARD_NB_VECTORS >= 544       
  BOARD_GEN_VECTOR(Board_Prio_ISR_544, 544, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 544
#endif                            
#if BOARD_NB_VECTORS >= 545       
  BOARD_GEN_VECTOR(Board_Prio_ISR_545, 545, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 545
#endif                            
#if BOARD_NB_VECTORS >= 546       
  BOARD_GEN_VECTOR(Board_Prio_ISR_546, 546, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 546
#endif                            
#if BOARD_NB_VECTORS >= 547       
  BOARD_GEN_VECTOR(Board_Prio_ISR_547, 547, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 547
#endif                            
#if BOARD_NB_VECTORS >= 548       
  BOARD_GEN_VECTOR(Board_Prio_ISR_548, 548, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 548
#endif                            
#if BOARD_NB_VECTORS >= 549       
  BOARD_GEN_VECTOR(Board_Prio_ISR_549, 549, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 549
#endif                            
#if BOARD_NB_VECTORS >= 550       
  BOARD_GEN_VECTOR(Board_Prio_ISR_550, 550, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 550
#endif                            
#if BOARD_NB_VECTORS >= 551       
  BOARD_GEN_VECTOR(Board_Prio_ISR_551, 551, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 551
#endif                            
#if BOARD_NB_VECTORS >= 552       
  BOARD_GEN_VECTOR(Board_Prio_ISR_552, 552, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 552
#endif                            
#if BOARD_NB_VECTORS >= 553       
  BOARD_GEN_VECTOR(Board_Prio_ISR_553, 553, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 553
#endif                            
#if BOARD_NB_VECTORS >= 554       
  BOARD_GEN_VECTOR(Board_Prio_ISR_554, 554, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 554
#endif                            
#if BOARD_NB_VECTORS >= 555       
  BOARD_GEN_VECTOR(Board_Prio_ISR_555, 555, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 555
#endif                            
#if BOARD_NB_VECTORS >= 556       
  BOARD_GEN_VECTOR(Board_Prio_ISR_556, 556, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 556
#endif                            
#if BOARD_NB_VECTORS >= 557       
  BOARD_GEN_VECTOR(Board_Prio_ISR_557, 557, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 557
#endif                            
#if BOARD_NB_VECTORS >= 558       
  BOARD_GEN_VECTOR(Board_Prio_ISR_558, 558, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 558
#endif                            
#if BOARD_NB_VECTORS >= 559       
  BOARD_GEN_VECTOR(Board_Prio_ISR_559, 559, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 559
#endif                            
#if BOARD_NB_VECTORS >= 560       
  BOARD_GEN_VECTOR(Board_Prio_ISR_560, 560, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 560
#endif                            
#if BOARD_NB_VECTORS >= 561       
  BOARD_GEN_VECTOR(Board_Prio_ISR_561, 561, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 561
#endif                            
#if BOARD_NB_VECTORS >= 562       
  BOARD_GEN_VECTOR(Board_Prio_ISR_562, 562, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 562
#endif                            
#if BOARD_NB_VECTORS >= 563       
  BOARD_GEN_VECTOR(Board_Prio_ISR_563, 563, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 563
#endif                            
#if BOARD_NB_VECTORS >= 564       
  BOARD_GEN_VECTOR(Board_Prio_ISR_564, 564, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 564
#endif                            
#if BOARD_NB_VECTORS >= 565       
  BOARD_GEN_VECTOR(Board_Prio_ISR_565, 565, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 565
#endif                            
#if BOARD_NB_VECTORS >= 566       
  BOARD_GEN_VECTOR(Board_Prio_ISR_566, 566, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 566
#endif                            
#if BOARD_NB_VECTORS >= 567       
  BOARD_GEN_VECTOR(Board_Prio_ISR_567, 567, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 567
#endif                            
#if BOARD_NB_VECTORS >= 568       
  BOARD_GEN_VECTOR(Board_Prio_ISR_568, 568, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 568
#endif                            
#if BOARD_NB_VECTORS >= 569       
  BOARD_GEN_VECTOR(Board_Prio_ISR_569, 569, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 569
#endif                            
#if BOARD_NB_VECTORS >= 570       
  BOARD_GEN_VECTOR(Board_Prio_ISR_570, 570, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 570
#endif                            
#if BOARD_NB_VECTORS >= 571       
  BOARD_GEN_VECTOR(Board_Prio_ISR_571, 571, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 571
#endif                            
#if BOARD_NB_VECTORS >= 572       
  BOARD_GEN_VECTOR(Board_Prio_ISR_572, 572, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 572
#endif                            
#if BOARD_NB_VECTORS >= 573       
  BOARD_GEN_VECTOR(Board_Prio_ISR_573, 573, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 573
#endif                            
#if BOARD_NB_VECTORS >= 574       
  BOARD_GEN_VECTOR(Board_Prio_ISR_574, 574, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 574
#endif                            
#if BOARD_NB_VECTORS >= 575       
  BOARD_GEN_VECTOR(Board_Prio_ISR_575, 575, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 575
#endif                            
#if BOARD_NB_VECTORS >= 576       
  BOARD_GEN_VECTOR(Board_Prio_ISR_576, 576, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 576
#endif                            
#if BOARD_NB_VECTORS >= 577       
  BOARD_GEN_VECTOR(Board_Prio_ISR_577, 577, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 577
#endif                            
#if BOARD_NB_VECTORS >= 578       
  BOARD_GEN_VECTOR(Board_Prio_ISR_578, 578, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 578
#endif                            
#if BOARD_NB_VECTORS >= 579       
  BOARD_GEN_VECTOR(Board_Prio_ISR_579, 579, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 579
#endif                            
#if BOARD_NB_VECTORS >= 580       
  BOARD_GEN_VECTOR(Board_Prio_ISR_580, 580, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 580
#endif                            
#if BOARD_NB_VECTORS >= 581       
  BOARD_GEN_VECTOR(Board_Prio_ISR_581, 581, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 581
#endif                            
#if BOARD_NB_VECTORS >= 582       
  BOARD_GEN_VECTOR(Board_Prio_ISR_582, 582, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 582
#endif                            
#if BOARD_NB_VECTORS >= 583       
  BOARD_GEN_VECTOR(Board_Prio_ISR_583, 583, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 583
#endif                            
#if BOARD_NB_VECTORS >= 584       
  BOARD_GEN_VECTOR(Board_Prio_ISR_584, 584, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 584
#endif                            
#if BOARD_NB_VECTORS >= 585       
  BOARD_GEN_VECTOR(Board_Prio_ISR_585, 585, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 585
#endif                            
#if BOARD_NB_VECTORS >= 586       
  BOARD_GEN_VECTOR(Board_Prio_ISR_586, 586, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 586
#endif                            
#if BOARD_NB_VECTORS >= 587       
  BOARD_GEN_VECTOR(Board_Prio_ISR_587, 587, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 587
#endif                            
#if BOARD_NB_VECTORS >= 588       
  BOARD_GEN_VECTOR(Board_Prio_ISR_588, 588, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 588
#endif                            
#if BOARD_NB_VECTORS >= 589       
  BOARD_GEN_VECTOR(Board_Prio_ISR_589, 589, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 589
#endif                            
#if BOARD_NB_VECTORS >= 590       
  BOARD_GEN_VECTOR(Board_Prio_ISR_590, 590, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 590
#endif                            
#if BOARD_NB_VECTORS >= 591       
  BOARD_GEN_VECTOR(Board_Prio_ISR_591, 591, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 591
#endif                            
#if BOARD_NB_VECTORS >= 592       
  BOARD_GEN_VECTOR(Board_Prio_ISR_592, 592, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 592
#endif                            
#if BOARD_NB_VECTORS >= 593       
  BOARD_GEN_VECTOR(Board_Prio_ISR_593, 593, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 593
#endif                            
#if BOARD_NB_VECTORS >= 594       
  BOARD_GEN_VECTOR(Board_Prio_ISR_594, 594, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 594
#endif                            
#if BOARD_NB_VECTORS >= 595       
  BOARD_GEN_VECTOR(Board_Prio_ISR_595, 595, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 595
#endif                            
#if BOARD_NB_VECTORS >= 596       
  BOARD_GEN_VECTOR(Board_Prio_ISR_596, 596, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 596
#endif                            
#if BOARD_NB_VECTORS >= 597       
  BOARD_GEN_VECTOR(Board_Prio_ISR_597, 597, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 597
#endif                            
#if BOARD_NB_VECTORS >= 598       
  BOARD_GEN_VECTOR(Board_Prio_ISR_598, 598, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 598
#endif                            
#if BOARD_NB_VECTORS >= 599       
  BOARD_GEN_VECTOR(Board_Prio_ISR_599, 599, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 599
#endif
#if BOARD_NB_VECTORS >= 600
  BOARD_GEN_VECTOR(Board_Prio_ISR_600, 600, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 600
#endif                            
#if BOARD_NB_VECTORS >= 601       
  BOARD_GEN_VECTOR(Board_Prio_ISR_601, 601, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 601
#endif                            
#if BOARD_NB_VECTORS >= 602       
  BOARD_GEN_VECTOR(Board_Prio_ISR_602, 602, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 602
#endif                            
#if BOARD_NB_VECTORS >= 603       
  BOARD_GEN_VECTOR(Board_Prio_ISR_603, 603, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 603
#endif                            
#if BOARD_NB_VECTORS >= 604       
  BOARD_GEN_VECTOR(Board_Prio_ISR_604, 604, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 604
#endif                            
#if BOARD_NB_VECTORS >= 605       
  BOARD_GEN_VECTOR(Board_Prio_ISR_605, 605, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 605
#endif                            
#if BOARD_NB_VECTORS >= 606       
  BOARD_GEN_VECTOR(Board_Prio_ISR_606, 606, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 606
#endif                            
#if BOARD_NB_VECTORS >= 607       
  BOARD_GEN_VECTOR(Board_Prio_ISR_607, 607, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 607
#endif                            
#if BOARD_NB_VECTORS >= 608       
  BOARD_GEN_VECTOR(Board_Prio_ISR_608, 608, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 608
#endif                            
#if BOARD_NB_VECTORS >= 609       
  BOARD_GEN_VECTOR(Board_Prio_ISR_609, 609, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 609
#endif                            
#if BOARD_NB_VECTORS >= 610       
  BOARD_GEN_VECTOR(Board_Prio_ISR_610, 610, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 610
#endif                            
#if BOARD_NB_VECTORS >= 611       
  BOARD_GEN_VECTOR(Board_Prio_ISR_611, 611, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 611
#endif                            
#if BOARD_NB_VECTORS >= 612       
  BOARD_GEN_VECTOR(Board_Prio_ISR_612, 612, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 612
#endif                            
#if BOARD_NB_VECTORS >= 613       
  BOARD_GEN_VECTOR(Board_Prio_ISR_613, 613, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 613
#endif                            
#if BOARD_NB_VECTORS >= 614       
  BOARD_GEN_VECTOR(Board_Prio_ISR_614, 614, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 614
#endif                            
#if BOARD_NB_VECTORS >= 615       
  BOARD_GEN_VECTOR(Board_Prio_ISR_615, 615, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 615
#endif                            
#if BOARD_NB_VECTORS >= 616       
  BOARD_GEN_VECTOR(Board_Prio_ISR_616, 616, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 616
#endif                            
#if BOARD_NB_VECTORS >= 617       
  BOARD_GEN_VECTOR(Board_Prio_ISR_617, 617, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 617
#endif                            
#if BOARD_NB_VECTORS >= 618       
  BOARD_GEN_VECTOR(Board_Prio_ISR_618, 618, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 618
#endif                            
#if BOARD_NB_VECTORS >= 619       
  BOARD_GEN_VECTOR(Board_Prio_ISR_619, 619, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 619
#endif                            
#if BOARD_NB_VECTORS >= 620       
  BOARD_GEN_VECTOR(Board_Prio_ISR_620, 620, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 620
#endif                            
#if BOARD_NB_VECTORS >= 621       
  BOARD_GEN_VECTOR(Board_Prio_ISR_621, 621, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 621
#endif                            
#if BOARD_NB_VECTORS >= 622       
  BOARD_GEN_VECTOR(Board_Prio_ISR_622, 622, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 622
#endif                            
#if BOARD_NB_VECTORS >= 623       
  BOARD_GEN_VECTOR(Board_Prio_ISR_623, 623, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 623
#endif                            
#if BOARD_NB_VECTORS >= 624       
  BOARD_GEN_VECTOR(Board_Prio_ISR_624, 624, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 624
#endif                            
#if BOARD_NB_VECTORS >= 625       
  BOARD_GEN_VECTOR(Board_Prio_ISR_625, 625, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 625
#endif                            
#if BOARD_NB_VECTORS >= 626       
  BOARD_GEN_VECTOR(Board_Prio_ISR_626, 626, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 626
#endif                            
#if BOARD_NB_VECTORS >= 627       
  BOARD_GEN_VECTOR(Board_Prio_ISR_627, 627, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 627
#endif                            
#if BOARD_NB_VECTORS >= 628       
  BOARD_GEN_VECTOR(Board_Prio_ISR_628, 628, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 628
#endif                            
#if BOARD_NB_VECTORS >= 629       
  BOARD_GEN_VECTOR(Board_Prio_ISR_629, 629, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 629
#endif                            
#if BOARD_NB_VECTORS >= 630       
  BOARD_GEN_VECTOR(Board_Prio_ISR_630, 630, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 630
#endif                            
#if BOARD_NB_VECTORS >= 631       
  BOARD_GEN_VECTOR(Board_Prio_ISR_631, 631, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 631
#endif                            
#if BOARD_NB_VECTORS >= 632       
  BOARD_GEN_VECTOR(Board_Prio_ISR_632, 632, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 632
#endif                            
#if BOARD_NB_VECTORS >= 633       
  BOARD_GEN_VECTOR(Board_Prio_ISR_633, 633, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 633
#endif                            
#if BOARD_NB_VECTORS >= 634       
  BOARD_GEN_VECTOR(Board_Prio_ISR_634, 634, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 634
#endif                            
#if BOARD_NB_VECTORS >= 635       
  BOARD_GEN_VECTOR(Board_Prio_ISR_635, 635, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 635
#endif                            
#if BOARD_NB_VECTORS >= 636       
  BOARD_GEN_VECTOR(Board_Prio_ISR_636, 636, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 636
#endif                            
#if BOARD_NB_VECTORS >= 637       
  BOARD_GEN_VECTOR(Board_Prio_ISR_637, 637, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 637
#endif                            
#if BOARD_NB_VECTORS >= 638       
  BOARD_GEN_VECTOR(Board_Prio_ISR_638, 638, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 638
#endif                            
#if BOARD_NB_VECTORS >= 639       
  BOARD_GEN_VECTOR(Board_Prio_ISR_639, 639, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 639
#endif                            
#if BOARD_NB_VECTORS >= 640       
  BOARD_GEN_VECTOR(Board_Prio_ISR_640, 640, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 640
#endif                            
#if BOARD_NB_VECTORS >= 641       
  BOARD_GEN_VECTOR(Board_Prio_ISR_641, 641, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 641
#endif                            
#if BOARD_NB_VECTORS >= 642       
  BOARD_GEN_VECTOR(Board_Prio_ISR_642, 642, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 642
#endif                            
#if BOARD_NB_VECTORS >= 643       
  BOARD_GEN_VECTOR(Board_Prio_ISR_643, 643, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 643
#endif                            
#if BOARD_NB_VECTORS >= 644       
  BOARD_GEN_VECTOR(Board_Prio_ISR_644, 644, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 644
#endif                            
#if BOARD_NB_VECTORS >= 645       
  BOARD_GEN_VECTOR(Board_Prio_ISR_645, 645, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 645
#endif                            
#if BOARD_NB_VECTORS >= 646       
  BOARD_GEN_VECTOR(Board_Prio_ISR_646, 646, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 646
#endif                            
#if BOARD_NB_VECTORS >= 647       
  BOARD_GEN_VECTOR(Board_Prio_ISR_647, 647, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 647
#endif                            
#if BOARD_NB_VECTORS >= 648       
  BOARD_GEN_VECTOR(Board_Prio_ISR_648, 648, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 648
#endif                            
#if BOARD_NB_VECTORS >= 649       
  BOARD_GEN_VECTOR(Board_Prio_ISR_649, 649, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 649
#endif                            
#if BOARD_NB_VECTORS >= 650       
  BOARD_GEN_VECTOR(Board_Prio_ISR_650, 650, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 650
#endif                            
#if BOARD_NB_VECTORS >= 651       
  BOARD_GEN_VECTOR(Board_Prio_ISR_651, 651, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 651
#endif                            
#if BOARD_NB_VECTORS >= 652       
  BOARD_GEN_VECTOR(Board_Prio_ISR_652, 652, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 652
#endif                            
#if BOARD_NB_VECTORS >= 653       
  BOARD_GEN_VECTOR(Board_Prio_ISR_653, 653, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 653
#endif                            
#if BOARD_NB_VECTORS >= 654       
  BOARD_GEN_VECTOR(Board_Prio_ISR_654, 654, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 654
#endif                            
#if BOARD_NB_VECTORS >= 655       
  BOARD_GEN_VECTOR(Board_Prio_ISR_655, 655, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 655
#endif                            
#if BOARD_NB_VECTORS >= 656       
  BOARD_GEN_VECTOR(Board_Prio_ISR_656, 656, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 656
#endif                            
#if BOARD_NB_VECTORS >= 657       
  BOARD_GEN_VECTOR(Board_Prio_ISR_657, 657, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 657
#endif                            
#if BOARD_NB_VECTORS >= 658       
  BOARD_GEN_VECTOR(Board_Prio_ISR_658, 658, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 658
#endif                            
#if BOARD_NB_VECTORS >= 659       
  BOARD_GEN_VECTOR(Board_Prio_ISR_659, 659, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 659
#endif                            
#if BOARD_NB_VECTORS >= 660       
  BOARD_GEN_VECTOR(Board_Prio_ISR_660, 660, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 660
#endif                            
#if BOARD_NB_VECTORS >= 661       
  BOARD_GEN_VECTOR(Board_Prio_ISR_661, 661, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 661
#endif                            
#if BOARD_NB_VECTORS >= 662       
  BOARD_GEN_VECTOR(Board_Prio_ISR_662, 662, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 662
#endif                            
#if BOARD_NB_VECTORS >= 663       
  BOARD_GEN_VECTOR(Board_Prio_ISR_663, 663, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 663
#endif                            
#if BOARD_NB_VECTORS >= 664       
  BOARD_GEN_VECTOR(Board_Prio_ISR_664, 664, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 664
#endif                            
#if BOARD_NB_VECTORS >= 665       
  BOARD_GEN_VECTOR(Board_Prio_ISR_665, 665, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 665
#endif                            
#if BOARD_NB_VECTORS >= 666       
  BOARD_GEN_VECTOR(Board_Prio_ISR_666, 666, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 666
#endif                            
#if BOARD_NB_VECTORS >= 667       
  BOARD_GEN_VECTOR(Board_Prio_ISR_667, 667, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 667
#endif                            
#if BOARD_NB_VECTORS >= 668       
  BOARD_GEN_VECTOR(Board_Prio_ISR_668, 668, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 668
#endif                            
#if BOARD_NB_VECTORS >= 669       
  BOARD_GEN_VECTOR(Board_Prio_ISR_669, 669, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 669
#endif                            
#if BOARD_NB_VECTORS >= 670       
  BOARD_GEN_VECTOR(Board_Prio_ISR_670, 670, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 670
#endif                            
#if BOARD_NB_VECTORS >= 671       
  BOARD_GEN_VECTOR(Board_Prio_ISR_671, 671, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 671
#endif                            
#if BOARD_NB_VECTORS >= 672       
  BOARD_GEN_VECTOR(Board_Prio_ISR_672, 672, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 672
#endif                            
#if BOARD_NB_VECTORS >= 673       
  BOARD_GEN_VECTOR(Board_Prio_ISR_673, 673, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 673
#endif                            
#if BOARD_NB_VECTORS >= 674       
  BOARD_GEN_VECTOR(Board_Prio_ISR_674, 674, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 674
#endif                            
#if BOARD_NB_VECTORS >= 675       
  BOARD_GEN_VECTOR(Board_Prio_ISR_675, 675, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 675
#endif                            
#if BOARD_NB_VECTORS >= 676       
  BOARD_GEN_VECTOR(Board_Prio_ISR_676, 676, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 676
#endif                            
#if BOARD_NB_VECTORS >= 677       
  BOARD_GEN_VECTOR(Board_Prio_ISR_677, 677, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 677
#endif                            
#if BOARD_NB_VECTORS >= 678       
  BOARD_GEN_VECTOR(Board_Prio_ISR_678, 678, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 678
#endif                            
#if BOARD_NB_VECTORS >= 679       
  BOARD_GEN_VECTOR(Board_Prio_ISR_679, 679, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 679
#endif                            
#if BOARD_NB_VECTORS >= 680       
  BOARD_GEN_VECTOR(Board_Prio_ISR_680, 680, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 680
#endif                            
#if BOARD_NB_VECTORS >= 681       
  BOARD_GEN_VECTOR(Board_Prio_ISR_681, 681, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 681
#endif                            
#if BOARD_NB_VECTORS >= 682       
  BOARD_GEN_VECTOR(Board_Prio_ISR_682, 682, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 682
#endif                            
#if BOARD_NB_VECTORS >= 683       
  BOARD_GEN_VECTOR(Board_Prio_ISR_683, 683, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 683
#endif                            
#if BOARD_NB_VECTORS >= 684       
  BOARD_GEN_VECTOR(Board_Prio_ISR_684, 684, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 684
#endif                            
#if BOARD_NB_VECTORS >= 685       
  BOARD_GEN_VECTOR(Board_Prio_ISR_685, 685, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 685
#endif                            
#if BOARD_NB_VECTORS >= 686       
  BOARD_GEN_VECTOR(Board_Prio_ISR_686, 686, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 686
#endif                            
#if BOARD_NB_VECTORS >= 687       
  BOARD_GEN_VECTOR(Board_Prio_ISR_687, 687, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 687
#endif                            
#if BOARD_NB_VECTORS >= 688       
  BOARD_GEN_VECTOR(Board_Prio_ISR_688, 688, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 688
#endif                            
#if BOARD_NB_VECTORS >= 689       
  BOARD_GEN_VECTOR(Board_Prio_ISR_689, 689, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 689
#endif                            
#if BOARD_NB_VECTORS >= 690       
  BOARD_GEN_VECTOR(Board_Prio_ISR_690, 690, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 690
#endif                            
#if BOARD_NB_VECTORS >= 691       
  BOARD_GEN_VECTOR(Board_Prio_ISR_691, 691, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 691
#endif                            
#if BOARD_NB_VECTORS >= 692       
  BOARD_GEN_VECTOR(Board_Prio_ISR_692, 692, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 692
#endif                            
#if BOARD_NB_VECTORS >= 693       
  BOARD_GEN_VECTOR(Board_Prio_ISR_693, 693, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 693
#endif                            
#if BOARD_NB_VECTORS >= 694       
  BOARD_GEN_VECTOR(Board_Prio_ISR_694, 694, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 694
#endif                            
#if BOARD_NB_VECTORS >= 695       
  BOARD_GEN_VECTOR(Board_Prio_ISR_695, 695, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 695
#endif                            
#if BOARD_NB_VECTORS >= 696       
  BOARD_GEN_VECTOR(Board_Prio_ISR_696, 696, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 696
#endif                            
#if BOARD_NB_VECTORS >= 697       
  BOARD_GEN_VECTOR(Board_Prio_ISR_697, 697, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 697
#endif                            
#if BOARD_NB_VECTORS >= 698       
  BOARD_GEN_VECTOR(Board_Prio_ISR_698, 698, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 698
#endif                            
#if BOARD_NB_VECTORS >= 699       
  BOARD_GEN_VECTOR(Board_Prio_ISR_699, 699, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 699
#endif
#if BOARD_NB_VECTORS >= 700
  BOARD_GEN_VECTOR(Board_Prio_ISR_700, 700, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 700
#endif                            
#if BOARD_NB_VECTORS >= 701       
  BOARD_GEN_VECTOR(Board_Prio_ISR_701, 701, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 701
#endif                            
#if BOARD_NB_VECTORS >= 702       
  BOARD_GEN_VECTOR(Board_Prio_ISR_702, 702, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 702
#endif                            
#if BOARD_NB_VECTORS >= 703       
  BOARD_GEN_VECTOR(Board_Prio_ISR_703, 703, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 703
#endif                            
#if BOARD_NB_VECTORS >= 704       
  BOARD_GEN_VECTOR(Board_Prio_ISR_704, 704, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 704
#endif                            
#if BOARD_NB_VECTORS >= 705       
  BOARD_GEN_VECTOR(Board_Prio_ISR_705, 705, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 705
#endif                            
#if BOARD_NB_VECTORS >= 706       
  BOARD_GEN_VECTOR(Board_Prio_ISR_706, 706, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 706
#endif                            
#if BOARD_NB_VECTORS >= 707       
  BOARD_GEN_VECTOR(Board_Prio_ISR_707, 707, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 707
#endif                            
#if BOARD_NB_VECTORS >= 708       
  BOARD_GEN_VECTOR(Board_Prio_ISR_708, 708, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 708
#endif                            
#if BOARD_NB_VECTORS >= 709       
  BOARD_GEN_VECTOR(Board_Prio_ISR_709, 709, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 709
#endif                            
#if BOARD_NB_VECTORS >= 710       
  BOARD_GEN_VECTOR(Board_Prio_ISR_710, 710, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 710
#endif                            
#if BOARD_NB_VECTORS >= 711       
  BOARD_GEN_VECTOR(Board_Prio_ISR_711, 711, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 711
#endif                            
#if BOARD_NB_VECTORS >= 712       
  BOARD_GEN_VECTOR(Board_Prio_ISR_712, 712, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 712
#endif                            
#if BOARD_NB_VECTORS >= 713       
  BOARD_GEN_VECTOR(Board_Prio_ISR_713, 713, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 713
#endif                            
#if BOARD_NB_VECTORS >= 714       
  BOARD_GEN_VECTOR(Board_Prio_ISR_714, 714, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 714
#endif                            
#if BOARD_NB_VECTORS >= 715       
  BOARD_GEN_VECTOR(Board_Prio_ISR_715, 715, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 715
#endif                            
#if BOARD_NB_VECTORS >= 716       
  BOARD_GEN_VECTOR(Board_Prio_ISR_716, 716, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 716
#endif                            
#if BOARD_NB_VECTORS >= 717       
  BOARD_GEN_VECTOR(Board_Prio_ISR_717, 717, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 717
#endif                            
#if BOARD_NB_VECTORS >= 718       
  BOARD_GEN_VECTOR(Board_Prio_ISR_718, 718, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 718
#endif                            
#if BOARD_NB_VECTORS >= 719       
  BOARD_GEN_VECTOR(Board_Prio_ISR_719, 719, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 719
#endif                            
#if BOARD_NB_VECTORS >= 720       
  BOARD_GEN_VECTOR(Board_Prio_ISR_720, 720, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 720
#endif                            
#if BOARD_NB_VECTORS >= 721       
  BOARD_GEN_VECTOR(Board_Prio_ISR_721, 721, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 721
#endif                            
#if BOARD_NB_VECTORS >= 722       
  BOARD_GEN_VECTOR(Board_Prio_ISR_722, 722, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 722
#endif                            
#if BOARD_NB_VECTORS >= 723       
  BOARD_GEN_VECTOR(Board_Prio_ISR_723, 723, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 723
#endif                            
#if BOARD_NB_VECTORS >= 724       
  BOARD_GEN_VECTOR(Board_Prio_ISR_724, 724, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 724
#endif                            
#if BOARD_NB_VECTORS >= 725       
  BOARD_GEN_VECTOR(Board_Prio_ISR_725, 725, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 725
#endif                            
#if BOARD_NB_VECTORS >= 726       
  BOARD_GEN_VECTOR(Board_Prio_ISR_726, 726, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 726
#endif                            
#if BOARD_NB_VECTORS >= 727       
  BOARD_GEN_VECTOR(Board_Prio_ISR_727, 727, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 727
#endif                            
#if BOARD_NB_VECTORS >= 728       
  BOARD_GEN_VECTOR(Board_Prio_ISR_728, 728, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 728
#endif                            
#if BOARD_NB_VECTORS >= 729       
  BOARD_GEN_VECTOR(Board_Prio_ISR_729, 729, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 729
#endif                            
#if BOARD_NB_VECTORS >= 730       
  BOARD_GEN_VECTOR(Board_Prio_ISR_730, 730, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 730
#endif                            
#if BOARD_NB_VECTORS >= 731       
  BOARD_GEN_VECTOR(Board_Prio_ISR_731, 731, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 731
#endif                            
#if BOARD_NB_VECTORS >= 732       
  BOARD_GEN_VECTOR(Board_Prio_ISR_732, 732, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 732
#endif                            
#if BOARD_NB_VECTORS >= 733       
  BOARD_GEN_VECTOR(Board_Prio_ISR_733, 733, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 733
#endif                            
#if BOARD_NB_VECTORS >= 734       
  BOARD_GEN_VECTOR(Board_Prio_ISR_734, 734, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 734
#endif                            
#if BOARD_NB_VECTORS >= 735       
  BOARD_GEN_VECTOR(Board_Prio_ISR_735, 735, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 735
#endif                            
#if BOARD_NB_VECTORS >= 736       
  BOARD_GEN_VECTOR(Board_Prio_ISR_736, 736, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 736
#endif                            
#if BOARD_NB_VECTORS >= 737       
  BOARD_GEN_VECTOR(Board_Prio_ISR_737, 737, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 737
#endif                            
#if BOARD_NB_VECTORS >= 738       
  BOARD_GEN_VECTOR(Board_Prio_ISR_738, 738, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 738
#endif                            
#if BOARD_NB_VECTORS >= 739       
  BOARD_GEN_VECTOR(Board_Prio_ISR_739, 739, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 739
#endif                            
#if BOARD_NB_VECTORS >= 740       
  BOARD_GEN_VECTOR(Board_Prio_ISR_740, 740, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 740
#endif                            
#if BOARD_NB_VECTORS >= 741       
  BOARD_GEN_VECTOR(Board_Prio_ISR_741, 741, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 741
#endif                            
#if BOARD_NB_VECTORS >= 742       
  BOARD_GEN_VECTOR(Board_Prio_ISR_742, 742, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 742
#endif                            
#if BOARD_NB_VECTORS >= 743       
  BOARD_GEN_VECTOR(Board_Prio_ISR_743, 743, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 743
#endif                            
#if BOARD_NB_VECTORS >= 744       
  BOARD_GEN_VECTOR(Board_Prio_ISR_744, 744, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 744
#endif                            
#if BOARD_NB_VECTORS >= 745       
  BOARD_GEN_VECTOR(Board_Prio_ISR_745, 745, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 745
#endif                            
#if BOARD_NB_VECTORS >= 746       
  BOARD_GEN_VECTOR(Board_Prio_ISR_746, 746, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 746
#endif                            
#if BOARD_NB_VECTORS >= 747       
  BOARD_GEN_VECTOR(Board_Prio_ISR_747, 747, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 747
#endif                            
#if BOARD_NB_VECTORS >= 748       
  BOARD_GEN_VECTOR(Board_Prio_ISR_748, 748, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 748
#endif                            
#if BOARD_NB_VECTORS >= 749       
  BOARD_GEN_VECTOR(Board_Prio_ISR_749, 749, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 749
#endif                            
#if BOARD_NB_VECTORS >= 750       
  BOARD_GEN_VECTOR(Board_Prio_ISR_750, 750, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 750
#endif                            
#if BOARD_NB_VECTORS >= 751       
  BOARD_GEN_VECTOR(Board_Prio_ISR_751, 751, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 751
#endif                            
#if BOARD_NB_VECTORS >= 752       
  BOARD_GEN_VECTOR(Board_Prio_ISR_752, 752, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 752
#endif                            
#if BOARD_NB_VECTORS >= 753       
  BOARD_GEN_VECTOR(Board_Prio_ISR_753, 753, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 753
#endif                            
#if BOARD_NB_VECTORS >= 754       
  BOARD_GEN_VECTOR(Board_Prio_ISR_754, 754, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 754
#endif                            
#if BOARD_NB_VECTORS >= 755       
  BOARD_GEN_VECTOR(Board_Prio_ISR_755, 755, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 755
#endif                            
#if BOARD_NB_VECTORS >= 756       
  BOARD_GEN_VECTOR(Board_Prio_ISR_756, 756, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 756
#endif                            
#if BOARD_NB_VECTORS >= 757       
  BOARD_GEN_VECTOR(Board_Prio_ISR_757, 757, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 757
#endif                            
#if BOARD_NB_VECTORS >= 758       
  BOARD_GEN_VECTOR(Board_Prio_ISR_758, 758, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 758
#endif                            
#if BOARD_NB_VECTORS >= 759       
  BOARD_GEN_VECTOR(Board_Prio_ISR_759, 759, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 759
#endif                            
#if BOARD_NB_VECTORS >= 760       
  BOARD_GEN_VECTOR(Board_Prio_ISR_760, 760, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 760
#endif                            
#if BOARD_NB_VECTORS >= 761       
  BOARD_GEN_VECTOR(Board_Prio_ISR_761, 761, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 761
#endif                            
#if BOARD_NB_VECTORS >= 762       
  BOARD_GEN_VECTOR(Board_Prio_ISR_762, 762, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 762
#endif                            
#if BOARD_NB_VECTORS >= 763       
  BOARD_GEN_VECTOR(Board_Prio_ISR_763, 763, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 763
#endif                            
#if BOARD_NB_VECTORS >= 764       
  BOARD_GEN_VECTOR(Board_Prio_ISR_764, 764, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 764
#endif                            
#if BOARD_NB_VECTORS >= 765       
  BOARD_GEN_VECTOR(Board_Prio_ISR_765, 765, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 765
#endif                            
#if BOARD_NB_VECTORS >= 766       
  BOARD_GEN_VECTOR(Board_Prio_ISR_766, 766, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 766
#endif                            
#if BOARD_NB_VECTORS >= 767       
  BOARD_GEN_VECTOR(Board_Prio_ISR_767, 767, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 767
#endif                            
#if BOARD_NB_VECTORS >= 768       
  BOARD_GEN_VECTOR(Board_Prio_ISR_768, 768, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 768
#endif                            
#if BOARD_NB_VECTORS >= 769       
  BOARD_GEN_VECTOR(Board_Prio_ISR_769, 769, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 769
#endif                            
#if BOARD_NB_VECTORS >= 770       
  BOARD_GEN_VECTOR(Board_Prio_ISR_770, 770, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 770
#endif                            
#if BOARD_NB_VECTORS >= 771       
  BOARD_GEN_VECTOR(Board_Prio_ISR_771, 771, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 771
#endif                            
#if BOARD_NB_VECTORS >= 772       
  BOARD_GEN_VECTOR(Board_Prio_ISR_772, 772, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 772
#endif                            
#if BOARD_NB_VECTORS >= 773       
  BOARD_GEN_VECTOR(Board_Prio_ISR_773, 773, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 773
#endif                            
#if BOARD_NB_VECTORS >= 774       
  BOARD_GEN_VECTOR(Board_Prio_ISR_774, 774, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 774
#endif                            
#if BOARD_NB_VECTORS >= 775       
  BOARD_GEN_VECTOR(Board_Prio_ISR_775, 775, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 775
#endif                            
#if BOARD_NB_VECTORS >= 776       
  BOARD_GEN_VECTOR(Board_Prio_ISR_776, 776, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 776
#endif                            
#if BOARD_NB_VECTORS >= 777       
  BOARD_GEN_VECTOR(Board_Prio_ISR_777, 777, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 777
#endif                            
#if BOARD_NB_VECTORS >= 778       
  BOARD_GEN_VECTOR(Board_Prio_ISR_778, 778, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 778
#endif                            
#if BOARD_NB_VECTORS >= 779       
  BOARD_GEN_VECTOR(Board_Prio_ISR_779, 779, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 779
#endif                            
#if BOARD_NB_VECTORS >= 780       
  BOARD_GEN_VECTOR(Board_Prio_ISR_780, 780, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 780
#endif                            
#if BOARD_NB_VECTORS >= 781       
  BOARD_GEN_VECTOR(Board_Prio_ISR_781, 781, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 781
#endif                            
#if BOARD_NB_VECTORS >= 782       
  BOARD_GEN_VECTOR(Board_Prio_ISR_782, 782, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 782
#endif                            
#if BOARD_NB_VECTORS >= 783       
  BOARD_GEN_VECTOR(Board_Prio_ISR_783, 783, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 783
#endif                            
#if BOARD_NB_VECTORS >= 784       
  BOARD_GEN_VECTOR(Board_Prio_ISR_784, 784, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 784
#endif                            
#if BOARD_NB_VECTORS >= 785       
  BOARD_GEN_VECTOR(Board_Prio_ISR_785, 785, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 785
#endif                            
#if BOARD_NB_VECTORS >= 786       
  BOARD_GEN_VECTOR(Board_Prio_ISR_786, 786, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 786
#endif                            
#if BOARD_NB_VECTORS >= 787       
  BOARD_GEN_VECTOR(Board_Prio_ISR_787, 787, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 787
#endif                            
#if BOARD_NB_VECTORS >= 788       
  BOARD_GEN_VECTOR(Board_Prio_ISR_788, 788, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 788
#endif                            
#if BOARD_NB_VECTORS >= 789       
  BOARD_GEN_VECTOR(Board_Prio_ISR_789, 789, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 789
#endif                            
#if BOARD_NB_VECTORS >= 790       
  BOARD_GEN_VECTOR(Board_Prio_ISR_790, 790, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 790
#endif                            
#if BOARD_NB_VECTORS >= 791       
  BOARD_GEN_VECTOR(Board_Prio_ISR_791, 791, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 791
#endif                            
#if BOARD_NB_VECTORS >= 792       
  BOARD_GEN_VECTOR(Board_Prio_ISR_792, 792, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 792
#endif                            
#if BOARD_NB_VECTORS >= 793       
  BOARD_GEN_VECTOR(Board_Prio_ISR_793, 793, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 793
#endif                            
#if BOARD_NB_VECTORS >= 794       
  BOARD_GEN_VECTOR(Board_Prio_ISR_794, 794, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 794
#endif                            
#if BOARD_NB_VECTORS >= 795       
  BOARD_GEN_VECTOR(Board_Prio_ISR_795, 795, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 795
#endif                            
#if BOARD_NB_VECTORS >= 796       
  BOARD_GEN_VECTOR(Board_Prio_ISR_796, 796, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 796
#endif                            
#if BOARD_NB_VECTORS >= 797       
  BOARD_GEN_VECTOR(Board_Prio_ISR_797, 797, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 797
#endif                            
#if BOARD_NB_VECTORS >= 798       
  BOARD_GEN_VECTOR(Board_Prio_ISR_798, 798, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 798
#endif                            
#if BOARD_NB_VECTORS >= 799       
  BOARD_GEN_VECTOR(Board_Prio_ISR_799, 799, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 799
#endif
#if BOARD_NB_VECTORS >= 800
  BOARD_GEN_VECTOR(Board_Prio_ISR_800, 800, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 800
#endif                            
#if BOARD_NB_VECTORS >= 801       
  BOARD_GEN_VECTOR(Board_Prio_ISR_801, 801, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 801
#endif                            
#if BOARD_NB_VECTORS >= 802       
  BOARD_GEN_VECTOR(Board_Prio_ISR_802, 802, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 802
#endif                            
#if BOARD_NB_VECTORS >= 803       
  BOARD_GEN_VECTOR(Board_Prio_ISR_803, 803, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 803
#endif                            
#if BOARD_NB_VECTORS >= 804       
  BOARD_GEN_VECTOR(Board_Prio_ISR_804, 804, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 804
#endif                            
#if BOARD_NB_VECTORS >= 805       
  BOARD_GEN_VECTOR(Board_Prio_ISR_805, 805, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 805
#endif                            
#if BOARD_NB_VECTORS >= 806       
  BOARD_GEN_VECTOR(Board_Prio_ISR_806, 806, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 806
#endif                            
#if BOARD_NB_VECTORS >= 807       
  BOARD_GEN_VECTOR(Board_Prio_ISR_807, 807, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 807
#endif                            
#if BOARD_NB_VECTORS >= 808       
  BOARD_GEN_VECTOR(Board_Prio_ISR_808, 808, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 808
#endif                            
#if BOARD_NB_VECTORS >= 809       
  BOARD_GEN_VECTOR(Board_Prio_ISR_809, 809, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 809
#endif                            
#if BOARD_NB_VECTORS >= 810       
  BOARD_GEN_VECTOR(Board_Prio_ISR_810, 810, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 810
#endif                            
#if BOARD_NB_VECTORS >= 811       
  BOARD_GEN_VECTOR(Board_Prio_ISR_811, 811, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 811
#endif                            
#if BOARD_NB_VECTORS >= 812       
  BOARD_GEN_VECTOR(Board_Prio_ISR_812, 812, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 812
#endif                            
#if BOARD_NB_VECTORS >= 813       
  BOARD_GEN_VECTOR(Board_Prio_ISR_813, 813, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 813
#endif                            
#if BOARD_NB_VECTORS >= 814       
  BOARD_GEN_VECTOR(Board_Prio_ISR_814, 814, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 814
#endif                            
#if BOARD_NB_VECTORS >= 815       
  BOARD_GEN_VECTOR(Board_Prio_ISR_815, 815, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 815
#endif                            
#if BOARD_NB_VECTORS >= 816       
  BOARD_GEN_VECTOR(Board_Prio_ISR_816, 816, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 816
#endif                            
#if BOARD_NB_VECTORS >= 817       
  BOARD_GEN_VECTOR(Board_Prio_ISR_817, 817, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 817
#endif                            
#if BOARD_NB_VECTORS >= 818       
  BOARD_GEN_VECTOR(Board_Prio_ISR_818, 818, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 818
#endif                            
#if BOARD_NB_VECTORS >= 819       
  BOARD_GEN_VECTOR(Board_Prio_ISR_819, 819, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 819
#endif                            
#if BOARD_NB_VECTORS >= 820       
  BOARD_GEN_VECTOR(Board_Prio_ISR_820, 820, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 820
#endif                            
#if BOARD_NB_VECTORS >= 821       
  BOARD_GEN_VECTOR(Board_Prio_ISR_821, 821, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 821
#endif                            
#if BOARD_NB_VECTORS >= 822       
  BOARD_GEN_VECTOR(Board_Prio_ISR_822, 822, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 822
#endif                            
#if BOARD_NB_VECTORS >= 823       
  BOARD_GEN_VECTOR(Board_Prio_ISR_823, 823, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 823
#endif                            
#if BOARD_NB_VECTORS >= 824       
  BOARD_GEN_VECTOR(Board_Prio_ISR_824, 824, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 824
#endif                            
#if BOARD_NB_VECTORS >= 825       
  BOARD_GEN_VECTOR(Board_Prio_ISR_825, 825, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 825
#endif                            
#if BOARD_NB_VECTORS >= 826       
  BOARD_GEN_VECTOR(Board_Prio_ISR_826, 826, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 826
#endif                            
#if BOARD_NB_VECTORS >= 827       
  BOARD_GEN_VECTOR(Board_Prio_ISR_827, 827, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 827
#endif                            
#if BOARD_NB_VECTORS >= 828       
  BOARD_GEN_VECTOR(Board_Prio_ISR_828, 828, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 828
#endif                            
#if BOARD_NB_VECTORS >= 829       
  BOARD_GEN_VECTOR(Board_Prio_ISR_829, 829, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 829
#endif                            
#if BOARD_NB_VECTORS >= 830       
  BOARD_GEN_VECTOR(Board_Prio_ISR_830, 830, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 830
#endif                            
#if BOARD_NB_VECTORS >= 831       
  BOARD_GEN_VECTOR(Board_Prio_ISR_831, 831, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 831
#endif                            
#if BOARD_NB_VECTORS >= 832       
  BOARD_GEN_VECTOR(Board_Prio_ISR_832, 832, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 832
#endif                            
#if BOARD_NB_VECTORS >= 833       
  BOARD_GEN_VECTOR(Board_Prio_ISR_833, 833, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 833
#endif                            
#if BOARD_NB_VECTORS >= 834       
  BOARD_GEN_VECTOR(Board_Prio_ISR_8834, 834, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 834
#endif                            
#if BOARD_NB_VECTORS >= 835       
  BOARD_GEN_VECTOR(Board_Prio_ISR_8835, 835, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 835
#endif                            
#if BOARD_NB_VECTORS >= 836       
  BOARD_GEN_VECTOR(Board_Prio_ISR_8836, 836, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 836
#endif                            
#if BOARD_NB_VECTORS >= 837       
  BOARD_GEN_VECTOR(Board_Prio_ISR_837, 837, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 837
#endif                            
#if BOARD_NB_VECTORS >= 838       
  BOARD_GEN_VECTOR(Board_Prio_ISR_838, 838, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 838
#endif                            
#if BOARD_NB_VECTORS >= 839       
  BOARD_GEN_VECTOR(Board_Prio_ISR_839, 839, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 839
#endif                            
#if BOARD_NB_VECTORS >= 840       
  BOARD_GEN_VECTOR(Board_Prio_ISR_840, 840, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 840
#endif                            
#if BOARD_NB_VECTORS >= 841       
  BOARD_GEN_VECTOR(Board_Prio_ISR_841, 841, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 841
#endif                            
#if BOARD_NB_VECTORS >= 842       
  BOARD_GEN_VECTOR(Board_Prio_ISR_842, 842, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 842
#endif                            
#if BOARD_NB_VECTORS >= 843       
  BOARD_GEN_VECTOR(Board_Prio_ISR_843, 843, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 843
#endif                            
#if BOARD_NB_VECTORS >= 844       
  BOARD_GEN_VECTOR(Board_Prio_ISR_844, 844, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 844
#endif                            
#if BOARD_NB_VECTORS >= 845       
  BOARD_GEN_VECTOR(Board_Prio_ISR_845, 845, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 845
#endif                            
#if BOARD_NB_VECTORS >= 846       
  BOARD_GEN_VECTOR(Board_Prio_ISR_846, 846, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 846
#endif                            
#if BOARD_NB_VECTORS >= 847       
  BOARD_GEN_VECTOR(Board_Prio_ISR_847, 847, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 847
#endif                            
#if BOARD_NB_VECTORS >= 848       
  BOARD_GEN_VECTOR(Board_Prio_ISR_848, 848, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 848
#endif                            
#if BOARD_NB_VECTORS >= 849       
  BOARD_GEN_VECTOR(Board_Prio_ISR_849, 849, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 849
#endif                            
#if BOARD_NB_VECTORS >= 850       
  BOARD_GEN_VECTOR(Board_Prio_ISR_850, 850, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 850
#endif                            
#if BOARD_NB_VECTORS >= 851       
  BOARD_GEN_VECTOR(Board_Prio_ISR_851, 851, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 851
#endif                            
#if BOARD_NB_VECTORS >= 852       
  BOARD_GEN_VECTOR(Board_Prio_ISR_852, 852, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 852
#endif                            
#if BOARD_NB_VECTORS >= 853       
  BOARD_GEN_VECTOR(Board_Prio_ISR_853, 853, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 853
#endif                            
#if BOARD_NB_VECTORS >= 854       
  BOARD_GEN_VECTOR(Board_Prio_ISR_854, 854, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 854
#endif                            
#if BOARD_NB_VECTORS >= 855       
  BOARD_GEN_VECTOR(Board_Prio_ISR_855, 855, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 855
#endif                            
#if BOARD_NB_VECTORS >= 856       
  BOARD_GEN_VECTOR(Board_Prio_ISR_856, 856, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 856
#endif                            
#if BOARD_NB_VECTORS >= 857       
  BOARD_GEN_VECTOR(Board_Prio_ISR_857, 857, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 857
#endif                            
#if BOARD_NB_VECTORS >= 858       
  BOARD_GEN_VECTOR(Board_Prio_ISR_858, 858, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 858
#endif                            
#if BOARD_NB_VECTORS >= 859       
  BOARD_GEN_VECTOR(Board_Prio_ISR_859, 859, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 859
#endif                            
#if BOARD_NB_VECTORS >= 860       
  BOARD_GEN_VECTOR(Board_Prio_ISR_860, 860, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 860
#endif                            
#if BOARD_NB_VECTORS >= 861       
  BOARD_GEN_VECTOR(Board_Prio_ISR_861, 861, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 861
#endif                            
#if BOARD_NB_VECTORS >= 862       
  BOARD_GEN_VECTOR(Board_Prio_ISR_862, 862, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 862
#endif                            
#if BOARD_NB_VECTORS >= 863       
  BOARD_GEN_VECTOR(Board_Prio_ISR_863, 863, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 863
#endif                            
#if BOARD_NB_VECTORS >= 864       
  BOARD_GEN_VECTOR(Board_Prio_ISR_864, 864, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 864
#endif                            
#if BOARD_NB_VECTORS >= 865       
  BOARD_GEN_VECTOR(Board_Prio_ISR_865, 865, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 865
#endif                            
#if BOARD_NB_VECTORS >= 866       
  BOARD_GEN_VECTOR(Board_Prio_ISR_866, 866, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 866
#endif                            
#if BOARD_NB_VECTORS >= 867       
  BOARD_GEN_VECTOR(Board_Prio_ISR_867, 867, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 867
#endif                            
#if BOARD_NB_VECTORS >= 868       
  BOARD_GEN_VECTOR(Board_Prio_ISR_868, 868, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 868
#endif                            
#if BOARD_NB_VECTORS >= 869       
  BOARD_GEN_VECTOR(Board_Prio_ISR_869, 869, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 869
#endif                            
#if BOARD_NB_VECTORS >= 870       
  BOARD_GEN_VECTOR(Board_Prio_ISR_870, 870, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 870
#endif                            
#if BOARD_NB_VECTORS >= 871       
  BOARD_GEN_VECTOR(Board_Prio_ISR_871, 871, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 871
#endif                            
#if BOARD_NB_VECTORS >= 872       
  BOARD_GEN_VECTOR(Board_Prio_ISR_872, 872, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 872
#endif                            
#if BOARD_NB_VECTORS >= 873       
  BOARD_GEN_VECTOR(Board_Prio_ISR_873, 873, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 873
#endif                            
#if BOARD_NB_VECTORS >= 874       
  BOARD_GEN_VECTOR(Board_Prio_ISR_874, 874, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 874
#endif                            
#if BOARD_NB_VECTORS >= 875       
  BOARD_GEN_VECTOR(Board_Prio_ISR_875, 875, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 875
#endif                            
#if BOARD_NB_VECTORS >= 876       
  BOARD_GEN_VECTOR(Board_Prio_ISR_876, 876, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 876
#endif                            
#if BOARD_NB_VECTORS >= 877       
  BOARD_GEN_VECTOR(Board_Prio_ISR_877, 877, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 877
#endif                            
#if BOARD_NB_VECTORS >= 878       
  BOARD_GEN_VECTOR(Board_Prio_ISR_878, 878, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 878
#endif                            
#if BOARD_NB_VECTORS >= 879       
  BOARD_GEN_VECTOR(Board_Prio_ISR_879, 879, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 879
#endif                            
#if BOARD_NB_VECTORS >= 880       
  BOARD_GEN_VECTOR(Board_Prio_ISR_880, 880, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 880
#endif                            
#if BOARD_NB_VECTORS >= 881       
  BOARD_GEN_VECTOR(Board_Prio_ISR_881, 881, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 881
#endif                            
#if BOARD_NB_VECTORS >= 882       
  BOARD_GEN_VECTOR(Board_Prio_ISR_882, 882, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 882
#endif                            
#if BOARD_NB_VECTORS >= 883       
  BOARD_GEN_VECTOR(Board_Prio_ISR_883, 883, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 883
#endif                            
#if BOARD_NB_VECTORS >= 884       
  BOARD_GEN_VECTOR(Board_Prio_ISR_884, 884, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 884
#endif                            
#if BOARD_NB_VECTORS >= 885       
  BOARD_GEN_VECTOR(Board_Prio_ISR_885, 885, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 885
#endif                            
#if BOARD_NB_VECTORS >= 886       
  BOARD_GEN_VECTOR(Board_Prio_ISR_886, 886, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 886
#endif                            
#if BOARD_NB_VECTORS >= 887       
  BOARD_GEN_VECTOR(Board_Prio_ISR_887, 887, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 887
#endif                            
#if BOARD_NB_VECTORS >= 888       
  BOARD_GEN_VECTOR(Board_Prio_ISR_888, 888, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 888
#endif                            
#if BOARD_NB_VECTORS >= 889       
  BOARD_GEN_VECTOR(Board_Prio_ISR_889, 889, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 889
#endif                            
#if BOARD_NB_VECTORS >= 890       
  BOARD_GEN_VECTOR(Board_Prio_ISR_890, 890, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 890
#endif                            
#if BOARD_NB_VECTORS >= 891       
  BOARD_GEN_VECTOR(Board_Prio_ISR_891, 891, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 891
#endif                            
#if BOARD_NB_VECTORS >= 892       
  BOARD_GEN_VECTOR(Board_Prio_ISR_892, 892, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 892
#endif                            
#if BOARD_NB_VECTORS >= 893       
  BOARD_GEN_VECTOR(Board_Prio_ISR_893, 893, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 893
#endif                            
#if BOARD_NB_VECTORS >= 894       
  BOARD_GEN_VECTOR(Board_Prio_ISR_894, 894, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 894
#endif                            
#if BOARD_NB_VECTORS >= 895       
  BOARD_GEN_VECTOR(Board_Prio_ISR_895, 895, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 895
#endif                            
#if BOARD_NB_VECTORS >= 896       
  BOARD_GEN_VECTOR(Board_Prio_ISR_896, 896, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 896
#endif                            
#if BOARD_NB_VECTORS >= 897       
  BOARD_GEN_VECTOR(Board_Prio_ISR_897, 897, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 897
#endif                            
#if BOARD_NB_VECTORS >= 898       
  BOARD_GEN_VECTOR(Board_Prio_ISR_898, 898, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 898
#endif                            
#if BOARD_NB_VECTORS >= 899       
  BOARD_GEN_VECTOR(Board_Prio_ISR_899, 899, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 899
#endif
#if BOARD_NB_VECTORS >= 900
  BOARD_GEN_VECTOR(Board_Prio_ISR_900, 900, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 900
#endif                            
#if BOARD_NB_VECTORS >= 901       
  BOARD_GEN_VECTOR(Board_Prio_ISR_901, 901, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 901
#endif                            
#if BOARD_NB_VECTORS >= 902       
  BOARD_GEN_VECTOR(Board_Prio_ISR_902, 902, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 902
#endif                            
#if BOARD_NB_VECTORS >= 903       
  BOARD_GEN_VECTOR(Board_Prio_ISR_903, 903, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 903
#endif                            
#if BOARD_NB_VECTORS >= 904       
  BOARD_GEN_VECTOR(Board_Prio_ISR_904, 904, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 904
#endif                            
#if BOARD_NB_VECTORS >= 905       
  BOARD_GEN_VECTOR(Board_Prio_ISR_905, 905, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 905
#endif                            
#if BOARD_NB_VECTORS >= 906       
  BOARD_GEN_VECTOR(Board_Prio_ISR_906, 906, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 906
#endif                            
#if BOARD_NB_VECTORS >= 907       
  BOARD_GEN_VECTOR(Board_Prio_ISR_907, 907, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 907
#endif                            
#if BOARD_NB_VECTORS >= 908       
  BOARD_GEN_VECTOR(Board_Prio_ISR_908, 908, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 908
#endif                            
#if BOARD_NB_VECTORS >= 909       
  BOARD_GEN_VECTOR(Board_Prio_ISR_909, 909, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 909
#endif                            
#if BOARD_NB_VECTORS >= 910       
  BOARD_GEN_VECTOR(Board_Prio_ISR_910, 910, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 910
#endif                            
#if BOARD_NB_VECTORS >= 911       
  BOARD_GEN_VECTOR(Board_Prio_ISR_911, 911, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 911
#endif                            
#if BOARD_NB_VECTORS >= 912       
  BOARD_GEN_VECTOR(Board_Prio_ISR_912, 912, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 912
#endif                            
#if BOARD_NB_VECTORS >= 913       
  BOARD_GEN_VECTOR(Board_Prio_ISR_913, 913, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 913
#endif                            
#if BOARD_NB_VECTORS >= 914       
  BOARD_GEN_VECTOR(Board_Prio_ISR_914, 914, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 914
#endif                            
#if BOARD_NB_VECTORS >= 915       
  BOARD_GEN_VECTOR(Board_Prio_ISR_915, 915, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 915
#endif                            
#if BOARD_NB_VECTORS >= 916       
  BOARD_GEN_VECTOR(Board_Prio_ISR_916, 916, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 916
#endif                            
#if BOARD_NB_VECTORS >= 917       
  BOARD_GEN_VECTOR(Board_Prio_ISR_917, 917, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 917
#endif                            
#if BOARD_NB_VECTORS >= 918       
  BOARD_GEN_VECTOR(Board_Prio_ISR_918, 918, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 918
#endif                            
#if BOARD_NB_VECTORS >= 919       
  BOARD_GEN_VECTOR(Board_Prio_ISR_919, 919, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 919
#endif                            
#if BOARD_NB_VECTORS >= 920       
  BOARD_GEN_VECTOR(Board_Prio_ISR_920, 920, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 920
#endif                            
#if BOARD_NB_VECTORS >= 921       
  BOARD_GEN_VECTOR(Board_Prio_ISR_921, 921, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 921
#endif                            
#if BOARD_NB_VECTORS >= 922       
  BOARD_GEN_VECTOR(Board_Prio_ISR_922, 922, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 922
#endif                            
#if BOARD_NB_VECTORS >= 923       
  BOARD_GEN_VECTOR(Board_Prio_ISR_923, 923, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 923
#endif                            
#if BOARD_NB_VECTORS >= 924       
  BOARD_GEN_VECTOR(Board_Prio_ISR_924, 924, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 924
#endif                            
#if BOARD_NB_VECTORS >= 925       
  BOARD_GEN_VECTOR(Board_Prio_ISR_925, 925, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 925
#endif                            
#if BOARD_NB_VECTORS >= 926       
  BOARD_GEN_VECTOR(Board_Prio_ISR_926, 926, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 926
#endif                            
#if BOARD_NB_VECTORS >= 927       
  BOARD_GEN_VECTOR(Board_Prio_ISR_927, 927, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 927
#endif                            
#if BOARD_NB_VECTORS >= 928       
  BOARD_GEN_VECTOR(Board_Prio_ISR_928, 928, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 928
#endif                            
#if BOARD_NB_VECTORS >= 929       
  BOARD_GEN_VECTOR(Board_Prio_ISR_929, 929, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 929
#endif                            
#if BOARD_NB_VECTORS >= 930       
  BOARD_GEN_VECTOR(Board_Prio_ISR_930, 930, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 930
#endif                            
#if BOARD_NB_VECTORS >= 931       
  BOARD_GEN_VECTOR(Board_Prio_ISR_931, 931, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 931
#endif                            
#if BOARD_NB_VECTORS >= 932       
  BOARD_GEN_VECTOR(Board_Prio_ISR_932, 932, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 932
#endif                            
#if BOARD_NB_VECTORS >= 933       
  BOARD_GEN_VECTOR(Board_Prio_ISR_933, 933, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 933
#endif                            
#if BOARD_NB_VECTORS >= 934       
  BOARD_GEN_VECTOR(Board_Prio_ISR_934, 934, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 934
#endif                            
#if BOARD_NB_VECTORS >= 935       
  BOARD_GEN_VECTOR(Board_Prio_ISR_935, 935, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 935
#endif                            
#if BOARD_NB_VECTORS >= 936       
  BOARD_GEN_VECTOR(Board_Prio_ISR_936, 936, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 936
#endif                            
#if BOARD_NB_VECTORS >= 937       
  BOARD_GEN_VECTOR(Board_Prio_ISR_937, 937, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 937
#endif                            
#if BOARD_NB_VECTORS >= 938       
  BOARD_GEN_VECTOR(Board_Prio_ISR_938, 938, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 938
#endif                            
#if BOARD_NB_VECTORS >= 939       
  BOARD_GEN_VECTOR(Board_Prio_ISR_939, 939, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 939
#endif                            
#if BOARD_NB_VECTORS >= 940       
  BOARD_GEN_VECTOR(Board_Prio_ISR_940, 940, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 940
#endif                            
#if BOARD_NB_VECTORS >= 941       
  BOARD_GEN_VECTOR(Board_Prio_ISR_941, 941, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 941
#endif                            
#if BOARD_NB_VECTORS >= 942       
  BOARD_GEN_VECTOR(Board_Prio_ISR_942, 942, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 942
#endif                            
#if BOARD_NB_VECTORS >= 943       
  BOARD_GEN_VECTOR(Board_Prio_ISR_943, 943, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 943
#endif                            
#if BOARD_NB_VECTORS >= 944       
  BOARD_GEN_VECTOR(Board_Prio_ISR_944, 944, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 944
#endif                            
#if BOARD_NB_VECTORS >= 945       
  BOARD_GEN_VECTOR(Board_Prio_ISR_945, 945, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 945
#endif                            
#if BOARD_NB_VECTORS >= 946       
  BOARD_GEN_VECTOR(Board_Prio_ISR_946, 946, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 946
#endif                            
#if BOARD_NB_VECTORS >= 947       
  BOARD_GEN_VECTOR(Board_Prio_ISR_947, 947, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 947
#endif                            
#if BOARD_NB_VECTORS >= 948       
  BOARD_GEN_VECTOR(Board_Prio_ISR_948, 948, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 948
#endif                            
#if BOARD_NB_VECTORS >= 949       
  BOARD_GEN_VECTOR(Board_Prio_ISR_949, 949, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 949
#endif                            
#if BOARD_NB_VECTORS >= 950       
  BOARD_GEN_VECTOR(Board_Prio_ISR_950, 950, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 950
#endif                            
#if BOARD_NB_VECTORS >= 951       
  BOARD_GEN_VECTOR(Board_Prio_ISR_951, 951, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 951
#endif                            
#if BOARD_NB_VECTORS >= 952       
  BOARD_GEN_VECTOR(Board_Prio_ISR_952, 952, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 952
#endif                            
#if BOARD_NB_VECTORS >= 953       
  BOARD_GEN_VECTOR(Board_Prio_ISR_953, 953, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 953
#endif                            
#if BOARD_NB_VECTORS >= 954       
  BOARD_GEN_VECTOR(Board_Prio_ISR_954, 954, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 954
#endif                            
#if BOARD_NB_VECTORS >= 955       
  BOARD_GEN_VECTOR(Board_Prio_ISR_955, 955, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 955
#endif                            
#if BOARD_NB_VECTORS >= 956       
  BOARD_GEN_VECTOR(Board_Prio_ISR_956, 956, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 956
#endif                            
#if BOARD_NB_VECTORS >= 957       
  BOARD_GEN_VECTOR(Board_Prio_ISR_957, 957, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 957
#endif                            
#if BOARD_NB_VECTORS >= 958       
  BOARD_GEN_VECTOR(Board_Prio_ISR_958, 958, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 958
#endif                            
#if BOARD_NB_VECTORS >= 959       
  BOARD_GEN_VECTOR(Board_Prio_ISR_959, 959, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 959
#endif                            
#if BOARD_NB_VECTORS >= 960       
  BOARD_GEN_VECTOR(Board_Prio_ISR_960, 960, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 960
#endif                            
#if BOARD_NB_VECTORS >= 961       
  BOARD_GEN_VECTOR(Board_Prio_ISR_961, 961, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 961
#endif                            
#if BOARD_NB_VECTORS >= 962       
  BOARD_GEN_VECTOR(Board_Prio_ISR_962, 962, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 962
#endif                            
#if BOARD_NB_VECTORS >= 963       
  BOARD_GEN_VECTOR(Board_Prio_ISR_963, 963, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 963
#endif                            
#if BOARD_NB_VECTORS >= 964       
  BOARD_GEN_VECTOR(Board_Prio_ISR_964, 964, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 964
#endif                            
#if BOARD_NB_VECTORS >= 965       
  BOARD_GEN_VECTOR(Board_Prio_ISR_965, 965, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 965
#endif                            
#if BOARD_NB_VECTORS >= 966       
  BOARD_GEN_VECTOR(Board_Prio_ISR_966, 966, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 966
#endif                            
#if BOARD_NB_VECTORS >= 967       
  BOARD_GEN_VECTOR(Board_Prio_ISR_967, 967, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 967
#endif                            
#if BOARD_NB_VECTORS >= 968       
  BOARD_GEN_VECTOR(Board_Prio_ISR_968, 968, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 968
#endif                            
#if BOARD_NB_VECTORS >= 969       
  BOARD_GEN_VECTOR(Board_Prio_ISR_969, 969, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 969
#endif                            
#if BOARD_NB_VECTORS >= 970       
  BOARD_GEN_VECTOR(Board_Prio_ISR_970, 970, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 970
#endif                            
#if BOARD_NB_VECTORS >= 971       
  BOARD_GEN_VECTOR(Board_Prio_ISR_971, 971, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 971
#endif                            
#if BOARD_NB_VECTORS >= 972       
  BOARD_GEN_VECTOR(Board_Prio_ISR_972, 972, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 972
#endif                            
#if BOARD_NB_VECTORS >= 973       
  BOARD_GEN_VECTOR(Board_Prio_ISR_973, 973, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 973
#endif                            
#if BOARD_NB_VECTORS >= 974       
  BOARD_GEN_VECTOR(Board_Prio_ISR_974, 974, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 974
#endif                            
#if BOARD_NB_VECTORS >= 975       
  BOARD_GEN_VECTOR(Board_Prio_ISR_975, 975, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 975
#endif                            
#if BOARD_NB_VECTORS >= 976       
  BOARD_GEN_VECTOR(Board_Prio_ISR_976, 976, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 976
#endif                            
#if BOARD_NB_VECTORS >= 977       
  BOARD_GEN_VECTOR(Board_Prio_ISR_977, 977, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 977
#endif                            
#if BOARD_NB_VECTORS >= 978       
  BOARD_GEN_VECTOR(Board_Prio_ISR_978, 978, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 978
#endif                            
#if BOARD_NB_VECTORS >= 979       
  BOARD_GEN_VECTOR(Board_Prio_ISR_979, 979, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 979
#endif                            
#if BOARD_NB_VECTORS >= 980       
  BOARD_GEN_VECTOR(Board_Prio_ISR_980, 980, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 980
#endif                            
#if BOARD_NB_VECTORS >= 981       
  BOARD_GEN_VECTOR(Board_Prio_ISR_981, 981, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 981
#endif                            
#if BOARD_NB_VECTORS >= 982       
  BOARD_GEN_VECTOR(Board_Prio_ISR_982, 982, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 982
#endif                            
#if BOARD_NB_VECTORS >= 983       
  BOARD_GEN_VECTOR(Board_Prio_ISR_983, 983, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 983
#endif                            
#if BOARD_NB_VECTORS >= 984       
  BOARD_GEN_VECTOR(Board_Prio_ISR_984, 984, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 984
#endif                            
#if BOARD_NB_VECTORS >= 985       
  BOARD_GEN_VECTOR(Board_Prio_ISR_985, 985, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 985
#endif                            
#if BOARD_NB_VECTORS >= 986       
  BOARD_GEN_VECTOR(Board_Prio_ISR_986, 986, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 986
#endif                            
#if BOARD_NB_VECTORS >= 987       
  BOARD_GEN_VECTOR(Board_Prio_ISR_987, 987, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 987
#endif                            
#if BOARD_NB_VECTORS >= 988       
  BOARD_GEN_VECTOR(Board_Prio_ISR_988, 988, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 988
#endif                            
#if BOARD_NB_VECTORS >= 989       
  BOARD_GEN_VECTOR(Board_Prio_ISR_989, 989, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 989
#endif                            
#if BOARD_NB_VECTORS >= 990       
  BOARD_GEN_VECTOR(Board_Prio_ISR_990, 990, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 990
#endif                            
#if BOARD_NB_VECTORS >= 991       
  BOARD_GEN_VECTOR(Board_Prio_ISR_991, 991, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 991
#endif                            
#if BOARD_NB_VECTORS >= 992       
  BOARD_GEN_VECTOR(Board_Prio_ISR_992, 992, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 992
#endif                            
#if BOARD_NB_VECTORS >= 993       
  BOARD_GEN_VECTOR(Board_Prio_ISR_993, 993, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 993
#endif                            
#if BOARD_NB_VECTORS >= 994       
  BOARD_GEN_VECTOR(Board_Prio_ISR_994, 994, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 994
#endif                            
#if BOARD_NB_VECTORS >= 995       
  BOARD_GEN_VECTOR(Board_Prio_ISR_995, 995, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 995
#endif                            
#if BOARD_NB_VECTORS >= 996       
  BOARD_GEN_VECTOR(Board_Prio_ISR_996, 996, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 996
#endif                            
#if BOARD_NB_VECTORS >= 997       
  BOARD_GEN_VECTOR(Board_Prio_ISR_997, 997, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 997
#endif                            
#if BOARD_NB_VECTORS >= 998       
  BOARD_GEN_VECTOR(Board_Prio_ISR_998, 998, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 998
#endif                            
#if BOARD_NB_VECTORS >= 999       
  BOARD_GEN_VECTOR(Board_Prio_ISR_999, 999, BoardIsrHandler, BOARD_INTC_ALIGN_NB_P2)    # interrupt vector 999
#endif


/* All Exception mapped to BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2

 * !LINKSTO OsekCore.Platforms.Board.015, 1
 */
    PLATFORMS_ASM_SECTION_EXCTAB
    PLATFORMS_ASM_VLE
 
BoardExceptionTable:

#if BOARD_NB_EXCEPTIONS >= 0
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_00, 0, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 0hj0
#endif
#if BOARD_NB_EXCEPTIONS >= 1
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_01, 1, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 01
#endif
#if BOARD_NB_EXCEPTIONS >= 2
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_02, 2, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 02
#endif
#if BOARD_NB_EXCEPTIONS >= 3
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_03, 3, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 03
#endif
#if BOARD_NB_EXCEPTIONS >= 4
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_04, 4, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 04
#endif
#if BOARD_NB_EXCEPTIONS >= 5
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_05, 5, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 05
#endif
#if BOARD_NB_EXCEPTIONS >= 6
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_06, 6, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 06
#endif
#if BOARD_NB_EXCEPTIONS >= 7
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_07, 7, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 07
#endif
#if BOARD_NB_EXCEPTIONS >= 8
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_08, 8, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 08
#endif
#if BOARD_NB_EXCEPTIONS >= 9
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_09, 9, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 09
#endif
#if BOARD_NB_EXCEPTIONS >= 10
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_10, 10, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 10
#endif
#if BOARD_NB_EXCEPTIONS >= 11
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_11, 11, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 11
#endif
#if BOARD_NB_EXCEPTIONS >= 12
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_12, 12, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 12
#endif
#if BOARD_NB_EXCEPTIONS >= 13
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_13, 13, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 13
#endif
#if BOARD_NB_EXCEPTIONS >= 14
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_14, 14, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 14
#endif
#if BOARD_NB_EXCEPTIONS >= 15
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_15, 15, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 15
#endif
#if BOARD_NB_EXCEPTIONS >= 16
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_16, 16, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 16
#endif
#if BOARD_NB_EXCEPTIONS >= 17
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_17, 17, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 17
#endif
#if BOARD_NB_EXCEPTIONS >= 18
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_18, 18, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 18
#endif
#if BOARD_NB_EXCEPTIONS >= 19
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_19, 19, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 19
#endif
#if BOARD_NB_EXCEPTIONS >= 20
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_20, 20, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 20
#endif
#if BOARD_NB_EXCEPTIONS >= 21
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_21, 21, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 21
#endif
#if BOARD_NB_EXCEPTIONS >= 22
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_22, 22, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 22
#endif
#if BOARD_NB_EXCEPTIONS >= 23
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_23, 23, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 23
#endif
#if BOARD_NB_EXCEPTIONS >= 24
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_24, 24, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 24
#endif
#if BOARD_NB_EXCEPTIONS >= 25
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_25, 25, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 25
#endif
#if BOARD_NB_EXCEPTIONS >= 26
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_26, 26, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 26
#endif
#if BOARD_NB_EXCEPTIONS >= 27
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_27, 27, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 27
#endif
#if BOARD_NB_EXCEPTIONS >= 28
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_28, 28, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 28
#endif
#if BOARD_NB_EXCEPTIONS >= 29
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_29, 29, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 29
#endif
#if BOARD_NB_EXCEPTIONS >= 30
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_30, 30, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 30
#endif
#if BOARD_NB_EXCEPTIONS >= 31
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_31, 31, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 31
#endif
#if BOARD_NB_EXCEPTIONS >= 32
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_32, 32, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 32
#endif
#if BOARD_NB_EXCEPTIONS >= 33
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_33, 33, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 33
#endif
#if BOARD_NB_EXCEPTIONS >= 34
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_34, 34, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 34
#endif
#if BOARD_NB_EXCEPTIONS >= 35
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_35, 35, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 35
#endif
#if BOARD_NB_EXCEPTIONS >= 36
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_36, 36, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 36
#endif
#if BOARD_NB_EXCEPTIONS >= 37
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_37, 37, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 37
#endif
#if BOARD_NB_EXCEPTIONS >= 38
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_38, 38, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 38
#endif
#if BOARD_NB_EXCEPTIONS >= 39
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_39, 39, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 39
#endif
#if BOARD_NB_EXCEPTIONS >= 40
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_40, 40, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 40
#endif
#if BOARD_NB_EXCEPTIONS >= 41
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_41, 41, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 41
#endif
#if BOARD_NB_EXCEPTIONS >= 42
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_42, 42, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 42
#endif
#if BOARD_NB_EXCEPTIONS >= 43
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_43, 43, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 43
#endif
#if BOARD_NB_EXCEPTIONS >= 44
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_44, 44, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 44
#endif
#if BOARD_NB_EXCEPTIONS >= 45
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_45, 45, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 45
#endif
#if BOARD_NB_EXCEPTIONS >= 46
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_46, 46, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 46
#endif
#if BOARD_NB_EXCEPTIONS >= 47
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_47, 47, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 47
#endif
#if BOARD_NB_EXCEPTIONS >= 48
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_48, 48, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 48
#endif
#if BOARD_NB_EXCEPTIONS >= 49
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_49, 49, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 49
#endif
#if BOARD_NB_EXCEPTIONS >= 50
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_50, 50, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 50
#endif
#if BOARD_NB_EXCEPTIONS >= 51
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_51, 51, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 51
#endif
#if BOARD_NB_EXCEPTIONS >= 52
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_52, 52, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 52
#endif
#if BOARD_NB_EXCEPTIONS >= 53
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_53, 53, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 53
#endif
#if BOARD_NB_EXCEPTIONS >= 54
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_54, 54, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 54
#endif
#if BOARD_NB_EXCEPTIONS >= 55
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_55, 55, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 55
#endif
#if BOARD_NB_EXCEPTIONS >= 56
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_56, 56, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 56
#endif
#if BOARD_NB_EXCEPTIONS >= 57
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_57, 57, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 57
#endif
#if BOARD_NB_EXCEPTIONS >= 58
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_58, 58, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 58
#endif
#if BOARD_NB_EXCEPTIONS >= 59
  BOARD_GEN_EXCEPTIONS(Board_Prio_EXC_59, 59, BoardInvalidException, BOARD_EXCEP_ALIGN_NB_P2)    # Exception vector 59
#endif


    PLATFORMS_ASM_SRC_END
