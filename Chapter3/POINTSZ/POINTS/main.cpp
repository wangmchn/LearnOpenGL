//
//  main.cpp
//  POINTS
//
//  Created by Mark on 16/3/30.
//  Copyright © 2016年 Wecan Studio. All rights reserved.
//

#include <iostream>
#include <math.h>
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>

static GLfloat xRot = 60.0f;
static GLfloat yRot = 30.0f;

void SetupRenderContext() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
}

void RenderScence() {
    GLfloat x, y, z, angle;
    GLfloat curSize = 1.0;
    GLfloat step = 1.0;
    GLfloat sizes[2] = {0, 0};
    // 向系统查询一个状态变量的当前值
    glGetFloatv(GL_POINT_SIZE_RANGE, sizes);
    glGetFloatv(GL_POINT_SIZE_GRANULARITY, &step);
    
    glClear(GL_COLOR_BUFFER_BIT);
    // 保存矩阵状态
    glPushMatrix();
    // 旋转, 以 (x, y, z) 向量旋转
    glRotatef(xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);

    z = -50.0f;
    for (angle = 0.0f; angle < 3 * M_PI * 2; angle += 0.1f) {
        x = 50.0f * sin(angle);
        y = 50.0f * cos(angle);
        
        // 画点
        glPointSize(curSize);
        glBegin(GL_POINTS);
        glVertex3f(x, y, z);
        glEnd();
        z += 0.5f;
        curSize += step;
    }
    // 返回到保存的矩阵状态
    glPopMatrix();
    glutSwapBuffers();
}

// 接收键盘输入
void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            xRot -= 10;
            break;
        case GLUT_KEY_RIGHT:
            yRot -= 10;
            break;
        case GLUT_KEY_LEFT:
            yRot += 10;
            break;
        case GLUT_KEY_DOWN:
            xRot += 10;
        default:
            break;
    }
    
    glutPostRedisplay();
}

void ChangeSize(int w, int h) {
    GLfloat nRange = 100.0f;
    
    // Prevent a divide by zero
    if(h == 0) { h = 1; }
    
    // Set Viewport to window dimensions
    glViewport(0, 0, w, h);
    
    // Reset projection matrix stack
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // Establish clipping volume (left, right, bottom, top, near, far)
    if (w <= h) {
        glOrtho (-nRange, nRange, -nRange*h/w, nRange*h/w, -nRange, nRange);
    } else {
        glOrtho (-nRange*w/h, nRange*w/h, -nRange, nRange, -nRange, nRange);
    }
    // Reset Model view matrix stack
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char * argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutCreateWindow("POINTS");
    glutDisplayFunc(RenderScence);
    glutSpecialFunc(specialKeys);
    glutReshapeFunc(ChangeSize);
    
    SetupRenderContext();
    
    glutMainLoop();
    return 0;
}
