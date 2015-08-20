#include <QtGui>
#include "widget.h"
//#include "objects.h"
#include <QApplication>
#include <QTextCodec>



int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    Scene3D scene1;
    scene1.setWindowTitle("Test_enviroment");
    scene1.resize(500, 500);
    scene1.show();

   return app.exec();
}
