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
* File Name: parsingncidata.cpp
*
* Abstract:  This is the class for NCI frame - data packets.
*            This class inherits ParsingNCI.
*            This class deals packets from templates:
*                   NCI_DATA_SEND
*                   NCI_DATA_RECEIVE
*
* Functions:
*
*******************************************************************************/

#include "parsingncidata.h"
#include <sstream>

ParsingNciData::ParsingNciData(unsigned int ID,
                               unsigned int byteLength,unsigned char rawData[],
                               unsigned int direction
//                               #ifdef FILE_OUTPUT
                               , char * pFilePath
//                               #endif
                               )
                    : ParsingNCI::ParsingNCI(ID,byteLength,
                          rawData
//                          #ifdef FILE_OUTPUT
                          , pFilePath
//                          #endif
                          )
{
    m_parsingType=NCI_DATA;
    if (direction==0) {
        m_out.direction='<';//Device to Host
    }
    else if (direction==1) {
        m_out.direction='>';//Hose to Device
    }
    m_out.swpIdx=SWPIDX_SKIP;//no swpIdx
    m_out.nciConnID = (int)( (m_pDataPacket->m_ID) >> 8 & 0x0f);
    m_PBF=(int)( (m_pDataPacket->m_ID) >> 12 & 0x01);
    m_CREDIT = (int)( (m_pDataPacket->m_ID) & 0x3f);
}

int ParsingNciData::parse() {
    m_lineNum=0;
    m_out.m_pOut[m_lineNum]->desc="Packet Header";
    m_out.m_pOut[m_lineNum]->sequence="NCI-Frame";

    if (m_out.direction=='<') {//Device to Host
       m_out.m_pOut[m_lineNum]->meaning="NCI_DATA_RECEIVE. ";
    }
    else {
       m_out.m_pOut[m_lineNum]->meaning="NCI_DATA_SEND. ";
    }

    stringstream meaningStream;
    meaningStream<<"PBF: 0x"<<hex<<uppercase<<m_PBF<<". ";
    meaningStream<<"CREDIT: 0x"<<hex<<uppercase<<m_CREDIT<<". ";

    m_out.m_pOut[m_lineNum]->meaning += meaningStream.str();
    Parsing::lineOutput(OUTPUT_ID,OUTPUT_ID);

//    //line .1
//    stringstream meaningStream;
//    meaningStream<<"PBF: 0x"<<hex<<uppercase<<m_PBF<<". ";
//    meaningStream<<"CREDIT: 0x"<<hex<<uppercase<<m_CREDIT<<". ";
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
