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
* File Name: parsingclt.cpp
*
* Abstract:  The is the class for SWP - CLT frame.
*            This class inherits ParsingSWP.
*
* Functions:
*
*******************************************************************************/

#include "parsingclt.h"

parsingCLT::parsingCLT(unsigned int ID,
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
    m_parsingType=CLT;
}

int parsingCLT::parse(){
    if (m_data==NULL)
        return 1;
    m_lineNum=0;
    m_out.m_pOut[m_lineNum]->desc="Control field";
    m_out.m_pOut[m_lineNum]->sequence="SWP CLT-Frame";
    int DATA_FIELD_STRUCTURE = (m_data[0]>>4)&0x01;
    int ADMIN_FIELD = m_data[0] & 0x0f;
    switch (DATA_FIELD_STRUCTURE) {
    case 0: m_out.m_pOut[m_lineNum]->meaning=m_out.m_pOut[m_lineNum]->meaning
                ="Type A aligned. "; break;
    case 1: m_out.m_pOut[m_lineNum]->meaning=m_out.m_pOut[m_lineNum]->meaning
                ="Byte aligned. "; break;
    }

    if (m_dir==0) {
        //from UICC to CLF
        switch (ADMIN_FIELD) {
        case 0: m_out.m_pOut[m_lineNum]->meaning
                    +="No administrative command. "; break;
        case 1: m_out.m_pOut[m_lineNum]->meaning +="CL_GOTO_INIT. "; break;
        case 2: m_out.m_pOut[m_lineNum]->meaning +="CL_GOTO_HALT. "; break;
        default: m_out.m_pOut[m_lineNum]->meaning +="RFU. "; break;
        }
    }
    //from CLF to UICC
    else {
        switch (ADMIN_FIELD) {
        case 0: m_out.m_pOut[m_lineNum]->meaning
                    +="No administrative command. "; break;
        case 8: m_out.m_pOut[m_lineNum]->meaning +="CL_PROTO_INF(A). "; break;
        case 9: m_out.m_pOut[m_lineNum]->meaning +="CL_PROTO_INF(F). "; break;
        default: m_out.m_pOut[m_lineNum]->meaning +="RFU."; break;
        }
    }
    ParsingSWP::lineOutput(5,8);
    //line .1
    m_out.m_pOut[m_lineNum]->sequence="";
    m_out.m_pOut[m_lineNum]->desc="Data field";
    m_out.m_pOut[m_lineNum]->meaning="";
    ParsingSWP::lineOutput(6,m_bytes+5);
    return 0;
}
