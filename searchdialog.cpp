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
* File Name: searchdialog.cpp
*
* Abstract:  Display the search dialog on screen.
*
* Functions:
*
*******************************************************************************/

#include "searchdialog.h"
#include <QtWidgets>
#include "mainwindow.h"

searchDialog::searchDialog(QWidget *parent) :
    QDialog(parent)
{
    m_rect=QRect(584,533,306,88);
    m_flag = Qt::MatchRecursive;
    m_w=( MainWindow *) parent;
    m_pLabel = new QLabel(tr("Search &what:"));
    m_pLineEdit = new QLineEdit;
    m_pLabel->setBuddy(m_pLineEdit);

    m_pCaseCheckBox = new QCheckBox(tr("Match &case"),this);
    m_pFromStartCheckBox = new QCheckBox(tr("Search from &start"));

    //initialization
    caseCheckBoxChange(false);
    m_isFromStart=false;

    m_pSearchButton = new QPushButton(tr("&Search"));
    m_pSearchButton->setDefault(true);

    m_pMoreButton = new QPushButton(tr("&More"));
    m_pMoreButton->setCheckable(true);

    m_pMoreButton->setAutoDefault(false);

    m_pButtonBox = new QDialogButtonBox(Qt::Vertical);
    m_pButtonBox->addButton(m_pSearchButton, QDialogButtonBox::ActionRole);
    m_pButtonBox->addButton(m_pMoreButton, QDialogButtonBox::ActionRole);



    m_pExtension = new QWidget;

    m_pWholeWordsCheckBox = new QCheckBox(tr("&Whole words"));
    m_pBackwardCheckBox = new QCheckBox(tr("Search &backward"));
    m_pSearchAllCheckBox = new QCheckBox(tr("Search &all"));

    //initialization
    wholeWordsCheckBoxChange(false);
    m_isSearchAll=false;
    m_isBackward=false;

    connect(m_pMoreButton, SIGNAL(toggled(bool)),
            m_pExtension, SLOT(setVisible(bool)));
    connect(m_pSearchButton, SIGNAL(clicked()),
            this, SLOT(findClicked()));
    connect(m_pCaseCheckBox,  SIGNAL(toggled(bool)),
            this, SLOT(caseCheckBoxChange(bool)));
    connect(m_pWholeWordsCheckBox, SIGNAL(toggled(bool)),
            this,SLOT(wholeWordsCheckBoxChange(bool)));
//    connect(m_pSearchAllCheckBox, SIGNAL(toggled(bool)),
//            this,SLOT(searchAllCheckBoxChange(bool)));
//    connect(m_pBackwardCheckBox, SIGNAL(toggled(bool)),
//            this, SLOT(searchBackwardCheckBoxChange(bool)));
//    connect(m_pFromStartCheckBox, SIGNAL(toggled(bool)),
//            this,SLOT(fromStartCheckBoxChange(bool)));

    QVBoxLayout *pExtensionLayout = new QVBoxLayout;
    pExtensionLayout->setMargin(0);
    pExtensionLayout->addWidget(m_pWholeWordsCheckBox);
    pExtensionLayout->addWidget(m_pBackwardCheckBox);
    pExtensionLayout->addWidget(m_pSearchAllCheckBox);
    m_pExtension->setLayout(pExtensionLayout);



    QHBoxLayout *pTopLeftLayout = new QHBoxLayout;
    pTopLeftLayout->addWidget(m_pLabel);
    pTopLeftLayout->addWidget(m_pLineEdit);

    QVBoxLayout *pLeftLayout = new QVBoxLayout;
    pLeftLayout->addLayout(pTopLeftLayout);
    pLeftLayout->addWidget(m_pCaseCheckBox);
    pLeftLayout->addWidget(m_pFromStartCheckBox);
    pLeftLayout->addStretch(1);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    mainLayout->addLayout(pLeftLayout, 0, 0);
    mainLayout->addWidget(m_pButtonBox, 0, 1);
    mainLayout->addWidget(m_pExtension, 1, 0, 1, 2);
    setLayout(mainLayout);

    setWindowTitle(tr("Find..."));

    m_pExtension->hide();
}

void searchDialog::setVisible(bool visible) {
    QDialog::setVisible(visible);
    if (visible==false) {
        m_rect=this->geometry();
    }
}

void searchDialog::findClicked()
{
    QString text = m_pLineEdit->text();
    if (text.isEmpty()) {
        QMessageBox::information(this, tr("Empty Field"),
            tr("Please enter search text."));
        return;
    } else {
        m_FindText = text;
        if (m_isSearchAll) {
            m_w->searchAllExcecuted(m_FindText,m_flag);
        }
        else {
            m_w->searchExcecuted(m_FindText,m_flag,m_isBackward,m_isFromStart);
        }
    }
}

void searchDialog::caseCheckBoxChange(bool checked) {
    if (checked) {
        m_flag = m_flag | Qt::MatchCaseSensitive;
    }
    else {
        m_flag = m_flag & (~Qt::MatchCaseSensitive);
    }
}

void searchDialog::wholeWordsCheckBoxChange(bool checked) {
    if (checked) {
        m_flag = m_flag & (~Qt::MatchContains);
        m_flag = m_flag | Qt::MatchRegExp;
    }
    else {
        m_flag = m_flag & (~Qt::MatchRegExp);
        m_flag = m_flag | Qt::MatchContains;
    }
}

//void searchDialog::searchAllCheckBoxChange(bool checked) {
//    m_isSearchAll=m_pSearchAllCheckBox->isChecked();
//}

//void searchDialog::searchBackwardCheckBoxChange(bool checked) {
//    m_isBackward=m_pBackwardCheckBox->isChecked();
//}

//void searchDialog::fromStartCheckBoxChange(bool checked) {
//    m_isFromStart=m_pFromStartCheckBox->isChecked();
//}
