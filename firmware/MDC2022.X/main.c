/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
    main.c

  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.171.1
        Device            :  PIC32MM0256GPM064
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.70
        MPLAB 	          :  MPLAB X v5.50
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Defines
*/




/**
  Section: Included Files
*/
#include <stdint.h>

#include "mcc_generated_files/system.h"
#include "mcc_generated_files/pin_manager.h"
#include "MD_MotorDrivers.h"
#include "EE_RegisterStorage.h"



/**
    Prototypes
 */
void MD_setHoriz(int8_t dir, uint16_t speed);
void MD_setVert(int8_t dir, uint16_t speed);
void MD_setFocus(int8_t dir, uint16_t speed);



// Version Tags
char VersionTag1[] = __DATE__;     // "Jan 24 2011"
char VersionTag2[] = __TIME__;     // "14:45:20"
char RevBuild[] = "Rev.: 001";



/*
    Main application
 */
int main(void)
{
    uint32_t horizSpeedIndex, vertSpeedIndex, focusSpeedIndex;
    
    
    // initialize the device
    SYSTEM_Initialize();

/*
    POWER_OK_SetHigh();
    OP_STATUS_SetHigh();
    FOCUS_END_IN_LED_SetHigh();
    FOCUS_END_OUT_LED_SetHigh();
    HIGH_TEMP_LED_SetHigh();
    
    RLY_IGNITE_ON_SetLow();
    RLY_LAMP_ON_SetLow();
    RLY_STEP_SetLow();
*/
    
    // Read out Persistent variables
    horizSpeedIndex = EERS_Read(HORIZ_SPEED_INDEX);
    if(horizSpeedIndex == 0xFFFFFFFF) {
        horizSpeedIndex = 3;
        EERS_Write(HORIZ_SPEED_INDEX,horizSpeedIndex);
    }
    vertSpeedIndex = EERS_Read(VERT_SPEED_INDEX);
    if(vertSpeedIndex == 0xFFFFFFFF) {
        vertSpeedIndex = 3;
        EERS_Write(VERT_SPEED_INDEX,vertSpeedIndex);
    }
    focusSpeedIndex = EERS_Read(FOCUS_SPEED_INDEX);
    if(focusSpeedIndex == 0xFFFFFFFF) {
        focusSpeedIndex = 6;  // Full speed...
        EERS_Write(FOCUS_SPEED_INDEX,focusSpeedIndex);
    }

    
    // Init. all Motor Driver in lock - no motion
    MD_setHoriz( CCW, 0);
    MD_setVert( CCW, 0);
    MD_setFocus( CCW, 0);
    
    
    // Main loop
    while (1)
    {
        // Add your application code
        if(LAMP_ON_GetValue()==1)
            POWER_OK_SetHigh();
        else
            POWER_OK_SetLow();
        
    }
    return 1; 
}
/**
 End of File
*/

