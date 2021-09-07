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

int nciParser=0;
int swpParser=1;
int rfParser=1;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setAcceptDrops(true);

    isfileOpen = false;

    m_pIm=NULL;
    m_outputFilePath="";

    pClipboard = QApplication::clipboard();
    m_pTreeWidget=new QTreeWidget(this);
    m_pFindDialog=new searchDialog(this);
    mfilterBox=new parsingoptioncheckbox;

    //Display labels in main window
    QStringList strings;
    strings<<"#"<<"Time Stamp"<<"Sequence"<<"Dir"
          <<"Description"<<"Value"<<"Meaning"<<"swpIdx"<<"Conn ID";
    m_pTreeWidget->setHeaderLabels(strings);
    m_pTreeWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_pTreeWidget->setColumnCount(COLUMN_COUNT);
    setCentralWidget(m_pTreeWidget);

    loadSettings();
    this->setWindowTitle("NFC Frame Viewer");

}

MainWindow::~MainWindow()
{
    restoreSettings();//restore before deletion
    if (m_pIm)
        delete m_pIm;
    delete (m_pFindDialog);

    delete ui;
}

void MainWindow::loadSettings() {

    m_pSettings=new mainSettings;
    m_pSettings->readSettings();

    if (m_pSettings->m_isWinMaximized) {
        this->showMaximized();
    }


    m_importMsgFilePath=m_pSettings->m_importMsgFilePath;
    m_logFilePath=m_pSettings->m_logFilePath;

    for (int i=0; i<COLUMN_COUNT-1; i++) {
        m_pTreeWidget->setColumnWidth(i,m_pSettings->m_columnWidth[i]);
    }
}

void MainWindow::restoreSettings() {

    m_pSettings->m_importMsgFilePath=m_importMsgFilePath;
    m_pSettings->m_logFilePath=m_logFilePath;

    for (int i=0; i<COLUMN_COUNT-1; i++) {
        m_pSettings->m_columnWidth[i]=m_pTreeWidget->columnWidth(i);
    }
    m_pSettings->writeSettings();
}

void MainWindow::resizeEvent ( QResizeEvent * event ) {
    m_size = event->oldSize();
}

void MainWindow::moveEvent ( QMoveEvent * event ) {
    m_pos = event->oldPos();
}


void MainWindow::drawScreen(QFont font, QBrush brush, QColor color,
                            bool *pColorChoice, Parsing *p){
    if (p==NULL)
        return;
    if (p->getLineNum()==0)
        return;

    QTreeWidgetItem * pItem=new QTreeWidgetItem(FRAME_HEADER_ID);
    if (*pColorChoice) {
        color = QColor(COLOR_BLANCHED_ALMOND,200);
        *pColorChoice=false;
    }
    else {
        color = QColor(COLOR_LAVENDER,255);
        *pColorChoice=true;
    }
    for (int j=0;j<COLUMN_COUNT;j++) {
        pItem->setBackgroundColor(j,color);
        pItem->setForeground(j,brush);
        pItem->setFont(j,font);
        pItem->setTextAlignment(j,Qt::AlignTop|Qt::AlignLeft);
    }

    pItem->setText(COLUMN_INDEX,QString::fromStdString(
                       p->m_out.m_pOut[0]->idxString));
    pItem->setText(COLUMN_SEQUENCE,QString::fromStdString(
                       p->m_out.m_pOut[0]->sequence));//"Sequence"
    pItem->setText(COLUMN_DIRECTION,QString::fromLatin1(
                       &p->m_out.direction).at(0));//"Dir",
    if (p->m_out.swpIdx!=SWPIDX_SKIP) {
        pItem->setText(COLUMN_SWP_INDEX,QString::number(
                           p->m_out.swpIdx));//"swp Index"
    }
    if (p->m_out.nciConnID!=NCI_CONN_ID_SKIP) {
        pItem->setText(COLUMN_NCI_CONN_ID,QString::number(
                           p->m_out.nciConnID));
    }
    pItem->setText(COLUMN_DESCRIPTION,QString::fromStdString(
                       p->m_out.m_pOut[0]->desc));//"Description",
    pItem->setText(COLUMN_VALUE,QString::fromStdString(
                       p->m_out.m_pOut[0]->value));//"Value",
    pItem->setText(COLUMN_MEANING,QString::fromStdString(
                       p->m_out.m_pOut[0]->meaning));//"Meaning"
    pItem->setText(COLUMN_TIME_STAMP,QString::fromStdString(
                       p->m_out.timestamp.toStdString()));//"timestamp"
    m_pTreeWidget->addTopLevelItem(pItem);

    //sub
    for (unsigned int i=1; i<p->getLineNum(); i++) {
        ////////////////////////////////////////////
        QTreeWidgetItem *pChildItem=new QTreeWidgetItem(FRAME_CONTENT_ID);
        if (*pColorChoice) {
            color = QColor(COLOR_LAVENDER,255);
        }
        else {
            color = QColor(COLOR_BLANCHED_ALMOND,200);
        }
        for (int j=0;j<COLUMN_COUNT;j++) {
            pChildItem->setBackgroundColor(j,color);
            pChildItem->setForeground(j,brush);
            pChildItem->setFont(j,font);
            pChildItem->setTextAlignment(j,Qt::AlignTop|Qt::AlignLeft);
        }
        pChildItem->setText(COLUMN_INDEX,QString::fromStdString(
                                p->m_out.m_pOut[i]->idxString));
        pChildItem->setText(COLUMN_SEQUENCE,QString::fromStdString(
                                p->m_out.m_pOut[i]->sequence));//"Sequence"
        pChildItem->setText(COLUMN_DIRECTION,QString::fromLatin1(
                                &p->m_out.direction).at(0));//"Dir",
        if (p->m_out.swpIdx!=SWPIDX_SKIP) {
            pChildItem->setText(COLUMN_SWP_INDEX,QString::number(
                                    p->m_out.swpIdx));//"swp Index"
        }
        if (p->m_out.nciConnID!=NCI_CONN_ID_SKIP) {
            pChildItem->setText(COLUMN_NCI_CONN_ID,QString::number(
                                    p->m_out.nciConnID));
        }
        pChildItem->setText(COLUMN_DESCRIPTION,QString::fromStdString(
                                p->m_out.m_pOut[i]->desc));//"Description",
        pChildItem->setText(COLUMN_VALUE,QString::fromStdString(
                                p->m_out.m_pOut[i]->value));//"Value",
        pChildItem->setText(COLUMN_MEANING,QString::fromStdString(
                                p->m_out.m_pOut[i]->meaning));//"Meaning"
        pChildItem->setText(COLUMN_TIME_STAMP,QString::fromStdString(
                                p->m_out.timestamp.toStdString()));//"timestamp"
        // adding a row to an item starts a subtree
        pItem->addChild(pChildItem);
    }
}

void MainWindow::searchAllExcecuted(QString itemText, Qt::MatchFlags flag) {
    if (m_pTreeWidget==NULL || m_pTreeWidget->topLevelItemCount()==0) {
        QMessageBox::information(this, tr("Warning"),
                                 tr("No text could be searched."));
        return;
    }
    QTreeWidgetItem *pItem;
    QList<QTreeWidgetItem *> found;
    for(int i=0; i<COLUMN_COUNT; i++) {
        found += m_pTreeWidget->findItems(itemText, flag, i);
    }
    m_pTreeWidget->clearSelection();
    foreach (pItem, found) {
        m_pTreeWidget->setItemSelected(pItem, true);
    }
}

void MainWindow::searchExcecuted(QString itemText, Qt::MatchFlags flag,
                                 bool isBackward, bool isFromStart) {
    if (m_pTreeWidget==NULL || m_pTreeWidget->topLevelItemCount()==0) {
        QMessageBox::information(this, tr("Warning"),
                                 tr("No text could be searched."));
        return;
    }
    ////////////////////////////////////////////////////
    QTreeWidgetItem *pFoundItem;
    QString searchStr="";
    bool isFound=false;
    int topIndex;
    int childIndex=-1;
    //get the current index->
    //if no current index, return (-1,-1)
    //if current is toplevel, return (current top index, -1)
    //else return (current top index, current child index)
    if (isFromStart) {
        topIndex=-1;
        childIndex=-1;
    }
    else {
        if (m_pTreeWidget->currentItem()!=NULL) {
            topIndex = m_pTreeWidget->indexOfTopLevelItem(
                        m_pTreeWidget->currentItem());
            if (topIndex<0) {
                topIndex = m_pTreeWidget->indexOfTopLevelItem(
                            m_pTreeWidget->currentItem()->parent());
                childIndex = m_pTreeWidget->currentIndex().row();
            }
        }
        else {
            topIndex=-1;
            childIndex=-1;
        }
    }
    //<-end of getting the current index
    int currTopIndex=topIndex;
    int currChildIndex=childIndex;
    pFoundItem=getNextItem(currTopIndex,currChildIndex,isBackward);
    if (topIndex==-1 && childIndex==-1) {
        //->if no item is current item
        if (isBackward) {
            topIndex=currChildIndex;
            childIndex=m_pTreeWidget->topLevelItem(topIndex)->childCount()-1;
        }
        else {
            topIndex=0;
        }
        for (int m=0; m<COLUMN_COUNT; m++) {
            searchStr += pFoundItem->text(m);
            searchStr += "\t";
        }
        if (searchFound(searchStr,pFoundItem,itemText,flag)) {
            return;
        }
        else {
            pFoundItem=getNextItem(currTopIndex,currChildIndex,isBackward);
        }
    }
    while (currTopIndex!=topIndex || currChildIndex!=childIndex) {
        for (int m=0; m<COLUMN_COUNT; m++) {
            searchStr += pFoundItem->text(m);
            searchStr += "\t";
        }
        if (searchFound(searchStr,pFoundItem,itemText,flag))
            return;
        pFoundItem=getNextItem(currTopIndex,currChildIndex,isBackward);
    }
    if (isFound==false) {
        QMessageBox::information(this, tr("Warning"),
                                 tr("No search result found."));
    }
}

QTreeWidgetItem * MainWindow::getNextItem(int &currTopIndex,
                                          int &currChildIndex,
                                          bool isBackward) {
    QTreeWidgetItem *pItem;
    if (isBackward) {
        if (currChildIndex<0 || currTopIndex<0) {
            currTopIndex--;
            if (currTopIndex<0) {
                currTopIndex=m_pTreeWidget->topLevelItemCount()-1;
            }
            currChildIndex=m_pTreeWidget->topLevelItem(
                        currTopIndex)->childCount();
        }
        currChildIndex--;
        if (currChildIndex<0) {
            pItem=m_pTreeWidget->topLevelItem(currTopIndex);
        }
        else {
            pItem=m_pTreeWidget->topLevelItem(
                        currTopIndex)->child(currChildIndex);
        }
    }
    else {
        if (currTopIndex<0) {
            currTopIndex=0;
            currChildIndex=-1;
            pItem=m_pTreeWidget->topLevelItem(currTopIndex);
            return pItem;
        }
        currChildIndex++;
        if (currChildIndex==
                m_pTreeWidget->topLevelItem(currTopIndex)->childCount()) {
            currTopIndex++;
            currChildIndex=-1;
            if (currTopIndex==m_pTreeWidget->topLevelItemCount()) {
                currTopIndex=0;
            }
            pItem=m_pTreeWidget->topLevelItem(currTopIndex);
        }
        else {
            pItem=m_pTreeWidget->topLevelItem(currTopIndex)->child(currChildIndex);
        }
    }
    return pItem;
}

bool MainWindow::searchFound(QString searchStr, QTreeWidgetItem *pFoundItem,
                             QString itemText, Qt::MatchFlags flag) {
    bool isContains=false;
    Qt::CaseSensitivity isSensitive=Qt::CaseInsensitive;
    if (flag&0x01) {
        isContains=true;
    }
    if (flag&0x10) {
        isSensitive=Qt::CaseSensitive;
    }
    if (isContains) {
        if ( searchStr.contains(itemText,isSensitive) ) {
            m_pTreeWidget->setCurrentItem(pFoundItem);
            m_pTreeWidget->clearSelection();
            m_pTreeWidget->setItemSelected(pFoundItem, true);
            return true;
        }
    }
    //match whole words
    else {
        int startPos = searchStr.indexOf(itemText,0,isSensitive);
        if (startPos > 0) {
            if (searchStr.at(startPos-1).isLetterOrNumber()==false &&
                    searchStr.at(startPos+itemText.length()).isLetterOrNumber()
                    ==false) {
                m_pTreeWidget->setCurrentItem(pFoundItem);
                m_pTreeWidget->clearSelection();
                m_pTreeWidget->setItemSelected(pFoundItem, true);
                return true;
            }
        }
    }
    return false;
}



void MainWindow::dragEnterEvent(QDragEnterEvent* event)
{
    // if some actions should not be usable, like move, this code must be adopted
    event->acceptProposedAction();
}


void MainWindow::dragMoveEvent(QDragMoveEvent* event)
{
     // if some actions should not be usable, like move, this code must be adopted
     event->acceptProposedAction();
}

void MainWindow::dragLeaveEvent(QDragLeaveEvent* event)
{
     event->accept();
}

void MainWindow::dropEvent(QDropEvent* event)
{
    //on_actionOpen_triggered();
    const QMimeData* mimeData = event->mimeData();
    QString filepath;

    // check for our needed mime type, here a file or a list of files
     if (mimeData->hasUrls())
        {
            QStringList pathList;
            QList<QUrl> urlList = mimeData->urls();

            // extract the local paths of the files
            for (int i = 0; i < urlList.size() && i < 32; ++i)
            {
                pathList.append(urlList.at(i).toLocalFile());
            }

            filepath = pathList[0];
            m_logFilePath = filepath;
            this->setWindowTitle("NFC Frame Viewer  "+m_logFilePath);
            QByteArray ba = m_logFilePath.toLocal8Bit();
            char * filePathChar=ba.data();
            MainWindow::parseLog(filePathChar);
        }

}




