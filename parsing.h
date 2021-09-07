#ifndef PARSING_H
#define PARSING_H
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
* File Name: parsing.h
*
* Abstract:  The is the base class.
*            All the other class should inherit this base class.
*
* Functions:
*
*******************************************************************************/

#include <QString>
#include "parsermacros.h"

using namespace std;

class Parsing
{
public:
    typedef struct  {
        string meaning;
        string desc;
        string sequence;
        string value;
        string idxString;
    } LINE_OUTPUT_t;
    struct output {
        int swpIdx;
        int nciConnID;
        QString timestamp;
        char direction;
        LINE_OUTPUT_t *m_pOut[8];
    } m_out;
    unsigned int getLineNum();
    struct pDataPacket {
//      unsigned char m_fixed;
        unsigned int m_ID;
        unsigned int m_byteLength;
        unsigned char * m_rawData;
    } *m_pDataPacket;
    virtual int parse();
    void virtual init(unsigned int index, QString timestamp);
    void virtual dataUpdate(unsigned int ID,
                            unsigned int byteLength, unsigned char rawData[]);
    unsigned int m_parsingType;
    Parsing();
    Parsing(unsigned int ID,
            unsigned int byteLength, unsigned char rawData[],
            char * pFilePath);
    virtual ~Parsing();
protected:
    unsigned int m_index;
    unsigned int m_subIndex;
    unsigned int m_lineNum;
    ofstream * m_pFileStream;
    int lineOutput(int start, int end, bool outputAll=false);
};

#endif // PARSING_H
