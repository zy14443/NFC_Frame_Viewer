#ifndef PARSINGAPPCMD_H
#define PARSINGAPPCMD_H
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
* File Name: parsingappcmd.h
*
* Abstract:  The is the class for NCI - template NCI_APP_CMD.
*            This class inherits ParsingNCI.
*
* Functions:
*
*******************************************************************************/

#include "parsing.h"
#include "parsingnci.h"

class ParsingAppCmd : public ParsingNCI
{
public:
    ParsingAppCmd(unsigned int ID,
                  unsigned int byteLength, unsigned char rawData[]
//              #ifdef FILE_OUTPUT
                  , char * pFilePath
//              #endif
                  );
    virtual int parse();
private:
    int m_MT;
    int m_PBF;
    int m_GID;
    int m_OID;
    int m_APPType;
    int m_APPDest;
    int m_APPOper;
    int m_OFFSET;
};

#endif // PARSINGAPPCMD_H

