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
* File Name: parsingshdlc_s.cpp
*
* Abstract:  The is the class for SWP - SHDLC - S frame.
*            This class inherits ParsingSWP.
*
* Functions:
*
*******************************************************************************/
#include "parsingshdlc_s.h"
#include <sstream>

parsingSHDLC_S::parsingSHDLC_S(unsigned int ID,
                               unsigned int byteLength,
                               unsigned char rawData[]
//                               #ifdef FILE_OUTPUT
                               , char * pFilePath
//                               #endif
                               )
    : ParsingSWP::ParsingSWP(ID,byteLength,rawData
//          #ifdef FILE_OUTPUT
          , pFilePath
//          #endif
          )
{
    m_parsingType=SHDLC_S;
}

int parsingSHDLC_S::parse() {
    if (m_data==NULL)
        return 1;
    m_lineNum=0;
    m_out.m_pOut[m_lineNum]->desc="Control field";
    int Nr;
    switch ((m_data[0]>>3)&0x03) {
    case 0: m_out.m_pOut[m_lineNum]->meaning="Receive Ready. ";break;
    case 1: m_out.m_pOut[m_lineNum]->meaning="Reject. ";break;
    case 2: m_out.m_pOut[m_lineNum]->meaning="Receive Not Ready. ";break;
    case 3: m_out.m_pOut[m_lineNum]->meaning="Selective Reject. ";break;
    }
    stringstream meaningStream;
    Nr=(int)(m_data[0]&0x07);
    meaningStream<<"N(R):"<<Nr<<";";
    m_out.m_pOut[m_lineNum]->meaning += meaningStream.str();
    m_out.m_pOut[m_lineNum]->sequence="SWP S-Frame";
    ParsingSWP::lineOutput(5,m_bytes+5);
    return 0;
}
