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

struct task
{
    int com;//заменить на enum, но пока 0 - движение, 1 - движение с ответом, 2 - зацепить куб под собой, 3 - отпустить
    GLfloat x;
    GLfloat y;
    GLfloat z;
};



class Cube : public QObject
{
    Q_OBJECT

public:
    //id последнего созданного куба
    static int last_id;
    //порядковый номер куба, по хорошему должен совпадать с тем, где он хранится в массиве
    int id;
    //скорость кубика
    GLfloat speed;
    //ignores gravity
    bool flying;
    //id дрона, несущего куб
    int carried_by;
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


    //дефолтный конструктор, создает куб с центром (x, y, z) и ребром длины edge
    Cube(GLfloat x = -1.0f, GLfloat y = 0.0f, GLfloat z = 0.0f, GLfloat edge = 1.0f, QObject *parent = 0);
    ~Cube();
    //смещает куб на вектор (x, y, z)
    void move(GLfloat x, GLfloat y, GLfloat z);
    //переносит центр куба в точку (x, y, z)
    void move_to(GLfloat x, GLfloat y, GLfloat z);
    //пересчитывает координаты вершин
    void update_coord();

};

class Drone : public Cube
{
    Q_OBJECT
public:
    static int last_id_;
    //ну ты понял
    std::queue<task> task_queue;

    //id куба, который в данный момент перетаскивает дрон, -1 если дрон свободен
    int cargo_id;

    //текущая цель куба, если очередь заданий пуста, то игнорируется
    GLfloat dest_x;
    GLfloat dest_y;
    GLfloat dest_z;

    //дефолтный конструктор, создает куб с центром (x, y, z) и ребром длины edge
    Drone(GLfloat x = -1.0f, GLfloat y = 0.0f, GLfloat z = 0.0f, GLfloat edge = 1.0f, QObject *parent = 0);

    //добавляет в задачи путь от текущей цели до точки (x, y, z) (по прямой)
    void add_path(GLfloat x, GLfloat y, GLfloat z);
    //добавляет разные другие команды в очередь (их пока не много)
    void add_command(int cmd);
    //tcp stuff
    //Создает сервер и слушает на порт, если кто-то подает запрос на коннект, вызывает слот коннектед.
    void DoConnect();
    void SendAnswer(std::string answer);
    //Если есть коннект = true
    bool m_Connected;

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
