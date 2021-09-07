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
* File Name: parsingshdlc_i.cpp
*
* Abstract:  The is the class for SWP - SHDLC - I frame.
*            This class inherits ParsingSWP.
*
* Functions:
*
*******************************************************************************/
#include "parsingshdlc_i.h"
#include <sstream>

bool m_skipMsgHdr_I [2] = {false,false};

parsingSHDLC_I::parsingSHDLC_I(unsigned int ID,
                               unsigned int byteLength,
                               unsigned char rawData[]
//                               #ifdef FILE_OUTPUT
                               , char * pFilePath
//                               #endif
                               )
    : ParsingSWP::ParsingSWP(ID,byteLength,rawData
//          #ifdef FILE_OUTPUT
          , pFilePath
//          #endif
          )
{
    m_parsingType=SHDLC_I;
}

int parsingSHDLC_I::parse() {
    if (m_data==NULL)
        return 1;
    m_lineNum=0;
    m_out.m_pOut[m_lineNum]->desc="Control field";

    int Ns, Nr;
    Ns=(int)((m_data[0]>>3)&0x07);
    Nr=(int)(m_data[0]&0x07);

    string strInstruction = "";
    int type=2, instruction;

    stringstream meaningStream;
    meaningStream<<"N(S):"<<Ns<<", N(R):"<<Nr<<"; ";
    string s_type[4]={"Command","Event","Response","RFU"};

    if (m_skipMsgHdr_I[m_dir]==false){
        type=(int)((m_data[2]>>6) & 0x3);
        instruction=(int)(m_data[2] & 0x3f);

        strInstruction = parsingSHDLC_I::getInstructionString_I(type,instruction);
        meaningStream<<strInstruction;
    }

    m_out.m_pOut[m_lineNum]->meaning = meaningStream.str();
    m_out.m_pOut[m_lineNum]->sequence="SWP I-Frame";
    ParsingSWP::lineOutput(5,6);
    if (m_bytes==1)
        return 0;
    int cb;
    int pid;
    cb = (int)((m_data[1]>>7) & 0x1);
    pid = (int)(m_data[1] & 0x7f);


    //display the detailed parsing
    //line .1
    meaningStream.str("");
    m_out.m_pOut[m_lineNum]->sequence="HCP packet";
    m_out.m_pOut[m_lineNum]->desc="Packet Header";
    meaningStream<<"Chaining bit: "<<cb<<" Pipe ID: "<<pid;
    m_out.m_pOut[m_lineNum]->meaning = meaningStream.str();
    ParsingSWP::lineOutput(6,7);
    //line .1 end

    if (m_skipMsgHdr_I[m_dir]==false) {
        //line .2 //HCP message //Message header
        meaningStream.str("");
        m_out.m_pOut[m_lineNum]->sequence="HCP message";
        m_out.m_pOut[m_lineNum]->desc="Message header";
        meaningStream<<"Type: "<<s_type[type]<<" Instruction: "
                    <<strInstruction;
        m_out.m_pOut[m_lineNum]->meaning = meaningStream.str();
        ParsingSWP::lineOutput(7,8);
        //line .2 end

        //line .3 //m_data
        if (m_bytes>3) {
            m_out.m_pOut[m_lineNum]->desc="Message Data";
            ParsingSWP::lineOutput(8,m_bytes+5);
        }
        //line .3 end
    }
    else {
        if (cb==1) {
            m_skipMsgHdr_I[m_dir]=false;
        }
        //line .2&3 //HCP message //Message header
        m_out.m_pOut[m_lineNum]->sequence="HCP message fragment";
        m_out.m_pOut[m_lineNum]->desc="Message Data";
        if (m_bytes>2) {
            m_out.m_pOut[m_lineNum]->desc="Message Data";
            ParsingSWP::lineOutput(7,m_bytes+5);
        }
        //line .2&3 end
    }

    if (cb==0){
       m_skipMsgHdr_I[m_dir] = true;
    }

    return 0;
}


string parsingSHDLC_I::getInstructionString_I(int type, int instruction) {
    if (type==0) {//"Command"
        switch (instruction) {
        case 1: return "ANY_SET_PARAMETER"; break;
        case 2: return "ANY_GET_PARAMETER"; break;
        case 3: return "ANY_OPEN_PIPE"; break;
        case 4: return "ANY_CLOSE_PIPE"; break;
        case 16: return "ADM_CREATE_PIPE"; break;//0x10
        case 17: return "ADM_DELETE_PIPE"; break;//0x11
        case 18: return "ADM_NOTIFY_PIPE_CREATED"; break;//0x12
        case 19: return "ADM_NOTIFY_PIPE_DELETED"; break;//0x13
        case 20: return "ADM_CLEAR_ALL_PIPE"; break;//0x14
        case 21: return "ADM_NOTIFY_ALL_PIPE_CLEARED"; break;//0x15
        default: return "RFU"; break;
        }
    }
    else if(type==2) {//"Response"
        switch (instruction) {
        case 0: return "ANY_OK"; break;
        case 1: return "ANY_E_NOT_CONNECTED"; break;
        case 2: return "ANY_E_CMD_PAR_UNKNOWN"; break;
        case 3: return "ANY_E_NOK"; break;
        case 4: return "ADM_E_NO_PIPES_AVAILABLE"; break;
        case 5: return "ANY_E_REG_PAR_UNKNOWN"; break;
        case 6: return "ANY_E_PIPE_NOT_OPENED"; break;
        case 7: return "ANY_E_CMD_NOT_SUPPORTED"; break;
        case 8: return "ANY_E_INHIBITED"; break;
        case 9: return "ANY_E_TIMEOUT"; break;
        case 10: return "ANY_E_REG_ACCESS_DENIED"; break;//0x0A
        case 11: return "ANY_E_PIPE_ACCESS_DENIED"; break;//0x0B
        default: return "RFU"; break;
        }
    }
    else if (type == 1) {//"Event"
        switch (instruction) {
        case 1: return "EVT_HCI_END_OF_OPERATION"; break;
        case 2: return "EVT_POST_DATA"; break;
        case 3: return "EVT_HOT_PLUG"; break;
            //possible events->
        case 0x10: return "possible EVT_SEND_DATA"; break;
        case 0x11: return "possible EVT_FIELD_ON"; break;
        case 0x12: return "possible EVT_CARD_DEACTIVATED"; break;
        case 0x13: return "possible EVT_CARD_ACTIVATED"; break;
        case 0x14: return "possible EVT_FIELD_OFF"; break;
        default: return "RFU"; break;
        }
    }
    else {//"RFU"
        switch (instruction) {
        default: return "RFU"; break;
        }
    }
}
