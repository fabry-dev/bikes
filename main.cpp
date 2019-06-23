#include <QApplication>
#include "qwindow.h"
#include "qscreen.h"
#include "qwidget.h"
#include "mainscreen.h"
#include "qdebug.h"
#include "QPushButton"


#include "serialwatcher.h"

#define PATH_DEFAULT (QString)"/home/fred/Dropbox/Taf/Cassiopee/bikes/files/"
#define TIMEOUT 15000

#define defaultSpeed (10)

int main(int argc, char *argv[])

{
    QApplication a(argc, argv);




    QString PATH;
    QStringList params = a.arguments();
    if(params.size()>1)
        PATH = params[1];
    else
        PATH=PATH_DEFAULT;

    bool HIDE_CURSOR=false;
    bool DEBUG=false;
    int rotations = 5;
    int stripSize = 170;

    QFile file(PATH+"config.cfg");
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug()<<"no config file";

    }
    else
    {

        QTextStream in(&file);

        QString  line;
        QString paramName,paramValue;
        QStringList params;


        while(!in.atEnd()) {
            line = in.readLine();
            line = (line.split("#"))[0];
            params = line.split("=");
            if(params.size()>=2)
            {
                paramName = params[0];
                paramValue = params[1];

                if (paramName.mid(0,6)=="CURSOR")
                    HIDE_CURSOR = (paramValue=="NO");
                else if (paramName.mid(0,5)=="DEBUG")
                    DEBUG = (paramValue=="YES");
                else if (paramName=="ROTATIONS")
                {
                    rotations = paramValue.toInt();

                }
                else if (paramName=="STRIPSIZE")
                {
                    stripSize = paramValue.toInt();

                }


                else
                    qDebug()<<paramName<<" - "<<paramValue;
            }
        }
        file.close();

    }




    if (HIDE_CURSOR)
    {
        QCursor cursor(Qt::BlankCursor);
        a.setOverrideCursor(cursor);
        a.changeOverrideCursor(cursor);
    }


    serialWatcher * serialwatch = new serialWatcher(NULL);



    qDebug()<<"Screens count: "<<a.screens().size();

    mainScreen * ms = new mainScreen(NULL,PATH,rotations,stripSize);

    uint screenNumber = 2;

    if(screenNumber>a.screens().size()-1)
        screenNumber = a.screens().size()-1;


    ms->setGeometry(a.screens()[screenNumber]->geometry().x(),a.screens()[screenNumber]->geometry().y(),1080,1920);
    ms->show();

    if(DEBUG)
    {
        qDebug()<<"debug";
    QPushButton *pb0 = new QPushButton(ms);
    pb0->move(0,0);
    pb0->resize(200,200);
    pb0->setText("close app");
    a.connect(pb0,SIGNAL(clicked(bool)),&a,SLOT(closeAllWindows()));
    pb0->show();
    pb0->raise();
    }


    a.connect(serialwatch,SIGNAL(getData(int)),ms,SLOT(getData(int)));




    return a.exec();

}
