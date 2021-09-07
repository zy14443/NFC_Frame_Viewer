#include "mainwindow.h"
#include "parsingswp.h"
#include <QtGui>
#include <QtWidgets>
#include <fstream>
#include <string>
#include <iostream>

using namespace std;

//#define FILE_OUTPUT

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    app.setOrganizationName("Broadcom");
    app.setApplicationName("SWP Parser");
    w.show();

    //Import XML message file
    w.on_actionImport_Message_File_triggered();

    return app.exec();
}
