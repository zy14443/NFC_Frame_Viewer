#ifndef PARSINGFACTORY_H
#define PARSINGFACTORY_H
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
* File Name: parsingfactory.h
*
* Abstract:  This class defines which class the packet should follow.
*
* Functions:
*
*******************************************************************************/

/*
  The Class Structure for the parser:
    <>Parsing (Base Class)
        <>ParsingSWP (ID=0x6f1e)    (Inherits Class Parsing)
            <>ParsingRFU                        (Inherits Class ParsingSWP)
            <>ParsingCLT                        (Inherits Class ParsingSWP)
            <>ParsingACT                        (Inherits Class ParsingSWP)
            <>ParsingSHDLC                      (Inherits Class ParsingSWP)
        <>ParsingNCI                (Inherits Class Parsing)
            <>ParsingNciData                    (Inherits Class ParsingNCI)
            <>ParsingNciDataCmd                 (Inherits Class ParsingNCI)
            <>ParsingCmdNotif                   (Inherits Class ParsingNCI)
            <>ParsingSpecificData               (Inherits Class ParsingNCI)
            <>ParsingAppCmd                     (Inherits Class ParsingNCI)
            <>ParsingRFFrame                    (Inherits Class ParsingNCI)

 */
#include "importmessages.h"
#include "parsing.h"
#include "parsingswp.h"
#include "parsingrfu.h"
#include "parsingact.h"
#include "parsingclt.h"
#include "parsingshdlc_u.h"
#include "parsingshdlc_s.h"
#include "parsingshdlc_i.h"
#include "parsingncidata.h"
#include "parsingncidatacmd.h"
#include "parsingncicmdnotif.h"
#include "parsingspecificdata.h"
#include "parsingappcmd.h"
#include "parsingrfframe.h"

#include "parsermacros.h"

class ParsingFactory
{
public:
    unsigned char m_swpPrevParType;
    unsigned char m_swpParsingType;
    ParsingFactory();
    ParsingFactory(unsigned int ID,
                   unsigned int byteLength, unsigned char rawData[]);
    bool swpSameType(unsigned char type);
    Parsing * createParsing(unsigned int ID,
                            unsigned int byteLength, unsigned char rawData[],
                            Parsing * &p, unsigned int direction

                            , ImportMessages * pIm, char * pFilePath);
    ~ParsingFactory();
};

#endif // PARSINGFACTORY_H
