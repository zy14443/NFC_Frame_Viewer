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
* File Name: parsingncicmdnotif.cpp
*
* Abstract:  The is the class for NCI frame - template NCI_CMD_NOTIF.
*            This class inherits ParsingNCI.
*
* Functions:
*
*******************************************************************************/

#include "parsingncicmdnotif.h"
#include <sstream>
#include <QList>

ParsingNciCmdNotif::ParsingNciCmdNotif(unsigned int ID,
                                       unsigned int byteLength,
                                       unsigned char rawData[]
//                                       #ifdef FILE_OUTPUT

//                                       #endif
                                       , TYPE_PACKET_LINKED_LIST_t *pPacket, char * pFilePath)
    : ParsingNCI::ParsingNCI(ID,byteLength,rawData
//          #ifdef FILE_OUTPUT
          , pFilePath
//          #endif
          )
{
    m_parsingType=NCI_CMD_NOTIF;
    m_out.direction=pPacket->direction;
    m_pPacket=pPacket;
    m_out.swpIdx=SWPIDX_SKIP;//no swpIdx
    m_out.nciConnID = NCI_CONN_ID_SKIP;//no conn ID
    m_MT = (int)( (m_pDataPacket->m_ID >> 13) & 0x07);
    m_PBF = (int)( (m_pDataPacket->m_ID >> 12) & 0x01);
    m_GID = (int)( (m_pDataPacket->m_ID >> 8) & 0x0f);
    m_OID = (int)( (m_pDataPacket->m_ID) & 0x3f);
}

int ParsingNciCmdNotif::parse() {
    m_lineNum=0;
    m_out.m_pOut[m_lineNum]->desc="Packet Header";
    m_out.m_pOut[m_lineNum]->sequence="NCI-Frame";

    m_out.m_pOut[m_lineNum]->meaning=m_pPacket->name.toStdString();

    string msgName=getCtrlMsgName(m_GID,m_OID,m_MT);

    stringstream meaningStream;
    meaningStream<<".  ";
//    meaningStream<<"Msg Type: 0x"<<hex<<uppercase<<m_MT<<". ";
//    meaningStream<<"PBF: 0x"<<hex<<uppercase<<m_PBF<<". ";
//    meaningStream<<"GID: 0x"<<hex<<uppercase<<m_GID<<". ";
//    meaningStream<<"OID: 0x"<<hex<<uppercase<<m_OID<<". ";
    m_out.m_pOut[m_lineNum]->meaning += meaningStream.str();

    Parsing::lineOutput(OUTPUT_ID,OUTPUT_ID);

//    Parsing::lineOutput(0,2,true);

//    //line .1
//    stringstream meaningStream;
//    meaningStream<<msgName<<". ";
//    meaningStream<<"Msg Type: 0x"<<hex<<uppercase<<m_MT<<". ";
//    meaningStream<<"PBF: 0x"<<hex<<uppercase<<m_PBF<<". ";
//    meaningStream<<"GID: 0x"<<hex<<uppercase<<m_GID<<". ";
//    meaningStream<<"OID: 0x"<<hex<<uppercase<<m_OID<<". ";
//    m_out.m_pOut[m_lineNum]->sequence="";
//    m_out.m_pOut[m_lineNum]->desc="Packet Header";
//    m_out.m_pOut[m_lineNum]->meaning=meaningStream.str();


    //line .2
    if (m_pDataPacket->m_byteLength>0) {
        meaningStream.str("");
        m_out.m_pOut[m_lineNum]->sequence="";
        m_out.m_pOut[m_lineNum]->desc="Packet Data";
        m_pCurrVar = m_pPacket->pVarListHead;
        int currIndex=0;
        VAR_LINKED_LIST_t * pTempVar;
        int tempIndex=0;
        QList<int> arraySize;
        while (m_pCurrVar!=NULL) {
            bool present=true;
            pTempVar=m_pPacket->pVarListHead;
            arraySize.append(1);
            if (m_pCurrVar->condition.isDefault==false) {
                tempIndex=0;
                for (int i=0; pTempVar!=NULL && i<arraySize.length(); i++) {
                    if (m_pCurrVar->condition.var==pTempVar->dataName) {
                        int tempLeft=(int)m_pDataPacket->m_rawData[tempIndex];
                        if (m_pCurrVar->condition.cmp=='>') {
                            present = tempLeft > m_pCurrVar->condition.val;
                        }
                        else if (m_pCurrVar->condition.cmp=='<'){
                            present = tempLeft < m_pCurrVar->condition.val;
                        }
                        break;
                    }
                    tempIndex+=pTempVar->dataByte*arraySize.at(i);
                    pTempVar=pTempVar->pNext;
                }
            }
            else {
                present=true;
            }
            if (present) {
                if (m_pCurrVar->arraySizeStr=="default") {
                    meaningStream<<m_pCurrVar->dataName.toStdString()<<": 0x";
                    for (int j=0; j<m_pCurrVar->dataByte; j++) {
                        if (currIndex+j<(int)m_pDataPacket->m_byteLength) {
                            meaningStream.fill('0');
                            meaningStream.width(2);
                            meaningStream<<hex<<uppercase
                                        <<(int)m_pDataPacket->m_rawData[
                                          currIndex+j];
                        }
                        else if (m_pCurrVar->defaultValStruct.defaultValExist){
                            meaningStream.fill('0');
                            meaningStream.width(2);
                            meaningStream<<hex<<uppercase
                                        <<(int)m_pCurrVar->
                                          defaultValStruct.defaultVal;
                        }
                    }
                    meaningStream<<". ";
                }
                else {
                    if (m_pCurrVar->arraySizeStr=="TOEND") {
                        arraySize.replace(arraySize.length()-1,
                                          ((m_pDataPacket->m_byteLength
                                            -currIndex)
                                           /m_pCurrVar->dataByte));
                    }
                    else {
                        pTempVar=m_pPacket->pVarListHead;
                        tempIndex=0;
                        for (int i=0;pTempVar!=NULL&&i<arraySize.length();i++){
                            if (m_pCurrVar->arraySizeStr==pTempVar->dataName){
                                arraySize.replace(arraySize.length()-1,
                                                  ((int)m_pDataPacket->
                                                   m_rawData[tempIndex]));
                                break;
                            }
                            tempIndex+=pTempVar->dataByte*arraySize.at(i);
                            pTempVar=pTempVar->pNext;
                        }
                    }
                    if (arraySize.last()>0) {
                        meaningStream<<m_pCurrVar->dataName.toStdString()
                                    <<": 0x";
                        for (int j=0;j<arraySize.last(); j++) {
                            for (int k=0;k<m_pCurrVar->dataByte; k++) {
                                if (currIndex+j*m_pCurrVar->dataByte+k
                                        <(int)m_pDataPacket->m_byteLength) {
                                    meaningStream.fill('0');
                                    meaningStream.width(2);
                                    meaningStream<<hex<<uppercase
                                                <<(int)m_pDataPacket->
                                                  m_rawData[currIndex+
                                                  j*m_pCurrVar->dataByte+k];
                                }
                                else if (m_pCurrVar->defaultValStruct
                                         .defaultValExist) {
                                    meaningStream.fill('0');
                                    meaningStream.width(2);
                                    meaningStream<<hex<<uppercase
                                                <<(int)m_pCurrVar->
                                                  defaultValStruct.defaultVal;
                                }
                            }
                            meaningStream<<" ";
                        }
                        meaningStream<<". ";
                    }
                }
            }
            else {
                arraySize.replace(arraySize.length()-1,0);
            }
            currIndex+=m_pCurrVar->dataByte*arraySize.last();
            if (currIndex>=(int)m_pDataPacket->m_byteLength) {
                break;
            }
            m_pCurrVar=m_pCurrVar->pNext;
        }
        m_out.m_pOut[m_lineNum]->meaning=meaningStream.str();
        Parsing::lineOutput(0,m_pDataPacket->m_byteLength);
    }
    return 0;
}
