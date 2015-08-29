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
    ENABLE_FLYING = true;
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
    n_d = 1;
    int id;//fuck it for now
    float x, y, z;
    if (f != NULL)
    {
    fscanf(f, "%d %d\n", &n, &n_d);
    cubes = new Cube [n];
    drones = new Drone [n_d];
    for (int i = 0; i < n; i++)
    {
        fscanf(f, "%d %f %f %f", &id, &x, &y, &z);
        cubes[i].move_to(x, y, z);
    }
    for (int i = 0; i < n_d; i++)
    {
        fscanf(f, "%d %f %f %f", &id, &x, &y, &z);
        drones[i].move_to(x, y, z);
    }
    fclose(f);
    }
    else
    {
        fprintf(commands, "fail, file not found");
        cubes = new Cube[10];
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
    for (int i = 0; i < n + n_d; i++) drawCube(i);

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
        //cubes[3].add_path(-5, 3.9, 0.5);
        //drones[1].add_path(0, 7, 0.5);
        drones[1].add_path(0, 0, 1.5);
        drones[1].add_path(0, 8, 1.5);
        drones[1].add_command(2);
        drones[1].add_path(0, 8, 4.5);
        drones[1].add_path(0, 8, 1.6);
        drones[1].add_path(0, 8, 2.2);
        drones[1].add_path(0, 0, 2.2);
        drones[1].add_command(3);
        break;

    case Qt::Key_V:
        drones[0].add_path(7, 5, 0.5);
        //cubes[3].add_path(5, 3.9, 0.5);
        break;
    case Qt::Key_F:
        //cubes[4].add_path(0, 5, 0.5);
        break;

    default:
        break;

    }
    updateGL();
}



void Scene3D::drawCube(int id)
{
    if (id < n)
    {
        glVertexPointer(3, GL_FLOAT, 0, cubes[id].VertexArray);
        glColorPointer(3, GL_FLOAT, 0, cubes[id].ColorArray);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, cubes[id].IndexArray);
    }
    else
    {
        id -= n;
        glVertexPointer(3, GL_FLOAT, 0, drones[id].VertexArray);
        glColorPointer(3, GL_FLOAT, 0, drones[id].ColorArray);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, drones[id].IndexArray);
    }
}

bool Scene3D::move_cube(int id, GLfloat x, GLfloat y, GLfloat z)
{
    if (id < n)
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
                return false;
            }
            //иначе пересчитываем координаты вершин
            else cubes[id].update_coord();
        }
        else cubes[id].move(x, y, z);
        return true;
    }
    else
    {
        if (COLLIDE_CHECK)
        {
            id -= n;
            //меняем координаты центра
            drones[id].center_x += x;
            drones[id].center_y += y;
            drones[id].center_z += z;
            //если результирующий куб пересекается с чем-нибудь, меняем координаты на старые
            if (collide_check(id + n, x, y, z))
            {
                drones[id].center_x -= x;
                drones[id].center_y -= y;
                drones[id].center_z -= z;
                return false;
            }
            //иначе пересчитываем координаты вершин
            else drones[id].update_coord();
        }
        else drones[id].move(x, y, z);
        return true;
    }
}

bool Scene3D::move_cube_to(int id, GLfloat x, GLfloat y, GLfloat z)
{
    if (id < n)
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
    else
    {
        if (COLLIDE_CHECK)
        {
            id -= n;
            //меняем координаты центра
            GLfloat x_old, y_old, z_old;
            x_old = drones[id].center_x;
            y_old = drones[id].center_y;
            z_old = drones[id].center_z;
            drones[id].center_x = x;
            drones[id].center_y = y;
            drones[id].center_z = z;
            //если результирующий куб пересекается с чем-нибудь, меняем координаты на старые
            if (collide_check(id + n, x - x_old, y - y_old, z - z_old))
            {
                drones[id].center_x = x_old;
                drones[id].center_y = y_old;
                drones[id].center_z = z_old;
                return false;
            }
            //иначе пересчитываем координаты вершин
            else drones[id].update_coord();
        }
        else drones[id].move_to(x, y, z);
        return true;
    }

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
        if (cubes[i].fixed || cubes[i].flying)
            continue;
        move_cube_to(i, cubes[i].center_x, cubes[i].center_y, cubes[i].center_z - 0.01);
        //move_cube(i, 0, 0, -0.01);
    }
    //same for drones
    if (ENABLE_FLYING) return;
    for(int i = 0; i < n_d; i++)
    {
        if (drones[i].fixed)
            continue;

        move_cube(i + n_d, 0, 0, -0.01);//move_to не для этих целей, Миша..
    }
}


bool Scene3D::collide_check(int id, GLfloat x_dir, GLfloat y_dir, GLfloat z_dir)
{
    int ignore;
    float x, y, z, r;
    if (id < n)
    {
        x = cubes[id].center_x;
        y = cubes[id].center_y;
        z = cubes[id].center_z;
        r = cubes[id].size;
        ignore = cubes[id].carried_by;
    }
    else
    {
        x = drones[id-n].center_x;
        y = drones[id-n].center_y;
        z = drones[id-n].center_z;
        r = drones[id-n].size;
        ignore = drones[id-n].cargo_id;
    }


    float eps = 0.0001;//why not
    for (int i = 0; i < n; i++)
    {
        if (i == id || i == ignore) continue;
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
    for (int i = 0; i < n_d; i++)
    {
        if (i + n == id || i + n == ignore) continue;
        float x_diff = x - drones[i].center_x;
        float y_diff = y - drones[i].center_y;
        float z_diff = z - drones[i].center_z;
        if ((abs(x_diff) < r + drones[i].size - eps) &&
                (abs(y_diff) < r + drones[i].size - eps) &&
                (abs(z_diff) < r + drones[i].size - eps))
        {

            //cubes[id].SendAnswer("CollideCheckFail:id = " + std::to_string(id) + " with id = " + std::to_string(i));
            return true;
        }

    }
    return false;
}

void Scene3D::do_task()//доработать
{
    GLfloat x, y, z;
    int cmd;
    for (int i = 0; i < n_d; i++)
    {
        if (drones[i].task_queue.empty()) continue;
        x = drones[i].task_queue.front().x;
        y = drones[i].task_queue.front().y;
        z = drones[i].task_queue.front().z;
        cmd = drones[i].task_queue.front().com;
        if (cmd < 2)//move command
        {
            //drone must carry stuff
            if (drones[i].cargo_id == -1)//empty drone
            {
                if (move_cube(i + n, x, y, z))
                {
                    //check cubes[i].task_queue.front().com
                    if(cmd > 0.5)
                        drones[i].SendAnswer("OK|");
                    drones[i].task_queue.pop();
                    continue;
                }
                else continue; //fail to move, here should be an error message
            }
            else
            {
                //move_cube(i+n, x, y, z);
                //move_cube(drones[i].cargo_id, x, y, z);

                if (move_cube(i+n, x, y, z))
                {
                    if (move_cube(drones[i].cargo_id, x, y, z))
                    {
                        //if(cmd > 0.5)
                            //drones[i].SendAnswer("OK|");
                        drones[i].task_queue.pop();
                        continue;
                    }
                    else
                    {
                        //move drone back
                        move_cube(i+n, -x, -y, -z);
                        //err
                        fprintf(commands, "%d err bad move1 ", drones[i].task_queue.size());
                        continue;

                    }
                }
                else
                {
                    //err
                    fprintf(commands, "%d err bad move2 ", drones[i].task_queue.size());
                    continue;
                }
            }
        }
        if (cmd == 2) capture(i);
        if (cmd == 3) release(i);
        fprintf(commands, "%d err", drones[i].task_queue.size());
        drones[i].task_queue.pop();

    }
}

void Scene3D::release(int id)
{
    if (drones[id].cargo_id > -1)
    {
        cubes[drones[id].cargo_id].carried_by = -1;
        cubes[drones[id].cargo_id].flying = false;
        drones[id].cargo_id = -1;
    }

}

void Scene3D::capture(int id)
{
    if (drones[id].cargo_id > -1) return;// already carrying something
    //add here search for cubes
    float x, y, z, r;
    x = drones[id].center_x;
    y = drones[id].center_y;
    z = drones[id].center_z;
    r = drones[id].size;
    int target = -1;//id of the cube we're looking for
    for (int i = 1; i < n; i++)
    {
        if (abs(x - cubes[i].center_x) + abs(y - cubes[i].center_y) + abs(z - (cubes[i].center_z + r + cubes[i].size)) < 0.01)
        {
            target = i;
            break;
        }
    }
    if (target < 0) return;//err
    drones[id].cargo_id = target;
    cubes[target].carried_by = id+n;
    cubes[target].flying = true;
}
