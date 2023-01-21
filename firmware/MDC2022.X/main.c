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

#define CW 1
#define CCW 0

/**
  Section: Included Files
*/
#include <stdint.h>

#include "mcc_generated_files/system.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/mccp1_compare.h"
#include "mcc_generated_files/mccp2_compare.h"
#include "mcc_generated_files/mccp3_compare.h"
#include "mcc_generated_files/sccp4_compare.h"
#include "mcc_generated_files/sccp5_compare.h"


/*
    Prototypes
 */
void setHorizMD(uint8_t dir, uint16_t speed);
void setVertMD(uint8_t dir, uint16_t speed);
void setFocusMD(uint8_t dir, uint16_t speed);



/**
  @Summary
    Sets the Direction and Speed for the Motor Driver driving the Horizontal Motor.
 
  @Description
    This routine Sets the Direction and Speed for the Motor Driver driving the Horizontal Motor.

  @Preconditions
    SYSTEM_Initialize() function should have been called 

  @Param
    dir - 0=CCW and !0(any other value than 0)=CW.
    speed - set the PWM for the H-Bridge [0-7FFF] = 0-100% speed.	
	
  @Returns
    None.

  @Example 
    <code>
        uint8_t dir = 0x1000;
        uint16_t speed = 0x2000;
        void setHorizMD( dir, speed );
    <code>
*/
void setHorizMD(uint8_t dir, uint16_t speed)
{
    // Set PWM channel depending on the Direction
    if(dir!=CCW) {  // CW

        // Then set Speed
        MCCP1_COMPARE_DualEdgeBufferedConfig( 0, speed );        
        SCCP5_COMPARE_DualEdgeBufferedConfig( 0, 0 );
        
    } else {  // CCW

        // Then set Speed
        MCCP1_COMPARE_DualEdgeBufferedConfig( 0, 0 );        
        SCCP5_COMPARE_DualEdgeBufferedConfig( 0, speed );

    }

}


/**
  @Summary
    Sets the Direction and Speed for the Motor Driver driving the Vertical Motor.
 
  @Description
    This routine Sets the Direction and Speed for the Motor Driver driving the Vertical Motor.

  @Preconditions
    SYSTEM_Initialize() function should have been called 

  @Param
    dir - 0=CCW and !0(any other value than 0)=CW.
    speed - set the PWM for the H-Bridge [0-7FFF] = 0-100% speed.	
	
  @Returns
    None.

  @Example 
    <code>
        uint8_t dir = 0x1000;
        uint16_t speed = 0x2000;
        void setVertMD( dir, speed );
    <code>
*/
void setVertMD(uint8_t dir, uint16_t speed)
{
    // Set PWM channel depending on the Direction
    if(dir!=CCW) {  // CW

        // Then set Speed
        MCCP3_COMPARE_DualEdgeBufferedConfig( 0, speed );        
        SCCP4_COMPARE_DualEdgeBufferedConfig( 0, 0 );
        
    } else {  // CCW

        // Then set Speed
        MCCP3_COMPARE_DualEdgeBufferedConfig( 0, 0 );        
        SCCP4_COMPARE_DualEdgeBufferedConfig( 0, speed );

    }

}


/**
  @Summary
    Sets the Direction and Speed for the Motor Driver driving the Focus Motor.
 
  @Description
    This routine Sets the Direction and Speed for the Motor Driver driving the Focus Motor.

  @Preconditions
    SYSTEM_Initialize() function should have been called 

  @Param
    dir - 0=CCW and !0(any other value than 0)=CW.
    speed - set the PWM for the H-Bridge [0-7FFF] = 0-100% speed.	
	
  @Returns
    None.

  @Example 
    <code>
        uint8_t dir = 0x1000;
        uint16_t speed = 0x2000;
        void setFocusMD( dir, speed );
    <code>
*/
void setFocusMD(uint8_t dir, uint16_t speed)
{
    // First set Direction
    if(dir!=CCW) {
        MD_FOCUS_DIR_SetHigh();
    } else {
        MD_FOCUS_DIR_SetLow();
    }
    
    // Then set Speed
    MCCP2_COMPARE_DualEdgeBufferedConfig( 0, speed );
        
}


/*
    Main application
 */
int main(void)
{
    
    // initialize the device
    SYSTEM_Initialize();

    POWER_OK_SetHigh();
    OP_STATUS_SetHigh();
    FOCUS_END_IN_LED_SetHigh();
    FOCUS_END_OUT_LED_SetHigh();
    HIGH_TEMP_LED_SetHigh();
    
    RLY_IGNITE_ON_SetLow();
    RLY_LAMP_ON_SetLow();
    RLY_STEP_SetLow();

    // Sett alt til 0
    setHorizMD( CCW, 0);
    setVertMD( CCW, 0);
    setFocusMD( CCW, 0);
    
    
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

