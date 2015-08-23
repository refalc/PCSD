#include "cubes.h"
#include <QtGui>
#include <math.h>

#include <stdio.h>
#include <iostream>
#include <string>

int cube::last_id = -1;

cube::cube(GLfloat x, GLfloat y, GLfloat z, GLfloat edge, QObject *parent)
{
    speed = 0.1;
    last_id++;
    id = last_id;
    m_Port = 48120 + 2*id;
    fixed = false;
    center_x = x;
    center_y = y;
    center_z = z;
    size = edge/2;
    //extreme hardcode, i'll change it later
    //vertex positions
    //filling index
    update_coord();
    IndexArray[0][0] = 0;
    IndexArray[0][1] = 1;
    IndexArray[0][2] = 2;
    IndexArray[1][0] = 0;
    IndexArray[1][1] = 3;
    IndexArray[1][2] = 2;
    IndexArray[2][0] = 2;
    IndexArray[2][1] = 1;
    IndexArray[2][2] = 5;
    IndexArray[3][0] = 2;
    IndexArray[3][1] = 6;
    IndexArray[3][2] = 5;
    IndexArray[4][0] = 0;
    IndexArray[4][1] = 1;
    IndexArray[4][2] = 4;
    IndexArray[5][0] = 1;
    IndexArray[5][1] = 4;
    IndexArray[5][2] = 5;
    IndexArray[6][0] = 0;
    IndexArray[6][1] = 3;
    IndexArray[6][2] = 7;
    IndexArray[7][0] = 0;
    IndexArray[7][1] = 4;
    IndexArray[7][2] = 7;
    IndexArray[8][0] = 4;
    IndexArray[8][1] = 5;
    IndexArray[8][2] = 6;
    IndexArray[9][0] = 4;
    IndexArray[9][1] = 7;
    IndexArray[9][2] = 6;
    IndexArray[10][0] = 2;
    IndexArray[10][1] = 3;
    IndexArray[10][2] = 6;
    IndexArray[11][0] = 3;
    IndexArray[11][1] = 7;
    IndexArray[11][2] = 6;
    //colors
    for (int i = 0; i < 8; i++)
    {
        ColorArray[i][0] = 0.3f*(i%8)/9.0f;
        ColorArray[i][1] = (i%8)/9.0f;
        ColorArray[i][2] = (i%8)/9.0f;
    }

    DoConnect();
}

cube::~cube()
{
    for (int i = 0; i < 8; i++)
    {
        delete[] VertexArray[i];
        delete[] ColorArray[i];
    }
    for (int i = 0; i < 12; i++)
    {
        delete[] IndexArray[i];
    }
    delete [] VertexArray;
    delete [] ColorArray;
    delete [] IndexArray;
    //delete some stuff
}

void cube::update_coord()
{
    //updates coordinates of cube vertices based on size and center's coodrinates
    VertexArray[0][0] = center_x - size;
    VertexArray[0][1] = center_y + size;
    VertexArray[0][2] = center_z - size;
    VertexArray[1][0] = center_x + size;
    VertexArray[1][1] = center_y + size;
    VertexArray[1][2] = center_z - size;
    VertexArray[2][0] = center_x + size;
    VertexArray[2][1] = center_y + size;
    VertexArray[2][2] = center_z + size;
    VertexArray[3][0] = center_x - size;
    VertexArray[3][1] = center_y + size;
    VertexArray[3][2] = center_z + size;
    VertexArray[4][0] = center_x - size;
    VertexArray[4][1] = center_y - size;
    VertexArray[4][2] = center_z - size;
    VertexArray[5][0] = center_x + size;
    VertexArray[5][1] = center_y - size;
    VertexArray[5][2] = center_z - size;
    VertexArray[6][0] = center_x + size;
    VertexArray[6][1] = center_y - size;
    VertexArray[6][2] = center_z + size;
    VertexArray[7][0] = center_x - size;
    VertexArray[7][1] = center_y - size;
    VertexArray[7][2] = center_z + size;
}

void cube::move(GLfloat x, GLfloat y, GLfloat z)
{
    center_x += x;
    center_y += y;
    center_z += z;
    update_coord();
}

void cube::move_to(GLfloat x, GLfloat y, GLfloat z)
{
    center_x = x;
    center_y = y;
    center_z = z;
    update_coord();
}

void cube::DoConnect()
{
    connect(&m_Server, SIGNAL(newConnection()),
        this, SLOT(Connected()));

    m_Server.listen(QHostAddress::Any, m_Port);

}

void cube::Connected()
{
    std::cout << "Connected\n";
    m_Client = m_Server.nextPendingConnection(); //m_Client on connection
    connect(m_Client, SIGNAL(readyRead()), this, SLOT(ReadCmd()));  //If recive data call ReadCmd()
    m_Client->write("Hello Client\n");
}

void cube::Disconnected()
{
    //if disconnected
    std::cout << "Disonnected\n";
}

void cube::ReadCmd()
{
    GLfloat x, y, z;
    //When recive data
    char buffer[1024] = {0};
    m_Client->read(buffer, m_Client->bytesAvailable());

    std::string answer("My Z crd = ");
    answer.append(std::to_string(center_z));
    m_Client->write(answer.c_str());

    //Decode cmd there

    //Do Task there
    add_path(x, y, z);

}

void cube::do_task()
{
    GLfloat x, y, z;
    if (task_queue.size() < 4) return;
    //if (task_queue.front() > 0.5) m_Client->write("OK|");
    task_queue.pop();
    x = task_queue.front();
    task_queue.pop();
    y = task_queue.front();
    task_queue.pop();
    z = task_queue.front();
    task_queue.pop();
    move(x, y, z);
}

void cube::add_path(GLfloat x, GLfloat y, GLfloat z)
{
    if (task_queue.empty())
    {
        dest_x = center_x;
        dest_y = center_y;
        dest_z = center_z;
    }
    double dist = sqrt((x - dest_x)*(x - dest_x) + (y - dest_y)*(y - dest_y) + (z - dest_z)*(z - dest_z));
    int iter = dist/speed;
    double dir_x = (x - dest_x)/dist;
    double dir_y = (y - dest_y)/dist;
    double dir_z = (z - dest_z)/dist;
    for (int i = 0; i < iter; i++)
    {
        task_queue.push(0);
        task_queue.push(speed*dir_x);
        task_queue.push(speed*dir_y);
        task_queue.push(speed*dir_z);
    }
    task_queue.push(1);
    task_queue.push(x - (iter*speed*dir_x + dest_x));
    task_queue.push(y - (iter*speed*dir_y + dest_y));
    task_queue.push(z - (iter*speed*dir_z + dest_z));
    dest_x = x;
    dest_y = y;
    dest_z = z;
}
