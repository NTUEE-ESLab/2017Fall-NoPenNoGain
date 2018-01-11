#include "paint.h"
#include "ui_paint.h"



Paint::Paint(QWidget *parent) :
    QWidget(parent),
    _curpngID(1),
    _newpngID(1),
    changewindow(true),
    ui(new Ui::Paint)
{
    ui->setupUi(this);

//set screen resolution
    QSize Screensize =QApplication::desktop()->size();
// Init graphics scene
    scene = new paintScene();
    scene->setSceneRect(0,0, ui->graphicsView->width(), ui->graphicsView->height());

// Set graphics scene
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setStyleSheet("background-color: rgba(255, 255, 255, 0.01);");
    ui->graphicsView->resize(Screensize.width()+20,Screensize.height()+20);

//set button position
    ui->exit_button->setGeometry(Screensize.width()-70,Screensize.height()-50,70,50);
    ui->save_button->setGeometry(Screensize.width()-140,Screensize.height()-50,70,50);

    ui->graphicsView->installEventFilter(this);

    setWindowFlags(Qt::FramelessWindowHint);    //取消視窗外框按鈕
    setAttribute(Qt::WA_TranslucentBackground); //設滑鼠是否能穿透視窗
    setWindowTitle("Screen");
}


Paint::~Paint()
{
    delete ui;
}

void Paint::on_exit_button_clicked()
{
    scene->clear();
    this->close();
    changewindow=true;
}

void Paint::on_save_button_clicked()
{
    QString _filename=QString("%1.png").arg(_curpngID);
    if(_curpngID==_newpngID)_curpngID=++_newpngID;
    QScreen *screen = QGuiApplication::primaryScreen();
    screen->grabWindow(0).save("./Screenshot/"+_filename);
    ui->graphicsView->grab().save("./note/"+_filename);
    scene->clear();
    this->close();
    changewindow=true;

}
void Paint::mysetScene(QPixmap &g)
{
    scene->clear();
    scene->addPixmap(g);
    scene->setSceneRect(0,0, ui->graphicsView->width()-20, ui->graphicsView->height()-20);
    showFullScreen();
}
