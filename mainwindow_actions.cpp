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
* File Name: mainwindow.cpp
*
* Abstract:  Display the parsing results on screen.
*
* Functions:
*
*******************************************************************************/
#include <QFileDialog>
#include "mainwindow.h"
#include <fstream>
#include <iostream>
#include "ui_mainwindow.h"
#include "parsingfactory.h"
#include <QMessageBox>
#include <QTextEdit>
#include <QClipboard>
#include "searchdialog.h"
#include <QDate>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QStringList>
#include <QList>
#include <QUrl>
#include <Qpoint>
using namespace std;

void MainWindow::parsefile(){

    QByteArray ba = m_logFilePath.toLocal8Bit();
    char * filePathChar=ba.data();
    if (MainWindow::parseLog(filePathChar)){
        QMessageBox::information(this, QObject::tr("Message"),
                           QObject::tr("Parsing Finished!"));
    }

}

void MainWindow::on_actionOpen_triggered()
{
    QString filepath;
    filepath = QFileDialog::getOpenFileName(this, tr("Open Log File"),
                                            m_logFilePath,
                                            tr("File (*.txt *.dat *.log);;"
                                               "All file (*.*)"));
    if (filepath != NULL) {
        m_logFilePath = filepath;
        this->setWindowTitle("NFC Frame Viewer  "+m_logFilePath);

            parsefile();
            if (!isfileOpen){
                connect(mfilterBox->Okbutton, SIGNAL(clicked()), this, SLOT(parsefile()));
                isfileOpen = true;
            }
    }
}

void MainWindow::on_actionSettings_triggered()
{
      mfilterBox->show();

}

void MainWindow::on_actionParse_SWP_Ring_Buff_triggered()
{
    QString filepath;
    filepath = QFileDialog::getOpenFileName(this, tr("Open Log File"),
                                            m_logFilePath,
                                            tr("File (*.txt *.dat *.log);;"
                                               "All file (*.*)"));
    if (filepath != NULL) {
        m_logFilePath = filepath;
        this->setWindowTitle("NFC Frame Viewer  "+m_logFilePath);
        QByteArray ba = m_logFilePath.toLocal8Bit();
        char * filePathChar=ba.data();
        MainWindow::parseSwpRing(filePathChar);
    }
}

void MainWindow::on_actionExit_triggered()
{
    qApp->quit();
}


void MainWindow::on_actionExpand_All_triggered()
{
    m_pTreeWidget->expandAll();
}

void MainWindow::on_actionCollapse_All_triggered()
{
    m_pTreeWidget->collapseAll();
}

void MainWindow::on_actionClear_triggered()
{
    m_pTreeWidget->clear();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, tr("About NFC Frame Viewer"),
                       tr("<b>NFC Frame Viewer version 1.21</b>"
                          "</p><p> Released on November 17, 2014. "
                          "</p><p> Copyright 2014 Broadcom Corporation. "
                          "</p><p> All rights reserved. "
                          "</p>"));
}

void MainWindow::on_actionExport_triggered()
{
    //////////write binary data to a stream///////////////////////////
    QString filepath;
    filepath = QFileDialog::getSaveFileName(this,"Export as",m_exportFilePath);
    if (filepath == NULL) {
        return;
    }
    m_exportFilePath = filepath;
    QFile file(filepath);
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);   // we will serialize the data into the file
    int level=0;
    ////////////////////////////////////////////////////////////////
    QTreeWidgetItem * pItem=new QTreeWidgetItem(FRAME_HEADER_ID);
    for (int i=0; i<m_pTreeWidget->topLevelItemCount(); i++) {
        pItem = m_pTreeWidget->topLevelItem(i);
        level = 0;
        out << level << "\n";
        pItem->write(out);
        level = 1;
        for (int j=0; j<pItem->childCount(); j++) {
            out << level << "\n";
            pItem->child(j)->write(out);
        }
    }
}

void MainWindow::on_actionImport_triggered()
{
    //////////read binary data from a stream//////////////////////////
    QString filepath;
    filepath = QFileDialog::getOpenFileName(this, tr("Import file"),
                                     m_importFilePath,
                                     tr("File (*.swp)"));
    if (filepath == NULL) {
        return;
    }
    m_importFilePath = filepath;
    QFile file(filepath);
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);    // read the data serialized from the file
    int level;
    QString str;
    ////////////////////////////////////////////////////////////////
    QTreeWidgetItem * pItem;
    QTreeWidgetItem * pChildItem;
    m_pTreeWidget->clear();
    in >> level >> str;
    for (int i=0; !in.atEnd(); i++) {
        if (level == 0) {
            pItem=new QTreeWidgetItem(FRAME_HEADER_ID);
            pItem->read(in);
            m_pTreeWidget->addTopLevelItem(pItem);
            in >> level >> str;
            while (level==1) {
                pChildItem=new QTreeWidgetItem(FRAME_CONTENT_ID);
                pChildItem->read(in);
                pItem->addChild(pChildItem);
                in >> level >> str;
            }
        }
    }
    m_pTreeWidget->expandAll();
}

void MainWindow::on_action_Copy_triggered()
{
    QString copyStr="";
    QList<QTreeWidgetItem *> selected;
    for (int i=0; i<m_pTreeWidget->topLevelItemCount(); i++) {
        if (m_pTreeWidget->topLevelItem(i)->isSelected()) {
            selected.append(m_pTreeWidget->topLevelItem(i));
        }
        for (int j=0; j<m_pTreeWidget->topLevelItem(i)->childCount();j++) {
            if (m_pTreeWidget->topLevelItem(i)->child(j)->isSelected()) {
                selected.append(m_pTreeWidget->topLevelItem(i)->child(j));
            }
        }
    }
    for (int i=0; i<selected.length(); i++) {
        for (int j=0; j<COLUMN_COUNT; j++) {
            copyStr += selected.at(i)->text(j);
            copyStr += "\t";
        }
        copyStr += "\n";
    }
    pClipboard->setText(copyStr);
}

void MainWindow::on_actionCopy_All_triggered()
{
    QString copyStr="";
    for (int i=0; i<m_pTreeWidget->topLevelItemCount(); i++) {
        for (int j=0; j<COLUMN_COUNT; j++) {
            copyStr += m_pTreeWidget->topLevelItem(i)->text(j);
            copyStr += "\t";
        }
        copyStr += "\n";
        for (int k=0; k<m_pTreeWidget->topLevelItem(i)->childCount();k++) {
            for (int m=0; m<COLUMN_COUNT; m++) {
                copyStr += m_pTreeWidget->topLevelItem(i)->child(k)->text(m);
                copyStr += "\t";
            }
            copyStr += "\n";
        }
    }
    pClipboard->setText(copyStr);
}

void MainWindow::on_actionSearch_triggered()
{
    m_pFindDialog->setVisible(true);
    m_pFindDialog->setGeometry(m_pFindDialog->m_rect);
}

void MainWindow::on_actionImport_Message_File_triggered()
{
    //////////read binary data from a stream//////////////////////////
    QString filepath;
    filepath = QFileDialog::getOpenFileName(this, tr("Import Message File"),
                                     m_importMsgFilePath,
                                     tr("File (*.xml)"));
    if (filepath == NULL)
        return;

    m_importMsgFilePath = filepath;

    QByteArray ba = m_importMsgFilePath.toLocal8Bit();
    char * filePathChar=ba.data();
    if (m_pIm) {
        delete m_pIm;
    }
    m_pIm=new ImportMessages;
    bool isImportMsg = m_pIm->import(filePathChar);
    if (isImportMsg) {
        //QMessageBox::information(this, QObject::tr("Congratulations!"),
        //                       QObject::tr("Import Successful!"));
    }
}

void MainWindow::on_actionSave_as_triggered()
{
    QString filepath;
    filepath = QFileDialog::getSaveFileName(this,tr("Save as"),
                                            m_outputFilePath,
                                            tr("File (*.logdata)"));
    if (filepath != NULL) {
        m_outputFilePath = filepath;
    }
    parsefile();
}
