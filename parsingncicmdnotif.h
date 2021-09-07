#ifndef PARSINGNCICMDNOTIF_H
#define PARSINGNCICMDNOTIF_H
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
* File Name: parsingncicmdnotif.h
*
* Abstract:  The is the class for NCI frame - template NCI_CMD_NOTIF.
*            This class inherits ParsingNCI.
*
* Functions:
*
*******************************************************************************/

#include "parsing.h"
#include "parsingnci.h"
#include "importmessages.h"

class ParsingNciCmdNotif : public ParsingNCI
{
public:
public:
    ParsingNciCmdNotif(unsigned int ID,
                       unsigned int byteLength, unsigned char rawData[],
//                   #ifdef FILE_OUTPUT

//                   #endif
                       TYPE_PACKET_LINKED_LIST_t* pPacket=0, char * pFilePath=NULL);
    virtual int parse();
private:
    int m_MT;
    int m_PBF;
    int m_GID;
    int m_OID;
    TYPE_PACKET_LINKED_LIST_t* m_pPacket;
    VAR_LINKED_LIST_t * m_pCurrVar;
};

#endif // PARSINGNCICMDNOTIF_H
