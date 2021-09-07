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
* Abstract:  The is the class for NCI - template NCI_APP_CMD.
*            This class inherits ParsingNCI.
*
* Functions:
*
*******************************************************************************/

#include "parsingappcmd.h"
#include <sstream>

ParsingAppCmd::ParsingAppCmd(unsigned int ID,
                             unsigned int byteLength,
                             unsigned char rawData[]
//                             #ifdef FILE_OUTPUT
                             , char * pFilePath
//                             #endif
                             )
    : ParsingNCI::ParsingNCI(ID,byteLength,rawData
//          #ifdef FILE_OUTPUT
          , pFilePath
//          #endif
          )
{
    m_parsingType=NCI_APP_CMD;
    m_out.direction='>';//Hose to Device
    m_out.swpIdx=SWPIDX_SKIP;//no swpIdx
    m_out.nciConnID = NCI_CONN_ID_SKIP;//no conn ID
    m_MT = (int)( (m_pDataPacket->m_ID >> 13) & 0x07);
    m_PBF = (int)( (m_pDataPacket->m_ID >> 12) & 0x01);
    m_GID = (int)( (m_pDataPacket->m_ID >> 8) & 0x0f);
    m_OID = (int)( (m_pDataPacket->m_ID) & 0x3f);

    m_APPType = (int)( (m_pDataPacket->m_rawData[0] >> 4) & 0x0f);
    m_APPDest = (int)( (m_pDataPacket->m_rawData[0] >> 1) & 0x07);
    m_APPOper = (int)( (m_pDataPacket->m_rawData[0]) & 0x01);
    m_OFFSET = (int)( (m_pDataPacket->m_rawData[1]<<8)
                      + (m_pDataPacket->m_rawData[2]));
}

int ParsingAppCmd::parse() {
    m_lineNum=0;
    m_out.m_pOut[m_lineNum]->desc="Packet Header";
    m_out.m_pOut[m_lineNum]->sequence="NCI-Frame";

    string msgName=getCtrlMsgName(m_GID,m_OID,m_MT);

    stringstream meaningStream;
    meaningStream<<"ACCESS_APP_CMD"<<". ";
//    meaningStream<<"Msg Type: 0x"<<hex<<uppercase<<m_MT<<". ";
//    meaningStream<<"PBF: 0x"<<hex<<uppercase<<m_PBF<<". ";
//    meaningStream<<"GID: 0x"<<hex<<uppercase<<m_GID<<". ";
//    meaningStream<<"OID: 0x"<<hex<<uppercase<<m_OID<<". ";
    m_out.m_pOut[m_lineNum]->meaning=meaningStream.str();
    Parsing::lineOutput(OUTPUT_ID,OUTPUT_ID);

//    Parsing::lineOutput(0,2,true);

//    //line .1
//    stringstream meaningStream;
//    meaningStream<<msgName<<". ";
//    meaningStream<<"Msg Type: 0x"<<hex<<uppercase<<m_MT<<". ";
//    meaningStream<<"PBF: 0x"<<hex<<uppercase<<m_PBF<<". ";
//    meaningStream<<"GID: 0x"<<hex<<uppercase<<m_GID<<". ";
//    meaningStream<<"OID: 0x"<<hex<<uppercase<<m_OID<<". ";
//    m_out.m_pOut[m_lineNum]->sequence="";
//    m_out.m_pOut[m_lineNum]->desc="Packet Header";
//    m_out.m_pOut[m_lineNum]->meaning=meaningStream.str();
//    Parsing::lineOutput(OUTPUT_ID,OUTPUT_ID);

    //line .1
    meaningStream.str("");
    meaningStream<<"MSG_Length: 0x"<<hex<<uppercase
                <<m_pDataPacket->m_byteLength<<". ";
    meaningStream<<"APPType: 0x"<<hex<<uppercase<<m_APPType<<". ";
    meaningStream<<"APPDest: 0x"<<hex<<uppercase<<m_APPDest<<". ";
    meaningStream<<"APPOper: 0x"<<hex<<uppercase<<m_APPOper<<". ";
    m_out.m_pOut[m_lineNum]->sequence="";
    m_out.m_pOut[m_lineNum]->desc="Control Bytes";
    m_out.m_pOut[m_lineNum]->meaning=meaningStream.str();
    Parsing::lineOutput(0,1);

    //line .2
    meaningStream.str("");
    m_out.m_pOut[m_lineNum]->sequence="";
    m_out.m_pOut[m_lineNum]->desc="Packet Data";
    meaningStream<<"OFFSET: 0x"<<hex<<uppercase<<m_OFFSET<<". ";
    if (m_pDataPacket->m_byteLength>3) {
        meaningStream<<"DATA: 0x";
        for (int i=0; i<(int)m_pDataPacket->m_byteLength-3; i++) {
            meaningStream.fill('0');
            meaningStream.width(2);
            meaningStream<<hex<<uppercase
                        <<(int)m_pDataPacket->m_rawData[3+i]<<" ";
        }
    }
    m_out.m_pOut[m_lineNum]->meaning=meaningStream.str();
    Parsing::lineOutput(1,m_pDataPacket->m_byteLength);
    return 0;
}
