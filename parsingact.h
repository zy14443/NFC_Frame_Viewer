#ifndef PARSINGACT_H
#define PARSINGACT_H
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
* File Name: parsingact.h
*
* Abstract:  The is the class for SWP - ACT frame.
*            This class inherits ParsingSWP.
*
* Functions:
*
*******************************************************************************/

#include "parsingswp.h"

class parsingACT : public ParsingSWP
{
public:
    parsingACT(unsigned int ID,
               unsigned int byteLength, unsigned char rawData[]
//           #ifdef FILE_OUTPUT
               , char * pFilePath
//           #endif
               );

protected:
    virtual int parse();
};

#endif // PARSINGACT_H
