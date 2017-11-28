//
//  main.cpp
//  scanline
//
//  Created by 李浩辰 on 2017/3/25.
//  Copyright © 2017年 李浩辰. All rights reserved.
//

#include <iostream>
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include <stack>

const int w = 500, h = 500;
GLubyte black[3] = {0, 0, 0};
GLubyte white[3] = {0xff, 0xff, 0xff};

struct Point {
    int x, y;
    Point(int _x = 0, int _y = 0):x(_x),y(_y) {}
};

bool SameColor(GLubyte a[3], GLubyte b[3]) {
    return a[0] == b[0] && a[1] == b[1] && a[2] == b[2];
}

void ScanLineFill(int x, int y, int width, int height, GLubyte oldColor[3], GLubyte newColor[3]) {
    std::stack<Point> s;
    s.push(Point(x, y));
    int xleft, xright, i, j;
    GLubyte mat[height][width][3];
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, mat);
    while (!s.empty()) {
        x = s.top().x;
        y = s.top().y;
        s.pop();
        for (i = x; i < width && SameColor(mat[y][i], oldColor); ++i) {
            mat[y][i][0] = newColor[0];
            mat[y][i][1] = newColor[1];
            mat[y][i][2] = newColor[2];
        }
        xright = i - 1;
        for (i = x - 1; i >= 0 && SameColor(mat[y][i], oldColor); --i) {
            mat[y][i][0] = newColor[0];
            mat[y][i][1] = newColor[1];
            mat[y][i][2] = newColor[2];
        }
        xleft = i + 1;
        j = y - 1;
        if (j >= 0 && j < height)
            for (i = xright; i >= xleft; --i)
                if (SameColor(mat[j][i], oldColor)) {
                    for (i--; i >= 0 && SameColor(mat[j][i], oldColor); --i);
                    s.push(Point(i + 1, j));
                }
        j = y + 1;
        if (j >= 0 && j < height)
            for (i = xright; i >= xleft; --i)
                if (SameColor(mat[j][i], oldColor)) {
                    for (i--; i >= 0 && SameColor(mat[j][i], oldColor); --i);
                    s.push(Point(i + 1, j));
                }
    }
    glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, mat);
}

void Draw() {
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_LINE_LOOP);
        glVertex2f(0.5, 0.5);
        glVertex2f(0.2, 0);
        glVertex2f(0.5, -0.5);
        glVertex2f(0, -0.2);
        glVertex2f(-0.5, -0.5);
        glVertex2f(-0.2, 0);
        glVertex2f(-0.5, 0.5);
        glVertex2f(0, 0.2);
    glEnd();
    //ScanLineFill(w * 0.5, h * 0.5, w, h, black, white);
    glFlush();
}

int main(int argc, char * argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowPosition(200, 200);
    glutInitWindowSize(w, h);
    glutCreateWindow("ScanLineFill by 1500012746 SodaLee");
    glutDisplayFunc(Draw);
    glutMainLoop();
    return 0;
}
