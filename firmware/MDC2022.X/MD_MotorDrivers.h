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
    FB-analog signal where the output is 0.1% of total load 
    (voltage on pin is A*2490). NFAULT connected to a red led.  
 */
/* ************************************************************************** */

#ifndef _MD_MOTOR_DRIVERS_H    /* Guard against multiple inclusion */
#define _MD_MOTOR_DRIVERS_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

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

#define MD_CW 1
#define MD_CCW -1
#define MD_STOP -1
#define MD_SPEED_STEPS 7
    

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
    void MD_setHorizPWM(uint32_t dirAndSpeed) 

  @Summary
    Sets the Direction and Speed for the Motor Driver driving the Horizontal Motor.
 
  @Description
    This routine Sets the Direction and Speed for the Motor Driver driving the Horizontal Motor.

  @Preconditions
    SYSTEM_Initialize() function should have been called 

  @Param
    dirAndSpeed - Values < 0 gives CCW direction, Values > gives CW direction. 0 = Stop.
                  Absolute value set the PWM for the H-Bridge [0-7FFF] = 0-100% speed.	
	
  @Returns
    None.

  @Example 
    <code>
        uint32_t dirAndSpeed = -4000;  // CCW, Medium Speed 
        MD_setHorizPWM( dirAndSpeed );
    <code>
*/
void MD_setHorizPWM(int32_t dirAndSpeed);


/**
  @Function
    void MD_setVertPWM(uint32_t dirAndSpeed) 

  @Summary
    Sets the Direction and Speed for the Motor Driver driving the Vertical Motor.
 
  @Description
    This routine Sets the Direction and Speed for the Motor Driver driving the Vertical Motor.

  @Preconditions
    SYSTEM_Initialize() function should have been called 

  @Param
    dirAndSpeed - Values < 0 gives CCW direction, Values > gives CW direction. 0 = Stop.
                  Absolute value set the PWM for the H-Bridge [0-7FFF] = 0-100% speed.	
	
  @Returns
    None.

  @Example 
    <code>
        uint32_t dirAndSpeed = -4000;  // CCW, Medium Speed 
        MD_setVertPWM( dirAndSpeed );
    <code>
*/
void MD_setVertPWM(int32_t dirAndSpeed);


/**
  @Function
    void MD_setFocusPWM(uint32_t dirAndSpeed) 

  @Summary
    Sets the Direction and Speed for the Motor Driver driving the Focus Motor.
 
  @Description
    This routine Sets the Direction and Speed for the Motor Driver driving the Focus Motor.

  @Preconditions
    SYSTEM_Initialize() function should have been called 

  @Param
    dirAndSpeed - Values < 0 gives CCW direction, Values > gives CW direction. 0 = Stop.
                  Absolute value set the PWM for the H-Bridge [0-7FFF] = 0-100% speed.	
	
  @Returns
    None.

  @Example 
    <code>
        uint32_t dirAndSpeed = -4000;  // CCW, Medium Speed 
        MD_setFocusPWM( dirAndSpeed );
    <code>
*/
void MD_setFocusPWM(int32_t dirAndSpeed);


/**
  @Function
    void MD_setHoriz(int8_t dirAndIndex) 

  @Summary
    Wrapper function for the MD_setHorizPWM where the speed is selected from a preset table. 
 
  @Description
    Wrapper function for the MD_setHorizPWM where the speed is selected from a preset table.

  @Preconditions
    SYSTEM_Initialize() function should have been called 

  @Param
    dirAndIndex - Values < 0 gives CCW direction, Values > gives CW direction. 0 = Stop.
                  Absolute value set the index for the used table MD_HorizSpeed[] for the speed set.	
	
  @Returns
    None.

  @Example 
    <code>
        int8_t dirAndIndex = -7;
        MD_setHoriz( dirAndIndex );
    <code>
*/
void MD_setHoriz(int8_t dirAndIndex);


/**
  @Function
    void MD_setVert(int8_t dirAndIndex) 

  @Summary
    Wrapper function for the MD_setVertPWM where the speed is selected from a preset table. 
 
  @Description
    Wrapper function for the MD_setVertPWM where the speed is selected from a preset table.

  @Preconditions
    SYSTEM_Initialize() function should have been called 

  @Param
    dirAndIndex - Values < 0 gives CCW direction, Values > gives CW direction. 0 = Stop.
                  Absolute value set the index for the used table MD_VertSpeed[] for the speed set.	
	
  @Returns
    None.

  @Example 
    <code>
        int8_t dirAndIndex = -7;
        MD_setVert( dirAndIndex );
    <code>
*/
void MD_setVert(int8_t dirAndIndex);


/**
  @Function
    void MD_setFocus(int8_t dirAndIndex) 

  @Summary
    Wrapper function for the MD_setFocusPWM where the speed is selected from a preset table. 
 
  @Description
    Wrapper function for the MD_setFocusPWM where the speed is selected from a preset table.

  @Preconditions
    SYSTEM_Initialize() function should have been called 

  @Param
    dirAndIndex - Values < 0 gives CCW direction, Values > gives CW direction. 0 = Stop.
                  Absolute value set the index for the used table MD_FocusSpeed[] for the speed set.	
	
  @Returns
    None.

  @Example 
    <code>
        int8_t dirAndIndex = -7;
        MD_setFocus( dirAndIndex );
    <code>
*/
void MD_setFocus(int8_t dirAndIndex);



    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _MD_MOTOR_DRIVERS_H */

/* *****************************************************************************
 End of File
 */
