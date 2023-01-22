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

#ifndef _MD_MOTOR_DRIVERS_H    /* Guard against multiple inclusion */
#define _MD_MOTOR_DRIVERS_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include <stdint.h>
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/mccp1_compare.h"
#include "mcc_generated_files/mccp2_compare.h"
#include "mcc_generated_files/mccp3_compare.h"
#include "mcc_generated_files/sccp4_compare.h"
#include "mcc_generated_files/sccp5_compare.h"


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif


    /* ************************************************************************** */
    /* ************************************************************************** */
    /* Section: Constants                                                         */
    /* ************************************************************************** */
    /* ************************************************************************** */

#define CW 1
#define CCW -1
    

    // *****************************************************************************
    // *****************************************************************************
    // Section: Data Types
    // *****************************************************************************
    // *****************************************************************************


    // *****************************************************************************
    // *****************************************************************************
    // Section: Interface Functions
    // *****************************************************************************
    // *****************************************************************************

    
/**
  @Function
    void MD_setHoriz(int8_t dir, uint16_t speed) 

  @Summary
    Sets the Direction and Speed for the Motor Driver driving the Horizontal Motor.
 
  @Description
    This routine Sets the Direction and Speed for the Motor Driver driving the Horizontal Motor.

  @Preconditions
    SYSTEM_Initialize() function should have been called 

  @Param
    dir - -1=CCW and !-1(any other value than -1)=CW.
    speed - set the PWM for the H-Bridge [0-7FFF] = 0-100% speed.	
	
  @Returns
    None.

  @Example 
    <code>
        int8_t dir = CW;
        uint16_t speed = 0x2000;
        void setHorizMD( dir, speed );
    <code>
*/
void MD_setHoriz(int8_t dir, uint16_t speed);


/**
  @Function
    void MD_setVert(int8_t dir, uint16_t speed) 

  @Summary
    Sets the Direction and Speed for the Motor Driver driving the Vertical Motor.
 
  @Description
    This routine Sets the Direction and Speed for the Motor Driver driving the Vertical Motor.

  @Preconditions
    SYSTEM_Initialize() function should have been called 

  @Param
    dir - -1=CCW and !-1(any other value than -1)=CW.
    speed - set the PWM for the H-Bridge [0-7FFF] = 0-100% speed.	
	
  @Returns
    None.

  @Example 
    <code>
        int8_t dir = CW;
        uint16_t speed = 0x2000;
        void setVertMD( dir, speed );
    <code>
*/
void MD_setVert(int8_t dir, uint16_t speed);


/**
  @Function
    void MD_setFocus(int8_t dir, uint16_t speed) 

  @Summary
    Sets the Direction and Speed for the Motor Driver driving the Focus Motor.
 
  @Description
    This routine Sets the Direction and Speed for the Motor Driver driving the Focus Motor.

  @Preconditions
    SYSTEM_Initialize() function should have been called 

  @Param
    dir - -1=CCW and !0(any other value than -1)=CW.
    speed - set the PWM for the H-Bridge [0-7FFF] = 0-100% speed.	
	
  @Returns
    None.

  @Example 
    <code>
        int8_t dir = CW;
        uint16_t speed = 0x2000;
        void setFocusMD( dir, speed );
    <code>
*/
void MD_setFocus(int8_t dir, uint16_t speed);



    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _MD_MOTOR_DRIVERS_H */

/* *****************************************************************************
 End of File
 */
