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



class cube : public QObject
{
    Q_OBJECT

public:
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
    cube(int port = 0, GLfloat x = -1.0f, GLfloat y = 0.0f, GLfloat z = 0.0f, GLfloat edge = 1.0f, QObject *parent = 0);
    ~cube();
    //смещает куб на вектор (x, y, z)
    void move(GLfloat x, GLfloat y, GLfloat z);
    //переносит центр куба в точку (x, y, z)
    void move_to(GLfloat x, GLfloat y, GLfloat z);
    //пересчитывает координаты вершин
    void update_coord();


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
    int m_Port;

};





#endif // CUBES_H

