#include "widget.h"
#include "cubes.h"
#include <QtGui>
#include <math.h>

#include <stdio.h>




Scene3D::Scene3D(QWidget* parent) : QGLWidget(parent)
{
    udpSocket = new QUdpSocket();
    udpSocket->bind(45454, QUdpSocket::ShareAddress);
    char buff[6];
    QByteArray datagram = itoa(135, buff, 10);
    QHostAddress Address = QHostAddress("127.0.0.1");
    udpSocket->writeDatagram(datagram, Address, 45454);
    datagram.clear();
    shift = 0.01;
    tmp = 0;
    commands = fopen("commands.txt", "w");
    aTimer = new QTimer();
    t = 0;
    timer.start(30, this);
    MOVE_SPEED = 0.1;
    COLLIDE_CHECK = true;
    eye[0] = -7;
    eye[1] = -5;
    eye[2] = 0.5;
    center[0] = 0;
    center[1] = -5;
    center[2] = 0.5;
    normal[0] = 0;
    normal[1] = 0;
    normal[2] = 1;
    FILE* f = fopen("starting_settings.txt", "r");
    n = 10;
    int id;
    float x, y, z;
    if (f != NULL)
    {
    fscanf(f, "%d\n", &n);
    cubes = new cube [n];
    for (int i = 0; i < n; i++)
    {
        fscanf(f, "%d %f %f %f", &id, &x, &y, &z);
        cubes[id].move_to(x, y, z);
    }
    fclose(f);
    }
    else fprintf(commands, "fail");
    fclose(commands);
    cubes[0].size = 200;
    cubes[0].center_x = 0;
    cubes[0].center_y = 0;
    cubes[0].center_z = -200;
    cubes[0].update_coord();

}

void Scene3D::timerEvent(QTimerEvent* event)
{
    update();
}

void Scene3D::initializeGL()
{
    qglClearColor(Qt::white);
    glEnable(GL_DEPTH_TEST);
    //glShadeModel(GL_FLAT);
    //glEnable(GL_CULL_FACE);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
}

void Scene3D::resizeGL(int nWidth, int nHeight)
{

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    GLfloat ratio=(GLfloat)nHeight/(GLfloat)nWidth;

    /*if (nWidth>=nHeight)
        glOrtho(-10.0/ratio, 10.0/ratio, -10.0, 10.0, -10.0, 10.0);
    else
        glOrtho(-10.0, 100.0, -10.0*ratio, 10.0*ratio, -10.0, 10.0);*/
    glFrustum(-1,1,-1,1, 1.5, 20);
    glViewport(0, 0, (GLint)nWidth, (GLint)nHeight);
}

void Scene3D::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye[0],eye[1],eye[2],center[0],center[1],center[2],normal[0],normal[1],normal[2]);
    //glScalef(nSca, nSca, nSca);
    //glTranslatef(0.0f, zTra, 0.0f);
    //glRotatef(0, 1.0f, 0.0f, 0.0f);
    gravity_check();
    move_cube_to(19, 4*sin(t), 5*cos(t), 0.5);
    move_cube_to(20, 0, 9*cos(t), 8*sin(t));
    move_cube(25, 0, 0.01, 0);

    move_cube(28, shift, 0, 0);
    if (abs(tmp - cubes[28].center_x) < 0.0001) shift = -shift;
    tmp = cubes[28].center_x;
    t += 0.01;
    for (int i = 0; i < n; i++) drawCube(i);

}

void Scene3D::mousePressEvent(QMouseEvent* pe)
{
    ptrMousePosition = pe->pos();
}

void Scene3D::mouseReleaseEvent(QMouseEvent* pe)
{

}

void Scene3D::mouseMoveEvent(QMouseEvent* pe)
{
    ptrMousePosition = pe->pos();

    updateGL();
}

void Scene3D::wheelEvent(QWheelEvent* pe)
{
    updateGL();
}

void Scene3D::keyPressEvent(QKeyEvent* pe)
{
    switch (pe->key())
    {
    case Qt::Key_Escape:
        this->close();
        break;
    case Qt::Key_W:
        move_forward();
        break;
    case Qt::Key_S:
        move_back();
        break;
    case Qt::Key_Q:
        move_left();
        break;
    case Qt::Key_E:
        move_right();
        break;
    case Qt::Key_A:
        turn_left();
        break;
    case Qt::Key_D:
        turn_right();
        break;
    case Qt::Key_Z:
        move_down();
        break;
    case Qt::Key_X:
        move_up();
        break;
    default:
        break;

    }
    updateGL();
}



void Scene3D::drawCube(int id)
{
    glVertexPointer(3, GL_FLOAT, 0, cubes[id].VertexArray);
    glColorPointer(3, GL_FLOAT, 0, cubes[id].ColorArray);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, cubes[id].IndexArray);
}

void Scene3D::move_cube(int id, GLfloat x, GLfloat y, GLfloat z)
{
    if (COLLIDE_CHECK)
    {
        //меняем координаты центра
        cubes[id].center_x += x;
        cubes[id].center_y += y;
        cubes[id].center_z += z;
        //если результирующий куб пересекается с чем-нибудь, меняем координаты на старые
        if (collide_check(id))
        {
            cubes[id].center_x -= x;
            cubes[id].center_y -= y;
            cubes[id].center_z -= z;
        }
        //иначе пересчитываем координаты вершин
        else cubes[id].update_coord();
    }
    else cubes[id].move(x, y, z);
}

void Scene3D::move_cube_to(int id, GLfloat x, GLfloat y, GLfloat z)
{
    if (COLLIDE_CHECK)
    {
        //меняем координаты центра
        GLfloat x_old, y_old, z_old;
        x_old = cubes[id].center_x;
        y_old = cubes[id].center_y;
        z_old = cubes[id].center_z;
        cubes[id].center_x = x;
        cubes[id].center_y = y;
        cubes[id].center_z = z;
        //если результирующий куб пересекается с чем-нибудь, меняем координаты на старые
        if (collide_check(id))
        {
            cubes[id].center_x = x_old;
            cubes[id].center_y = y_old;
            cubes[id].center_z = z_old;
        }
        //иначе пересчитываем координаты вершин
        else cubes[id].update_coord();
    }
    else cubes[id].move_to(x, y, z);
}

void Scene3D::move_up()
{
    center[2] += MOVE_SPEED;
    eye[2] += MOVE_SPEED;
}

void Scene3D::move_down()
{
    center[2] -= MOVE_SPEED;
    eye[2] -= MOVE_SPEED;
}

void Scene3D::move_forward()
{
    float dir_x = center[0] - eye[0];
    float dir_y = center[1] - eye[1];
    dir_x = dir_x/sqrt(dir_x*dir_x + dir_y*dir_y);
    dir_y = dir_y/sqrt(dir_x*dir_x + dir_y*dir_y);
    center[0] += MOVE_SPEED*dir_x;
    center[1] += MOVE_SPEED*dir_y;
    eye[0] += MOVE_SPEED*dir_x;
    eye[1] += MOVE_SPEED*dir_y;
}

void Scene3D::move_back()
{
    float dir_x = center[0] - eye[0];
    float dir_y = center[1] - eye[1];
    dir_x = dir_x/sqrt(dir_x*dir_x + dir_y*dir_y);
    dir_y = dir_y/sqrt(dir_x*dir_x + dir_y*dir_y);
    center[0] -= MOVE_SPEED*dir_x;
    center[1] -= MOVE_SPEED*dir_y;
    eye[0] -= MOVE_SPEED*dir_x;
    eye[1] -= MOVE_SPEED*dir_y;
}

void Scene3D::move_left()
{
    float dir_x = center[0] - eye[0];
    float dir_y = center[1] - eye[1];
    dir_x = dir_x/sqrt(dir_x*dir_x + dir_y*dir_y);
    dir_y = dir_y/sqrt(dir_x*dir_x + dir_y*dir_y);
    center[0] -= MOVE_SPEED*dir_y;
    center[1] += MOVE_SPEED*dir_x;
    eye[0] -= MOVE_SPEED*dir_y;
    eye[1] += MOVE_SPEED*dir_x;
}

void Scene3D::move_right()
{
    float dir_x = center[0] - eye[0];
    float dir_y = center[1] - eye[1];
    dir_x = dir_x/sqrt(dir_x*dir_x + dir_y*dir_y);
    dir_y = dir_y/sqrt(dir_x*dir_x + dir_y*dir_y);
    center[0] += MOVE_SPEED*dir_y;
    center[1] -= MOVE_SPEED*dir_x;
    eye[0] += MOVE_SPEED*dir_y;
    eye[1] -= MOVE_SPEED*dir_x;
}

void Scene3D::turn_right()
{
    float dir_x = center[0] - eye[0];
    float dir_y = center[1] - eye[1];
    float ort_x = MOVE_SPEED*dir_y/sqrt(dir_x*dir_x + dir_y*dir_y);
    float ort_y = -MOVE_SPEED*dir_x/sqrt(dir_x*dir_x + dir_y*dir_y);
    float r = sqrt(dir_x*dir_x + dir_y*dir_y);
    float r1 = sqrt((dir_x+ort_x)*(dir_x+ort_x) +  (dir_y+ort_y)*(dir_y+ort_y));
    center[0] = eye[0] + (dir_x + ort_x)*r/r1;
    center[1] = eye[1] + (dir_y + ort_y)*r/r1;
}

void Scene3D::turn_left()
{
    float dir_x = center[0] - eye[0];
    float dir_y = center[1] - eye[1];
    float ort_x = -MOVE_SPEED*dir_y/sqrt(dir_x*dir_x + dir_y*dir_y);
    float ort_y = MOVE_SPEED*dir_x/sqrt(dir_x*dir_x + dir_y*dir_y);
    float r = sqrt(dir_x*dir_x + dir_y*dir_y);
    float r1 = sqrt((dir_x+ort_x)*(dir_x+ort_x) +  (dir_y+ort_y)*(dir_y+ort_y));
    center[0] = eye[0] + (dir_x + ort_x)*r/r1;
    center[1] = eye[1] + (dir_y + ort_y)*r/r1;
}


void Scene3D::gravity_check()
{
    for(int i = 0; i < n; i++)
    {
        if (cubes[i].fixed) continue;
        if (cubes[i].center_z <= cubes[i].size) continue;
        float bot = cubes[i].center_z - cubes[i].size;
        float right_y = cubes[i].center_y + cubes[i].size;
        float right_x = cubes[i].center_x + cubes[i].size;
        float left_y = cubes[i].center_y - cubes[i].size;
        float left_x = cubes[i].center_x - cubes[i].size;
        //check if there is an object below;
        bool free = true;
        for (int j = 0; j < n; j++)
        {
            float szj = cubes[j].size;
            if (i == j) continue;
            if ((bot <= (cubes[j].center_z + szj +  0.03))&&
                    (bot >= (cubes[j].center_z + szj - 0.03))
                && (right_y > cubes[j].center_y - szj) && (left_y < cubes[j].center_y + szj)&&
                (right_x > cubes[j].center_x - szj) && (left_x < cubes[j].center_x + szj))
                {
                    free = false;
                    break;
                }
        }
        if (free)
        {
            cubes[i].center_z -= 0.01;
            cubes[i].update_coord();
        }
    }
}

bool Scene3D::collide_check(int id)
{
    float x = cubes[id].center_x;
    float y = cubes[id].center_y;
    float z = cubes[id].center_z;
    float r = cubes[id].size;
    for (int i = 0; i < n; i++)
    {
        if (i == id) continue;
        if ((abs(x - cubes[i].center_x) < r + cubes[i].size) &&
                (abs(y - cubes[i].center_y) < r + cubes[i].size) &&
                (abs(z - cubes[i].center_z) < r + cubes[i].size)) return true;

    }
    return false;
}





