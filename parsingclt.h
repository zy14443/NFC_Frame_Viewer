#ifndef PARSINGCLT_H
#define PARSINGCLT_H
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
* File Name: parsingclt.h
*
* Abstract:  The is the class for SWP - CLT frame.
*            This class inherits ParsingSWP.
*
* Functions:
*
*******************************************************************************/

#include "parsingswp.h"

class parsingCLT : public ParsingSWP
{
public:
    parsingCLT(unsigned int ID,
               unsigned int byteLength, unsigned char rawData[]
//           #ifdef FILE_OUTPUT
               , char * pFilePath
//           #endif
               );

protected:
    virtual int parse();
};

#endif // PARSINGCLT_H
