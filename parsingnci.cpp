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
* File Name: parsingnci.cpp
*
* Abstract:  The is the class for NCI frame.
*            This class inherits Parsing.
*            All the NCI packets should inherits this class.
*
* Functions:
*
*******************************************************************************/

#include "parsingnci.h"

ParsingNCI::ParsingNCI(unsigned int ID,
                       unsigned int byteLength, unsigned char rawData[]
//                       #ifdef FILE_OUTPUT
                       ,char * pFilePath
//                       #endif
                       )
    : Parsing::Parsing(ID,byteLength,rawData
//          #ifdef FILE_OUTPUT
          , pFilePath
//          #endif
          )
{
    m_parsingType=NCI;
}

ParsingNCI::~ParsingNCI()
{
}

int ParsingNCI::parse() {
    return 0;
}

string ParsingNCI::getCtrlMsgName(int GID, int OID, int MT) {
    string str="";
    string strMT="";
    switch (MT) {
    case 1: strMT="_CMD";break;
    case 2: strMT="_RSP";break;
    case 3: strMT="_NTF";break;
    default: strMT="!error";
    }
    if (GID==0) {
        str="CORE_";
        switch (OID) {
        case 0: return str+"RESET"+strMT;
        case 1: return str+"INIT"+strMT;
        case 2: return str+"SET_CONFIG"+strMT;
        case 3: return str+"GET_CONFIG"+strMT;
        case 4: return str+"DH_CONN"+strMT;
        case 5: return str+"CONN_CLOSE"+strMT;
        case 6: return str+"CONN_CREDITS"+strMT;
        case 7: return str+"GENERIC_ERROR"+strMT;
        case 8: return str+"INTERFACE_ERROR"+strMT;
        default: return "RFU";
        }
    }
    else if (GID==1) {
        str="RF_";
        switch (OID) {
        case 0: return str+"DISCOVER_MAP"+strMT;
        case 1: return str+"SET_LISTEN_MODE_ROUTING"+strMT;
        case 2: return str+"GET_LISTEN_MODE_ROUTING"+strMT;
        case 3: return str+"DISCOVER"+strMT;
        case 4: return str+"DISCOVER_SELECT"+strMT;
        case 5: return str+"INTF_ACTIVATED"+strMT;
        case 6: return str+"DEACTIVATE"+strMT;
        case 7: return str+"FIELD_INFO"+strMT;
        case 8: return str+"T3T_POLLING"+strMT;
        case 9: return str+"NFCEE_ACTION"+strMT;
        case 10: return str+"NFCEE_DISCOVERY_REQ"+strMT;
        case 11: return str+"PAPAMETER_UPDATE"+strMT;
        default: return "RFU";
        }
    }
    else if (GID==2) {
        str="NFCEE_";
        switch (OID) {
        case 0: return str+"DISCOVER"+strMT;
        case 1: return str+"MODE_SET"+strMT;
        default: return "RFU";
        }
    }
    else {
        return "RFU";
    }

}
