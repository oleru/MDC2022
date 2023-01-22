/* ************************************************************************** */
/** Data EEPROM Emulation - Register Storage

  @Company
    Torka as

  @File Name
    EE_RegisterStorage.c

  @Summary
    Brief description of the file.

  @Description
    This C source file contains functions implementations to emulate EEPROM.
    It is a ported version for PIC32MM based on Microchip NVM functions and 
    the dee.h/.c project.
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include "EE_RegisterStorage.h"

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */


/* ************************************************************************** */
/** DEE_DATA dee

  @Summary
    place "dee" in Flash memory.
    
 */
//NVM_ALLOCATE(_FLASH_BASE, FLASH_PAGE_SIZE_BYTES, FLASH_SIZE_BYTES); // allocate NVM flash logging buffer, aligned on erasable page
//void* FLASH_BASE = &_FLASH_BASE; //pointer to the array of data in flash... to be used for the write page function
DEE_DATA __attribute__((aligned(FLASH_PAGE_SIZE_BYTES), address(__KSEG0_PROGRAM_MEM_BASE+0x0000F000ul), persistent)) dee[NUM_PAGES_TO_USE][MAX_NUMBER_REGISTERS];
// DEE_DATA dee[NUM_PAGES_TO_USE][MAX_NUMBER_REGISTERS] CACHE_ALIGN;

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */

/* ************************************************************************** */

/* ************************************************************************** */
/** 
  @Function
    uint32_t DEE_BankErase(uint32_t bank) 

  @Summary
    Wrapper function of NVM_ErasePage( address ).
  
   @Param
    bank - 0/1 physical location from flash memory.

*/
uint32_t DEE_BankErase(uint32_t bank) {
    uint32_t address; // = APP_FLASH_ADDRESS + (bank*FLASH_PAGE_SIZE_BYTES);
    
    address = APP_FLASH_ADDRESS + (bank*FLASH_PAGE_SIZE_BYTES);

    // Program Valid Key for NVM Commands
    FLASH_Unlock(FLASH_UNLOCK_KEY);

    // Erase the page of flash at this address
    if(!FLASH_ErasePage(address))
        printf("EreasePage Fauilure\r\n");

    // Clear Key for NVM Commands so accidental call to flash routines will not corrupt flash
    FLASH_Lock();

    return 0;
}

/* ************************************************************************** */
/** 
  @Function
    uint32_t DEE_DataProgram(uint32_t bank, uint32_t offset, uint32_t key, uint32_t value) 

  @Summary
    Write "key" and "value" to the Flash.
  
   @Param
    bank - 0/1 physical location in flash memory.

    offset - where in the "bank" to store new "key"'s "value".

    key - The "key".

    value - The "value".

*/
uint32_t DEE_DataProgram(uint32_t bank, uint32_t offset, uint32_t key, uint32_t value)
{
    uint32_t *address = (uint32_t *)&dee[bank][offset].key;
    uintptr_t myAddress = (uintptr_t)address;
    
    // Program Valid Key for NVM Commands
    FLASH_Unlock(FLASH_UNLOCK_KEY);

    // Store New ID
    if(!FLASH_WriteDoubleWord(myAddress,key,value))
        printf("\r\nFlash Write Fauilure\r\n");

    // Clear Key for NVM Commands so accidental call to flash routines will not corrupt flash
    FLASH_Lock();

    return 0;
}

/* ************************************************************************** */
/** 
  @Function
    uint32_t DEE_GetBank(void) 

  @Summary
    Find Active "Bank".
 */
uint32_t DEE_GetBank(void)
{
    if(dee[0][0].key == 0xFFFFFFFF) {
        if(dee[1][0].key == 0xFFFFFFFF) {
            return 0;
        } else {
            return 1;
        }    
    }
    return 0;
}

/* ************************************************************************** */
/** 
  @Function
    uint32_t DEE_AddData(uint32_t bank, uint32_t key, uint32_t value, uint32_t skip) 

  @Summary
    Write "key" and "value" to the Flash.
  
   @Param
    bank - 0/1 physical location in flash memory.

    offset - where in the "bank" to store new "key"'s "value".

    key - The "key".

    value - The "value".

*/
uint32_t DEE_AddData(uint32_t bank, uint32_t key, uint32_t value, uint32_t skip)
{
    int32_t offset=0;

    while(offset < MAX_NUMBER_REGISTERS) {

        if(skip) {        
            if(dee[bank][offset].key == key) {
                return 0;
            }
        }
        if(dee[bank][offset].key == 0xFFFFFFFF) {
            return DEE_DataProgram(bank, offset, key, value);
        }
        offset++;    
    }
    return 1;
}

/* ************************************************************************** */
/** 
  @Function
    uint32_t DEE_SwitchBank(void) 

  @Summary
    Full, do the "SwitchBank"-limbo.
 */
uint32_t DEE_SwitchBank(void) {
    uint32_t current_bank;
    uint32_t new_bank;
    int32_t offset;
    uint32_t key;
    uint32_t value;

    current_bank = DEE_GetBank();
    if(current_bank) {
        new_bank = 0;
    } else {
        new_bank = 1;
    }   

    offset = MAX_NUMBER_REGISTERS-1;

    do {
        key = dee[current_bank][offset].key;
        if(key != 0xFFFFFFFF) {
            value = dee[current_bank][offset].value;
            if(DEE_AddData(new_bank, key, value, 1))           
                return 1;
        }
        offset--;
    } while(offset >= 0);

    return DEE_BankErase(current_bank);
}


/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */


// *****************************************************************************
/** 
  @Function
    void EERS_Init(void) 
 */
void EERS_Init(void)
{
/*
    NVM_CallbackRegister(eventHandler, (uintptr_t)NULL);
        
    if((NVMCON & _NVMCON_PFSWAP_MASK) != _NVMCON_PFSWAP_MASK) {
        NVM_Read((uint32_t *)dee, sizeof(dee), APP_FLASH_ADDRESS_I);
    } else {    
        NVM_Read((uint32_t *)dee, sizeof(dee), APP_FLASH_ADDRESS_II);
    }
*/        
}


// *****************************************************************************
/** 
  @Function
    uint32_t EERS_Write(uint32_t key, uint32_t value) 
 */
uint32_t EERS_Write(uint32_t key, uint32_t value)
{
    uint32_t bank, oldValue;

    // First check if value actually have changed...
    oldValue = EERS_Read(key);
    if(oldValue==value)
        return 0;

    bank = DEE_GetBank();
    if(DEE_AddData(bank, key, value, 0)) {
        if(DEE_SwitchBank())
            return 1;
        bank = DEE_GetBank();
        if(DEE_AddData(bank, key, value, 0))
            return 1;
    }

    return 0;
}

// *****************************************************************************
/** 
  @Function
    uint32_t EERS_Read(uint32_t key) 
 */
uint32_t EERS_Read(uint32_t key) {
    uint32_t bank;
    int32_t offset;

    bank = DEE_GetBank();
    offset = MAX_NUMBER_REGISTERS-1;

    do {
        if(dee[bank][offset].key == key) {
            return dee[bank][offset].value;
        }
        offset--;
    } while(offset >= 0);

    return 0xFFFFFFFF;
}


/* *****************************************************************************
 End of File
 */
