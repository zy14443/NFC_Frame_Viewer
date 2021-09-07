#ifndef PARSINGSWP_H
#define PARSINGSWP_H
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
* File Name: parsingswp.h
*
* Abstract:  The is the class for SWP frame.
*            This class inherits Parsing.
*            All the SWP packets should inherits this class.
*
* Functions:
*
*******************************************************************************/

#include "string.h"
#include "parsing.h"
#include <fstream>

using namespace std;

class ParsingSWP : public Parsing
{
public:
    ParsingSWP();
    ParsingSWP(unsigned int ID,
               unsigned int byteLength, unsigned char rawData[]
//           #ifdef FILE_OUTPUT
               , char * pFilePath
//           #endif
               );
    virtual void dataUpdate(unsigned int ID,
                            unsigned int byteLength, unsigned char rawData[]);

    virtual int parse();
     ~ParsingSWP();

protected:
    int m_dir; //isTx
    unsigned char * m_data; //stores swp raw data
    int m_bytes; //stores swp data length
};

#endif // PARSINGSWP_H
