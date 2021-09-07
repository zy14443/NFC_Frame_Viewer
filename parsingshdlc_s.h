#ifndef PARSINGSHDLC_S_H
#define PARSINGSHDLC_S_H
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
* File Name: parsingshdlc_s.h
*
* Abstract:  The is the class for SWP - SHDLC - S frame.
*            This class inherits ParsingSWP.
*
* Functions:
*
*******************************************************************************/

#include "parsingswp.h"

class parsingSHDLC_S : public ParsingSWP
{
public:
    parsingSHDLC_S(unsigned int ID,
                   unsigned int byteLength, unsigned char rawData[]
//               #ifdef FILE_OUTPUT
                   , char * pFilePath
//               #endif
                   );

protected:
    virtual int parse();
};

#endif // PARSINGSHDLC_S_H
