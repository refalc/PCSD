#ifndef SCENE3D_H
#define SCENE3D_H

#include <QGLWidget>
#include <QtOpenGL>
#include <QTimer>
#include <GL/glu.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <map>
#include <QtNetwork/QUdpSocket>
#include <QCoreApplication>

#include "cubes.h"
//#include "objects.h"



// на данный момент планковская длина около 0.01, на более близких расстояниях пока не проработано особо
class Scene3D : public QGLWidget
{
public:

    //сейчас просто отвечает за движение пары кубов по эллипсам
    float t;
    float shift;
    float tmp;
    //Скорость передвижения камеры
    float MOVE_SPEED;
    //проверять или не проверять столкновение кубов
    bool COLLIDE_CHECK;
    //размер массива кубов (их количество)
    int n;
    //собственно кубы
    cube* cubes;
    //параметры камеры
    //координаты наблюдателя
    float eye[3];
    //координаты точки, на которую смотрят
    float center[3];
    //направление вверх
    float normal[3];

    //здесь предположительно будет набор комманд (пока там ничего нет)
    FILE* commands;
    //пока не используется
    QPoint ptrMousePosition;

    //рисует куб с соответствующим id (id сейчас индекс в массиве)
    void drawCube(int id);
    //сдвигает соответствующий куб на вектор (x, y, z)
    void move_cube(int id, GLfloat x, GLfloat y, GLfloat z);
    //перемещает соответствующий куб в точку (x, y, z)
    void move_cube_to(int id, GLfloat x, GLfloat y, GLfloat z);


    //какие-то технические вещи
    QBasicTimer timer;
    QTimer * aTimer;

    //функции ниже отвечают за перемещение камеры
    void move_forward();
    void move_back();
    void move_left();
    void move_right();
    void move_down();
    void move_up();
    void turn_left();
    void turn_right();
    //гравитация, в данный момент заставляет куб падать линейно вниз, если он ни на чем не стоит
    void gravity_check();
    //проверяет, пересекается ли данный куб с другими (true - пересекается)
    bool collide_check(int id);



    //технические функции opengl
    void initializeGL();
    void resizeGL(int nWidth, int nHeight);
    void paintGL();
    void mousePressEvent(QMouseEvent* pe);
    void mouseMoveEvent(QMouseEvent* pe);
    void mouseReleaseEvent(QMouseEvent* pe);
    void wheelEvent(QWheelEvent* pe);
    void keyPressEvent(QKeyEvent* pe);

    void timerEvent(QTimerEvent* event);



    QUdpSocket *udpSocket;
    Scene3D(QWidget* parent = 0);

};

#endif // SCENE3D_H
