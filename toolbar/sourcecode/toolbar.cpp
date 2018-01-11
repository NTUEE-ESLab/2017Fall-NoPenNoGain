#include "toolbar.h"
#include "ui_toolbar.h"
#include <QEvent>


Toolbar::Toolbar(Paint &p,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Toolbar),
    _screen(&p)
{
    ui->setupUi(this);
    setGeometry(
     QApplication::desktop()->size().width()-500,
     QApplication::desktop()->size().height()-size().height()-100,
     size().width(),
     size().height()
    );

    ui->pen->setIcon(QIcon(":/image/pen"));
    ui->eraser->setIcon(QIcon(":/image/eraser"));
    ui->pgup->setIcon(QIcon(":/image/previouspage"));
    ui->pgdn->setIcon(QIcon(":/image/nextpage"));
    ui->minimize->setIcon(QIcon(":/image/minimize"));
    ui->exit->setIcon(QIcon(":/image/exit"));

    setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    setWindowTitle("Toolbar");


}

Toolbar::~Toolbar()
{
    delete ui;
}


void Toolbar::on_pen_pressed()
{
    _screen->scene->setmypen(PEN);

    if(_screen->changewindow)
    {
        QPixmap g;
        _screen->mysetScene(g);
        _screen->changewindow=false;
    }
    _screen->_curpngID = _screen->_newpngID;
}


void Toolbar::on_pgup_pressed()
{
    if(_screen->_curpngID!=1)--_screen->_curpngID;

    QPixmap g(QString("./note/%1.png").arg(_screen->_curpngID));
    _screen->mysetScene(g);
    _screen->changewindow=true;
}

void Toolbar::on_pgdn_pressed()
{
    if(_screen->_curpngID==_screen->_newpngID-1);
    else if(_screen->_curpngID!=_screen->_newpngID) ++_screen->_curpngID;
    else --_screen->_curpngID;

    QPixmap g(QString("./note/%1.png").arg(_screen->_curpngID));
    _screen->mysetScene(g);
    _screen->changewindow=true;
}


void Toolbar::on_minimize_clicked()
{
    showMinimized();
}

void Toolbar::on_exit_clicked()
{
    _screen->close();
    close();
}


void Toolbar ::mouseMoveEvent(QMouseEvent *event)
{
    if(event->windowPos().x()<28)
    setGeometry(
    event->screenPos().x()-15,
    event->screenPos().y()-15,
     size().width(),
     size().height()
    );


}

