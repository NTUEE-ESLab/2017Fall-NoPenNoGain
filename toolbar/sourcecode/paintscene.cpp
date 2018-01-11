#include "paintscene.h"
#include <QDebug>
#include <QGraphicsItem>
#include "windows.h"
paintScene::paintScene(QObject *parent) : QGraphicsScene(parent)
{
    mycolor=Qt::red;
    myr=5;
    mypen=PEN;
}
 
paintScene::~paintScene()
{
 
}

void paintScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    std::unique_lock<std::mutex> lk(m);
    click = true;
    lk.unlock();
    cv.notify_one();
    
    nowPoint=event->scenePos();
    switch (mypen)
    {
        case 0 : myaddEllipse();break;
        case 1 : myerase();break;
    }

    previousPoint = event->scenePos();
}

void paintScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    nowPoint=event->scenePos();
    switch (mypen)
    {
        case 0 : myaddLine();break;
        case 1 :
            nowPoint=(event->scenePos()+previousPoint)/2;
            myerase();break;
    }

    previousPoint = event->scenePos();
}

void paintScene::myaddEllipse()
{
    addEllipse(nowPoint.x()-myr,
                nowPoint.y()-myr,
                2*myr,2*myr,                //a,b
                QPen(Qt::NoPen),            //外框
                QBrush(mycolor));           //筆色
}
void paintScene::myaddLine()
{
    addLine(previousPoint.x(),
            previousPoint.y(),
            nowPoint.x(),
            nowPoint.y(),
            QPen(mycolor,2*myr,Qt::SolidLine,Qt::RoundCap));
            //顏色  粗細   種類    圓頭
}
void paintScene::myerase()
{
    QGraphicsItem *_n;
    for(int i=nowPoint.x()-myr;i<=nowPoint.x()+myr;++i)
    {
        for(int j=nowPoint.y()-myr;j<=nowPoint.y()+myr;++j)
        {
            _n=itemAt(i,j,QTransform());
            while (_n!=0)
            {
                removeItem(_n);
                _n=itemAt(i,j,QTransform());
            }
        }
    }
}
