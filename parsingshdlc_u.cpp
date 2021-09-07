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
* File Name: parsingshdlc_u.cpp
*
* Abstract:  The is the class for SWP - SHDLC - U frame.
*            This class inherits ParsingSWP.
*
* Functions:
*
*******************************************************************************/
#include "parsingshdlc_u.h"
#include <sstream>

parsingSHDLC_U::parsingSHDLC_U(unsigned int ID,
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
    m_parsingType=SHDLC_U;
}

int parsingSHDLC_U::parse() {
    if (m_data==NULL)
        return 1;
    m_lineNum=0;
    m_out.m_pOut[m_lineNum]->desc="Control field";
    unsigned char modifier = m_data[0]&0x1F;
    if(modifier == 0x19) {
        m_out.m_pOut[m_lineNum]->meaning="Reset of the data link layer";
    }
    else if (modifier == 0x06) {
        m_out.m_pOut[m_lineNum]->meaning="Unnumbered Acknowledgement";
    }
    m_out.m_pOut[m_lineNum]->sequence="SWP U-Frame";
    ParsingSWP::lineOutput(5,6);

    if (modifier == 0x19) {
        //line .1
        if (m_bytes>1) {
            m_out.m_pOut[m_lineNum]->sequence="";
            m_out.m_pOut[m_lineNum]->desc="Window size";
            stringstream meaningStream;
            meaningStream<<"Sliding window size: "<<(int)m_data[1];
            if ((int)m_data[1]>4 || (int)m_data[1]<2) {
                meaningStream<<" Error!";
            }
            m_out.m_pOut[m_lineNum]->meaning=meaningStream.str();
            ParsingSWP::lineOutput(6,7);

            //line .2
            if (m_bytes>2) {
                m_out.m_pOut[m_lineNum]->sequence="";
                m_out.m_pOut[m_lineNum]->desc="Endpoint capabilities";
                if (m_data[2]&0x01) {
                    m_out.m_pOut[m_lineNum]->meaning
                            ="Selective Reject supported";
                }
                else {
                    m_out.m_pOut[m_lineNum]->meaning
                            ="Selective Reject Not supported";
                }
                ParsingSWP::lineOutput(7,8);
            }
        }
    }

    return 0;
}
