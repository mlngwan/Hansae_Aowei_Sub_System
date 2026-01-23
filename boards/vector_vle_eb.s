/**
 * \file
 *
 * \blief Board startup files
 *
 * This file contains the implementation of the hardware
 * interrupt vector table
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2014 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* file is nammed vector_vle_eb.s
 * !LINKSTO OsekCore.Platforms.Board.012, 1
 */

/*define to exclude C Code in header files*/
#define BOARD_ASM    1

/* Must be first */
#include <Platforms_asm_PA.h>
#include <board.h>
 
  Platforms_file(vector_vle_eb.s)

  Platforms_global BoardInvalidInterrupt
  Platforms_global BoardIsrHandler

#define PLATFORMS_INTC_ALIGN Platforms_align(4,0)

  PLATFORMS_ASM_SECTION_INTCTAB
  PLATFORMS_ASM_VLE

/*
 * define symbols as global to allow easier debugging
 */
  Platforms_global IntcVectorTableHw


/*
 * intc vector table for HW vector mode at IVPR
 */
 /* All ISRs are mapped to BoardInvalidException to catch all unwanted exceptions
 * !LINKSTO OsekCore.Platforms.Board.014, 1
 */
IntcVectorTableHw:
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 0
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 1
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 2
  PLATFORMS_INTC_ALIGN
#ifdef BOARD_ISR_TEST
  b BoardIsrHandler          # interrupt vector 3
#else
  b BoardInvalidInterrupt    # interrupt vector 3
#endif
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 4
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 5
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 6
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 7
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 8
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 9
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 10
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 11
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 12
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 13
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 14
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 15
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 16
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 17
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 18
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 19
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 20
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 21
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 22
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 23
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 24
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 25
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 26
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 27
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 28
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 29
  PLATFORMS_INTC_ALIGN
  b BoardIsrHandler	  # BoardIsrHandler			 # interrupt vector 30 # edit by trueonot : STM interrupt, 2017.01.03.
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 31
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 32
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 33
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 34
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 35
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 36
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 37
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 38
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 39
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 40
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 41
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 42
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 43
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 44
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 45
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 46
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 47
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 48
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 49
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 50
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 51
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 52
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 53
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 54
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 55
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 56
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 57
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 58
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 59
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 60
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 61
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 62
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 63
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 64
#ifdef BOARD_ISR_CAN0
  PLATFORMS_INTC_ALIGN
  b BoardIsrHandler      # FlexCAN_ESR[ERR_INT] interrupt vector 65
  PLATFORMS_INTC_ALIGN
  b BoardIsrHandler      # FlexCAN_ESR_BOFF/FlexCAN_Transmit_Warning/FlexCAN_Receive_Warning interrupt vector 66
  PLATFORMS_INTC_ALIGN
  b BoardIsrHandler           # interrupt vector 67
  PLATFORMS_INTC_ALIGN
  b BoardIsrHandler    # FlexCAN_BUF_00_03 interrupt vector 68
  PLATFORMS_INTC_ALIGN
  b BoardIsrHandler    # FlexCAN_BUF_04_07 interrupt vector 69
  PLATFORMS_INTC_ALIGN
  b BoardIsrHandler    # FlexCAN_BUF_08_11 interrupt vector 70
  PLATFORMS_INTC_ALIGN
  b BoardIsrHandler    # FlexCAN_BUF_12_15 interrupt vector 71
  PLATFORMS_INTC_ALIGN
  b BoardIsrHandler    # FlexCAN_BUF_16_31 interrupt vector 72
  PLATFORMS_INTC_ALIGN
  b BoardIsrHandler    # FlexCAN_BUF_32_63 interrupt vector 73
  #else
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 65
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 66
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 67
  PLATFORMS_INTC_ALIGN
  b BoardIsrHandler    # interrupt vector 68
  PLATFORMS_INTC_ALIGN
  b BoardIsrHandler    # interrupt vector 69
  PLATFORMS_INTC_ALIGN
  b BoardIsrHandler    # interrupt vector 70
  PLATFORMS_INTC_ALIGN
  b BoardIsrHandler    # interrupt vector 71
  PLATFORMS_INTC_ALIGN
  b BoardIsrHandler    # interrupt vector 72
  PLATFORMS_INTC_ALIGN
  b BoardIsrHandler    		# interrupt vector 73	# edit by SB for CAN0 message buffer interrupt(32~63) 2019.06.25
  #endif
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 74
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 75
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 76
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 77
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 78
  PLATFORMS_INTC_ALIGN  
  b BoardIsrHandler   		 # interrupt vector 79 # edit by trueonot for UART interrupt 2017.01.03
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 80
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 81
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 82
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 83
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 84
#ifdef BOARD_ISR_CAN0
  PLATFORMS_INTC_ALIGN
  b BoardIsrHandler      # FlexCAN_ESR[ERR_INT] interrupt vector 85
  PLATFORMS_INTC_ALIGN
  b BoardIsrHandler      # FlexCAN_ESR_BOFF/FlexCAN_Transmit_Warning/FlexCAN_Receive_Warning interrupt vector 86
  PLATFORMS_INTC_ALIGN
  b BoardIsrHandler           # interrupt vector 87
  PLATFORMS_INTC_ALIGN
  b BoardIsrHandler    # FlexCAN_BUF_00_03 interrupt vector 88
  PLATFORMS_INTC_ALIGN
  b BoardIsrHandler    # FlexCAN_BUF_04_07 interrupt vector 89
  PLATFORMS_INTC_ALIGN
  b BoardIsrHandler    # FlexCAN_BUF_08_11 interrupt vector 90
  PLATFORMS_INTC_ALIGN
  b BoardIsrHandler    # FlexCAN_BUF_12_15 interrupt vector 91
  PLATFORMS_INTC_ALIGN
  b BoardIsrHandler    # FlexCAN_BUF_16_31 interrupt vector 92
  PLATFORMS_INTC_ALIGN
  b BoardIsrHandler    # FlexCAN_BUF_32_63 interrupt vector 93
  #else
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 85
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 86
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 87
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 88
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 89
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 90
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 91
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 92
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 93
  #endif
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 94
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 95
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 96
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 97
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 98
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 99
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 100
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 101
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 102
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 103
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 104
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 105
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 106
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 107
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 108
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 109
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 110
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 111
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 112
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 113
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 114
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 115
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 116
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 117
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 118
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 119
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 120
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 121
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 122
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 123
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 124
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 125
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 126
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 127
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 128
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 129
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 130
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 131
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 132
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 133
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 134
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 135
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 136
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 137
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 138
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 139
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 140
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 141
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 142
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 143
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 144
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 145
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 146
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 147
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 148
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 149
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 150
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 151
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 152
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 153
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 154
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 155
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 156
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 157
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 158
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 159
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 160
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 161
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 162
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 163
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 164
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 165
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 166
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 167
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 168
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 169
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 170
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 171
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 172
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 173
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 174
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 175
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 176
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 177
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 178
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 179
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 180
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 181
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 182
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 183
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 184
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 185
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 186
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 187
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 188
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 189
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 190
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 191
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 192
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 193
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 194
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 195
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 196
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 197
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 198
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 199
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 200
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 201
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 202
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 203
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 204
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 205
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 206
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 207
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 208
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 209
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 210
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 211
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 212
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 213
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 214
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 215
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt    # interrupt vector 216
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt     # interrupt vector 217
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt     # interrupt vector 218
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt     # interrupt vector 219
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt     # interrupt vector 220
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt     # interrupt vector 221
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt     # interrupt vector 222
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt     # interrupt vector 223
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt     # interrupt vector 224
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt     # interrupt vector 225
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt     # interrupt vector 226
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt     # interrupt vector 227
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt     # interrupt vector 228
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt     # interrupt vector 229
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt     # interrupt vector 230
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt     # interrupt vector 231
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt     # interrupt vector 232
  PLATFORMS_INTC_ALIGN
  b BoardInvalidInterrupt     # interrupt vector 233


/* All Exception mapped to BoardInvalidException

 * !LINKSTO OsekCore.Platforms.Board.015,1
 */
  PLATFORMS_ASM_SECTION_EXCTAB
  PLATFORMS_ASM_VLE

BoardExceptionTable:

        . = BoardExceptionTable + 0x00

/* Critical interrupt
*/
BoardInvalidException_0:
        li      r3,0
        b       BoardInvalidException

        . = BoardExceptionTable + 0x10
/* Machine check
*/
BoardInvalidException_1:
        li      r3,1
        b       BoardInvalidException

        . = BoardExceptionTable + 0x20
/* Data storage
*/
BoardInvalidException_2:
        li      r3,2
        b       BoardInvalidException

        . = BoardExceptionTable + 0x30
/* Instruction storage
*/
BoardInvalidException_3:
        li      r3,3
        b       BoardInvalidException

        . = BoardExceptionTable + 0x40
/* External input
*/
BoardInvalidException_4:
        li      r3,4
        b       BoardInvalidException

        . = BoardExceptionTable + 0x50
/* Alignment
*/
BoardInvalidException_5:
        li      r3,5
        b       BoardInvalidException

        . = BoardExceptionTable + 0x60
/* Program
*/
BoardInvalidException_6:
        li      r3,6
        b       BoardInvalidException

        . = BoardExceptionTable + 0x70
/* Reserved
*/
BoardInvalidException_7:
        li      r3,7
        b       BoardInvalidException

        . = BoardExceptionTable + 0x80
/* System call
*/
BoardInvalidException_8:
        li      r3,8
        b       BoardInvalidException

        . = BoardExceptionTable + 0x100


  PLATFORMS_ASM_SRC_END
