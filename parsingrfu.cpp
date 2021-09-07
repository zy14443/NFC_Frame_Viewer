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
* File Name: parsingrfu.cpp
*
* Abstract:  The is the class for SWP - RFU frame.
*            This class inherits ParsingSWP.
*
* Functions:
*
*******************************************************************************/
#include "parsingrfu.h"

parsingRFU::parsingRFU(unsigned int ID,
                       unsigned int byteLength, unsigned char rawData[]
//                       #ifdef FILE_OUTPUT
                       , char * pFilePath
//                       #endif
                       )
    : ParsingSWP::ParsingSWP(ID,byteLength,rawData
//          #ifdef FILE_OUTPUT
          , pFilePath
//          #endif
          )
{
    m_parsingType=RFU;
}

int parsingRFU::parse() {
    m_out.m_pOut[m_lineNum]->sequence="SWP RFU-Frame";
    ParsingSWP::lineOutput(5,m_bytes+5);
    return 0;
}
