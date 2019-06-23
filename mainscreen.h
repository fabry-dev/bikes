#ifndef MAINSCREEN_H
#define MAINSCREEN_H


#include <QWidget>
#include <qobject.h>
#include "qlabel.h"

#include "mpvwidget.h"
#include "qtimer.h"
#include "QDirIterator"
#include "qcolor.h"
#include "qcombobox.h"

#include "qmath.h"
#include "qlineedit.h"
#include "qstylefactory.h"
#include "qpropertyanimation.h"
#include "qdebug.h"
#include "qstatemachine.h"
#include "qpainter.h"
#include "qprocess.h"

class mainScreen : public QLabel
{
    Q_OBJECT
public:
    explicit mainScreen(QLabel *parent = nullptr, QString PATH="", int rotationsToWin=5, int stripSize = 170);

private:
    QString PATH;
    int rotationsToWin;
    int stripSize;
    int raceState;


    QTimer *timeOut;

    void setupStates();
    QStateMachine machine;
    mpvWidget *vp;

    bool raceStarted;
    uint score1,score2;


    QState *idle;
    QState *start;
    QState *loop;
    QState *state25;
    QState *state50;
    QState *state75;
    QState *win1;
    QState *win2;

    QProcess dmx1Proc,dmx2Proc;

    void sendDmx1(uint n);
    void sendDmx2(uint n);



    uint dmxLevel1,dmxLevel2;

signals:
    void gotStart(void);
    void doneWin1(void);
    void doneWin2(void);
    void done25p(void);
    void done50p(void);
    void done75p(void);


private slots:
    void getData(int bike);
    void updateScores(void);

    void goIdle(void);
    void goStart(void);
    void goLoop(void);
    void go25(void);
    void go50(void);
    void go75(void);
    void goWin1(void);
    void goWin2(void);



protected:
    void mousePressEvent(QMouseEvent *ev);
};

#endif // MAINSCREEN_H
