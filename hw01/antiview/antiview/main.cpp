//
//  main.cpp
//  antiview
//
//  Created by 李浩辰 on 2017/3/25.
//  Copyright © 2017年 李浩辰. All rights reserved.
//

#include <iostream>
#include <cmath>
#include <GLUT/GLUT.h>
#include <OpenGL/OpenGL.h>
using std::abs;
GLubyte white[3] = {0xff, 0xff, 0xff};
const float d = 1.0 / 3;
const float w[3][3] = {{1.0/16, 2.0/16, 1.0/16},
                       {2.0/16, 4.0/16, 2.0/16},
                       {1.0/16, 2.0/16, 1.0/16}};

bool covered(float x, float y, int x1, int y1, int x2, int y2, float linewidth) {
    float k, nx, ny;
    if (abs(x1 - x2) >= abs(y1 - y2)) {
        k = (float)(y2 - y1) / (x2 - x1);
        ny = (x - x1) * k + y1;
        return ny <= y && y < (ny + linewidth);
    }
    else {
        k = (float)(x2 - x1) / (y2 - y1);
        nx = (y - y1) * k + x1;
        return nx <= x && x < (nx + linewidth);
    }
}

void antiview(int x1, int y1, int x2, int y2, GLubyte color[3], int width, int height, float linewidth) {
    GLubyte mat[height][width][3];
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, mat);
    float x, y, res;
    for (int i = x1; i <= x2; ++i)
        for (int j = y1; j <= y2; ++j) {
            x = i + d / 2; y = j + d / 2; res = 0;
            for (int k = 0; k < 3; ++k, x += d)
                for (int l = 0; l < 3; ++l, y += d) {
                    if (covered(x, y, x1, y1, x2, y2, linewidth))
                        res += w[k][l];
                }
            mat[j][i][0] = color[0] * res;
            mat[j][i][1] = color[1] * res;
            mat[j][i][2] = color[2] * res;
        }
    glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, mat);
}

void Draw() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLineWidth(3.0);
    glBegin(GL_LINES);
        glVertex2f(-1.0, 0.1);
        glVertex2f(0.8, 0.4);
    glEnd();
    antiview(0, 60, 540, 150, white, 600, 600, 3);
    glFlush();
}

int main(int argc, char * argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Antiview by 1500012746 SodaLee");
    glutDisplayFunc(Draw);
    glutMainLoop();
    return 0;
}
