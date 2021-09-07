#ifndef PARSINGSHDLC_I_H
#define PARSINGSHDLC_I_H
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
* File Name: parsingshdlc_i.h
*
* Abstract:  The is the class for SWP - SHDLC - I frame.
*            This class inherits ParsingSWP.
*
* Functions:
*
*******************************************************************************/

#include "parsingswp.h"
#include <string>

using namespace std;


class parsingSHDLC_I : public ParsingSWP
{
public:
    parsingSHDLC_I(unsigned int ID,
                   unsigned int byteLength, unsigned char rawData[]
//               #ifdef FILE_OUTPUT
                   , char * pFilePath
//               #endif
                   );


protected:
    //for I-frame:
    virtual int parse();
    string getInstructionString_I(int type, int instruction);
};

#endif // PARSINGSHDLC_I_H
