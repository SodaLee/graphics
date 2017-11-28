//
//  main.cpp
//  Cohen-SutherLand
//
//  Created by 李浩辰 on 2017/3/25.
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

int calnum(int x, int y, int ll, int rl, int ul, int dl) {
    int ret = 0;
    ret <<= 1; if (x < ll) ret++;
    ret <<= 1; if (x > rl) ret++;
    ret <<= 1; if (y > ul) ret++;
    ret <<= 1; if (y < dl) ret++;
    return ret;
}

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

bool cl(int a, int b) {
    return (a >> 3) != (b >> 3);
}

bool cr(int a, int b) {
    return ((a >> 2) & 1) != ((b >> 2) & 1);
}

bool cu(int a, int b) {
    return ((a >> 1) & 1) != ((b >> 1) & 1);
}

bool cd(int a, int b) {
    return (a & 1) != (b & 1);
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

void C_S(int x1, int y1, int x2, int y2, int width, int height, GLubyte color[3]) {
    int n1 = calnum(x1, y1, ll, rl, ul, dl);
    int n2 = calnum(x2, y2, ll, rl, ul, dl);
    int xx = 0, yy = 0;
    if (cl(n1, n2)) {
        Calx(x1, y1, x2, y2, xx, yy, ll);
        if (x1 < xx) {
            x1 = xx;
            y1 = yy;
        }
        else {
            x2 = xx;
            y2 = yy;
        }
    }
    if (cr(n1, n2)) {
        Calx(x1, y1, x2, y2, xx, yy, rl);
        if (x1 > xx) {
            x1 = xx;
            y1 = yy;
        }
        else {
            x2 = xx;
            y2 = yy;
        }
    }
    n1 = calnum(x1, y1, ll, rl, ul, dl);
    n2 = calnum(x2, y2, ll, rl, ul, dl);
    if (cu(n1, n2)) {
        Caly(x1, y1, x2, y2, xx, yy, ul);
        if (y1 > yy) {
            x1 = xx;
            y1 = yy;
        }
        else {
            x2 = xx;
            y2 = yy;
        }
    }
    if (cd(n1, n2)) {
        Caly(x1, y1, x2, y2, xx, yy, dl);
        if (y1 < yy) {
            x1 = xx;
            y1 = yy;
        }
        else {
            x2 = xx;
            y2 = yy;
        }
    }
    DrawLine(x1, y1, x2, y2, width, height, color);
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
    C_S(40, 10, 340, 310, 400, 400, green);
    C_S(200, 50, 200, 350, 400, 400, red);
    C_S(150, 350, 390, 90, 400, 400, blue);
    C_S(120, 200, 50, 200, 400, 400, white);
    C_S(150, 150, 150, 250, 400, 400, yellow);
    glFlush();
}

int main(int argc, char * argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(400, 400);
    glutCreateWindow("Cohen-SutherLand by 1500012746 SodaLee");
    glutDisplayFunc(Draw);
    glutMainLoop();
    return 0;
}
