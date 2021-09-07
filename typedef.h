#ifndef TYPEDEF_H
#define TYPEDEF_H

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
* File Name: typedef.h
*
* Abstract:  This file constains structure type of the linked list
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
#include <QString>

using namespace std;

/***********************************************************
* Store FIELDDEF from XML message file
* dataName: NAME
* dataByte: DATATYPE = byte/unit8/uint16/uint32/bits/string
************************************************************/

typedef struct VAR_LINKED_LIST_t{
    QString dataName;
    int dataByte;
    QString arraySizeStr;
    struct {
        unsigned char defaultVal;
        bool defaultValExist;
    } defaultValStruct;
    struct {
        QString var;
        char cmp;
        int val;
        bool isDefault;
    } condition;
    VAR_LINKED_LIST_t * pNext;
} VAR_LINKED_LIST_t;

/***********************************************************
* Store STRUCTDEF from XML message file
* name: NAME
* ID:   ID
* packetIndicator: 0x10(20791,20795)/Empty(20796-20797)
* tmplt: TEMPLATE
* direction: HOST2DEVICE/DEVICE2HOST
************************************************************/
typedef struct TYPE_PACKET_LINKED_LIST_t{
    QString name;
    unsigned int ID;
    unsigned int packetIndicator;
    QString tmplt;
    char direction;
    VAR_LINKED_LIST_t * pVarListHead;
    TYPE_PACKET_LINKED_LIST_t * pNext;
} TYPE_PACKET_LINKED_LIST_t;

#endif // TYPEDEF_H
