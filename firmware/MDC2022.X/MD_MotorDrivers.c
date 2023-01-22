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

#include "MD_MotorDrivers.h"


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

// Preset PWM values 
uint16_t MD_HorizSpeed[] = {
    0x1249,
    0x2492,
    0x36DB,
    0x4924,
    0x5B6D,
    0x6DB6,
    0x7FFF
};

uint16_t MD_VertSpeed[] = {
    0x1249,
    0x2492,
    0x36DB,
    0x4924,
    0x5B6D,
    0x6DB6,
    0x7FFF
};

uint16_t MD_FocusSpeed[] = {
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



void MD_setHoriz(int8_t dir, uint16_t speed)
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


void MD_setVert(int8_t dir, uint16_t speed)
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


void MD_setFocus(int8_t dir, uint16_t speed)
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



/* *****************************************************************************
 End of File
 */
