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
* File Name: parsingact.cpp
*
* Abstract:  The is the class for SWP - ACT frame.
*            This class inherits ParsingSWP.
*
* Functions:
*
*******************************************************************************/

#include "parsingact.h"
#include "parsingswp.h"

parsingACT::parsingACT(unsigned int ID,
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
    m_parsingType=ACT;
}

int parsingACT::parse(){
    if (m_data==NULL)
        return 1;
    m_lineNum=0;
    m_out.m_pOut[m_lineNum]->desc="Control field";
    int ACT_CTRL=(int)(m_data[0]&0x07);
    int INF=(int)((m_data[0]&0x08) >> 3);
    int INF_start=1;
    switch (ACT_CTRL) {
    case 0: m_out.m_pOut[m_lineNum]->meaning="ACT initialization READY. ";
        break;
    case 1: m_out.m_pOut[m_lineNum]->meaning="SYNC management. "; break;
    case 2: m_out.m_pOut[m_lineNum]->meaning="Power mode indication. "; break;
    default: break;//reserved for future use.
    }
    m_out.m_pOut[m_lineNum]->sequence="SWP ACT-Frame";
    if (m_dir==0) {
        //from UICC to CLF
        if(INF) {
            m_out.m_pOut[m_lineNum]->meaning+="/Information field present. ";
        }
        //from CLF to UICC
        else {
        }
    }
    ParsingSWP::lineOutput(5,6);
    //line .1 or nothing
    if (ACT_CTRL==2) {
        m_out.m_pOut[m_lineNum]->sequence="";
        m_out.m_pOut[m_lineNum]->desc="Power mode";
        switch (m_data[1]) {
        case 0: m_out.m_pOut[m_lineNum]->meaning="Low power mode"; break;
        case 1: m_out.m_pOut[m_lineNum]->meaning="Full power mode"; break;
        default: m_out.m_pOut[m_lineNum]->meaning="Error! For future use"; break;
        }
        INF_start++;
        ParsingSWP::lineOutput(6,7);
    }
    else if (ACT_CTRL==1) {
        m_out.m_pOut[m_lineNum]->sequence="";
        m_out.m_pOut[m_lineNum]->desc="Sync ID";
        m_out.m_pOut[m_lineNum]->meaning="";
        INF_start+=2;
        ParsingSWP::lineOutput(6,8);
    }
    //continue from previous
    if (m_dir==0 && INF) {
        m_out.m_pOut[m_lineNum]->sequence="";
        m_out.m_pOut[m_lineNum]->desc="Extended capabilities";
        if (m_data[INF_start]&0x04) {
            m_out.m_pOut[m_lineNum]->meaning="extended resume supported. ";
        }
        else {
            m_out.m_pOut[m_lineNum]->meaning="";
        }
        switch (m_data[INF_start]&0x03) {
        case 0: m_out.m_pOut[m_lineNum]->meaning
                    ="SWP bit rates as the default range are supported.";
                break;
        case 1: m_out.m_pOut[m_lineNum]->meaning
                    ="SWP bit rates down to default and"
                    "up to 10,000 ns supported.";
                break;
        case 2: m_out.m_pOut[m_lineNum]->meaning
                    ="SWP bit rates down to 590 ns and "
                    "up to default supported.";
                break;
        case 3: m_out.m_pOut[m_lineNum]->meaning
                    ="SWP bit rates down to 590 ns and "
                    "up to 10,000 ns supported.";
                break;
        }
        ParsingSWP::lineOutput(INF_start+5,m_bytes+5);
    }
    return 0;
}
