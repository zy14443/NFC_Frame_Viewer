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
* File Name: parsingncidatacmd.h
*
* Abstract:  This is the class for NCI frame - Control Packet - Command Message.
*            This class inherits ParsingNCI.
*            This class deals packets from template NCI_DATA_SEND.
*                                          packet ENCAPSULATE_NCI_CMD.
*
* Functions:
*
*******************************************************************************/
#ifndef PARSINGNCIDATACMD_H
#define PARSINGNCIDATACMD_H

#include "parsing.h"
#include "parsingnci.h"

class ParsingNciDataCmd : public ParsingNCI
{
public:
    ParsingNciDataCmd(unsigned int ID,
                      unsigned int byteLength, unsigned char rawData[]
//                  #ifdef FILE_OUTPUT
                      , char * pFilePath
//                  #endif
                      );
    virtual int parse();
private:
    int m_MT;
    int m_PBF;
    int m_GID;
    int m_OID;
};

#endif // PARSINGNCIDATACMD_H
