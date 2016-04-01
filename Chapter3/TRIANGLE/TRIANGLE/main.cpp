//
//  main.cpp
//  TRIANGLE
//
//  Created by Mark on 16/3/30.
//  Copyright © 2016年 Wecan Studio. All rights reserved.
//

#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include <math.h>

static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;

int bCull = 0;
int bOutline = 0;
int bDepth = 0;

void SetupRenderContext() {
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    // 单色着色，即使用当前多边形最后一个顶点的颜色填充这个多边形
    // GL_SMOOTH 渐变着色
    glShadeModel(GL_FLAT);
    // 顺时针环绕的多边形为正面，原本是逆时针
    glFrontFace(GL_CW);
}

// key 指示按下的键, x, y 为按下键时鼠标的坐标
void SpecialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_DOWN:
            xRot += 5.0f;
            break;
        case GLUT_KEY_UP:
            xRot -= 5.0f;
            break;
        case GLUT_KEY_LEFT:
            yRot += 5.0f;
            break;
        case GLUT_KEY_RIGHT:
            yRot -= 5.0f;
            break;
        default:
            break;
    }
    
    glutPostRedisplay();
}

void ChangeSize(GLsizei w, GLsizei h) {
    GLfloat nRange = 100.0f;
    if (h == 0) { h = 1; }
    
    glViewport(0, 0, w, h);
    
    // 指定哪一个矩阵是当前矩阵
    glMatrixMode(GL_PROJECTION);
    // 重设矩阵
    glLoadIdentity();
    
    GLfloat a = (GLfloat)w / (GLfloat)h;
    if (w <= h) {
        glOrtho(-nRange, nRange, -nRange / a, nRange / a, -nRange, nRange);
    } else {
        glOrtho(-nRange * a, nRange * a, -nRange, nRange, -nRange, nRange);
    }
    // MODEVIEW 和 PROJECTION 顺序？？
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void ProcessMenu(int value) {
    switch (value) {
        case 1:
            bDepth = !bDepth;
            break;
        case 2:
            bCull = !bCull;
            break;
        case 3:
            bOutline = !bOutline;
        default:
            break;
    }
    glutPostRedisplay();
}

void RenderTriangleFan(GLfloat xTop, GLfloat yTop, GLfloat zTop) {
    GLfloat x, y, angle;
    int iPivot = 1;
    
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(xTop, yTop, zTop);
    for (angle = 0.0f; angle < 2 * 3.14; angle += (3.14 / 8.0f)) {
        x = 50.0f * sin(angle);
        y = 50.0f * cos(angle);
        
        if ((iPivot % 2) == 0) {
            glColor3f(0.0f, 1.0f, 0.0f);
        } else {
            glColor3f(1.0f, 0.0f, 0.0f);
        }
        iPivot++;
        
        glVertex2f(x, y);
    }
    glEnd();
}

void PreRendering() {
    // 清除窗口和深度缓冲区
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // 剔除操作，隐藏遮掩表面
    if (bCull) {
        glEnable(GL_CULL_FACE);
    } else {
        glDisable(GL_CULL_FACE);
    }
    // 由于后绘制的图元默认会叠加在先绘制的图元之上
    // 开启深度测试可使近的遮挡住远的图元
    if (bDepth) {
        glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }
    // 如果开启，将背部绘制成多边形
    if (bOutline) {
        glPolygonMode(GL_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_BACK, GL_FILL);
    }

}

void RenderScence() {
    PreRendering();
    
    glPushMatrix();
    glRotatef(xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);
    
    RenderTriangleFan(0.0f, 0.0f, 75.0f);
    RenderTriangleFan(0.0f, 0.0f, 0.0f);
    
    glPopMatrix();
    glutSwapBuffers();
}

int main(int argc, char * argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("TRIANGLE");
    glutCreateMenu(ProcessMenu);
    glutAddMenuEntry("Cull", 1);
    glutAddMenuEntry("Depth", 2);
    glutAddMenuEntry("Outline", 3);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
    glutReshapeFunc(ChangeSize);
    glutSpecialFunc(SpecialKeys);
    glutDisplayFunc(RenderScence);
    
    SetupRenderContext();
    
    glutMainLoop();
    return 0;
}
