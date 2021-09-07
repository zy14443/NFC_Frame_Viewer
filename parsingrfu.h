#ifndef PARSINGRFU_H
#define PARSINGRFU_H
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
* File Name: parsingrfu.h
*
* Abstract:  The is the class for SWP - RFU frame.
*            This class inherits ParsingSWP.
*
* Functions:
*
*******************************************************************************/

#include "parsingswp.h"

class parsingRFU : public ParsingSWP
{
public:
    parsingRFU(unsigned int ID,
               unsigned int byteLength, unsigned char rawData[]
//           #ifdef FILE_OUTPUT
               , char * pFilePath
//           #endif
               );

protected:
    virtual int parse();
};

#endif // PARSINGRFU_H
