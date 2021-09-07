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
* File Name: mainsettings.cpp
*
* Abstract:  Read/write proper information from/to windows registry.
*
* Functions:
*
*******************************************************************************/

#include "mainsettings.h"

mainSettings::mainSettings(QObject *parent) :
    QSettings(parent)
{
}

void mainSettings::readSettings()
{
    QSettings settings("Broadcom", "NFC Frame Viewer");

    m_importMsgFilePath = settings.value("m_importMsgFilePath").toString();
    m_logFilePath = settings.value("m_logFilePath").toString();

    for (int i=0; i<COLUMN_COUNT-1; i++) {
        m_columnWidth[i]=settings.value("m_columnWidth"+QString::number(i),
                                        100).toInt();
    }
    m_isWinMaximized = settings.value("m_isWinMaximized",true).toBool();

}

void mainSettings::writeSettings()
{
    QSettings settings("Broadcom", "NFC Frame Viewer");

    settings.setValue("m_importMsgFilePath",m_importMsgFilePath);
    settings.setValue("m_logFilePath",m_logFilePath);

    for (int i=0; i<COLUMN_COUNT-1; i++) {
        settings.setValue("m_columnWidth"+QString::number(i),m_columnWidth[i]);
    }
    settings.setValue("m_isWinMaximized",true);
}
