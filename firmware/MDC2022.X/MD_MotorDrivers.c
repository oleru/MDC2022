/* ************************************************************************** */
/** MD_MotorDrivers

  @Company
 Torka AS

  @File Name
    MD_MotorDrivers.h

  @Summary
    Shared file hold Motor Control Drivers for the MDC2022.

  @Description
    Shared file hold Motor Control Drivers for the MDC2022.
     
    Horiz. and Vert. control each MC34931EK H-Bridge chips driven with 
    two PWM channels. Driver Power are read back on FB-analog signal where
    the output is 0.24% of total load (voltage on pin is A*270).
    !SF connected to a red led.    

    Focus is connected to a DRV8876RGTR half H-Bridge driven with on 
    Direction bit and one PWM channel. Driver Power are read back on 
    FB-analog signal where the output is 0.24% of total load 
    (voltage on pin is A*270). NFAULT connected to a red led.  
 */
/* ************************************************************************** */


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include <stdint.h>
#include <stdlib.h>
#include "MD_MotorDrivers.h"


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

// Preset PWM values 
int32_t MD_HorizSpeed[] = {
    0,
    0x1249,
    0x2492,
    0x36DB,
    0x4924,
    0x5B6D,
    0x6DB6,
    0x7FFF
};

int32_t MD_VertSpeed[] = {
    0,
    0x1249,
    0x2492,
    0x36DB,
    0x4924,
    0x5B6D,
    0x6DB6,
    0x7FFF
};

int32_t MD_FocusSpeed[] = {
    0,
    0x1249,
    0x2492,
    0x36DB,
    0x4924,
    0x5B6D,
    0x6DB6,
    0x7FFF
};


/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */



/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */


void MD_setHorizPWM(int32_t dirAndSpeed)
{
    static int32_t newDirAndSpeed;

    // Leaky integrator (a=0.1)
    if(dirAndSpeed!=0) {
        newDirAndSpeed = 0.9*newDirAndSpeed + 0.1*dirAndSpeed;
    } else {
        newDirAndSpeed = 0.25*newDirAndSpeed;
        if(abs(newDirAndSpeed)<100) {
            newDirAndSpeed = 0;
        }    
    }    

    // Set PWM channel depending on the Direction
    if(newDirAndSpeed>=0) {  // CW

        // Then set Speed
        MCCP1_COMPARE_DualEdgeBufferedConfig( 0, newDirAndSpeed );        
        SCCP5_COMPARE_DualEdgeBufferedConfig( 0, 0 );
        
    } else {  // CCW

        // Then set Speed
        MCCP1_COMPARE_DualEdgeBufferedConfig( 0, 0 );        
        SCCP5_COMPARE_DualEdgeBufferedConfig( 0, abs(newDirAndSpeed) );

    }

}


void MD_setVertPWM(int32_t dirAndSpeed)
{
    static int32_t newDirAndSpeed;

    // Leaky integrator (a=0.1)
    if(dirAndSpeed!=0) {
        newDirAndSpeed = 0.9*newDirAndSpeed + 0.1*dirAndSpeed;
    } else {
        newDirAndSpeed = 0.25*newDirAndSpeed;
        if(abs(newDirAndSpeed)<100) {
            newDirAndSpeed = 0;
        }    
    }    

    // Set PWM channel depending on the Direction
    if(newDirAndSpeed>=0) {  // CW

        // Then set Speed
        MCCP3_COMPARE_DualEdgeBufferedConfig( 0, newDirAndSpeed );        
        SCCP4_COMPARE_DualEdgeBufferedConfig( 0, 0 );
        
    } else {  // CCW

        // Then set Speed
        MCCP3_COMPARE_DualEdgeBufferedConfig( 0, 0 );        
        SCCP4_COMPARE_DualEdgeBufferedConfig( 0, abs(newDirAndSpeed) );

    }
    
}


void MD_setFocusPWM(int32_t dirAndSpeed)
{
    static int32_t newDirAndSpeed;

    // Leaky integrator (a=0.1)
    if(dirAndSpeed!=0) {
        newDirAndSpeed = 0.9*newDirAndSpeed + 0.1*dirAndSpeed;
    } else {
        newDirAndSpeed = 0.25*newDirAndSpeed;
        if(abs(newDirAndSpeed)<100) {
            newDirAndSpeed = 0;
        }    
    }    

    // First set Direction
    if(newDirAndSpeed<0) {
        MD_FOCUS_DIR_SetHigh();
    } else {
        MD_FOCUS_DIR_SetLow();
    }
    
    // Then set Speed
    MCCP2_COMPARE_DualEdgeBufferedConfig( 0, abs(newDirAndSpeed) );
    
}


void MD_setHoriz(int8_t dirAndIndex)
{
    int32_t mySpeed = 0;

    if(abs(dirAndIndex)<=MD_SPEED_STEPS) {
        mySpeed = MD_HorizSpeed[abs(dirAndIndex)]; 
    }
    if(dirAndIndex<0) {
        mySpeed = -mySpeed;
    }
    
    MD_setHorizPWM(mySpeed);

}


void MD_setVert(int8_t dirAndIndex)
{
    int32_t mySpeed = 0;
    
    if(abs(dirAndIndex)<=MD_SPEED_STEPS) {
        mySpeed = MD_VertSpeed[abs(dirAndIndex)]; 
    }
    if(dirAndIndex<0) {
        mySpeed = -mySpeed;
    }
    
    MD_setVertPWM(mySpeed);
 
}


void MD_setFocus(int8_t dirAndIndex)
{
    int32_t mySpeed = 0;
    
    if(abs(dirAndIndex)<=MD_SPEED_STEPS) {
        mySpeed = MD_FocusSpeed[abs(dirAndIndex)]; 
    }
    if(dirAndIndex<0) {
        mySpeed = -mySpeed;
    }
        
    MD_setFocusPWM(mySpeed);

}



/* *****************************************************************************
 End of File
 */
