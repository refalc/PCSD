#include "cubes.h"
#include <QtGui>
#include <math.h>

#include <stdio.h>
#include <iostream>
#include <string>

int Cube::last_id = -1;
int Drone::last_id_ = -1;

Cube::Cube(GLfloat x, GLfloat y, GLfloat z, GLfloat edge, QObject *parent)
{
    speed = 0.1;
    last_id++;
    id = last_id;
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

Cube::~Cube()
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

void Cube::update_coord()
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

void Cube::move(GLfloat x, GLfloat y, GLfloat z)
{
    center_x += x;
    center_y += y;
    center_z += z;
    update_coord();
}

void Cube::move_to(GLfloat x, GLfloat y, GLfloat z)
{
    center_x = x;
    center_y = y;
    center_z = z;
    update_coord();
}

Drone::Drone(GLfloat x, GLfloat y, GLfloat z, GLfloat edge, QObject *parent):Cube(x, y, z, edge, parent)
{
    //
    flying = false;
    carried_by = -1;
    last_id_++;
    id = last_id_;
    cargo_id = -1;
    m_Port = 48120 + id;
    m_Connected  = false;
    DoConnect();
}

void Drone::DoConnect()
{
    connect(&m_Server, SIGNAL(newConnection()),
        this, SLOT(Connected()));

    m_Server.listen(QHostAddress::Any, m_Port);

}

void Drone::Connected()
{
    std::cout << "Connected\n";
    m_Client = m_Server.nextPendingConnection(); //m_Client on connection
    connect(m_Client, SIGNAL(readyRead()), this, SLOT(ReadCmd()));  //If recive data call ReadCmd()
    m_Connected = true;
    SendAnswer("Hello Client\n");
}

void Drone::Disconnected()
{
    //if disconnected
    std::cout << "Disonnected\n";
}

void Drone::ReadCmd()
{
    GLfloat x, y, z;
    //When recive data
    char buffer[1024] = {0};
    m_Client->read(buffer, m_Client->bytesAvailable());

    //std::string answer("My Z crd = ");
    //answer.append(std::to_string(center_z));
    //m_Client->write(answer.c_str());

    //Decode cmd there

    std::string cmd(buffer);
    if(cmd.find("M_") != std::string::npos)
    {
        cmd = cmd.substr(2);
        int pos = 0, size = cmd.size();
        char symb = cmd[pos];
        std::string tempStr;
        while(symb != ':')
        {
            tempStr.push_back(symb);
            pos++;
            if(pos == size)
            {
                std::string answer("Error cmd:");
                answer.append(buffer);
                SendAnswer(answer);
                return;
            }
            symb = cmd[pos];
        }
        x = atof(tempStr.c_str());
        tempStr.clear();

        pos++;
        symb = cmd[pos];
        while(symb != ':')
        {
            tempStr.push_back(symb);
            pos++;
            if(pos == size)
            {
                std::string answer("Error cmd:");
                answer.append(buffer);
                SendAnswer(answer);
                return;
            }
            symb = cmd[pos];
        }
        y = atof(tempStr.c_str());
        tempStr.clear();

        pos++;
        symb = cmd[pos];
        while(symb != '|')
        {
            tempStr.push_back(symb);
            pos++;
            if(pos == size)
            {
                std::string answer("Error cmd:");
                answer.append(buffer);
                SendAnswer(answer);
                return;
            }
            symb = cmd[pos];
        }
        z = atof(tempStr.c_str());

        add_path(x, y, z);
        std::string answer("Good cmd: add Path\n");
        answer.append(std::to_string(x));
        answer.push_back(' ');
        answer.append(std::to_string(y));
        answer.push_back(' ');
        answer.append(std::to_string(z));
        answer.push_back(' ');
        SendAnswer(answer);
    }
    else
    {
        std::string answer("Error cmd:");
        answer.append(buffer);
        SendAnswer(answer);
    }
    //Do Task there
    //add_path(x, y, z);
//TODO: добавить в парсер возможность других команд, кроме движения, исп add_command(int);

}


void Drone::SendAnswer(std::string answer)
{
    if(m_Connected)
        m_Client->write(answer.c_str());
}

void Drone::add_path(GLfloat x, GLfloat y, GLfloat z)
{
    task tsk;
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
    tsk.com = 0;
    tsk.x = speed*dir_x;
    tsk.y = speed*dir_y;
    tsk.z = speed*dir_z;
    for (int i = 0; i < iter; i++)
        task_queue.push(tsk);

    tsk.com = 1;
    tsk.x = x - (iter*speed*dir_x + dest_x);
    tsk.y = y - (iter*speed*dir_y + dest_y);
    tsk.z = z - (iter*speed*dir_z + dest_z);
    task_queue.push(tsk);

    dest_x = x;
    dest_y = y;
    dest_z = z;
}

void Drone::add_command(int cmd)
{
    task tsk;
    tsk.com = cmd;
    tsk.x = 0;
    tsk.y = 0;
    tsk.z = 0;
    task_queue.push(tsk);
}
