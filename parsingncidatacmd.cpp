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
* File Name: parsingncidatacmd.cpp
*
* Abstract:  This is the class for NCI frame - Control Packet - Command Message.
*            This class inherits ParsingNCI.
*            This class deals packets from template NCI_DATA_SEND.
*                                          packet ENCAPSULATE_NCI_CMD.
*
* Functions:
*
*******************************************************************************/
#include "parsingncidatacmd.h"
#include <sstream>

ParsingNciDataCmd::ParsingNciDataCmd(unsigned int ID,
                                     unsigned int byteLength,
                                     unsigned char rawData[]
//                                     #ifdef FILE_OUTPUT
                                     , char * pFilePath
//                                     #endif
                                     )
    : ParsingNCI::ParsingNCI(ID,byteLength,rawData
//          #ifdef FILE_OUTPUT
          , pFilePath
//          #endif
          )
{
    m_parsingType=NCI_DATA_CMD;
    m_out.direction='>';//Hose to Device
    m_out.swpIdx=SWPIDX_SKIP;//no swpIdx
    m_out.nciConnID = NCI_CONN_ID_SKIP;//no conn ID
    m_MT = (int)( (m_pDataPacket->m_ID >> 13) & 0x07);
    m_PBF = (int)( (m_pDataPacket->m_ID >> 12) & 0x01);
    m_GID = (int)( (m_pDataPacket->m_ID >> 8) & 0x0f);
    m_OID = (int)( (m_pDataPacket->m_ID) & 0x3f);
}

int ParsingNciDataCmd::parse() {
    m_lineNum=0;
    m_out.m_pOut[m_lineNum]->desc="Packet Header";
    m_out.m_pOut[m_lineNum]->sequence="NCI-Frame";
    string msgName=getCtrlMsgName(m_GID,m_OID,m_MT);

    stringstream meaningStream;
    meaningStream<<"Msg Name: "<<msgName<<". ";
//    meaningStream<<"Msg Type: 0x"<<hex<<uppercase<<m_MT<<". ";
//    meaningStream<<"PBF: 0x"<<hex<<uppercase<<m_PBF<<". ";
//    meaningStream<<"GID: 0x"<<hex<<uppercase<<m_GID<<". ";
//    meaningStream<<"OID: 0x"<<hex<<uppercase<<m_OID<<". ";

    m_out.m_pOut[m_lineNum]->meaning += meaningStream.str();

//    if (m_MT==1) {
//        m_out.m_pOut[m_lineNum]->meaning+="Command. ";
//    }
//    else if (m_MT==2) {
//        m_out.m_pOut[m_lineNum]->meaning+="Response. ";
//    }
//    else if (m_MT==3) {
//        m_out.m_pOut[m_lineNum]->meaning+="Notification. ";
//    }

//    m_out.m_pOut[m_lineNum]->meaning+=msgName;

//    Parsing::lineOutput(0,2,true);

    //line .1
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

    if (m_pDataPacket->m_byteLength>0) {
        //line .2
        m_out.m_pOut[m_lineNum]->sequence="";
        m_out.m_pOut[m_lineNum]->desc="Packet Data";
        m_out.m_pOut[m_lineNum]->meaning="";
        Parsing::lineOutput(0,m_pDataPacket->m_byteLength);
    }
    return 0;
}
