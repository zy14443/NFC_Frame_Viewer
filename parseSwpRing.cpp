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
#include <QString>
using namespace std;

extern int nciParser;
extern int swpParser;
extern int rfParser;

void MainWindow::parseSwpRing(char * filepath)
{
    if(filepath==NULL)
        return;
    swpParser = 1;
    m_pTreeWidget->clear();
    QApplication::setOverrideCursor(Qt::WaitCursor);

    QFont font("" , 11, QFont::Normal);
    QBrush brush(Qt::black,Qt::SolidPattern);
    QColor color;
    bool colorChoice=true;
    ///////////////////////////////////////////////////////
//#ifdef FILE_OUTPUT
    QByteArray ba=m_outputFilePath.toLocal8Bit();
    char * pFilepath=ba.data();
//    if (!m_outputEnabled) {
//        pFilepath=NULL;
//    }
//#endif
    Parsing *p= new Parsing();
    ParsingFactory * pF=new ParsingFactory;
    ifstream indata;
    indata.open(filepath,ifstream::in);
    if (indata.fail()) {
        return;
    }

    string str;
    unsigned char * rawData=NULL;
    unsigned int startPoint;
    unsigned int endPoint;
    bool isfull;
    int tempData=0;
    unsigned int i;
    unsigned int byteLength = 0;
    unsigned int sIndex;
    unsigned int eIndex;
    //unsigned char fixed = 0x10;
    unsigned int ID=0x6f1e;
    unsigned int direction=0;
    unsigned int index=0;
    QString timestamp = "00:00:00.000";
    bool direcfind=0;
    bool swpIdxfind = 0;
    bool finish = 0;
    bool forward = 0;
    unsigned int counter=0;
    unsigned int byteperline=0;
    unsigned int charperline = 0;

    while(1)
    {
         indata >> str;
         counter++;
         if(str.compare("********")==0)
             break;
    }
    if (counter<=2)
        return;
    else
    {
       byteperline = counter-2;
       charperline = counter;
       counter = 0;
    }
    indata.close();
    indata.open(filepath,ifstream::in);

    indata >> str;

    if(str.compare("0x0003A82C")!=0)
        return;

    indata >> hex >> startPoint;
    indata >> tempData;
    startPoint=(startPoint)+(unsigned int)(tempData<<8);

    indata >> hex >> endPoint;
    indata >> tempData;
    endPoint=(endPoint)+(unsigned int)(tempData<<8);

    indata >> str;
    indata >> str;
    indata >> str;
    indata >> hex >>isfull;
    indata >> str;

    sIndex = (startPoint/byteperline)*charperline+startPoint%byteperline+1;
    eIndex = (endPoint/byteperline)*charperline+endPoint%byteperline+1;

    for(i=0;i<sIndex;i++)
        indata >> str;

    while(!finish)
    {
        if(indata.eof())
        {
            indata.close();
            indata.open(filepath,ifstream::in);
            //indata.seekg (50, ios::beg);
            //len = indata.tellg();
            forward = 1;
            for(i=0; i<10; i++)
                indata>>str;
        }
        if (!direcfind)
        {
            indata >> str;
            if (forward)
                counter++;
            if (str.compare("********")==0 || (str.compare(0,7, "0x0003A")==0) )
            continue;
        else
        {
            char * temp;
            temp = &str[0];
            tempData = (int)strtol(temp, NULL, 16);
            direcfind = 1;
        }
        }

        if(!swpIdxfind)
        {
        indata >> str;
        if(forward) counter++;
        if (str.compare("********")==0 || (str.compare(0,7, "0x0003A")==0))
            continue;
        else
        {
            char * temp;
            temp = &str[0];
            byteLength = (int)strtol(temp, NULL, 16);
            swpIdxfind = 1;
        }
        }

    if (!rawData && byteLength>0)
    {
        rawData = new unsigned char [byteLength+2];
    }

    rawData[0] = tempData & 0x01;
    rawData[1] = tempData & 0x80;

    i=0;
    while (i!=byteLength)
    {
        indata >> str;
        if(forward) counter++;
        if (str.compare("********")==0 || (str.compare(0,7, "0x0003A")==0))
            continue;
        else
        {
            char * temp;
            temp = &str[0];
            tempData = (int)strtol(temp, NULL, 16);
            rawData[i+2] = (unsigned char)tempData;
            i++;
        }
    }

    direction = (unsigned int)rawData[0];

    p= pF->createParsing(ID,byteLength+2,rawData,
                         p,direction
// #ifdef FILE_OUTPUT
//                         ,pFilepath
// #endif
                         ,m_pIm ,pFilepath);


   if(p!=NULL)
   {
      if (p->m_parsingType!=BASE_PARSING_TYPE)
      {
         p->init(index,timestamp);
         index++;
         p->parse();
       }
       //MainWindow::drawScreen(font, brush, color,
       //                       &colorChoice, p);

      this->drawScreen(font, brush, color,
            &colorChoice, p);
   }
   else
   {
       pF->~ParsingFactory();
   }

   if (counter==eIndex) finish=1;
   //packetnum++;

   if (rawData)
       delete(rawData);
   rawData=NULL;
   direcfind = 0;
   swpIdxfind = 0;
    }

   indata.close();
   delete p;
   delete pF;
   QApplication::restoreOverrideCursor();

   //////////////////////////////
   m_pTreeWidget->expandAll();
   m_pTreeWidget->setWordWrap(true);

}
