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

#define FRAME_MAXSIZE       200 // max size of packet

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
  Broadcast All µC Ports in use
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
