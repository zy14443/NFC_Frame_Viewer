#ifndef PARSINGRFFRAME_H
#define PARSINGRFFRAME_H
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
* File Name: parsingrfframe.h
*
* Abstract:  The is the class for NCI frame with the name of NFC_FRAME_LOG_NTF.
*            This class inherits ParsingNCI.
*
* Functions:
*
*******************************************************************************/

#include "parsing.h"
#include "parsingnci.h"
#include "importmessages.h"

class ParsingRFFrame : public ParsingNCI
{
public:
    ParsingRFFrame(unsigned int ID,
                       unsigned int byteLength, unsigned char rawData[]
//                   #ifdef FILE_OUTPUT
                       , char * pFilePath
//                   #endif
                   );
    virtual int parse();
private:
    int m_MT;
    int m_PBF;
    int m_GID;
    int m_OID;
    int m_TimeStamp;
    int m_Protocol;
    int m_Bytelen;
    int m_Bitlen;
    int m_dir;
};


#endif // PARSINGRFFRAME_H
