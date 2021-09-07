#ifndef PARSINGSHDLC_U_H
#define PARSINGSHDLC_U_H
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
* File Name: parsingshdlc_u.h
*
* Abstract:  The is the class for SWP - SHDLC - U frame.
*            This class inherits ParsingSWP.
*
* Functions:
*
*******************************************************************************/

#include "parsingswp.h"

class parsingSHDLC_U : public ParsingSWP
{
public:
    parsingSHDLC_U(unsigned int ID,
                   unsigned int byteLength, unsigned char rawData[]
//               #ifdef FILE_OUTPUT
                   , char * pFilePath
//               #endif
                   );

protected:
    virtual int parse();
};

#endif // PARSINGSHDLC_U_H
