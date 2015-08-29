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


    //Скорость передвижения камеры
    float MOVE_SPEED;
    //проверять или не проверять столкновение кубов (Warning: гравитация не будет работать правильно, если не включено)
    bool COLLIDE_CHECK;
    // if true, дроны могут летать и игнорируют гравитацию
    bool ENABLE_FLYING;
    //размер массива кубов (их количество)
    int n;
    int n_d;
    //собственно кубы
    Cube* cubes;
    Drone* drones;
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
    //все функции ниже будут работать и для дронов, считая, что их id начинаются после кубов
    //я хотел запихнуть все в один массив, но с ходу я слишком киво все запилю, но потом возможно стоит переделать, чтобы не дублировать код
    //рисует куб с соответствующим id (id сейчас индекс в массиве)
    void drawCube(int id);
    //сдвигает соответствующий куб на вектор (x, y, z), возвращает true в случае успешного сдвига, и false иначе
    bool move_cube(int id, GLfloat x, GLfloat y, GLfloat z);
    //перемещает соответствующий куб в точку (x, y, z), возвращает true в случае успешного сдвига, и false иначе
    bool move_cube_to(int id, GLfloat x, GLfloat y, GLfloat z);


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
    //проверяет, пересекается ли данный куб с другими (true - пересекается) + проверка направления
    bool collide_check(int id, GLfloat x_dir, GLfloat y_dir, GLfloat z_dir);
    //обработка очереди задач для дронов
    void do_task();
    //приказывает дрону с соответствующим id захватить кубик под ним
    //в случае, если там ничего нет, должно быть отправлено сообщение об ошибке (пока его нет)
    void capture(int id);
    //приказывает дрону с соответствующим id отпустить кубик (если ничего не было захвачено, то ничего плохого не случится)
    void release (int id);




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



    Scene3D(QWidget* parent = 0);

};

#endif // SCENE3D_H
