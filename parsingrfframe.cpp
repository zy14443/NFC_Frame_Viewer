/*******************************************************************************
* THIS INFORMATION IS PROPRIETARY TO BROADCOM CORP
*
* ------------------------------------------------------------------------------
*
* Copyright (c) 2011 Broadcom Corp.
*
*          ALL RIGHTS RESERVED
*
********************************************************************************
*
* File Name: parsingappcmd.cpp
*
* Abstract:  The is the class for NCI - NFC_FRAME_LOG_NTF.
*            This class inherits ParsingNCI.
*
* Functions:
*
*******************************************************************************/

#include "parsingrfframe.h"
#include <sstream>

ParsingRFFrame::ParsingRFFrame(unsigned int ID,
                                   unsigned int byteLength, unsigned char rawData[]
//                               #ifdef FILE_OUTPUT
                                   , char * pFilePath
//                               #endif
                               )
    : ParsingNCI::ParsingNCI(ID,byteLength,rawData
//          #ifdef FILE_OUTPUT
          , pFilePath
//          #endif
          )
{
    m_parsingType=NCI_RF_FRAME;
    m_dir = m_pDataPacket->m_rawData[0]&(0x01);
    if (m_dir == 0x00)
        m_out.direction='<';//Receive
    else
        m_out.direction='>'; // Transmit

    m_out.swpIdx=SWPIDX_SKIP;//no swpIdx
    m_out.nciConnID = NCI_CONN_ID_SKIP;//no conn ID
    m_MT = (int)( (m_pDataPacket->m_ID >> 13) & 0x07);
    m_PBF = (int)( (m_pDataPacket->m_ID >> 12) & 0x01);
    m_GID = (int)( (m_pDataPacket->m_ID >> 8) & 0x0f);
    m_OID = (int)( (m_pDataPacket->m_ID) & 0x3f);

    m_TimeStamp = (int)((m_pDataPacket->m_rawData[5]<<24)
                        + (m_pDataPacket->m_rawData[4]<<16)
                        + (m_pDataPacket->m_rawData[3]<<8)
                        + (m_pDataPacket->m_rawData[2]));
    m_Protocol = (int)(m_pDataPacket->m_rawData[6]);
    m_Bytelen = (int)((m_pDataPacket->m_rawData[1]<<3)
                       + ((m_pDataPacket->m_rawData[0]&(0xE0))>>5));
    m_Bitlen = (int)(m_pDataPacket->m_rawData[0]&(0x1C));
}

int ParsingRFFrame::parse() {
    m_lineNum=0;
    m_out.m_pOut[m_lineNum]->desc="Packet Header";
    m_out.m_pOut[m_lineNum]->sequence="NCI-Frame";
//    m_out.m_pOut[m_lineNum]->meaning="NCI_CMD_NOTIF. ";

    string msgName=getCtrlMsgName(m_GID,m_OID,m_MT);

    stringstream meaningStream;
    meaningStream<<"RF_FRAME_LOG_NTF"<<". ";
//    meaningStream<<"Msg Type: 0x"<<hex<<uppercase<<m_MT<<". ";
//    meaningStream<<"PBF: 0x"<<hex<<uppercase<<m_PBF<<". ";
//    meaningStream<<"GID: 0x"<<hex<<uppercase<<m_GID<<". ";
//    meaningStream<<"OID: 0x"<<hex<<uppercase<<m_OID<<". ";

    m_out.m_pOut[m_lineNum]->meaning+=meaningStream.str();

    Parsing::lineOutput(OUTPUT_ID,OUTPUT_ID);

//    Parsing::lineOutput(0,2,true);

//    //line .1
//    stringstream meaningStream;
//    meaningStream<<"Msg Name: "<<msgName<<". ";
//    meaningStream<<"Msg Type: 0x"<<hex<<uppercase<<m_MT<<". ";
//    meaningStream<<"PBF: 0x"<<hex<<uppercase<<m_PBF<<". ";
//    meaningStream<<"GID: 0x"<<hex<<uppercase<<m_GID<<". ";
//    meaningStream<<"OID: 0x"<<hex<<uppercase<<m_OID<<". ";
//    m_out.m_pOut[m_lineNum]->sequence="";
//    m_out.m_pOut[m_lineNum]->desc="Packet Header";
//    m_out.m_pOut[m_lineNum]->meaning=meaningStream.str();
//    Parsing::lineOutput(OUTPUT_ID,OUTPUT_ID);

    //line .2
    meaningStream.str("");
    meaningStream<<"TimeStamp : 0x"<<hex<<uppercase<<m_TimeStamp<<". ";
    meaningStream<<"Protocol : 0x"<<hex<<uppercase<<m_Protocol<<". ";
    meaningStream<<"ByteLen : 0x"<<hex<<uppercase<<m_Bytelen<<". ";
    meaningStream<<"BitLen : 0x"<<hex<<uppercase<<m_Bitlen<<". ";
    m_out.m_pOut[m_lineNum]->sequence="";
    m_out.m_pOut[m_lineNum]->desc="Control Bytes";
    m_out.m_pOut[m_lineNum]->meaning=meaningStream.str();
    Parsing::lineOutput(0,7);

    //line .3
    meaningStream.str("");
    m_out.m_pOut[m_lineNum]->sequence="";
    m_out.m_pOut[m_lineNum]->desc="Packet Data";
    if (m_pDataPacket->m_byteLength>8) {
        meaningStream<<"DATA: 0x";
        for (int i=0; i<(int)m_pDataPacket->m_byteLength-8; i++) {
            meaningStream.fill('0');
            meaningStream.width(2);
            meaningStream<<hex<<uppercase
                        <<(int)m_pDataPacket->m_rawData[8+i]<<" ";
        }
    }
    m_out.m_pOut[m_lineNum]->meaning=meaningStream.str();
    Parsing::lineOutput(7,m_pDataPacket->m_byteLength);
    return 0;
}

