#ifndef MAINSETTINGS_H
#define MAINSETTINGS_H
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
* File Name: mainsettings.h
*
* Abstract:  Read/write proper information from/to windows registry.
*
* Functions:
*
*******************************************************************************/

#include <QSettings>
#include <QTreeWidget>
#include "parsermacros.h"

class mainSettings : public QSettings
{
    Q_OBJECT
public:
    QRect m_rect;
    QRect m_findDialogRect;

    QString m_importMsgFilePath;
    QString m_logFilePath;

    bool m_isWinMaximized;
    int m_columnWidth[COLUMN_COUNT-1];
    explicit mainSettings(QObject *parent = 0);
    void writeSettings();
    void readSettings();

signals:

public slots:

};

#endif // MAINSETTINGS_H
