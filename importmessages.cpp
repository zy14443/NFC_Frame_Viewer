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
* File Name: importmessages.cpp
*
* Abstract:  This class establish the linked list from input .xml file.
*
* Functions:
*
*******************************************************************************/


/* the overview of the linked list structure:

linked list:

header: ---+
           |
   +-------+
   |    +-------------------------------+
   +--->|  TYPE_PACKET_LINKED_LIST_t    |
        |  item1                        |
        +-------------------------------+
        |   name                        |   VAR_LINKED_LIST_t:
        +-------------------------------+       +-----------+
        |   ID                          |   +-->|  dataName |   +--> ...
        +-------------------------------+   |   +-----------+   |
        |   template                    |   |   |  dataByte |   |
        +-------------------------------+   |   +-----------+   |
        |   direction                   |   |   |   ...     |   |
        +-------------------------------+   |   +-----------+   |
        |   VAR_LINKED_LIST_t head      |---+   |   next    |---+
        +-------------------------------+       +-----------+
        |   next                        |---+
        +-------------------------------+   |
   +----------------------------------------+
   |    +-------------------------------+
   +--->|  TYPE_PACKET_LINKED_LIST_t    |
        |  item2                        |
        +-------------------------------+
        |   name                        |   VAR_LINKED_LIST_t:
        +-------------------------------+       +-----------+
        |   ID                          |   +-->|  dataName |   +--> ...
        +-------------------------------+   |   +-----------+   |
        |   template                    |   |   |  dataByte |   |
        +-------------------------------+   |   +-----------+   |
        |   direction                   |   |   |   ...     |   |
        +-------------------------------+   |   +-----------+   |
        |   VAR_LINKED_LIST_t head      |---+   |   next    |---+
        +-------------------------------+       +-----------+
        |   next                        |---+
        +-------------------------------+   |
   +----------------------------------------+
   |
   +---> ...

 */

#include "importmessages.h"
#include <fstream>
#include <QtWidgets>
#include <QMessageBox>
#include <iostream>

using namespace std;

ImportMessages::ImportMessages()
{
    m_pTypePacketHead=NULL;
    m_pCurrPacket=NULL;
    m_pTempPacket=NULL;
    m_pTempVar=NULL;
    m_pCurrVar=NULL;
    packetIndicator = 0;

//#ifdef MSG_XML_LIST
//    m_pFileStream=new ofstream;
//    m_pFileStream->open("msg_xml.dat", ios::out);
//#endif

}

ImportMessages::~ImportMessages() {
    m_pCurrPacket=m_pTypePacketHead;
    while (m_pCurrPacket!=NULL) {
        m_pTempPacket=m_pCurrPacket;
        m_pCurrPacket=m_pCurrPacket->pNext;
        m_pCurrVar=m_pTempPacket->pVarListHead;
        while (m_pCurrVar!=NULL) {
            m_pTempVar=m_pCurrVar;
            m_pCurrVar=m_pCurrVar->pNext;
            delete m_pTempVar;
        }
        delete m_pTempPacket;
    }

//#ifdef MSG_XML_LIST
//    if (m_pFileStream) {
//        m_pFileStream->close();
//        delete m_pFileStream;
//        m_pFileStream=NULL;
//    }
//#endif
}

/********************************************************************************
* bool ImportMessages::import(char * filepath)
*
* This function establish the linked list from input .xml file.
*
* Input: char * filepath -> the input message XML file path
*
* Return: TRUE/FALSE -> succeed/fail
*******************************************************************************/

bool ImportMessages::import(char * filepath) {
    if (filepath==NULL)
        return false;
    QFile file(filepath);
    bool ret = file.open(QIODevice::ReadOnly);
    if (!ret) {
        return ret;
    }
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QTextStream instream(&file);
    QString str;
    while (!instream.atEnd()) {
        str = instream.readLine();

        //Check packetIndicator (0x10/Empty)
        if (str.contains("<TEMPLATE")){
            str = instream.readLine();
            if (str.contains("FIXED"))
                    packetIndicator = 0x10;
            continue;
        }

        //Import NCI CMD/RES/NTF
        if (str.contains("<STRUCTDEF")) {

            //Filter out HCI messages
            if (str.contains("TEMPLATE=\"HCI"))
                continue;

            if (m_pCurrPacket==NULL) {
                m_pTypePacketHead=new TYPE_PACKET_LINKED_LIST_t;
                m_pTypePacketHead->pNext=NULL;
                m_pTypePacketHead->pVarListHead=NULL;
                m_pCurrPacket=m_pTypePacketHead;
            }
            else {
                m_pTempPacket=new TYPE_PACKET_LINKED_LIST_t;
                m_pTempPacket->pNext=NULL;
                m_pTempPacket->pVarListHead=NULL;
                m_pCurrPacket->pNext=m_pTempPacket;
                m_pCurrPacket=m_pCurrPacket->pNext;
            }


            QStringList items;
            items<<"NAME"<<"ID"<<"TEMPLATE"<<"KIND";
            QStringList itemValues;

            //Store NAME,ID,TEMPLATE,KIND into itemValues
            int beginIndex;
            int endIndex=0;
            for (int j=0; j<items.length(); j++) {
                int indexOfName=str.indexOf(items.at(j),endIndex);
                if (indexOfName!=-1) {
                    beginIndex = str.indexOf("\"",indexOfName+
                                             items.at(j).length());
                    endIndex = str.indexOf("\"",beginIndex+1);
                    itemValues << str.mid(beginIndex+1,endIndex-beginIndex-1);
                }
                if (j>=itemValues.length()){
                    itemValues<<QString("");
                }
            }

//#ifdef MSG_XML_LIST
//            if (m_pFileStream!=NULL) {
//                for (int j=0; j<itemValues.length(); j++) {
//                    (*m_pFileStream)<<itemValues.at(j).toStdString()<<"\t";
//                }
//                (*m_pFileStream)<<"\n";
//            }
//#endif
            m_pCurrPacket->name=itemValues.at(0);
            m_pCurrPacket->ID=itemValues.at(1).toUInt(0,16);
            m_pCurrPacket->tmplt=itemValues.at(2);
            m_pCurrPacket->packetIndicator = packetIndicator;

            if (itemValues.at(3)=="HOST2DEVICE") {
                m_pCurrPacket->direction='>';
            }
            else if (itemValues.at(3).compare("DEVICE2HOST")==0) {
                m_pCurrPacket->direction='<';
            }
            else {
                m_pCurrPacket->direction='n';
            }

            //Import FIELDDEF
            while (1) {
                str = instream.readLine();

                //Current STRUCTDEF Finished
                if (str.contains("STRUCTDEF")) {
                    break;
                }

                //Skip empty lines
                if (!str.contains("FIELDDEF")) {
                    continue;
                }

                QStringList vars;
                vars<<"NAME"<<"DATATYPE"<<"ARRAYSIZE"<<"PRESENT_IF"<<"DEFAULT"<<"LENGTH";
                QStringList varValues;

                //Store NAME DATATYPE...LENGTH into varValues
                int beginIndex;
                int endIndex;
                for (int j=0; j<vars.length(); j++) {
                    int indexOfName=str.indexOf(vars.at(j));
                    if (indexOfName!=-1) {
                        beginIndex = str.indexOf("\"",indexOfName+
                                                 vars.at(j).length());
                        endIndex = str.indexOf("\"",beginIndex+1);
                        varValues << str.mid(beginIndex+1,
                                             endIndex-beginIndex-1);
                    }
                    else {
                        varValues << "default";
                    }
                }

                m_pTempVar=new VAR_LINKED_LIST_t;

                //Set variable Length to dataByte
                m_pTempVar->dataName=varValues.at(0);
                if (varValues.at(1)=="byte" || varValues.at(1)=="uint8") {
                    m_pTempVar->dataByte=1;
                }
                else if (varValues.at(1)=="uint16" || varValues.at(1)=="short") {
                    m_pTempVar->dataByte=2;
                }
                else if (varValues.at(1)=="uint32" || varValues.at(1)=="int32") {
                    m_pTempVar->dataByte=4;
                }
                else if (varValues.at(1)=="string") {
                    m_pTempVar->dataByte=varValues.at(5).toInt();

                    //Special case when LENGTH is not a number
                    if (varValues.at(5)=="ChipNumLen")
                        m_pTempVar->dataByte=1;
                }
                else {
                    m_pTempVar->dataByte=1;
                }

                m_pTempVar->arraySizeStr=varValues.at(2);
                m_pTempVar->condition.isDefault=true;

                //PRESENT_IF
                QRegExp rxlen("(\\w+)(?:\\s*)(>|<)(?:\\s*)(\\d+)");
                int pos = rxlen.indexIn(varValues.at(3));
                if (pos > -1) {
                    m_pTempVar->condition.isDefault=false;
                    m_pTempVar->condition.var = rxlen.cap(1);
                    m_pTempVar->condition.cmp = rxlen.cap(2).at(0).toLatin1();
                    m_pTempVar->condition.val = rxlen.cap(3).toInt();
                }

                //DEFAULT
                m_pTempVar->defaultValStruct.defaultVal=varValues.at(4).toInt(
                            &m_pTempVar->defaultValStruct.defaultValExist,16);

                //Adjust the link list
                m_pTempVar->pNext=NULL;
                if (m_pCurrPacket->pVarListHead==NULL) {
                    m_pCurrPacket->pVarListHead=m_pTempVar;
                    m_pCurrVar=m_pTempVar;
                }
                else {
                    m_pCurrVar->pNext=m_pTempVar;
                    m_pCurrVar=m_pCurrVar->pNext;
                }
            }
        }
    }
    QApplication::restoreOverrideCursor();
    return true;
}

/*
 * This function gives the type packet according to the given ID.
 * @ID: the ID of the packet.
 * @ret: the whole packet which matches the ID.
 */
TYPE_PACKET_LINKED_LIST_t * ImportMessages::getStructType(unsigned int ID){
    m_pCurrPacket=m_pTypePacketHead;
    while (m_pCurrPacket!=NULL) {
        if (m_pCurrPacket->ID==ID) {
            return m_pCurrPacket;
        }
        m_pCurrPacket=m_pCurrPacket->pNext;
    }
    return 0;
}

unsigned int ImportMessages::getID(QString name){
    m_pCurrPacket=m_pTypePacketHead;
    while (m_pCurrPacket!=NULL){
        if (m_pCurrPacket->name == name)
            return m_pCurrPacket->ID;
        m_pCurrPacket = m_pCurrPacket->pNext;
    }
    return 0;
}
