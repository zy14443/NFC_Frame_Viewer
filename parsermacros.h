#ifndef PARSERMACROS_H
#define PARSERMACROS_H

#define RAW 0
//#define MSG_XML_LIST
#define FILE_OUTPUT

#define COLUMN_COUNT        9

#define OUTPUT_ID          -0x1000

#define SWPIDX_SKIP         0x3000
#define NCI_CONN_ID_SKIP    0x3001

#define PAR_TYPE_UNDEFINED  0xff

#define NFCC_Rx             0x00
#define NFCC_Tx             0x01


#define BASE_PARSING_TYPE   0x4000

#define SWP                 0x4100
#define RFU                 0x4101
#define ACT                 0x4102
#define CLT                 0x4103
#define SHDLC_I             0x4104
#define SHDLC_S             0x4105
#define SHDLC_U             0x4106

#define NCI                 0x4200
#define NCI_DATA            0x4201
#define NCI_DATA_CMD        0x4202
#define NCI_CMD_NOTIF       0x4203
#define NCI_SPECIFIC_DATA   0x4204
#define NCI_APP_CMD         0x4205
#define NCI_RF_FRAME        0x4206


#endif // PARSERMACROS_H
