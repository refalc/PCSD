#include "widget.h"
#include "cubes.h"
#include <QtGui>
#include <math.h>

#include <stdio.h>
#include <string>



Scene3D::Scene3D(QWidget* parent) : QGLWidget(parent)
{

    commands = fopen("errors.txt", "w");
    aTimer = new QTimer();
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
    else
    {
        fprintf(commands, "fail, file not found");
        cubes = new cube[10];
    }
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
    glFrustum(-1,1,-1,1, 1, 50);
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
    do_task();
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
    case Qt::Key_C:
        //cubes[29].add_path(-5, 5, 10);
        cubes[3].add_path(-5, 3.9, 0.5);
        break;
    case Qt::Key_V:
        cubes[3].add_path(5, 3.9, 0.5);
        break;
    case Qt::Key_F:
        cubes[4].add_path(0, 5, 0.5);
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

bool Scene3D::move_cube(int id, GLfloat x, GLfloat y, GLfloat z)
{
    if (COLLIDE_CHECK)
    {
        //меняем координаты центра
        cubes[id].center_x += x;
        cubes[id].center_y += y;
        cubes[id].center_z += z;
        //если результирующий куб пересекается с чем-нибудь, меняем координаты на старые
        if (collide_check(id, x, y, z))
        {
            cubes[id].center_x -= x;
            cubes[id].center_y -= y;
            cubes[id].center_z -= z;
            //delete after
            while(!cubes[id].task_queue.empty())
                cubes[id].task_queue.pop();
            cubes[id].SendAnswer("OK|");
            //
            return false;
        }
        //иначе пересчитываем координаты вершин
        else cubes[id].update_coord();
    }
    else cubes[id].move(x, y, z);
    return true;
}

bool Scene3D::move_cube_to(int id, GLfloat x, GLfloat y, GLfloat z)
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
        if (collide_check(id, x - x_old, y - y_old, z - z_old))
        {
            cubes[id].center_x = x_old;
            cubes[id].center_y = y_old;
            cubes[id].center_z = z_old;
            return false;
        }
        //иначе пересчитываем координаты вершин
        else cubes[id].update_coord();
    }
    else cubes[id].move_to(x, y, z);
    return true;
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
    for(int i = 1; i < n; i++)
    {
        if (cubes[i].fixed)
            continue;
        move_cube_to(i, cubes[i].center_x, cubes[i].center_y, cubes[i].center_z - 0.01);
        //move_cube(i, 0, 0, -0.01);
    }
}


bool Scene3D::collide_check(int id, GLfloat x_dir, GLfloat y_dir, GLfloat z_dir)
{
    float x = cubes[id].center_x;
    float y = cubes[id].center_y;
    float z = cubes[id].center_z;
    float r = cubes[id].size;
    float eps = 0;
    for (int i = 0; i < n; i++)
    {
        if (i == id) continue;
        float x_diff = x - cubes[i].center_x;
        float y_diff = y - cubes[i].center_y;
        float z_diff = z - cubes[i].center_z;
        if ((abs(x_diff) < r + cubes[i].size - eps) &&
                (abs(y_diff) < r + cubes[i].size - eps) &&
                (abs(z_diff) < r + cubes[i].size - eps))
        {

            //cubes[id].SendAnswer("CollideCheckFail:id = " + std::to_string(id) + " with id = " + std::to_string(i));
            return true;
        }

    }
    return false;
}

void Scene3D::do_task()
{
    GLfloat x, y, z;
    for (int i = 0; i < n; i++)
    {
        if (cubes[i].task_queue.empty()) continue;
        x = cubes[i].task_queue.front().x;
        y = cubes[i].task_queue.front().y;
        z = cubes[i].task_queue.front().z;
        if (move_cube(i, x, y, z))
        {
            //check cubes[i].task_queue.front().com
            if(cubes[i].task_queue.front().com > 0.5)
                cubes[i].SendAnswer("OK|");
            cubes[i].task_queue.pop();
            continue;
        }
        //else send some error message or smth

    }
}
