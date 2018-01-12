#include "toolbar.h"
#include <QApplication>
#include <QPalette>

int main(int argc, char *argv[])
{

//set Folder
    QDateTime _time;
    _time = QDateTime::currentDateTime();
    QString _filename=_time.toString("yyyy-MM-dd-hhmm");
    QDir* dir = new QDir();
    int version=1;
    while(true){
        if(!dir->exists(_filename)){
            dir->mkdir(_filename);
            break;
        }
        else{
            _filename=QString("%1 (%2)").arg(_time.toString("yyyy-MM-dd-hhmm")).arg(version);
            ++version;
        }
     }
    dir->cd(_filename);
    dir->mkdir("Screenshot");
    dir->mkdir("note");
    dir->setCurrent(_filename);

//set windows
    QApplication a(argc, argv);
    Paint _drawindow;
    Toolbar _toolbar(_drawindow);
    _toolbar.show();

return a.exec();

}

