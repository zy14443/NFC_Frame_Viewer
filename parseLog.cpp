#include <QFileDialog>
#include "mainwindow.h"
#include <fstream>
#include <iostream>
#include "ui_mainwindow.h"
#include "parsingfactory.h"
#include "parsermacros.h"
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

extern int nciParser;
extern int swpParser;
extern int rfParser;

bool MainWindow::parseLog(char * filepath) {

    if (filepath==NULL)
        return false;
    m_pTreeWidget->clear();
    QApplication::setOverrideCursor(Qt::WaitCursor);

    m_pTreeWidget->setWordWrap(true);
//    m_pTreeWidget->setStyleSheet("QTreeWidget::item { border-bottom: 1px solid black;}");

    QFont font("" , 11, QFont::Normal);
    QBrush brush(Qt::black,Qt::SolidPattern);
    QColor color;
    bool colorChoice=true;

    Parsing *p= new Parsing();
    ParsingFactory * pF=new ParsingFactory;


    unsigned int index=1;
    int byteLength=0;
    //unsigned char fixed = 0x10;
    unsigned int ID=0;
    unsigned int direction=0;
    QString timestamp;
    unsigned char * rawData=NULL;

    QFile file(filepath);
    bool ret = file.open(QIODevice::ReadOnly);
    if (!ret) {
        return ret;
    }
    QTextStream instream(&file);
    QString str;    

    QByteArray ba=m_outputFilePath.toLocal8Bit();
    char * pFilepath=ba.data();

    while(!instream.atEnd()) {

        str = instream.readLine();

        if (str.contains("RECV NCI")) {
            direction=NFCC_Rx;
        }
        else if (str.contains("SENT NCI")) {
            direction=NFCC_Tx;
        }

        //parse ADB log
        if (str.contains("Raw NCI Packet")) {
            str = instream.readLine();

                    //Get timestamp
                    QRegExp rx("\\d\\d:\\d\\d:\\d\\d");
                    if (str.indexOf(rx) != -1) {
                        int timePos = str.indexOf(rx);
                        timestamp = str.mid(timePos,12);
                    }

                    //Get ID
                    int idPos = -1;
                    while (idPos==-1){
                        rx.setPattern(" (\\d|[a-f])(\\d|[a-f]) (\\d|[a-f])(\\d|[a-f])");
                        idPos = str.indexOf(rx);
                        if (idPos==-1)
                            str = instream.readLine();
                    }

                    ID = str.mid(idPos,6).remove(" ").toUInt(0,16);

                    //Get Length
                    int lengthPos = idPos+7;
                    byteLength = str.mid(lengthPos,2).toUInt(0,16);

                    if (!rawData && byteLength>0) {
                        rawData = new unsigned char [byteLength];
                    }

                    //Get Data
                    int maxLineNum=13;
                    int base=0, dataPos = lengthPos+3;
                    bool isEnd=false;

                    while (byteLength>0) {
                        for (int i=0; i<maxLineNum; i++) {
                            //tempData =
                            (rawData[i+base])=str.mid(dataPos+i*3,2).toUInt(0,16);
                            if (byteLength==i+base+1) {
                                isEnd=true;
                                break;
                            }
                        }
                        if (!isEnd) {
                            base += maxLineNum;

                            str = instream.readLine();
                            rx.setPattern("\\d\\d\\d\\d: ");
                            dataPos = str.indexOf(rx)+6;
                            maxLineNum=16;
                            if (byteLength<=base+maxLineNum) {
                                maxLineNum=byteLength-base;
                                isEnd=true;
                            }
                        }
                        else {
                            break;
                        }
                    }
                    pF->createParsing(ID,byteLength,rawData,
                                      p,direction

                                      ,m_pIm, pFilepath);
                    if (p!=NULL)
                    {
                        if (p->m_parsingType!=BASE_PARSING_TYPE) {
                            p->init(index,timestamp);
                            index++;
                            p->parse();
                        }

                        this->drawScreen(font, brush, color,
                                         &colorChoice, p);
                    }
                    else
                    {
                        pF->~ParsingFactory();
                    }



                    if (rawData)
                        delete(rawData);
                    rawData=NULL;

        }//if (str.compare("Raw")==0)

        //Parse HWTester Log
        else if (str.contains("Sending:")||str.contains("(timestamp =")){

            direction = NFCC_Rx;
            if (str.contains("Sending:")){
                str = instream.readLine();
                str = instream.readLine();
                direction = NFCC_Tx;
            }

                //Get timestamp
                QRegExp rx("\\d\\d:\\d\\d:\\d\\d");
                if (str.indexOf(rx) != -1) {
                    int timePos = str.indexOf(rx);
                    timestamp = str.mid(timePos,12);
                }

                //Get ID
                if (m_pIm->packetIndicator)
                    rx.setPattern(" (1)(0) (\\d|[A-F])(\\d|[A-F])");

                else
                    rx.setPattern(" (\\d|[A-F])(\\d|[A-F]) (\\d|[A-F])(\\d|[A-F])");

                int idPos = str.indexOf(rx);
                if (idPos != -1){
                    if (m_pIm->packetIndicator)
                        idPos += 3;
                    ID = str.mid(idPos,6).remove(" ").toUInt(0,16);
                }
                else
                    continue;

                //Get Length
                int lengthPos = idPos+7;
                byteLength = str.mid(lengthPos,2).toUInt(0,16);

                //Get Data
                if (!rawData && byteLength>0) {
                    rawData = new unsigned char [byteLength];
                }

                int dataPos = lengthPos+3;
                for (int i=0; i<byteLength; i++){
                    rawData[i] = str.mid(dataPos+i*3,2).toUInt(0,16);
                }               

                pF->createParsing(ID,byteLength,rawData,
                                  p,direction

                                  ,m_pIm, pFilepath);
                if (p!=NULL)
                {
                    if (p->m_parsingType!=BASE_PARSING_TYPE) {
                        p->init(index,timestamp);
                        index++;
                        p->parse();
                    }

                    this->drawScreen(font, brush, color,
                                     &colorChoice, p);
                }
                else
                {
                    pF->~ParsingFactory();
                }



                if (rawData)
                    delete(rawData);
                rawData=NULL;
         }

        //Parse SVT log
        else if (str.contains("Send cmd: ")){

            direction = NFCC_Tx;

            //Get CMD ID
            int cmdPos = str.indexOf("cmd: ") +5;
            QRegExp rx("[A-Z]");
            int endPos = str.lastIndexOf(rx);
            QString nci_cmd = str.mid(cmdPos,endPos-cmdPos+1);
            ID = m_pIm->getID(nci_cmd);

            byteLength = 0;

//            if (!rawData && byteLength>0) {
//                rawData = new unsigned char [byteLength];
//            }

//            rawData[0] = (unsigned char)(0xFF);
            pF->createParsing(ID,byteLength,rawData,
                              p,direction

                              ,m_pIm, pFilepath);
            if (p!=NULL)
            {
                if (p->m_parsingType!=BASE_PARSING_TYPE) {
                    p->init(index,timestamp);
                    index++;
                    p->parse();
                }

                this->drawScreen(font, brush, color,
                                 &colorChoice, p);
            }
            else
            {
                pF->~ParsingFactory();
            }



            if (rawData)
                delete(rawData);
            rawData=NULL;

        }

        else if (str.contains("nTotRxBytes")){

            direction = NFCC_Rx;

            //Get timestamp
            QRegExp rx("\\d\\d:\\d\\d:\\d\\d");
            if (str.indexOf(rx) != -1)
                continue;

//            QString isError = instream.readLine();
//            if (isError.contains("ERROR")|isError.contains("error"))
//                continue;

            int currPos = 0;
            int strSize = str.size();

            while (currPos < strSize){

                //Get ID
                if (m_pIm->packetIndicator)
                    rx.setPattern(" (1)(0) (\\d|[A-F])(\\d|[A-F])");
                else
                    rx.setPattern(" (\\d|[A-F])(\\d|[A-F]) (\\d|[A-F])(\\d|[A-F])");

                int idPos = str.indexOf(rx,currPos);
                if (idPos != -1){
                    if (m_pIm->packetIndicator)
                        idPos += 3;
                    ID = str.mid(idPos,6).remove(" ").toUInt(0,16);
                }
                else
                    break;


                //Get Length
                int lengthPos = idPos+7;
//                if (lengthPos+2 < strSize)
                    byteLength = str.mid(lengthPos,2).toUInt(0,16);
//                else
//                    break;

                //Get Data
                if (!rawData && byteLength>0) {
                    rawData = new unsigned char [byteLength];
                }

                int dataPos = lengthPos+3;
                for (int i=0; i<byteLength; i++){
                    rawData[i] = str.mid(dataPos+i*3,2).toUInt(0,16);
                }

                currPos = dataPos + byteLength*3 -1;

                pF->createParsing(ID,byteLength,rawData,
                                  p,direction

                                  ,m_pIm, pFilepath);
                if (p!=NULL)
                {
                    if (p->m_parsingType!=BASE_PARSING_TYPE) {
                        p->init(index,timestamp);
                        index++;
                        p->parse();
                    }

                    this->drawScreen(font, brush, color,
                                     &colorChoice, p);
                }
                else
                {
                    pF->~ParsingFactory();
                }



                if (rawData)
                    delete(rawData);
                rawData=NULL;
            }

        }



    }//end of while(!instream.atEnd())



    delete p;
    delete pF;
    QApplication::restoreOverrideCursor();



    return true;
}//end of parselog function
