//
//  main.cpp
//  Liang_Barsky
//
//  Created by 李浩辰 on 2017/3/26.
//  Copyright © 2017年 李浩辰. All rights reserved.
//

#include <iostream>
#include <algorithm>
#include <GLUT/GLUT.h>
#include <OpenGL/OpenGL.h>
GLubyte red[3] = {0xff, 0, 0};
GLubyte green[3] = {0, 0xff, 0};
GLubyte blue[3] = {0, 0, 0xff};
GLubyte black[3] = {0, 0, 0};
GLubyte white[3] = {0xff, 0xff, 0xff};
GLubyte yellow[3] = {0xff, 0xff, 0};
struct Point {
    int x, y;
    Point(int _x = 0, int _y = 0):x(_x), y(_y) {}
};

int ll, rl, ul, dl;

void DrawLine(int x1, int y1, int x2, int y2, int width, int height, GLubyte color[3]) {
    GLubyte mat[height][width][3];
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, mat);
    if (x1 != x2) {
        float k = (float)(y2 - y1) / (float)(x2 - x1);
        for (int i = x1; i <= x2; ++i) {
            int j = (i - x1) * k + y1;
            mat[j][i][0] = color[0];
            mat[j][i][1] = color[1];
            mat[j][i][2] = color[2];
        }
    }
    else
        for (int j = y1; j <= y2; ++j) {
            mat[j][x1][0] = color[0];
            mat[j][x1][1] = color[1];
            mat[j][x1][2] = color[2];
        }
    glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, mat);
}

void Calx(int x1, int y1, int x2, int y2, int& ax, int& ay, int lx) {
    ax = lx;
    float k = (float)(y2 - y1) / (float)(x2 - x1);
    ay = (ax - x1) * k + y1;
}

void Caly(int x1, int y1, int x2, int y2, int& ax, int& ay, int ly) {
    ay = ly;
    float k = (float)(x2 - x1) / (float)(y2 - y1);
    ax = (ay - y1) * k + x1;
}

void L_B(int x1, int y1, int x2, int y2, int width, int height, GLubyte color[3]) {
    int p[4], q[4];
    float u[2];
    u[0] = 0; u[1] = 1;
    p[1] = x2 - x1;
    p[0] = -p[1];
    p[3] = y2 - y1;
    p[2] = -p[3];
    q[1] = rl - x1;
    q[0] = x1 - ll;
    q[3] = ul - y1;
    q[2] = y1 - dl;
    for (int i = 0; i < 4; ++i) {
        if (p[i] == 0) continue;
        if (p[i] > 0) u[1] = std::min(u[1], (float)q[i] / p[i]);
        else u[0] = std::max(u[0], (float)q[i] / p[i]);
    }
    DrawLine(x1+p[1]*u[0], y1+p[3]*u[0], x1+p[1]*u[1], y1+p[3]*u[1], width, height, color);
}

void Draw() {
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_LINE_LOOP);
        glVertex2f(-0.5, 0.5);
        glVertex2f(0.5, 0.5);
        glVertex2f(0.5, -0.5);
        glVertex2f(-0.5, -0.5);
    glEnd();
    ll = dl = 100; rl = ul = 300;
    //DrawLine(40, 10, 340, 310, 400, 400, green);
    //DrawLine(200, 50, 200, 350, 400, 400, red);
    //DrawLine(150, 350, 390, 90, 400, 400, blue);
    //DrawLine(120, 200, 50, 200, 400, 400, white);
    //DrawLine(150, 150, 150, 250, 400, 400, yellow);
    L_B(40, 10, 340, 310, 400, 400, green);
    L_B(200, 50, 200, 350, 400, 400, red);
    L_B(150, 350, 390, 90, 400, 400, blue);
    L_B(120, 200, 50, 200, 400, 400, white);
    L_B(150, 150, 150, 250, 400, 400, yellow);
    glFlush();
}

int main(int argc, char * argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(400, 400);
    glutCreateWindow("Liang_Barsky by 1500012746 SodaLee");
    glutDisplayFunc(Draw);
    glutMainLoop();
    return 0;
}
