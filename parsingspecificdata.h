#ifndef PARSINGSPECIFICDATA_H
#define PARSINGSPECIFICDATA_H
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
* File Name: parsingspecificdata.h
*
* Abstract:  This is the class for NCI frame - template NCI_SPECIFIC_DATA.
*            This class inherits ParsingNCI.
*
* Functions:
*
*******************************************************************************/

#include "parsing.h"
#include "parsingnci.h"

class ParsingSpecificData : public ParsingNCI
{
public:
    ParsingSpecificData(unsigned int ID,
                        unsigned int byteLength, unsigned char rawData[]
//                    #ifdef FILE_OUTPUT
                        , char * pFilePath
//                    #endif
                        );
    virtual int parse();
private:
    int m_PBF;
};

#endif // PARSINGSPECIFICDATA_H
