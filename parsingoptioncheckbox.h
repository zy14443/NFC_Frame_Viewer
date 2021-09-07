#ifndef PARSINGOPTIONCHECKBOX_H
#define PARSINGOPTIONCHECKBOX_H

#include <QWidget>
#include <QtWidgets>

class QCheckBox;

class parsingoptioncheckbox : public QWidget
{
    Q_OBJECT

public:
    explicit parsingoptioncheckbox(QWidget *parent = 0);
    QPushButton *Okbutton;
    QPushButton *Cancelbutton;

private slots:
    void itemchosen();

private:
    QCheckBox* createCheckBox(const QString &text);
    QCheckBox* ParseSWPFrame;
    QCheckBox* ParseRFFrame;
    QCheckBox* ParseNCIMessage;

};

#endif // PARSINGOPTIONCHECKBOX_H
