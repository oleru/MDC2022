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
#include "mcc_generated_files/uart1.h"
#include "mcc_generated_files/adc1.h"
#include "mcc_generated_files/usb/../usb/usb_device_cdc.h"
#include "ModbusSlave.h"
#include "MD_MotorDrivers.h"
#include "EE_RegisterStorage.h"
#include "DB_Debounce.h"
#include "protocol.h"



// Version Tags
char VersionTag1[] = __DATE__;     // "Jan 24 2011"
char VersionTag2[] = __TIME__;     // "14:45:20"
char RevBuild[] = "Rev.: 001";

volatile bool TimerEvent10ms;
volatile uint32_t myTime=0;
bool TimerEvent50ms=false, TimerEvent1s=false, TimerEvent15s=false;
uint32_t myLastTime=0;
uint32_t mySystemTimeOutTimer;


/**
    Prototypes
 */
void MCC_USB_CDC_DemoTasks(void);
void MBS_UART_Putch(uint8_t ch);
void UpdateTimers(void);


// Callback function for the ModBus Slave driver
void MBS_UART_Putch(uint8_t ch)
{
    UART1_Write(ch);
}           


void UpdateTimers(void)
{
    static int32_t my50msCnt, my1000msCnt, my15sCnt;
    int32_t delta;
    
    delta = myTime-myLastTime;
    myLastTime = myTime;
    my50msCnt += delta;
    MBS_TimerValue += delta;  // Modbus 10ms Time Keeper
    
    // Update Time Keepers
    while(my50msCnt>=50) {
        TimerEvent50ms = true;
        my50msCnt -= 50;
        my1000msCnt += 50;
        while(my1000msCnt>=1000) {
            TimerEvent1s=true;
            my1000msCnt -= 1000;
            mySystemTimeOutTimer++;
            my15sCnt += 1;
            while(my15sCnt>=15) {
                TimerEvent15s=true;
                my15sCnt -= 15;
            }  //..while(my15sCnt>=15)
        }  //..while(my1000msCnt>=1000)
    }  //..while(my50msCnt>=50)
    
}


/*
    Main application
 */
int main(void)
{
    uint8_t myModBusAddr;
    DB_debounce_struct_t Lamp_ON_Signal;
    bool Lamp_ON_Status=false;
    uint32_t xenonIgniteTime,myXenonIgniteTime=0;
    DB_debounce_struct_t joystick_Right_Signal;
    DB_debounce_struct_t joystick_Left_Signal;
    DB_debounce_struct_t joystick_Down_Signal;
    DB_debounce_struct_t joystick_Up_Signal;
    DB_debounce_struct_t HSP_Signal;
    DB_debounce_struct_t HSN_Signal;
    DB_debounce_struct_t VSP_Signal;
    DB_debounce_struct_t VSN_Signal;
    DB_debounce_struct_t focus_P_Signal;
    DB_debounce_struct_t focus_N_Signal;
    int8_t horizDir=0, vertDir=0, focusDir=0;
    uint32_t i,horizSpeedIndex, vertSpeedIndex, focusSpeedIndex;
    char ch;
    _frameType0x10 portDump;
    uint8_t * buff;
    ADC1_CHANNEL channel;
    _frameType0x11 ADDump;
    _frameType0xFF debugDump;

    
    // initialize the device
    SYSTEM_Initialize();

    // Set Modbus Slave Address
    //myAddr = 10 + (!SW8_GetValue()<<3) | (!SW4_GetValue()<<2) | (!SW2_GetValue() << 1) | (!SW1_GetValue());
    myModBusAddr = 11;
    MBS_InitModbus(myModBusAddr);
    MBS_HoldRegisters[1] = 10 + (!SW8_GetValue()<<3) | (!SW4_GetValue()<<2) | (!SW2_GetValue() << 1) | (!SW1_GetValue());  // OwnID_SW 
    MBS_HoldRegisters[2] = 10;       // OwnID_Prefix 
    MBS_HoldRegisters[3] = (HW_ID_2_RC8_GetValue()<<2) | (HW_ID_1_RC7_GetValue()<<1) | (HW_ID_0_RC6_GetValue());     // HW_ID 
    MBS_HoldRegisters[4] = 0x0100;  // SW_ID 
    
    

    POWER_OK_SetHigh();
/* 
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
        horizSpeedIndex = 5;
        EERS_Write(EE_HORIZ_SPEED_INDEX,horizSpeedIndex);
    }
    vertSpeedIndex = EERS_Read(EE_VERT_SPEED_INDEX);
    if(vertSpeedIndex == 0xFFFFFFFF) {
        vertSpeedIndex = 5;
        EERS_Write(EE_VERT_SPEED_INDEX,vertSpeedIndex);
    }
    focusSpeedIndex = EERS_Read(EE_FOCUS_SPEED_INDEX);
    if(focusSpeedIndex == 0xFFFFFFFF) {
        focusSpeedIndex = 10;  // Full speed...
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
    
    // Init portDump Structure
    portDump.frameType = 0x10;

    // Prepare the ADC
    ADC1_Enable();
    channel = MD_FOCUS_FB_AN4;
    ADC1_ChannelSelect(channel);
    ADC1_SoftwareTriggerEnable();
    ADDump.frameType = 0x11;
    
    // Debug Dump
    debugDump.frameType = 0xFF;
    
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
             
            horizDir=0;
            if(DB_DebounceSignal(JOYSTICK_RIGHT_GetValue(),&joystick_Right_Signal)==false) {
                horizDir=1;
            } 
            if(DB_DebounceSignal(JOYSTICK_LEFT_GetValue(),&joystick_Left_Signal)==false) {
                horizDir=-1;
            }
                
            vertDir=0;
            if(DB_DebounceSignal(JOYSTICK_DOWN_GetValue(),&joystick_Down_Signal)==false) {
                vertDir=1;
            } 
            if(DB_DebounceSignal(JOYSTICK_UP_GetValue(),&joystick_Up_Signal)==false) {
                vertDir=-1;
            }
                
            focusDir=0;
            if(DB_DebounceSignal(FOCUS_P_GetValue(),&focus_P_Signal)==false) {
                focusDir=1;
            } 
            if(DB_DebounceSignal(FOCUS_N_GetValue(),&focus_N_Signal)==false) {
                focusDir=-1;
            }

            DB_DebounceSignal(HORIZ_SPEED_P_GetValue(),&HSP_Signal); 
            DB_DebounceSignal(HORIZ_SPEED_N_GetValue(),&HSN_Signal);
            DB_DebounceSignal(VERT_SPEED_P_GetValue(),&VSP_Signal);
            DB_DebounceSignal(VERT_SPEED_N_GetValue(),&VSN_Signal);
            
            
        }  //..if(my10msTimer)
        
        
        // 50s Timer Event
        if(TimerEvent50ms) {
            TimerEvent50ms = false;

            OP_STATUS_SetLow();
            
            // Update PWM Control
            MD_setHoriz(horizDir*horizSpeedIndex);
            MD_setVert(vertDir*vertSpeedIndex);
            MD_setFocus(focusDir*focusSpeedIndex);
                        
            ADC1_SoftwareTriggerDisable();
            while(!ADC1_IsConversionComplete(channel));
            if(ADC1_IsConversionComplete(channel)) {
 
                switch(channel) {
                    case MD_FOCUS_FB_AN4:
                        ADDump.MD_FOCUS_FB_AN4 = ADC1_ConversionResultGet(channel);
                        channel = MD_VERT_FB_AN5;
                        break;
                    case MD_VERT_FB_AN5:
                        ADDump.MD_VERT_FB_AN5 = ADC1_ConversionResultGet(channel);
                        channel = MD_HORIZ_FB_AN6;
                        break;
                    case MD_HORIZ_FB_AN6:
                        ADDump.MD_HORIZ_FB_AN6 = ADC1_ConversionResultGet(channel);
                        channel = POWER_IN_V_AN7;
                        break;
                    case POWER_IN_V_AN7:
                        ADDump.POWER_IN_V_AN7 = ADC1_ConversionResultGet(channel);
                        channel = FOCUS_POS_AN18;
                        break;
                    case FOCUS_POS_AN18:
                        ADDump.FOCUS_POS_AN18 = ADC1_ConversionResultGet(channel);
                    default:
                        channel = MD_FOCUS_FB_AN4;
                        break;
                }
    
                // Next value
                ADC1_ChannelSelect(channel);
                ADC1_SoftwareTriggerEnable();

/*
                if(UART1_IsTxDone()) {
                    
                    // Send ADDump Telegram
                    ADDump.CS = 0;
                    buff = (uint8_t *)&ADDump; 
                    for(i=0;i<sizeof(_frameType0x11);i++) {
                        if(i<(sizeof(_frameType0x11)-1)) {
                            ADDump.CS ^= buff[i];
                        }    
                        if(buff[i] == FRAME_END) {
                            UART1_Write(FRAME_ESC);
                            UART1_Write(FRAME_ESC_END);
                      } else if(buff[i] == FRAME_ESC) {
                            UART1_Write(FRAME_ESC);
                            UART1_Write(FRAME_ESC_ESC);
                        } else {
                            UART1_Write(buff[i]);
                        }
                    }
                    UART1_Write(FRAME_END);

                }
  */
                
            } 
            
            
        }  //..if(TimerEvent50ms)
        
        
        // 1s Timer Event
        if(TimerEvent1s) {
            TimerEvent1s = false;
            
        }  //..if(TimerEvent1s)

        
        // 1s Timer Event
        if(TimerEvent15s) {
            TimerEvent15s = false;
            
            OP_STATUS_SetHigh();
            
            // Save Changes
            if(horizSpeedIndex!=debugDump.horizSpeedIndex) {
                debugDump.horizSpeedIndex = horizSpeedIndex;
                EERS_Write(EE_HORIZ_SPEED_INDEX,horizSpeedIndex);
            }
            if(vertSpeedIndex!=debugDump.vertSpeedIndex) {
                debugDump.vertSpeedIndex = vertSpeedIndex;
                EERS_Write(EE_VERT_SPEED_INDEX,vertSpeedIndex);
            }
            if(focusSpeedIndex!=debugDump.focusSpeedIndex) {
                debugDump.focusSpeedIndex = focusSpeedIndex;
                EERS_Write(EE_FOCUS_SPEED_INDEX,focusSpeedIndex);
            }

/*            
            // Send Debug data...
            debugDump.CS = 0;
            buff = (uint8_t *)&debugDump; 
            for(i=0;i<sizeof(_frameType0xFF);i++) {
                if(i<(sizeof(_frameType0xFF)-1)) {
                    portDump.CS ^= buff[i];
                }    
                if(buff[i] == FRAME_END) {
                    UART1_Write(FRAME_ESC);
                    UART1_Write(FRAME_ESC_END);
              } else if(buff[i] == FRAME_ESC) {
                    UART1_Write(FRAME_ESC);
                    UART1_Write(FRAME_ESC_ESC);
                } else {
                    UART1_Write(buff[i]);
                }
            }
            UART1_Write(FRAME_END);
*/            
            
        }  //..if(TimerEvent15s)

        
        // Check for incoming data
        if(UART1_IsRxReady()) {
            MBS_ReciveData(UART1_Read());
        }
        
        // Process Modbus
        MBS_ProcessModbus();
        
/*        
        if(UART1_IsTxDone()) {

            // If any activity on ports => send!
            if( (portDump.PortA != (PORTA&0b1111011110110000)) || (portDump.PortB != (PORTB&0b0000000110000010)) || (portDump.PortC != (PORTC&0b0110111111111111)) || (portDump.PortD != (PORTD&0b11101)) ) {

                // Build&Send portDump Telegram
                portDump.PortA = (PORTA&0b1111011110110000);
                portDump.PortB = (PORTB&0b0000000110000010);
                portDump.PortC = (PORTC&0b0110111111111111);
                portDump.PortD = (PORTD&0b11101);
                portDump.CS = 0;
                buff = (uint8_t *)&portDump; 
                for(i=0;i<sizeof(_frameType0x10);i++) {
                    if(i<(sizeof(_frameType0x10)-1)) {
                        portDump.CS ^= buff[i];
                    }    
                    if(buff[i] == FRAME_END) {
                        UART1_Write(FRAME_ESC);
                        UART1_Write(FRAME_ESC_END);
                  } else if(buff[i] == FRAME_ESC) {
                        UART1_Write(FRAME_ESC);
                        UART1_Write(FRAME_ESC_ESC);
                    } else {
                        UART1_Write(buff[i]);
                    }
                }
                UART1_Write(FRAME_END);

            }
        }
*/        
        
        // Lamp_ON
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

        // Handle Horizontal Speed +
        if(!HSP_Signal.handled) {
            HSP_Signal.handled=true;
            if(HSP_Signal.value==false) {
                if(horizSpeedIndex<MD_SPEED_STEPS) {
                    horizSpeedIndex++;                
                }
            }
        }
        
        // Handle Horizontal Speed -
        if(!HSN_Signal.handled) {
            HSN_Signal.handled=true;
            if(HSN_Signal.value==false) {
                if(horizSpeedIndex>1) {
                    horizSpeedIndex--;                
                }
            }
        }
        
        // Handle Vertical Speed +
        if(!VSP_Signal.handled) {
            VSP_Signal.handled=true;
            if(VSP_Signal.value==false) {
                if(vertSpeedIndex<MD_SPEED_STEPS) {
                    vertSpeedIndex++;                
                }
            }
        }
        
        // Handle Vertical Speed -
        if(!VSN_Signal.handled) {
            VSN_Signal.handled=true;
            if(VSN_Signal.value==false) {
                if(vertSpeedIndex>1) {
                    vertSpeedIndex--;                
                }
            }
        }
        

        MCC_USB_CDC_DemoTasks(); 
        
        // Guard the Watchdog
        WDTCONbits.WDTCLRKEY=0x5743;  // Magic sequence to reset WDT
                
        
    }
    return 1; 
}
/**
 End of File
*/

