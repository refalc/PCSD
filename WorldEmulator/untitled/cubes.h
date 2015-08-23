#ifndef CUBES_H
#define CUBES_H

#include <QGLWidget>
#include <QtOpenGL>
#include <QTimer>
#include <GL/glu.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <map>
#include <QObject>
#include <QtNetwork/QUdpSocket>
#include <QCoreApplication>
#include <QTcpSocket>
#include <QTcpServer>
#include<stack>
#include<queue>
#include<deque>



class cube : public QObject
{
    Q_OBJECT

public:
    //id последнего созданного куба
    static int last_id;
    //порядковый номер куба, по хорошему должен совпадать с тем, где он хранится в массиве
    int id;
    //скорость кубика
    GLfloat speed;
    //текущая цель куба, если очередь заданий пуста, то игнорируется
    GLfloat dest_x;
    GLfloat dest_y;
    GLfloat dest_z;
    //координаты центра куба
    GLfloat center_x;
    GLfloat center_y;
    GLfloat center_z;
    //size - половина ребра куба
    GLfloat size;
    //зафиксированный куб не падает (и скорее всего вообще не может двигаться)
    bool fixed;
    //технические массивы, задающие куб
    GLfloat VertexArray[12][3];
    GLfloat ColorArray[12][3];
    GLubyte IndexArray[20][3];

    //чтобы не делать очереди из сложных элементов, поскольку она будет часто обрабатываться, координаты будут извлекаться по тройкам подряд
    std::queue<double> task_queue;

    //дефолтный конструктор, создает куб с центром (x, y, z) и ребром длины edge
    cube(GLfloat x = -1.0f, GLfloat y = 0.0f, GLfloat z = 0.0f, GLfloat edge = 1.0f, QObject *parent = 0);
    ~cube();
    //смещает куб на вектор (x, y, z)
    void move(GLfloat x, GLfloat y, GLfloat z);
    //переносит центр куба в точку (x, y, z)
    void move_to(GLfloat x, GLfloat y, GLfloat z);
    //пересчитывает координаты вершин
    void update_coord();
    //проверяет, есть ли задания в очереди, и если есть, выполняет следующее.
    void do_task();
    //добавляет в задачи путь от текущей цели до точки (x, y, z) (по прямой)
    void add_path(GLfloat x, GLfloat y, GLfloat z);

    //tcp stuff
     void DoConnect();

 signals:

 public slots:

     //tcp test
     void Connected();
     void Disconnected();
     void ReadCmd();

 private:
     QTcpServer m_Server;
     QTcpSocket *m_Client;
     //отвечает за порт, использующийся в тсп соединении
     int m_Port;

};





#endif // CUBES_H

