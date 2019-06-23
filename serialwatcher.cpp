#include "serialwatcher.h"

#include <fcntl.h>
#include <unistd.h>



serialWatcher::serialWatcher(QObject *parent):QObject(parent)
{



    port = new QSerialPort;
    port->setPortName("/dev/ttyACM0");


    if(port->open(QIODevice::ReadWrite))
        qDebug()<<"serial created";
    else
        qDebug()<<"serial error";


    port->setBaudRate(QSerialPort::Baud19200)

            && port->setDataBits(QSerialPort::Data8)

            && port->setParity(QSerialPort::NoParity)

            && port->setStopBits(QSerialPort::OneStop)

            && port->setFlowControl(QSerialPort::NoFlowControl);




    connect(port, SIGNAL(readyRead()), this, SLOT(readData()));

}





void serialWatcher::readData()
{

    const QByteArray data = port->readAll();
    emit getData(data.at(0));


}




serialWatcher::~serialWatcher()
{
    qDebug()<<"closed";

}


