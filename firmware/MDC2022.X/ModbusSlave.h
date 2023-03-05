/* ************************************************************************** */
/** Modbus Slave Library

  @Company
    Torka AS

  @File Name
    ModbusSlave.h

  @Summary
    Modbus RTU Slave library.

  @Description
    Small - cut to the bone - Modbus RTU Slave library. 
    Support Modbus Functions 3, 6 and 16.

 */
/* ************************************************************************** */

#ifndef _MODBUS_SLAVE_H    /* Guard against multiple inclusion */
#define _MODBUS_SLAVE_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include <stdint.h>
#include <stdbool.h>


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif


    /* ************************************************************************** */
    /* ************************************************************************** */
    /* Section: Constants                                                         */
    /* ************************************************************************** */
    /* ************************************************************************** */

    
    /* ************************************************************************** */
    /** Modbus RTU Slave Output Register Number
     */
#define MBS_NUMBER_OF_OUTPUT_REGISTERS      100

    /* ************************************************************************** */
    /** Modbus RTU Slave Addresses
     */
#define MBS_BROADCAST_ADDRESS               0
#define MBS_DEFAULT_ADDRESS                 1    
    
    /* ************************************************************************** */
    /** Buffer sizes for Modbus RTU Slave
     */
#define MBS_RECEIVE_BUFFER_SIZE             (MBS_NUMBER_OF_OUTPUT_REGISTERS*2 + 5) 
#define MBS_TRANSMIT_BUFFER_SIZE            MBS_RECEIVE_BUFFER_SIZE
#define MBS_RXTX_BUFFER_SIZE                MBS_TRANSMIT_BUFFER_SIZE

    
    /* ************************************************************************** */
    /** Timeout Constant for Petit Modbus RTU Slave [millisecond]
     */
#define MBS_TIMEOUT                         200


    /* ************************************************************************** */
    /** Modbus Functions
     */
//#define MBS_READ_COILS                      1
//#define MBS_READ_DISCRETE_INPUTS            2
#define MBS_READ_HOLDING_REGISTERS          3
//#define MBS_READ_INPUT_REGISTERS            4
//#define MBS_WRITE_SINGLE_COIL               5
#define MBS_WRITE_SINGLE_REGISTER           6
//#define MBS_WRITE_MULTIPLE_COILS            15
#define MBS_WRITE_MULTIPLE_REGISTERS        16
  

    /* ************************************************************************** */
    /** Modbus Status Flags and Error Codes Const.
     */
#define MBS_FALSE_FUNCTION                  0
#define MBS_FALSE_SLAVE_ADDRESS             1
#define MBS_DATA_NOT_READY                  2
#define MBS_DATA_READY                      3

#define MBS_ERROR_CODE_01                   0x01                            // Function code is not supported
#define MBS_ERROR_CODE_02                   0x02                            // Register address is not allowed or write-protected

    
    // *****************************************************************************
    // *****************************************************************************
    // Section: Data Types
    // *****************************************************************************
    // *****************************************************************************

 
    // *****************************************************************************
    /** Modbus Slave State
     */
typedef enum
{
    MBS_RXTX_IDLE,
    MBS_RXTX_START,
    MBS_RXTX_DATABUF,
    MBS_RXTX_WAIT_ANSWER,
    MBS_RXTX_TIMEOUT
}MBS_RXTX_STATE;

    // *****************************************************************************
    /** Modbus Data Frame Structure Type
     */
typedef struct
{
  unsigned char     Address;
  unsigned char     Function;
  unsigned char     DataBuf[MBS_RXTX_BUFFER_SIZE];
  unsigned short    DataLen;
} __attribute__ ((packed)) stMBS_RxTxData_t;



    // *****************************************************************************
    // *****************************************************************************
    // Section: Interface Functions
    // *****************************************************************************


    extern uint8_t MBS_SlaveAddress;
    extern uint16_t MBS_HoldRegisters[MBS_NUMBER_OF_OUTPUT_REGISTERS];
    extern volatile uint32_t MBS_TimerValue;
    extern uint32_t mySystemTimeOutTimer;
    
    void MBS_InitModbus(uint8_t ModbusSlaveAddress);
    void MBS_ProcessModbus(void);
    void MBS_ReciveData(uint8_t Data);
    void MBS_UART_Putch(uint8_t ch);
    
    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _MODBUS_SLAVE_H */

/* *****************************************************************************
 End of File
 */
