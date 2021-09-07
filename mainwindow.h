#ifndef MAINWINDOW_H
#define MAINWINDOW_H
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
* File Name: mainwindow.h
*
* Abstract:  Display the parsing results on screen.
*
* Functions:
*
*******************************************************************************/
#include <QMainWindow>
#include <string>
#include <QTreeWidget>
#include <fstream>
#include "searchdialog.h"
#include "parsingswp.h"
#include "mainsettings.h"
#include <QResizeEvent>
#include "importmessages.h"
#include "parsermacros.h"
#include "parsingoptioncheckbox.h"
#include "parsingfactory.h"


#define FRAME_HEADER_ID      1000
#define FRAME_CONTENT_ID     1001

#define COLOR_BLANCHED_ALMOND  252,252,252       //white
#define COLOR_LAVENDER         202,225,255       //lavender

#define COLUMN_INDEX         0
#define COLUMN_TIME_STAMP    1
#define COLUMN_SEQUENCE      2
#define COLUMN_DIRECTION     3

#define COLUMN_DESCRIPTION   4
#define COLUMN_VALUE         5
#define COLUMN_MEANING       6


#define COLUMN_SWP_INDEX     7
#define COLUMN_NCI_CONN_ID   8

using namespace std;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    bool isfileOpen;

    QString m_exportFilePath;
    QString m_importFilePath;

    QString m_outputFilePath;

    QString m_importMsgFilePath;
    QString m_logFilePath;

    QString m_btLogFilePath;

    mainSettings * m_pSettings;

    QSize m_size;
    QPoint m_pos;
    QClipboard *pClipboard;

    searchDialog * m_pFindDialog;

    //Checkbox for parsing settings
    parsingoptioncheckbox* mfilterBox;

    ImportMessages * m_pIm;
    explicit MainWindow(QWidget *parent = 0);

    void loadSettings();
    void restoreSettings();
    void openFile(const QString &filePath);

    bool parseLog(char * filepath);

    void parseSwpRing(char * filepath);
    void resizeEvent ( QResizeEvent * event );
    void moveEvent ( QMoveEvent * event );
    void drawScreen(QFont font, QBrush brush, QColor color,
                    bool * pColorChoice, Parsing *p);
    bool searchFound(QString searchStr, QTreeWidgetItem *pFoundItem,
                     QString itemText, Qt::MatchFlags flag);
    QTreeWidgetItem * getNextItem(int &currTopIndex,int &currChildIndex,
                                  bool isBackward);
    ~MainWindow();

private slots:
    void on_actionExpand_All_triggered();
    void on_actionCollapse_All_triggered();
    void on_actionOpen_triggered();
    void on_actionParse_SWP_Ring_Buff_triggered();
    void on_actionExit_triggered();

//    void on_actionEnableBinaryParse_triggered();
//    void on_actionDisableBinaryParse_triggered();

    void on_actionClear_triggered();
    void on_actionAbout_triggered();
    void on_actionExport_triggered();
    void on_actionImport_triggered();
    void on_action_Copy_triggered();
    void on_actionCopy_All_triggered();
    void on_actionSearch_triggered();


public slots:
    void searchAllExcecuted(QString itemText, Qt::MatchFlags flag);
    void searchExcecuted(QString itemText, Qt::MatchFlags flag,
                         bool isBackward, bool isFromStart);
    //void itemIsClicked(QTreeWidgetItem * item, int column);
    void on_actionImport_Message_File_triggered();
    void on_actionSettings_triggered();
    void parsefile();
    void on_actionSave_as_triggered();

private:
    Ui::MainWindow *ui;
    QTreeWidget * m_pTreeWidget;

protected:
    void dragEnterEvent(QDragEnterEvent *);
    void dragMoveEvent(QDragMoveEvent *);
    void dragLeaveEvent(QDragLeaveEvent *);
    void dropEvent(QDropEvent *);
};

#endif // MAINWINDOW_H
