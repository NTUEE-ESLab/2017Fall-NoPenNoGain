#ifndef PAINT_H
#define PAINT_H

#include <QWidget>
#include <QResizeEvent>
#include <paintscene.h>

#include <QDebug>
#include <QDesktopWidget>
#include <QScreen>


namespace Ui {
class Paint;
}
class Paint : public QWidget
{
    Q_OBJECT

public:
    explicit Paint(QWidget *parent = 0);
    ~Paint();
    void mysetScene(QPixmap &g);
    int  _curpngID;
    int  _newpngID;
    bool changewindow;
    paintScene *scene;  // We declare a custom graphic scene

private:
    Ui::Paint *ui;

private slots :
    void on_exit_button_clicked();
    void on_save_button_clicked();
};


#endif // PAINT_H
