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
* File Name: parsingncidata.h
*
* Abstract:  This is the class for NCI frame - data packets.
*            This class inherits ParsingNCI.
*            This class deals packets from templates:
*                   NCI_DATA_SEND
*                   NCI_DATA_RECEIVE
*
* Functions:
*
*******************************************************************************/

#ifndef PARSINGDATARECEIVE_H
#define PARSINGDATARECEIVE_H

#include "parsing.h"
#include "parsingnci.h"

class ParsingNciData : public ParsingNCI
{
public:
    ParsingNciData(unsigned int ID,
                   unsigned int byteLength, unsigned char rawData[],
                   unsigned int direction
//               #ifdef FILE_OUTPUT
                   , char * pFilePath
//               #endif
                   );
    virtual int parse();
private:
    int m_PBF;
    int m_CREDIT;
};

#endif // PARSINGDATARECEIVE_H
