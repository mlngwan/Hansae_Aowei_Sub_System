/**************************************
 * Copyright 1998-2014 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 *
 **************************************/

 /* 
  * !LINKSTO OsekCore.Platforms.Board.012, 1
  */
  
MEMORY
{
    /******************************************************************
    ********************** FLASH **************************************/
    /*
    * !LINKSTO OsekCore.Platforms.Linker.007, 1
    */
    reset               : org = 0x00000000, len = 0x01000
    /*
    * !LINKSTO OsekCore.Platforms.Linker.006, 1
    */
    exctab               : org = 0x00001000, len = 0x00800
    intctab              : org = 0x00001800, len = 0x00800
    
    /* 
    * !LINKSTO OsekCore.Platforms.Linker.002, 1
    */
    mtextboot            : org = 0x00002000, len = 0x05E00
    
    /* 
    * !LINKSTO OsekCore.Platforms.Linker.008, 1
    */
    mtextflash           : org = 0x00007F00, len = 0x00100
    
    /* 
    * !LINKSTO OsekCore.Platforms.Linker.001, 1
    */
    mtext                : org = 0x00008000, len = 0x78000  /* end of 512 kb Flash */
    
    /******************************************************************
    ********************** RAM** **************************************/    
    /* 
    * !LINKSTO OsekCore.Platforms.Linker.009, 1
    * !LINKSTO OsekCore.Platforms.Startup.012, 1
    */
    mkernstack           : org = 0x40000000, len = 0x00E00

    /* 
    * !LINKSTO OsekCore.Platforms.Linker.008, 1
    */
    mdataflash           : org = 0x40000E00, len = 0x00100

    /* 
    * !LINKSTO OsekCore.Platforms.Linker.003, 1
    */
    mdata                : org = 0x40000F00, len = 0x0A000

    /* 
    * memory RAM section for Flash apis
    */     
    mflashapiram         : org = 0x4000AF00, len = 0x01000

    /* 
    * !LINKSTO OsekCore.Platforms.Linker.005, 1
    */
    section_noinit       : org = 0x4000BF00, len = 0x00100  /* 48 KB SRAM */

}

SECTIONS
{
    .reset : { *(.reset) *(.reset_code) } > reset

    .exctab :
    {
        __EXCEPTION_TABLE_START = .;
        *(.exctab)
        *(.exctab_vle)
        __EXCEPTION_TABLE_END = .;
    } > exctab

    .intctab ALIGN(2048) :
    {
        __INTC_TABLE_START = .;
        *(.intctab)
        *(.intctab_vle)
        __INTC_TABLE_END = .;
    } > intctab

    .text ALIGN(32) :
    {
        *(.text)
        *(.vletext)
        *(.platforms_text)
    } > mtext

    __GLBL_TEXT_END     = .;

    __GLBL_RODATA_START = .;
    .sdata2 ALIGN(32) :
    {
        *(.sdata2)
    } > mtext
    .rodata ALIGN(32) :
    {
        *(.rodata)
    } > mtext
    __GLBL_RODATA_END = .;

    .fls_driver ALIGN(4) : {*(.fls_driver)} > . /* Function executed from RAM need to be aligned on 4 bytes imperatively. */
        __FLS_DRV_ST = . - SIZEOF(.fls_driver); 
        __FLS_DRV_SIZE = . - __FLS_DRV_ST;

    // Sections to be copied from Flash -> RAM
    // Order MUST match the order of appropriate RAM Sections below
    .ROM.sdata   ALIGN(32) ROM(.sdata) : {}  > .
    .ROM.data              ROM(.data)  : {}  > .
    .ROMend                            : {}  > .

  __IDATSTART = .;
    .sdabase:
    {
        *(.sdabase)
    } > mdata

    .sdata ALIGN(32): AT(__IDATSTART)
    {
        __STARTDATA = .;
    } > mdata
    .data :
    {
        *(.sdata)
        *(.data)
    } > mdata
    __ENDDATA = .;
    .bss ALIGN(32):
    {
        __STARTBSS = .;
        *(.sbss)
        *(.bss)
    } > mdata
    __ENDBSS = .;
    
    .section_noinit ALIGN(32):
    {
        __STARTNOINIT = .;
    } > section_noinit
    __ENDNOINIT = .;
    
    __IVPR          = __EXCEPTION_TABLE_START;
    _EXCEPTION_TABLE_START = __EXCEPTION_TABLE_START;
    _INTC_TABLE_START = __INTC_TABLE_START;
    __SP_INIT       = ADDR(mkernstack) + SIZEOF(mkernstack);
    __SP_END        = ADDR(mkernstack);
    _SP_INIT_       = __SP_INIT;
    
    __INITDATA = __IDATSTART;

    __GLBL_DATA_START   = __STARTDATA;
    __GLBL_DATA_END     = __ENDBSS;


    _BSS_START_     = __STARTBSS;
    _BSS_END_       = __ENDBSS;
    _DATA_START_    = __STARTDATA;
    _DATA_END_      = __ENDDATA;
    _INIT_          =
    _NOINIT_START_  = __STARTNOINIT;
    _NOINIT_END_    = __ENDNOINIT;
    
    ___SP_INIT      = __SP_INIT;
    ___SP_END       = __SP_END;

    __DATA_ROM   = ADDR(.ROM.sdata);
    _INIT_       = __DATA_ROM;
    __GLBL_RODATA_SIZE =  __GLBL_RODATA_END - __GLBL_RODATA_START;

    __FLASH_API_RAM_BASE = ADDR(mflashapiram);   /* SSD Flash drivers are copied into this area in RAM and executed */
    /* RAM to be initialized if the complete RAM sections WITHOUT RamNoInit*/
    __GLBL_RAM_START    = ADDR(mkernstack);
    __GLBL_RAM_SIZE     = SIZEOF(mkernstack) + SIZEOF(mdataflash) + SIZEOF(mdata);
    __GLBL_RAM_END      = __GLBL_RAM_START + __GLBL_RAM_SIZE;

    __GLBL_TEXT_START   = ADDR(mtext);
    __GLBL_TEXT_SIZE    = (__GLBL_TEXT_END - __GLBL_TEXT_START);

}
