/* ************************************************************************** */
/** Data EEPROM Emulation - Register Storage

  @Company
    Satpos as

  @File Name
    EE_RegisterStorage.h

  @Summary
    Brief description of the file.

  @Description
    This header file contains functions implementations to emulate EEPROM.
    It is a ported version for PIC32MM based on Microchip NVM functions and 
    the dee.h/.c project.
 */
/* ************************************************************************** */

#ifndef _EE_REGISTER_STORAGE_H    /* Guard against multiple inclusion */
#define _EE_REGISTER_STORAGE_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include <stdio.h>
#include "xc.h"                // SYS function prototypes
#include "flash.h"


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif


    /* ************************************************************************** */
    /* ************************************************************************** */
    /* Section: Constants                                                         */
    /* ************************************************************************** */
    /* ************************************************************************** */


    /* ************************************************************************** */
    /** APP_FLASH_ADDRESS

      @Summary
        Where in ROM we place the data... 
    
      @Description
        ...
    
      @Remarks
        Must match File>Project Properties>xc32>xc32-ld>Option Categories: General>Additional driver options 
        and adding the -mreserve options: -mreserve=prog@0x1d00F000:0x1d00FFFF reserves program memory space 
        from 0x1d00F000 - 0x1d00FFFF.

     */
#define APP_FLASH_ADDRESS       (0x9d00F000ul) 
    
    
    /* ************************************************************************** */
    /** FLASH_PAGESIZE

      @Summary
        Flash Erase Page Size for PIC32MM-family. 
    
      @Description
        ...
    
      @Remarks
        ...

     */
#define FLASH_PAGESIZE          (0x00000800ul)

    
    /* ************************************************************************** */
    /** MAX_NUMBER_REGISTERS

      @Summary
        Max Number of Registers that can be used simultaneity 
    
      @Description
        Describe the number of Register cells that is available when 4k bytes/page
        for PIC32.
    
      @Remarks
        ...
     */
#define MAX_NUMBER_REGISTERS (FLASH_PAGESIZE / sizeof(DEE_DATA) )  // 512?
    
    
    /* ************************************************************************** */
    /** FLASH_PAGE_SIZE_BYTES

      @Summary
        2048 bytes/page for PIC32MM
    
      @Description
        The smallest section that can be erased 2048 bytes/page for PIC32MM.
    
      @Remarks
        ...
     */    
#define FLASH_PAGE_SIZE_BYTES (FLASH_PAGESIZE) // 2048 bytes/page for PIC32MM
    
    
    /* ************************************************************************** */
    /** NUM_PAGES_TO_USE

      @Summary
        2 Pages used, on always empty... 
    
      @Description
        2 Pages used so one can be cleared out when the other get full and needs
        an clean-out. This use the facts, that we can write 1->0 bit-by-bit and 
        0->1 Page by Page.
     
      @Remarks
        ...
     */    
#define NUM_PAGES_TO_USE (2)
    
    
    /* ************************************************************************** */
    /** FLASH_SIZE_BYTES

      @Summary
        FLASH_SIZE_BYTES equal the space we occupie.
    
      @Description
        FLASH_SIZE_BYTES is the total number of FLASH this Register occupy.
     
      @Remarks
        ...
     */    
#define FLASH_SIZE_BYTES (NUM_PAGES_TO_USE*FLASH_PAGE_SIZE_BYTES)
    
    
    
    /* ************************************************************************** */
    /** FD_MODE_FLAG

      @Summary
        FD_MODE_FLAG used to define the behavior and HW configuration system runs
        with/on.
    
      @Description
        FD_MODE_FLAG used to define the behavior and HW configuration system runs
        with/on.
     
      @Remarks
        ...
     */    
#define FD_MODE_FLAG_NOT_DEFINED 0xFFFFFFFF    
#define FD_MODE_FLAG_ONLY_BG96 1    
#define FD_MODE_FLAG_ONLY_ROCKBLOCK 2    
    
  
    // *****************************************************************************
    // *****************************************************************************
    // Section: Data Types
    // *****************************************************************************
    // *****************************************************************************

   
    // *****************************************************************************

    /** DEE_DATA

      @Summary
        Placeholder for Key and Data - both 32 bits (uint32_t).
    
      @Description
        ...
      @Remarks
        Any additional remarks
        <p>
        Describe enumeration elements and structure and union members above each 
        element or member.
     */
    typedef struct{
        uint32_t key;
        uint32_t value;
    } DEE_DATA;

    
    // *****************************************************************************

    /** FLASH_DATA_IDs

      @Summary
        Reference Index of DATA in FLASH.
    
      @Description
        ...
      @Remarks
        Do not Add new Indexes in list other places than to the end of list...
     */
    enum FLASH_DATA_IDs {

        HORIZ_SPEED_INDEX=1,
        VERT_SPEED_INDEX,
        FOCUS_SPEED_INDEX
        
    };

    

    // *****************************************************************************
    // *****************************************************************************
    // Section: Interface Functions
    // *****************************************************************************
    // *****************************************************************************


    // *****************************************************************************
    /**
      @Function
        void EERS_Init(void) 

      @Summary
        Map up callback function for signal xferDone-flag.

      @Description
        Map up callback function for signal xferDone-flag.
      
      @Precondition
        NVM_Initialize() must be ran before this function.

      @Parameters
        None

      @Returns
        None
     */
void EERS_Init(void);

    
    // *****************************************************************************
    /**
      @Function
        uint32_t EERS_Write(uint32_t key, uint32_t value); 

      @Summary
        This function writes "value" mapped by "key".

      @Description
        This function writes "value" mapped by "key".
      
      @Precondition
        NVM_Initialize() must be ran before this function.

      @Parameters
        @param key "key" is the unique value that marks the "value".
    
        @param value "value" is the information to store at location "key".

      @Returns
        Returns zero if successful.
        <ul>
          <li>!0  Indicates an error occurred
          <li>0   Indicates an error did not occur
        </ul>

      @Remarks
        Avoid using "value" and "key" as 0xFFFF_FFFF (-1).

      @Example
        <code>
        #define MY_KEY1 0x00000001
     
        ...
     
        uint32_t key=MY_KEY1, myPressiousData=72;
      
        DEE_Write(key, myPressiousData);
      
        </code>
     */
uint32_t EERS_Write(uint32_t key, uint32_t value);


    // *****************************************************************************
    /**
      @Function
        uint32_t EERS_Read(uint32_t key) 

      @Summary
        Finds/returns a value matching the "key" specified.

      @Description
        Finds/returns a value matching the "key" specified.
      
      @Precondition
        NVM_Initialize() must be ran before this function.

      @Parameters
        @param key "key" is the unique value that marks the "value" (if it exist).
    
      @Returns
        Returns zero if successful.
        <ul>
          <li>value  Return the "value" found at position "key" 
          <li>0xFFFF_FFFF   Indicates an error ("key" not found), but also the value of int = -1.
        </ul>

      @Remarks
        Avoid using "value" and "key" as 0xFFFF_FFFF (-1) 

      @Example
        <code>
        #define MY_KEY1 0x00000001
     
        ...
     
        uint32_t key=MY_KEY1, myPressiousData;
      
        myPressiousData = EERS_Read(key);
      
        </code>
     */
uint32_t EERS_Read(uint32_t key);


    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EE_REGISTER_STORAGE_H */

/* *****************************************************************************
 End of File
 */
