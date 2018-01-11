#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QDialog>

#include "paint.h"
#include <QDateTime>
#include <QDir>
#include <QString>
#include <QDebug>
#include <QDesktopWidget>


namespace Ui {
class Toolbar;
}

class Toolbar : public QDialog
{
    Q_OBJECT

public:
    explicit Toolbar(Paint &p,QWidget *parent = 0);
    ~Toolbar();

private slots:
    void on_pen_pressed();

    void on_red_pressed(){_screen->scene->setmycolor(Qt::red);}

    void on_blue_pressed(){_screen->scene->setmycolor(Qt::blue);}

    void on_black_pressed(){_screen->scene->setmycolor(Qt::black);}

    void on_small_pressed(){_screen->scene->setmyr(5);}

    void on_mid_pressed(){_screen->scene->setmyr(10);}

    void on_big_pressed(){_screen->scene->setmyr(20);}

    void on_eraser_pressed(){_screen->scene->setmypen(ERASER);}

    void on_pgup_pressed();

    void on_pgdn_pressed();

    void on_minimize_clicked();

    void on_exit_clicked();


private:
    Ui::Toolbar *ui;
    Paint *_screen;
    void mouseMoveEvent(QMouseEvent *event);

};

#endif // TOOLBAR_H
