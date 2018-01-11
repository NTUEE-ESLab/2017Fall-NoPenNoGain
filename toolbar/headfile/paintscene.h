#ifndef PAINTSCENE_H
#define PAINTSCENE_H
 
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
enum myPen
{
    PEN=0,
    ERASER=1
};

class paintScene : public QGraphicsScene
{
 
    Q_OBJECT
 
public:
    explicit paintScene(QObject *parent = 0);
    ~paintScene();
    void setmycolor(QColor c){mycolor=c;}
    void setmyr(unsigned int r){myr=r;}
    void setmypen(int pen){mypen=pen;}
 
private:
    QPointF     previousPoint;      // The coordinates of the previous point
    QPointF     nowPoint;
    QColor      mycolor;
    unsigned int    myr;
    unsigned int    mypen;
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void myaddEllipse();
    void myaddLine();
    void myerase();

 
};

#endif // PAINTSCENE_H
