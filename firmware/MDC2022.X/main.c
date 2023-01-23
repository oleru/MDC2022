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
#include "mcc_generated_files/coretimer.h"
#include "MD_MotorDrivers.h"
#include "EE_RegisterStorage.h"
#include "DB_Debounce.h"



/**
    Prototypes
 */
void UpdateTimers(void);


// Version Tags
char VersionTag1[] = __DATE__;     // "Jan 24 2011"
char VersionTag2[] = __TIME__;     // "14:45:20"
char RevBuild[] = "Rev.: 001";

volatile bool TimerEvent10ms;
volatile uint32_t myTime=0;
bool TimerEvent50ms=false, TimerEvent1s=false;
uint32_t myLastTime=0;



void UpdateTimers(void)
{
    static int32_t my50msCnt, my1000msCnt;
    int32_t delta;
    
    delta = myTime-myLastTime;
    myLastTime = myTime;
    my50msCnt += delta;
    
    // Update Time Keepers
    while(my50msCnt>=50) {
        TimerEvent50ms = true;
        my50msCnt -= 50;
        my1000msCnt += 50;
        while(my1000msCnt>=1000) {
            TimerEvent1s=true;
            my1000msCnt -= 1000; 
        }  //..while(my250msCnt>=250)
    }  //..while(my50msCnt>=50)
    
}


/*
    Main application
 */
int main(void)
{
    uint32_t horizSpeedIndex, vertSpeedIndex, focusSpeedIndex;
    uint32_t xenonIgniteTime,myXenonIgniteTime=0;
    DB_debounce_struct_t Lamp_ON_Signal;
    bool Lamp_ON_Status=false;
    
    
    // initialize the device
    SYSTEM_Initialize();

/*
    POWER_OK_SetHigh();
    OP_STATUS_SetHigh();
    FOCUS_END_IN_LED_SetHigh();
    FOCUS_END_OUT_LED_SetHigh();
    HIGH_TEMP_LED_SetHigh();
*/    
    // Initialize off
    RLY_IGNITE_ON_SetLow();
    RLY_LAMP_ON_SetLow();
    RLY_STEP_SetLow();

    
    // Read out Persistent variables
    horizSpeedIndex = EERS_Read(EE_HORIZ_SPEED_INDEX);
    if(horizSpeedIndex == 0xFFFFFFFF) {
        horizSpeedIndex = 3;
        EERS_Write(EE_HORIZ_SPEED_INDEX,horizSpeedIndex);
    }
    vertSpeedIndex = EERS_Read(EE_VERT_SPEED_INDEX);
    if(vertSpeedIndex == 0xFFFFFFFF) {
        vertSpeedIndex = 3;
        EERS_Write(EE_VERT_SPEED_INDEX,vertSpeedIndex);
    }
    focusSpeedIndex = EERS_Read(EE_FOCUS_SPEED_INDEX);
    if(focusSpeedIndex == 0xFFFFFFFF) {
        focusSpeedIndex = 6;  // Full speed...
        EERS_Write(EE_FOCUS_SPEED_INDEX,focusSpeedIndex);
    }
    xenonIgniteTime = EERS_Read(EE_XENON_IGNITE_TIME);
    if(xenonIgniteTime == 0xFFFFFFFF) {
        xenonIgniteTime = 300;  // [ms]
        EERS_Write(EE_XENON_IGNITE_TIME,xenonIgniteTime);
    } 

    
    // Init. all Motor Driver in lock - no motion
    MD_setHoriz(0);
    MD_setVert(0);
    MD_setFocus(0);
   
    // Enable WDT
    WDTCONbits.ON=1;
    
    
    // Main loop
    while (1)
    {
        
    
        // 10ms Timer Event
        if(TimerEvent10ms) {
            TimerEvent10ms = false;
            UpdateTimers();

            DB_DebounceSignal(LAMP_ON_GetValue(),&Lamp_ON_Signal);

            // Lamp_ON_Ignition timer running?
            if(myXenonIgniteTime!=0) {
                if(myXenonIgniteTime>20) {
                    myXenonIgniteTime-=10;
                } else {
                    RLY_IGNITE_ON_SetLow();
                    myXenonIgniteTime=0;
                }
            }
            
            
        }  //..if(my10msTimer)
        
        
        // 50s Timer Event
        if(TimerEvent50ms) {
            TimerEvent50ms = false;

            OP_STATUS_SetLow();

        }  //..if(TimerEvent50ms)
        
        
        // 1s Timer Event
        if(TimerEvent1s) {
            TimerEvent1s = false;
            
            OP_STATUS_SetHigh();
            
        }  //..if(TimerEvent1s)
    

        // Lamp_ON pressed and handled?
        if(!Lamp_ON_Signal.handled) {
            Lamp_ON_Signal.handled = true;
            if(Lamp_ON_Signal.value==false) {
                if(!Lamp_ON_Status) {
                    Lamp_ON_Status=true;
                    RLY_LAMP_ON_SetHigh();
                    RLY_IGNITE_ON_SetHigh();
                    myXenonIgniteTime = xenonIgniteTime+10;
                } else {
                    Lamp_ON_Status=false;
                    RLY_LAMP_ON_SetLow();
                    RLY_IGNITE_ON_SetLow();
                }
            }
        }

        
        
        // Guard the Watchdog
        WDTCONbits.WDTCLRKEY=0x5743;  // Magic sequence to reset WDT
                
        
    }
    return 1; 
}
/**
 End of File
*/

