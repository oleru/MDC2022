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

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include "protocol.h"


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */



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

void protocolParser(_frameTypeParserData *Header, uint8_t Ch)
{
    
    // First check if we are at a end...
    if( Ch == FRAME_END ) {

        // Clear out - prepare for next packets
        Header->frameType = 0;
        Header->LastByte = FRAME_END;
        Header->indexPtr = 0;
        Header->CS = 0;
        
        return;
        
    } else if( Ch == FRAME_ESC ) {  // ESC Slip?
        
        Header->LastByte = FRAME_ESC;
        return;
        
    } else if( Header->LastByte == FRAME_ESC ) {  // LastCh FRAME_ESC?
            
        Header->LastByte = Ch;
        if( Ch == FRAME_ESC_END )
            Ch = FRAME_END;  // Swap it for coded data.
        else if( Ch == FRAME_ESC_ESC ) {
            Ch = FRAME_ESC;
        } else {
            ;
            // if "c" is not one of these two, then we
            // have a protocol violation.
        }                
    }
    
    

        
/*    uint8_t Ch, *buff;
    int8_t index;
    uint32_t CRC, CRCcalc, i, cnt;

    _frameType0x20 * pFT0x20;
    _frameType0x21 * pFT0x21;
    _CRC24 * pCRC24;
    _frameType0x2AHead * pFT0x2AH;
    _frameType0x2BHead * pFT0x2BH;
            
    while( PullFIFO(FIFO, &Ch,1) ) {
        
        // First check if we are at a end...
        if( Ch == FRAME_END ) {

            if(Header->indexPtr>1)
                syslog(SYSLOG_INFO, "%d bytes\r\n",Header->indexPtr);

 */ 
/*
            // Check CRC here for logg on frameType C_FRAMETYPE_BROADCAST_RTCM_PACKET 
            if(Header->frameType == C_FRAMETYPE_BROADCAST_RTCM_PACKET) {
                checkRTCM0x11Packet(Header);
            }
*/
/*            
            if(Header->frameType != 0) {
                memset(Header,0,sizeof(_frameTypeParserData)-FRAME_MAXSIZE);
            }
            Header->indexPtr = 0;
            Header->LastByte = FRAME_END;
            continue;
        }
        
        // DEBUG
//        if((Header->indexPtr>0) && (Header->indexPtr<6))
//            syslog(SYSLOG_INFO, "[%2.2X]",Header->Message[Header->indexPtr-1]);

        
        // ESC Slip?
        if( Ch == FRAME_ESC ) {
            Header->LastByte = FRAME_ESC;
            continue;
        }
        
        // LastCh FRAME_ESC?
        if( Header->LastByte == FRAME_ESC ) {
            
            Header->LastByte = Ch;
            if( Ch == FRAME_ESC_END )
                Ch = FRAME_END;  // Swap it for coded data.
            else if( Ch == FRAME_ESC_ESC ) {
                Ch = FRAME_ESC;
            } else {
                ;
                // if "c" is not one of these two, then we
                // have a protocol violation.
                syslog(SYSLOG_ERR, "%d::FRAME_ESC protocol violation.\r\n",upTime);
            }                
        }
        
        // Handle Data by FrameType
        switch(Header->frameType) {
            
            // FrameType not sat
            case 0:
                if(Header->LastByte == FRAME_END) {
                    Header->frameType = Ch;  // Set FrameType
                    Header->Message[Header->indexPtr++] = Ch;
                    if(Header->frameType==C_FRAMETYPE_BROADCAST_RTCM) {
                        // Some Response back on preformance
                        if((ddRF.timer-ddRF.timeStampLast0x10Pkt)>1500) {
                            syslog(SYSLOG_WARN, "%d::Most likely Missing 0x10 packet (%d ms since last pkt.)\r\n",upTime,ddRF.timer-ddRF.timeStampLast0x10Pkt);
                        }
                        ddRF.timeStampLast0x10Pkt = ddRF.timer;
                        cnt = (ddRF.cnt0x10Pkt++);

                        if(Satel_CR.state == SATEL_STANDBY_STATE)
                            Satel_initStateMachine(&Satel_CR, SATEL_REQ_RSSI_STATE);                

                    }
                    if(Header->frameType==C_FRAMETYPE_BROADCAST_RTCM_PACKET) {
                        // Some Response back on preformance
                        if((ddRF.timer-ddRF.timeStampLast0x11Pkt)>1500) {
                            syslog(SYSLOG_WARN, "%d::Most likely Missing 0x11 packet (%d ms since last pkt.)\r\n",upTime,ddRF.timer-ddRF.timeStampLast0x11Pkt);
                        }         
                        ddRF.timeStampLast0x11Pkt = ddRF.timer;
                        cnt = (ddRF.cnt0x11Pkt++);
                       
                        if(Satel_CR.state == SATEL_STANDBY_STATE)
                            Satel_initStateMachine(&Satel_CR, SATEL_REQ_RSSI_STATE);                
                    }
                    if(Header->frameType==C_FRAMETYPE_POLL_REQUEST) {
                        if((ddRF.timer-ddRF.timeStampLast0x20Pkt)>1500) {
                            syslog(SYSLOG_WARN, "%d::Most likely Missing 0x20 packet (%d ms since last pkt.)\r\n",upTime,ddRF.timer-ddRF.timeStampLast0x20Pkt);
                        }
                        ddRF.timeStampLast0x20Pkt = ddRF.timer;
                        cnt = (ddRF.cnt0x20Pkt++);
                        //if(Satel_CntEOT>0)
                        //  syslog(SYSLOG_WARN, "(Satel_EOT count=%d)\r\n",Satel_CntEOT);
                    }

                    if(Header->frameType==C_FRAMETYPE_POLL_REQUEST_X) {
                        if((ddRF.timer-ddRF.timeStampLast0x21Pkt)>1500) {
                            syslog(SYSLOG_WARN, "%d::Most likely Missing 0x21 packet (%d ms since last pkt.)\r\n",upTime,ddRF.timer-ddRF.timeStampLast0x21Pkt);
                        }
                        ddRF.timeStampLast0x21Pkt = ddRF.timer;
                        cnt = (ddRF.cnt0x21Pkt++);
                    }
                    
                    if((Header->frameType!=C_FRAMETYPE_MSG_RTK_01) && (Header->frameType!=C_FRAMETYPE_MSG_RTK_02) ) {
                        if((Header->frameType!=C_FRAMETYPE_POLL_REQUEST_RTK_TS1) && (Header->frameType!=C_FRAMETYPE_POLL_REQUEST_RTK_TS2)) {
                            syslog(SYSLOG_INFO, "FrameType: 0x%x (%d), %d ",Header->frameType,ddRF.byteCnt,ddRF.timer);
                        } else {
                            syslog(SYSLOG_INFO, "FrameType: 0x%x (%d)",Header->frameType,ddRF.byteCnt);                          
                        }
                    }
                    
                }    
                break;
                
            // C_FRAMETYPE_BROADCAST_RTCM
            case C_FRAMETYPE_BROADCAST_RTCM:
                // Stream data direct to UBlox
                PushFIFO(&UART4TxFIFO, Ch,1);
                Header->Message[Header->indexPtr++] = Ch;
                if(Header->indexPtr >= FRAME_MAXSIZE) { // Packet not good...
                    Header->frameType = 0xFF;  // Throw out data until we Retrigger...
                }
                break;
            
            // C_FRAMETYPE_BROADCAST_RTCM_PACKET
            case C_FRAMETYPE_BROADCAST_RTCM_PACKET:
                // Stream data direct to UBlox (3 byte delay seens CRC <= NOT FOR NOW!)
                Header->Message[Header->indexPtr++] = Ch;
                if(Header->indexPtr >= FRAME_MAXSIZE) { // Packet not good...
                    Header->frameType = 0xFF;  // Throw out data until we Retrigger...
                }
                PushFIFO(&UART4TxFIFO, Ch,1);
*/
/*
                if(Header->indexPtr>4)
                    PushFIFO(&UART4TxFIFO, Header->Message[Header->indexPtr-4]);
*/
/*                break;
            
            // C_FRAMETYPE_POLL_REQUEST
            case C_FRAMETYPE_POLL_REQUEST:
                if(Header->indexPtr<sizeof(_frameType0x20)) {
                    Header->Message[Header->indexPtr++] = Ch;
                    if(Header->indexPtr==sizeof(_frameType0x20)) {
                        CRCcalc = rtk_crc24q(Header->Message, Header->indexPtr-sizeof(_CRC24));
                        pFT0x20 = (_frameType0x20 *)&(Header->Message);
                        CRC = pFT0x20->CRC.B2;
                        CRC <<= 8; 
                        CRC |= pFT0x20->CRC.B1;
                        CRC <<= 8; 
                        CRC |= pFT0x20->CRC.B0; 
                        if(CRCcalc == CRC) {
//                            if(pFT0x20->UID==POD_UID) {
                            if((pFT0x20->UID==POD_UID) || ((10000<POD_UID) && (pFT0x20->UID>POD_UID) && (pFT0x20->UID<(POD_UID+100)))) {
                                FT0x30.frameType = 0x30;
                                FT0x30.UID = pFT0x20->UID;//POD_UID;
                                CRC = rtk_crc24q((uint8_t *)&FT0x30, sizeof(_frameType0x30)-sizeof(_CRC24));
                                FT0x30.CRC.B2 = (uint8_t)(CRC>>16);
                                FT0x30.CRC.B1 = (uint8_t)(CRC>>8);
                                FT0x30.CRC.B0 = (uint8_t)(CRC);                                
                                PushFIFO(&UART5TxFIFO,FRAME_END,1);
                                buff = (uint8_t *)&FT0x30;
                                for(i=0;i<sizeof(_frameType0x30);i++) {
                                    if(buff[i] == FRAME_END) {
                                        PushFIFO(&UART5TxFIFO,FRAME_ESC,1);
                                        PushFIFO(&UART5TxFIFO,FRAME_ESC_END,1);
                                  } else if(buff[i] == FRAME_ESC) {
                                        PushFIFO(&UART5TxFIFO,FRAME_ESC,1);
                                        PushFIFO(&UART5TxFIFO,FRAME_ESC_ESC,1);
                                    } else {
                                        PushFIFO(&UART5TxFIFO,buff[i],1);
                                    }    
                                }
                                PushFIFO(&UART5TxFIFO,FRAME_END,1);
                                UART5TxFIFO.quarantineTimeout += 5;
                                
                                // Some Response back on preformance
                                LED_R_Toggle();
                            }
                        } else {
                          if(pFT0x20->UID==POD_UID)
                              syslog(SYSLOG_ERR, "%d::CRC error on 0x20 pkt. (0x%8.8X <> 0x%8.8X)\r\n",upTime,CRCcalc,CRC);                            
                        }
                        
                    }
                } 
                break;
                
            // C_FRAMETYPE_POLL_REQUEST_X
            case C_FRAMETYPE_POLL_REQUEST_X:
                if(Header->indexPtr<sizeof(_frameType0x21)) {
                    Header->Message[Header->indexPtr++] = Ch;
                    if(Header->indexPtr==sizeof(_frameType0x21)) {
                        CRCcalc = rtk_crc24q(Header->Message, Header->indexPtr-sizeof(_CRC24));
                        pFT0x21 = (_frameType0x21 *)&(Header->Message);
                        CRC = pFT0x21->CRC.B2;
                        CRC <<= 8; 
                        CRC |= pFT0x21->CRC.B1;
                        CRC <<= 8; 
                        CRC |= pFT0x21->CRC.B0; 
                        if(CRCcalc == CRC) {
                            if((pFT0x21->UID==POD_UID) || ((10000<POD_UID) && (pFT0x21->UID>POD_UID) && (pFT0x21->UID<(POD_UID+100)))) {
                                FT0x31.frameType = 0x31;
                                FT0x31.UID = pFT0x21->UID;//POD_UID;
                                CRC = rtk_crc24q((uint8_t *)&FT0x31, sizeof(_frameType0x31)-sizeof(_CRC24));
                                FT0x31.CRC.B2 = (uint8_t)(CRC>>16);
                                FT0x31.CRC.B1 = (uint8_t)(CRC>>8);
                                FT0x31.CRC.B0 = (uint8_t)(CRC);                                
                                PushFIFO(&UART5TxFIFO,FRAME_END,1);
                                buff = (uint8_t *)&FT0x31;
                                for(i=0;i<sizeof(_frameType0x31);i++) {
                                    if(buff[i] == FRAME_END) {
                                        PushFIFO(&UART5TxFIFO,FRAME_ESC,1);
                                        PushFIFO(&UART5TxFIFO,FRAME_ESC_END,1);
                                  } else if(buff[i] == FRAME_ESC) {
                                        PushFIFO(&UART5TxFIFO,FRAME_ESC,1);
                                        PushFIFO(&UART5TxFIFO,FRAME_ESC_ESC,1);
                                    } else {
                                        PushFIFO(&UART5TxFIFO,buff[i],1);
                                    }    
                                }
                                PushFIFO(&UART5TxFIFO,FRAME_END,1);
                                UART5TxFIFO.quarantineTimeout += 5;
                                
                                // Some Response back on preformance
                                LED_R_Toggle();
                            }
                        } else {
                          if(pFT0x21->UID==POD_UID)
                              syslog(SYSLOG_ERR, "%d::CRC error on 0x21 pkt. (0x%8.8X <> 0x%8.8X)\r\n",upTime,CRCcalc,CRC);                            
                        }
                        
                    }
                } 
                break;

            // C_FRAMETYPE_POLL_REQUEST_RTK_TS1
            case C_FRAMETYPE_POLL_REQUEST_RTK_TS1:
                if(Header->indexPtr<FRAME_MAXSIZE-1)
                    Header->Message[Header->indexPtr++] = Ch;
                if(Header->indexPtr>=sizeof(_frameType0x2AHead)+sizeof(uint16_t)+sizeof(_CRC24)) {
                    CRCcalc = rtk_crc24q(Header->Message, Header->indexPtr-sizeof(_CRC24));
                    pCRC24 = (_CRC24 *)&(Header->Message[Header->indexPtr-sizeof(_CRC24)]);
                    CRC = pCRC24->B2;
                    CRC <<= 8; 
                    CRC |= pCRC24->B1;
                    CRC <<= 8; 
                    CRC |= pCRC24->B0; 
                    if(CRCcalc == CRC) {  // If CRC OK asume Packet complete
                        index = indexOfPod(POD_UID, (uint16_t *)&(Header->Message[sizeof(_frameType0x2AHead)]), (Header->indexPtr-sizeof(_frameType0x2BHead)-sizeof(_CRC24)) >> 1);
                        // IF POD ID located set quarantineTimeout by index * slotTime + 5ms                      
                        if(index>-1) {
                            pFT0x2AH = (_frameType0x2AHead *)&(Header->Message);
                            FT0x30.frameType = 0x30;
                            FT0x30.UID = POD_UID;
                            CRC = rtk_crc24q((uint8_t *)&FT0x30, sizeof(_frameType0x30)-sizeof(_CRC24));
                            FT0x30.CRC.B2 = (uint8_t)(CRC>>16);
                            FT0x30.CRC.B1 = (uint8_t)(CRC>>8);
                            FT0x30.CRC.B0 = (uint8_t)(CRC);                                
                            PushFIFO(&UART5TxFIFO,FRAME_END,1);
                            buff = (uint8_t *)&FT0x30;
                            for(i=0;i<sizeof(_frameType0x30);i++) {
                                if(buff[i] == FRAME_END) {
                                    PushFIFO(&UART5TxFIFO,FRAME_ESC,1);
                                    PushFIFO(&UART5TxFIFO,FRAME_ESC_END,1);
                              } else if(buff[i] == FRAME_ESC) {
                                    PushFIFO(&UART5TxFIFO,FRAME_ESC,1);
                                    PushFIFO(&UART5TxFIFO,FRAME_ESC_ESC,1);
                                } else {
                                    PushFIFO(&UART5TxFIFO,buff[i],1);
                                }    
                            }
                            PushFIFO(&UART5TxFIFO,FRAME_END,1);
                            UART5TxFIFO.quarantineTimeout += index*pFT0x2AH->slotTime + 10;

                            // Some Response back on preformance
                            LED_R_Toggle();
                        }
                    }

                } 
                break;
                
                
            // C_FRAMETYPE_POLL_REQUEST_RTK_TS2
            case C_FRAMETYPE_POLL_REQUEST_RTK_TS2:
                if(Header->indexPtr<FRAME_MAXSIZE-1)
                    Header->Message[Header->indexPtr++] = Ch;
                if(Header->indexPtr>=sizeof(_frameType0x2BHead)+sizeof(uint16_t)+sizeof(_CRC24)) {
                    CRCcalc = rtk_crc24q(Header->Message, Header->indexPtr-sizeof(_CRC24));
                    pCRC24 = (_CRC24 *)&(Header->Message[Header->indexPtr-sizeof(_CRC24)]);
                    CRC = pCRC24->B2;
                    CRC <<= 8; 
                    CRC |= pCRC24->B1;
                    CRC <<= 8; 
                    CRC |= pCRC24->B0; 
                    if(CRCcalc == CRC) {  // If CRC OK asume Packet complete
                        index = indexOfPod(POD_UID, (uint16_t *)&(Header->Message[sizeof(_frameType0x2BHead)]), (Header->indexPtr-sizeof(_frameType0x2BHead)-sizeof(_CRC24)) >> 1);
                        // IF POD ID located set quarantineTimeout by index * slotTime + 5ms                      
                        if(index>-1) {
                            pFT0x2BH = (_frameType0x2BHead *)&(Header->Message);
                            FT0x31.frameType = 0x31;
                            FT0x31.UID = POD_UID;
                            CRC = rtk_crc24q((uint8_t *)&FT0x31, sizeof(_frameType0x31)-sizeof(_CRC24));
                            FT0x31.CRC.B2 = (uint8_t)(CRC>>16);
                            FT0x31.CRC.B1 = (uint8_t)(CRC>>8);
                            FT0x31.CRC.B0 = (uint8_t)(CRC);                                
                            PushFIFO(&UART5TxFIFO,FRAME_END,1);
                            buff = (uint8_t *)&FT0x31;
                            for(i=0;i<sizeof(_frameType0x31);i++) {
                                if(buff[i] == FRAME_END) {
                                    PushFIFO(&UART5TxFIFO,FRAME_ESC,1);
                                    PushFIFO(&UART5TxFIFO,FRAME_ESC_END,1);
                              } else if(buff[i] == FRAME_ESC) {
                                    PushFIFO(&UART5TxFIFO,FRAME_ESC,1);
                                    PushFIFO(&UART5TxFIFO,FRAME_ESC_ESC,1);
                                } else {
                                    PushFIFO(&UART5TxFIFO,buff[i],1);
                                }    
                            }
                            PushFIFO(&UART5TxFIFO,FRAME_END,1);
                            UART5TxFIFO.quarantineTimeout += index*pFT0x2BH->slotTime + 10;

                            // Some Response back on preformance
                            LED_R_Toggle();
                        }
                    }

                } 
                break;
                                
            // Move on - nothing to see... (Responds from other POD's)     
            case C_FRAMETYPE_MSG_RTK_01:
            case C_FRAMETYPE_MSG_RTK_02:
                break;
                
            // Unknown Frame>Type...
            default:
                syslog(SYSLOG_INFO, "{0x%2.2X}", Ch);
                break;
                
        }

        // Do some Tricks on the Header->LastByte to avoid FRAME_ERROR
        if( (Header->LastByte != FRAME_ESC_END ) && (Header->LastByte != FRAME_ESC_ESC ) ) 
            Header->LastByte = Ch;
        
    }  // ..while
*/    
}


/* *****************************************************************************
 End of File
 */
