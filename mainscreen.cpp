#include "mainscreen.h"

mainScreen::mainScreen(QLabel *parent,QString PATH,int rotationsToWin) : QLabel(parent),PATH(PATH),rotationsToWin(rotationsToWin)
{
    showFullScreen();
    resize(1080,1920);

    raceStarted = false;
    score1=score2 = 0;


     qDebug()<<"rotations to win: "<<rotationsToWin;

    vp = new mpvWidget(this);
    vp->resize(size());
    vp->setLoop(false);
    vp->setProperty("pause", false);
    vp->setProperty("keep-open",true);
    vp->hide();

    setupStates();
}



void mainScreen::setupStates()
{


    idle = new QState();
    start = new QState();
    loop = new QState();
    state25 = new QState();
    state50 = new QState();
    state75 = new QState();
    win1  = new QState();
    win2 = new QState();

    connect(idle,SIGNAL(entered()),this,SLOT(goIdle()));
    idle->addTransition(this,SIGNAL(gotStart()),start);

    connect(start,SIGNAL(entered()),this,SLOT(goStart()));
    start->addTransition(vp,SIGNAL(videoOver()),loop);

    connect(loop,SIGNAL(entered()),this,SLOT(goLoop()));

    loop->addTransition(this,SIGNAL(done25p()),state25);
    connect(state25,SIGNAL(entered()),this,SLOT(go25()));
    state25->addTransition(vp,SIGNAL(videoOver()),loop);

    loop->addTransition(this,SIGNAL(done50p()),state50);
    connect(state50,SIGNAL(entered()),this,SLOT(go50()));
    state50->addTransition(vp,SIGNAL(videoOver()),loop);

    loop->addTransition(this,SIGNAL(done75p()),state75);
    connect(state75,SIGNAL(entered()),this,SLOT(go75()));
    state75->addTransition(vp,SIGNAL(videoOver()),loop);


    loop->addTransition(this,SIGNAL(doneWin1()),win1);
    connect(win1,SIGNAL(entered()),this,SLOT(goWin1()));
    win1->addTransition(vp,SIGNAL(videoOver()),idle);

    loop->addTransition(this,SIGNAL(doneWin2()),win2);
    connect(win2,SIGNAL(entered()),this,SLOT(goWin2()));
    win2->addTransition(vp,SIGNAL(videoOver()),idle);



    machine.addState(idle);
    machine.addState(start);
    machine.addState(loop);
    machine.addState(state25);
    machine.addState(state50);
    machine.addState(state75);
    machine.addState(win1);
    machine.addState(win2);

    machine.setInitialState(idle);
    machine.start();
}





void mainScreen::goIdle(void)
{
    qDebug()<<"idle state";

    raceStarted = false;

    vp->show();
    vp->lower();
    vp->loadFile(PATH+"saving.mp4");
    vp->setProperty("pause", false);
    vp->setProperty("loop", true);
    vp->raise();
}

void mainScreen::goStart()
{
    qDebug()<<"start state";
    score1=score2 = 0;
    raceState = 0;

    vp->show();
    vp->lower();
    vp->loadFile(PATH+"start.mp4");
    vp->setProperty("pause", false);
    vp->setProperty("loop", false);
    vp->raise();
}

void mainScreen::goLoop()
{
    qDebug()<<"loop state";
    raceStarted = true;


    vp->show();
    vp->lower();
    vp->loadFile(PATH+"loop.mp4");
    vp->setProperty("pause", false);
    vp->setProperty("loop", true);
    vp->raise();
}


void mainScreen::go25()
{
    qDebug()<<"25% state";
    vp->show();
    vp->lower();
    vp->loadFile(PATH+"25.mp4");
    vp->setProperty("pause", false);
    vp->setProperty("loop", false);
    vp->raise();
}

void mainScreen::go50()
{
    qDebug()<<"50% state";
    vp->show();
    vp->lower();
    vp->loadFile(PATH+"50.mp4");
    vp->setProperty("pause", false);
    vp->setProperty("loop", false);
    vp->raise();
}

void mainScreen::go75()
{
    qDebug()<<"75% state";
    vp->show();
    vp->lower();
    vp->loadFile(PATH+"75.mp4");
    vp->setProperty("pause", false);
    vp->setProperty("loop", false);
    vp->raise();
}


void mainScreen::goWin1(void)
{
    qDebug()<<"win1 state";
    vp->show();
    vp->lower();
    vp->loadFile(PATH+"winner1.mp4");
    vp->setProperty("pause", false);
    vp->setProperty("loop", false);
    vp->raise();
}


void mainScreen::goWin2(void)
{
    qDebug()<<"winner2 state";
    vp->show();
    vp->lower();
    vp->loadFile(PATH+"win2.mp4");
    vp->setProperty("pause", false);
    vp->setProperty("loop", false);
    vp->raise();
}



void mainScreen::updateScores()
{

    qDebug()<<"score1 "<<score1;
    qDebug()<<"score2 "<<score2;

    if(score1>=rotationsToWin)
    {
        emit doneWin1();
        raceStarted = false;
        return;
    }
    else if(score2>=rotationsToWin)
    {
        emit doneWin2();
        raceStarted = false;
        return;
    }

    double a1,a2;
    a1 = a2 = 0;


    a1 = (double)score1/rotationsToWin;
    a2 = (double)score2/rotationsToWin;
    double a = qMax(a1,a2);


    qDebug()<<a;

    if((raceState == 0)&&(a>=0.25))//0-25%
    {
        qDebug()<<"go 25";
        raceState = 1;
        emit done25p();
        return;
    }
    else if((raceState <= 1)&&(a>=0.5))//25-50%
    {
        qDebug()<<"go 50";
        raceState = 2;
        emit done50p();
        return;
    }
    else if((raceState <= 2)&&(a>=0.75))//50-75%
    {
        qDebug()<<"go 75";
        raceState = 3;
        emit done75p();
        return;
    }





}



void mainScreen::getData(int data)
{

    if(data == 3)
    {
        emit gotStart();
        qDebug()<<"start button";
        return;
    }
    else if ((data == 1)||(data == 2))
    {
        qDebug()<<"bike "<<data;
        if(raceStarted)
        {
            if(data == 1)
                score1++;
            else if(data == 2)
                score2++;


         updateScores();
        }



    }



}


void mainScreen::mousePressEvent(QMouseEvent *ev)
{
    qDebug()<<ev->pos();

}
