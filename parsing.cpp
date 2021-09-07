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
* File Name: parsing.cpp
*
* Abstract:  The is the base class.
*            All the other class should inherit this base class.
*
* Functions:
*
*******************************************************************************/

#include "parsing.h"
#include <QString>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

Parsing::Parsing()
{
    m_parsingType=BASE_PARSING_TYPE;
    m_lineNum=0;
    m_subIndex=0;
    m_out.m_pOut[m_lineNum]=new LINE_OUTPUT_t;
    m_pDataPacket=NULL;
    m_pFileStream=NULL;
}

Parsing::Parsing(unsigned int ID,
                 unsigned int byteLength, unsigned char rawData[],
                 char * pFilePath=NULL)
{
    m_parsingType=BASE_PARSING_TYPE;

    m_pFileStream=NULL;
    if (pFilePath) {
        m_pFileStream=new ofstream;
        m_pFileStream->open(pFilePath, ios::out | ios::app);
    }

    m_lineNum=0;
    m_subIndex=0;
    m_out.m_pOut[m_lineNum]=new LINE_OUTPUT_t;
    m_pDataPacket=NULL;
    m_pDataPacket=new pDataPacket;
//  m_pDataPacket->m_fixed=0x00;
    m_pDataPacket->m_ID=ID;
    m_pDataPacket->m_byteLength=byteLength;
    m_pDataPacket->m_rawData=rawData;
}

int Parsing::parse() {
    return 0;
}

Parsing:: ~Parsing() {

    for (unsigned int i=0; i<=m_lineNum; i++) {
        delete(m_out.m_pOut[i]);
    }
    if (m_pDataPacket)
        delete m_pDataPacket;
    m_pDataPacket=NULL;

    if (m_pFileStream) {
        m_pFileStream->close();
        delete m_pFileStream;
    }
}

void Parsing::init(unsigned int index, QString timestamp) {
    m_index=index;
    m_out.timestamp=timestamp;
}

void Parsing::dataUpdate(unsigned int ID,
                         unsigned int byteLength, unsigned char rawData[]) {
    m_subIndex=0;
    if (!m_pDataPacket)
        m_pDataPacket=new pDataPacket;
  //m_pDataPacket->m_fixed=0x00;
    m_pDataPacket->m_ID=ID;
    m_pDataPacket->m_byteLength=byteLength;
    m_pDataPacket->m_rawData=rawData;
}

unsigned int Parsing::getLineNum() {
    return m_lineNum;
}

int Parsing::lineOutput(int start, int end, bool outputAll){

    int count = 0;
    stringstream idxStringStream;
    if (m_lineNum==0) {
        idxStringStream<<dec<<m_index;
    }
    else {
        m_subIndex++;
        idxStringStream<<dec<<m_index<<"."<<m_subIndex;
    }
    m_out.m_pOut[m_lineNum]->idxString=idxStringStream.str();

    stringstream valueStream;

    if (start == OUTPUT_ID || outputAll){
        valueStream.fill('0');
        valueStream.width(2);
        valueStream<<hex<<uppercase<<right
                  <<((int)(m_pDataPacket->m_ID >> 8))<<" ";
        valueStream.fill('0');
        valueStream.width(2);
        valueStream<<hex<<uppercase<<right
                  <<((int)(m_pDataPacket->m_ID & 0xff))<<" ";
        count += 2;
    }
    if (outputAll) {
        valueStream.fill('0');
        valueStream.width(2);
        valueStream<<hex<<uppercase<<right
                  <<(int)m_pDataPacket->m_byteLength<<" ";
        count++;

    }
    if ( (start>=0 && end>=start) || outputAll) {
        for (int i=start; i<end; i++,count++) {
            if (count%16==0 && count>0)
                valueStream<<"\n";
            valueStream.fill('0');
            valueStream.width(2);
            valueStream<<hex<<uppercase<<right
                      <<((int)m_pDataPacket->m_rawData[i])<<" ";
        }
    }
    m_out.m_pOut[m_lineNum]->value=valueStream.str();
//#ifdef FILE_OUTPUT
    if (m_pFileStream) {

        if(m_lineNum == 0){
            (*m_pFileStream)<<m_out.m_pOut[m_lineNum]->idxString<<"\n";
        }

        (*m_pFileStream)<<m_out.timestamp.toStdString()<<"\t";
        (*m_pFileStream)<<m_out.m_pOut[m_lineNum]->sequence<<"\t";
        if(m_out.m_pOut[m_lineNum]->sequence=="")
            (*m_pFileStream)<<"\t\t\t\t\t";
        (*m_pFileStream)<<m_out.direction<<"\t";
        if (m_out.swpIdx!=SWPIDX_SKIP) {
            (*m_pFileStream)<<m_out.swpIdx;
        }
        (*m_pFileStream)<<"\t";
        if (m_out.nciConnID!=NCI_CONN_ID_SKIP) {
            (*m_pFileStream)<<m_out.nciConnID;
        }
        (*m_pFileStream)<<"\t";
        (*m_pFileStream)<<m_out.m_pOut[m_lineNum]->desc<<"\t";
        (*m_pFileStream)<<m_out.m_pOut[m_lineNum]->value;
        (*m_pFileStream)<<"\t"<<m_out.m_pOut[m_lineNum]->meaning<<"\n\n";
    }
//#endif
    if (start==0) {
        m_subIndex=0;
    }
    m_lineNum++;
    m_out.m_pOut[m_lineNum]=new LINE_OUTPUT_t;
    return 0;
}
