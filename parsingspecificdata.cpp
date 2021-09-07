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
* File Name: parsingspecificdata.cpp
*
* Abstract:  This is the class for NCI frame - template NCI_SPECIFIC_DATA.
*            This class inherits ParsingNCI.
*
* Functions:
*
*******************************************************************************/
#include "parsingspecificdata.h"
#include <sstream>

ParsingSpecificData::ParsingSpecificData(unsigned int ID,
                                         unsigned int byteLength,
                                         unsigned char rawData[]
//                                         #ifdef FILE_OUTPUT
                                         , char * pFilePath
//                                         #endif
                                         )
    : ParsingNCI::ParsingNCI(ID,byteLength,rawData
//          #ifdef FILE_OUTPUT
          , pFilePath
//          #endif
          )
{
    m_parsingType=NCI_SPECIFIC_DATA;
    m_out.direction='>';//Hose to Device
    m_out.swpIdx=SWPIDX_SKIP;//no swpIdx
    m_out.nciConnID = (int)( (m_pDataPacket->m_ID) >> 8 & 0x0f);
    m_PBF=(int)( (m_pDataPacket->m_ID) >> 12 & 0x01);
}

int ParsingSpecificData::parse() {
    m_lineNum=0;
    m_out.m_pOut[m_lineNum]->desc="Packet Header";

    stringstream meaningStream;
    meaningStream<<"PBF: 0x"<<hex<<uppercase<<m_PBF<<". ";

    m_out.m_pOut[m_lineNum]->meaning="NCI_SPECIFIC_DATA. ";
    m_out.m_pOut[m_lineNum]->sequence="NCI-Frame";

//    m_out.m_pOut[m_lineNum]->meaning+=meaningStream.str();
    Parsing::lineOutput(OUTPUT_ID,OUTPUT_ID);

//    Parsing::lineOutput(0,2,true);

    //line .1
//    stringstream meaningStream;
//    meaningStream<<"PBF: 0x"<<hex<<uppercase<<m_PBF<<". ";
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
