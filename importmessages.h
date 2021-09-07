#ifndef IMPORTMESSAGES_H
#define IMPORTMESSAGES_H
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
* File Name: importmessages.h
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

#include "parsermacros.h"
#include "typedef.h"

class ImportMessages
{
public:

#ifdef MSG_XML_LIST
    ofstream * m_pFileStream;
#endif

    TYPE_PACKET_LINKED_LIST_t * m_pTypePacketHead;
    TYPE_PACKET_LINKED_LIST_t * m_pCurrPacket;
    TYPE_PACKET_LINKED_LIST_t * m_pTempPacket;
    VAR_LINKED_LIST_t * m_pTempVar;
    VAR_LINKED_LIST_t * m_pCurrVar;
    TYPE_PACKET_LINKED_LIST_t * getStructType(unsigned int ID);
    unsigned int getID(QString name);
    ImportMessages();
    ~ImportMessages();
    bool import(char * filepath);

    unsigned int packetIndicator;

};

#endif // IMPORTMESSAGES_H
