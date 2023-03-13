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
    2047,
    1884,
    1721,
    1558,
    1395,
    1232,
    1069,
    906,
    743,
    580,
    0,
    580,
    743,
    906,
    1069,
    1232,
    1395,
    1558,
    1721,
    1884,
    2047
};

int32_t MD_VertSpeed[] = {
    2047,
    1884,
    1721,
    1558,
    1395,
    1232,
    1069,
    906,
    743,
    580,
    0,
    580,
    743,
    906,
    1069,
    1232,
    1395,
    1558,
    1721,
    1884,
    2047
};

int32_t MD_FocusSpeed[] = {
    2047,
    1884,
    1721,
    1558,
    1395,
    1232,
    1069,
    906,
    743,
    580,
    0,
    580,
    743,
    906,
    1069,
    1232,
    1395,
    1558,
    1721,
    1884,
    2047
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

    // Smooth change filter 
    if(abs(newDirAndSpeed-dirAndSpeed)>MD_MAX_CHANGE_VALUE) {
        if(newDirAndSpeed<dirAndSpeed) {
            newDirAndSpeed += MD_MAX_CHANGE_VALUE;
        } else {
            newDirAndSpeed -= MD_MAX_CHANGE_VALUE;
        }
    } else {
        newDirAndSpeed = dirAndSpeed;
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

    // Smooth change filter 
    if(abs(newDirAndSpeed-dirAndSpeed)>MD_MAX_CHANGE_VALUE) {
        if(newDirAndSpeed<dirAndSpeed) {
            newDirAndSpeed += MD_MAX_CHANGE_VALUE;
        } else {
            newDirAndSpeed -= MD_MAX_CHANGE_VALUE;
        }
    } else {
        newDirAndSpeed = dirAndSpeed;
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

    // Smooth change filter 
    if(abs(newDirAndSpeed-dirAndSpeed)>MD_MAX_CHANGE_VALUE) {
        if(newDirAndSpeed<dirAndSpeed) {
            newDirAndSpeed += MD_MAX_CHANGE_VALUE;
        } else {
            newDirAndSpeed -= MD_MAX_CHANGE_VALUE;
        }
    } else {
        newDirAndSpeed = dirAndSpeed;
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
        mySpeed = MD_HorizSpeed[MD_SPEED_STEPS+dirAndIndex]; 
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
        mySpeed = MD_VertSpeed[MD_SPEED_STEPS+dirAndIndex]; 
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
        mySpeed = MD_FocusSpeed[MD_SPEED_STEPS+dirAndIndex]; 
    }
    if(dirAndIndex<0) {
        mySpeed = -mySpeed;
    }
        
    MD_setFocusPWM(mySpeed);

}



/* *****************************************************************************
 End of File
 */
