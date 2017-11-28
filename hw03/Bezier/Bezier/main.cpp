//
//  main.cpp
//  Bezier
//
//  Created by 李浩辰 on 2017/3/28.
//  Copyright © 2017年 李浩辰. All rights reserved.
//

#include <iostream>
#include <utility>
#include <vector>
#include <queue>
#include <GLUT/GLUT.h>
#include <OpenGL/OpenGL.h>
#define mp make_pair
#define X first
#define Y second
using namespace std;

typedef pair<float, float> Point;

vector<Point> P;
queue<Point> T;

void DrawLine() {
    Point p;
    for (int i = 0; i < P.size(); ++i) {
        p = P[i];
        glVertex2f(p.X, p.Y);
    }
}

void countp(float t, float &x, float &y, Point a, Point b) {
    float dx = b.X - a.X, dy = b.Y - a.Y;
    x = a.X + dx * t; y = a.Y + dy * t;
}

void countPoint(float t, float &x, float &y) {
    while (!T.empty()) T.pop();
    int tt = P.size();
    Point a, b;
    float xx, yy;
    for (int i = 0; i < tt - 1; ++i) {
        countp(t, xx, yy, P[i], P[i + 1]);
        T.push(mp(xx, yy));
    }
    for (--tt; tt > 1; --tt) {
        for (int i = 1; i < tt; ++i) {
            a = T.front();
            T.pop();
            b = T.front();
            countp(t, xx, yy, a, b);
            T.push(mp(xx, yy));
        }
        T.pop();
    }
    a = T.front();
    x = a.X; y = a.Y;
}

void Bezier(int n) {
    float d = 1.0 / n, x, y;
    glColor3ub(0, 0xff, 0);//green
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= n; ++i) {
        countPoint(d * i, x, y);
        glVertex2f(x, y);
    }
    glEnd();
}

void Draw() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3ub(0xff, 0xff, 0);//yellow
    P.clear();
    P.push_back(mp(-0.9, -0.9));
    P.push_back(mp(-0.8, -0.6));
    P.push_back(mp(-0.4, -0.3));
    P.push_back(mp(-0.2, -0.5));
    glBegin(GL_LINE_STRIP);
    DrawLine();
    glEnd();
    Bezier(50);
    glColor3ub(0xff, 0xff, 0);//yellow
    P.clear();
    P.push_back(mp(-0.9, 0.5));
    P.push_back(mp(-0.8, 0.9));
    P.push_back(mp(-0.6, 0.4));
    P.push_back(mp(-0.4, 0.95));
    P.push_back(mp(0, 0.8));
    glBegin(GL_LINE_STRIP);
    DrawLine();
    glEnd();
    Bezier(50);
    glColor3ub(0xff, 0xff, 0);//yellow
    P.clear();
    P.push_back(mp(0, 0));
    P.push_back(mp(0.2, -0.2));
    P.push_back(mp(0.9, -0.1));
    P.push_back(mp(0.4, 0.4));
    P.push_back(mp(-0.2, 0.2));
    P.push_back(mp(0.7, -0.3));
    glBegin(GL_LINE_STRIP);
    DrawLine();
    glEnd();
    Bezier(50);
    glFlush();
}

int main(int argc, char * argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(400, 400);
    glutCreateWindow("Bezier by 1500012746 SodaLee");
    glutDisplayFunc(Draw);
    glutMainLoop();
    return 0;
}
