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
* File Name: parsingfactory.cpp
*
* Abstract:  This class defines which class the packet should follow.
*
* Functions:
*
*******************************************************************************/
#include "parsingfactory.h"

extern int nciParser;
extern int swpParser;
extern int rfParser;
extern bool m_skipMsgHdr_I[2];

ParsingFactory::ParsingFactory()
{
}

bool ParsingFactory::swpSameType(unsigned char type=0)
{
    m_swpParsingType=(type>>5) & 0x07;

    if (m_swpParsingType==3){
        m_skipMsgHdr_I[0]=false;
        m_skipMsgHdr_I[1]=false;
    }

    if (m_swpParsingType==4) {
        m_swpParsingType=5;
    }
    else if (m_swpParsingType==0) {
        m_swpParsingType=1;
    }

    if (m_swpPrevParType==m_swpParsingType) {
        return true;
    }
    else {
        m_swpPrevParType=m_swpParsingType;
        return false;
    }
}

ParsingFactory::~ParsingFactory(){
    m_swpPrevParType=PAR_TYPE_UNDEFINED;
    m_swpParsingType=PAR_TYPE_UNDEFINED;
}

Parsing * ParsingFactory::createParsing(unsigned int ID,
                                        unsigned int byteLength,
                                        unsigned char rawData[],
                                        Parsing * &p, unsigned int direction
                                        , ImportMessages* pIm, char* pFilepath){
      if ((ID==0x6f20) && swpParser) {
        if (!swpSameType(rawData[5])) {
            delete p;
            switch (m_swpParsingType) {
            case 0:
            case 1: p= new parsingRFU(ID,byteLength,rawData,pFilepath
                                      ); break;
            case 2: p= new parsingCLT(ID,byteLength,rawData,pFilepath
                                      ); break;
            case 3: p= new parsingACT(ID,byteLength,rawData,pFilepath
                                      ); break;
            case 4:
            case 5: p= new parsingSHDLC_I(ID,byteLength,rawData,pFilepath
                                          ); break;
            case 6: p= new parsingSHDLC_S(ID,byteLength,rawData,pFilepath
                                          ); break;
            case 7: p= new parsingSHDLC_U(ID,byteLength,rawData,pFilepath
                                          ); break;
            }
        }
        else {
            //update the value in p
            p->dataUpdate(ID,byteLength,rawData);
        }
    }
    else if (ID==0x6F09 && rfParser){
        delete p;
        m_swpPrevParType=PAR_TYPE_UNDEFINED;
        m_swpParsingType=PAR_TYPE_UNDEFINED;
        p=new ParsingRFFrame(ID,byteLength,rawData,pFilepath
                           );

    }
    else if(ID!=0x6F09 && ID!=0x6F1E && nciParser){
        delete p;
        m_swpPrevParType=PAR_TYPE_UNDEFINED;
        m_swpParsingType=PAR_TYPE_UNDEFINED;
        TYPE_PACKET_LINKED_LIST_t * pPacket=pIm->getStructType(ID);
        if (ID==0) {
            //Template: NCI_SPECIFIC_DATA
            p=new ParsingSpecificData(ID,byteLength,rawData,pFilepath
                                      );
        }
        else if ( ((ID>>13)&0x07) == 0) {
            //NCI_DATA_RECEIVE
            p=new ParsingNciData(ID,byteLength,rawData,
                                 direction,pFilepath
                                 );
        }

        else if (ID==0x2F16) {//else if ( ((ID>>13)&0x07) == 1) {
            //ENCAPSULATE_NCI_CMD
            p=new ParsingNciDataCmd(ID,byteLength,rawData,pFilepath
                                    );
        }
        else if (ID==0x2F22) {
            //ACCESS_APP_CMD
            p=new ParsingAppCmd(ID,byteLength,rawData,pFilepath
                                );
        }
        else if (pPacket!=NULL && pPacket->tmplt=="NCI_CMD_NOTIF") {
            //NCI_CMD_NOTIF
            p=new ParsingNciCmdNotif(ID,byteLength,rawData
                                     ,pPacket,pFilepath);
        }
        else {
            p=new Parsing(ID,byteLength,rawData,pFilepath
                          );
        }
    }
    else
    {
        delete p;
        p = NULL;
    }

    return p;
}
