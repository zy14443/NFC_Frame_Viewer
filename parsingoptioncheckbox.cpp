#include "parsingoptioncheckbox.h"
#include <QtGui>
#include <qwidget.h>

extern int nciParser;
extern int swpParser;
extern int rfParser;

parsingoptioncheckbox::parsingoptioncheckbox(QWidget *parent) :
    QWidget(parent)
{
    ParseSWPFrame  = createCheckBox(tr("Parse SWP Frame"));
    ParseRFFrame = createCheckBox(tr("Parse RF Frame"));
    ParseNCIMessage = createCheckBox(tr("Parse NCI Message"));

    Okbutton = new QPushButton("OK");
    Cancelbutton = new QPushButton("Cancel");

    connect(Okbutton, SIGNAL(clicked()), this, SLOT(itemchosen()));
    connect(Cancelbutton, SIGNAL(clicked()), this, SLOT(close()));

    QVBoxLayout *layout = new QVBoxLayout;
    QHBoxLayout *layout1 = new QHBoxLayout;

    layout->addWidget(ParseSWPFrame, 0, 0);
    layout->addWidget(ParseRFFrame, 1, 0);
    layout->addWidget(ParseNCIMessage, 2, 0);

    layout1->addWidget(Okbutton);
    layout1->addWidget(Cancelbutton);

    layout->addLayout(layout1);
    setLayout(layout);
    setWindowTitle(tr("Settings"));

}

QCheckBox* parsingoptioncheckbox::createCheckBox(const QString &text)
{
    QCheckBox *checkBox = new QCheckBox(text);
    checkBox->setChecked(true);
    return checkBox;
}

void parsingoptioncheckbox::itemchosen()
{
    if (ParseSWPFrame->isChecked())
        swpParser = 1;
    else
        swpParser = 0;

    if (ParseRFFrame->isChecked())
        rfParser = 1;
    else
        rfParser = 0;

    if (ParseNCIMessage->isChecked())
        nciParser = 1;
    else
        nciParser = 0;


    this->close();
}


