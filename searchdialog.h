#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H
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
* File Name: searchdialog.h
*
* Abstract:  Display the search dialog on screen.
*
* Functions:
*
*******************************************************************************/

#include <QDialog>

class MainWindow;

QT_BEGIN_NAMESPACE
class QCheckBox;
class QDialogButtonBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QPushButton;
QT_END_NAMESPACE

class searchDialog : public QDialog
{
    Q_OBJECT
public:
    MainWindow * m_w;
    QRect m_rect;
    Qt::MatchFlags m_flag;
    QString m_FindText;
    bool m_isSearchAll;
    bool m_isBackward;
    bool m_isFromStart;
    searchDialog(QWidget *parent = 0);
    void setVisible(bool visible);

signals:

public slots:
    void findClicked();
    void caseCheckBoxChange(bool checked);
    void wholeWordsCheckBoxChange(bool checked);
//    void searchAllCheckBoxChange(bool checked);
//    void searchBackwardCheckBoxChange(bool checked);
//    void fromStartCheckBoxChange(bool checked);

private:
    QLabel *m_pLabel;
    QLineEdit *m_pLineEdit;
    QCheckBox *m_pCaseCheckBox;
    QCheckBox *m_pFromStartCheckBox;
    QCheckBox *m_pWholeWordsCheckBox;
    QCheckBox *m_pSearchAllCheckBox;
    QCheckBox *m_pBackwardCheckBox;
    QDialogButtonBox *m_pButtonBox;
    QPushButton *m_pSearchButton;
    QPushButton *m_pMoreButton;
    QWidget *m_pExtension;

};

#endif // SEARCHDIALOG_H
