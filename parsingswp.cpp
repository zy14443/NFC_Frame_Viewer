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
* File Name: parsingswp.cpp
*
* Abstract:  The is the class for SWP frame.
*            This class inherits Parsing.
*            All the SWP packets should inherits this class.
*
* Functions:
*
*******************************************************************************/

#include "parsingswp.h"
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

ParsingSWP::ParsingSWP(unsigned int ID,
                       unsigned int byteLength, unsigned char rawData[]
//                       #ifdef FILE_OUTPUT
                       , char * pFilePath
//                       #endif
                       )
    : Parsing::Parsing(ID,byteLength,rawData
//          #ifdef FILE_OUTPUT
          , pFilePath
//          #endif
          )
{
    m_parsingType=SWP;
    m_out.nciConnID=NCI_CONN_ID_SKIP;

    //first byte in raw data isTx
    m_dir=m_pDataPacket->m_rawData[0]&(0x80);

    //second byte in raw data swpIndx
    m_out.swpIdx=m_pDataPacket->m_rawData[0]&(0x0F);

    //m_data stores raw swp data
    if (m_pDataPacket->m_byteLength>5) {
        m_data=new unsigned char [m_pDataPacket->m_byteLength-5];
    }

    //m_bytes stores swp data length
    m_bytes=m_pDataPacket->m_byteLength-5;

    //copy data
    for (int i=0; i<m_bytes; i++) {
        m_data[i]=m_pDataPacket->m_rawData[i+5];
    }

    if(m_dir==0) {
        m_out.direction='<';
    }
    else {
        m_out.direction='>';
    }
}

ParsingSWP::~ParsingSWP()
{
}

int ParsingSWP::parse() {
    return 0;
}

void ParsingSWP::dataUpdate(unsigned int ID,
                            unsigned int byteLength, unsigned char rawData[]) {
    Parsing::dataUpdate(ID,byteLength,rawData);
    m_dir=m_pDataPacket->m_rawData[0]&(0x80);
    if (m_pDataPacket->m_byteLength>5) {
        m_data=new unsigned char [m_pDataPacket->m_byteLength-5];
    }
    m_bytes=m_pDataPacket->m_byteLength-5;
    for (int i=0; i<m_bytes; i++) {
        m_data[i]=m_pDataPacket->m_rawData[i+5];
    }
    m_out.swpIdx=m_pDataPacket->m_rawData[0]&(0x0F);
    if(m_dir==0) {
        m_out.direction='<';
    }
    else {
        m_out.direction='>';
    }
}




