#ifndef PARSINGNCI_H
#define PARSINGNCI_H
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
* File Name: parsingnci.h
*
* Abstract:  The is the class for NCI frame.
*            This class inherits Parsing.
*            All the NCI packets should inherits this class.
*
* Functions:
*
*******************************************************************************/

#include "parsing.h"

class ParsingNCI : public Parsing
{
public:
    ParsingNCI();
    ParsingNCI(unsigned int ID,
               unsigned int byteLength, unsigned char rawData[]
//           #ifdef FILE_OUTPUT
               , char * pFilePath
//           #endif
               );
    ~ParsingNCI();
    virtual int parse();
    string getCtrlMsgName(int GID, int OID, int MT);
};

#endif // PARSINGNCI_H
