#include "cubes.h"
#include <QtGui>
#include <math.h>

#include <stdio.h>
#include <iostream>

cube::cube(int port, GLfloat x, GLfloat y, GLfloat z, GLfloat edge, QObject *parent)
{
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

    m_Server.listen(QHostAddress::Any, 8888);

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
    //When recive data
    char buffer[1024] = {0};
    m_Client->read(buffer, m_Client->bytesAvailable());

    //m_Client->write(buffer);

    //Decode cmd there

    //Do Task there

}
