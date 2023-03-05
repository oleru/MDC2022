/* ************************************************************************** */
/** MDC2022 Internal Protocol Description

  @Company
    Torka AS

  @File Name
    protocol.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _PROTOCOL_H    /* Guard against multiple inclusion */
#define _PROTOCOL_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include <stdint.h>


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif


    /* ************************************************************************** */
    /* ************************************************************************** */
    /* Section: Constants                                                         */
    /* ************************************************************************** */
    /* ************************************************************************** */

#define FRAME_MAXSIZE       100 // max size of packet

/* SLIP special character codes */
#define FRAME_END			0xC0    /* indicates end of packet */
#define FRAME_ESC			0xDB    /* indicates byte stuffing */
#define FRAME_ESC_END		0xDC    /* ESC ESC_END means END data byte */
#define FRAME_ESC_ESC		0xDD    /* ESC ESC_ESC means ESC data byte */

/* SLIP ERRORS */
#define FRAME_OK			0x10
#define FRAME_ERROR			0x11
#define FRAME_TOO_LARGE		0x12
#define FRAME_INPROGRESS	0x13
#define FRAME_NOFRAME		0x14
#define FRAME_TIMEOUT		0x15

/* FRAME TYPES () */
#define C_FRAMETYPE_RAW_DATA 0x10
#define C_FRAMETYPE_RAW_AD 0x11
#define C_FRAMETYPE_COMMAND 0x20
#define C_FRAMETYPE_DEBUG 0xFF


    // *****************************************************************************
    // *****************************************************************************
    // Section: Data Types
    // *****************************************************************************
    // *****************************************************************************

typedef struct {
    uint8_t frameType;
    uint8_t LastByte;
    uint32_t indexPtr;
    uint8_t CS;
    uint8_t Message[FRAME_MAXSIZE];
} __attribute__((packed)) _frameTypeParserData;


/*
Frametype 0x10:
  [C_FRAMETYPE_RAW_DATA]
  Broadcast All Ports in use
  payload:
  PORTA, PORTB, PORTC and PORTD. CS on All Bytes (including frametype)
 */
typedef struct {
    uint8_t frameType;  // 0x10
    uint16_t PortA;       
    uint16_t PortB;       
    uint16_t PortC;       
    uint8_t PortD;       
    uint8_t CS;
} __attribute__((packed)) _frameType0x10;

/*
Frametype 0x11:
  [C_FRAMETYPE_RAW_AD]
  Broadcast All AD-Channels in use
  payload:
  ... CS on All Bytes (including frametype)
 */
typedef struct {
    uint8_t frameType;  // 0x11
    uint16_t MD_FOCUS_FB_AN4;  //Channel Name:AN4   Assigned to:Shared Channel
    uint16_t MD_VERT_FB_AN5;  //Channel Name:AN5   Assigned to:Shared Channel
    uint16_t MD_HORIZ_FB_AN6;  //Channel Name:AN6   Assigned to:Shared Channel
    uint16_t POWER_IN_V_AN7;  //Channel Name:AN7   Assigned to:Shared Channel
    uint16_t FOCUS_POS_AN18;  //Channel Name:AN18   Assigned to:Shared Channel
    uint8_t CS;
} __attribute__((packed)) _frameType0x11;


/*
Frametype 0x20:
  [C_FRAMETYPE_COMMAND]
  Command Input Data, override local signal from arrival to around 500ms  
  payload:
  ... CS on All Bytes (including frametype)
 */
typedef struct {
    uint8_t frameType;  // 0x20
    int16_t horizInput;  // Dir (+/-) and PWM [0,32767];
    int16_t vertInput;  // Dir (+/-) and PWM [0,32767];
    int16_t focusInput;  // Dir (+/-) and PWM [0,32767];
    int8_t Command;  // Bit1=Change Lamp position 1(yes)/0(no), Bit0=Lamp ON/OFF, ....
    uint8_t CS;
} __attribute__((packed)) _frameType0x20;


/*
Frametype 0xFF:
  [C_FRAMETYPE_DEBUG]
  Debug variabler
  payload:
  ... CS on All Bytes (including frametype)
 */
typedef struct {
    uint8_t frameType;  // 0xFF
    int8_t horizSpeedIndex;
    int8_t vertSpeedIndex;
    int8_t focusSpeedIndex;
    uint8_t CS;
} __attribute__((packed)) _frameType0xFF;

    // *****************************************************************************
    // *****************************************************************************
    // Section: Interface Functions
    // *****************************************************************************
    // *****************************************************************************

void protocolParser(_frameTypeParserData *Header, uint8_t Ch);    

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _PROTOCOL_H */

/* *****************************************************************************
 End of File
 */
