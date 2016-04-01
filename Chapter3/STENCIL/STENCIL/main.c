//
//  main.c
//  STENCIL
//
//  Created by Mark on 16/3/24.
//  Copyright © 2016年 Wecan Studio. All rights reserved.
//

#include <stdio.h>
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>

const GLfloat width  = 600;
const GLfloat height = 400;
GLfloat aspectRatio;
// 方块的初始位置及大小
GLfloat x = 0.0f;
GLfloat y = 0.0f;
GLfloat rsize = 100;

// 移动的像素量
GLfloat xstep = 2.0f;
GLfloat ystep = 2.0f;

// 追踪窗口的宽高变化
GLfloat windowWidth;
GLfloat windowHeight;

void RenderScene() {
    GLdouble dRadius = 0.1;
    GLdouble dAngle;
    
    glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
    // 使用 0 表示清除模板
    glClearStencil(0);
    glEnable(GL_STENCIL_TEST);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    // 所有通过模板测试的绘图命令并不进行绘图，而只是增加模板缓冲区内的值
    glStencilFunc(GL_NEVER, 0x0, 0x0);
    // 将模板缓冲区内的值从 0x0 —> 0x1
    glStencilOp(GL_INCR, GL_INCR, GL_INCR);
    // (1) 用白线绘制螺旋, 因为 glStencilFunc(GL_NEVER, 0x0, 0x0); 不会被绘制
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINE_STRIP);
    for (dAngle = 0; dAngle < 400.0; dAngle += 0.01) {
        glVertex2d(dRadius *cos(dAngle), dRadius * sin(dAngle));
        dRadius *= 1.0005;
    }
    glEnd();
    
    // 允许绘图，但不包括那些模板模型是 0x1 的地方，并且不对模板缓冲区进行修改
    glStencilFunc(GL_NOTEQUAL, 0x1, 0x1);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    // (2) 绘制方块， 但不包括 0x1 的地方，即步骤 (1) 绘制的地方
    glColor3f(1.0f, 0.0f, 0.0f);
    glRectf(x, y, x + rsize, y + rsize);
    
    glutSwapBuffers();
}

void SetupRenderContext() {
    glClearColor(0.0, 0.0, 1.0, 1.0);
}

void ChangeSize(GLsizei w, GLsizei h) {
    
    if (h == 0) { h = 1; }
    windowWidth  = w/2;
    windowHeight = h/2;
    // 把视口设置为窗口大小
    glViewport(0, 0, w, h);
    
    // 重置坐标系统 (1)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // 裁剪超出正方形的部分
    aspectRatio = (GLfloat)w / (GLfloat)h;
    // 以小的边长为基础，计算出窗口比例的宽高以保持物体大小随窗口拉伸变大
    GLfloat rwidth, rheight;
    // glOrtho 相当于将图元绘制在投影面积上，后缩放到当前视口上
    // 例如视口大小为 (100,100) , 投影为 (-50, 50, -50, 50, -1.0, 1.0) 物体为正常大小
    // 若为 -100, 100, 则将缩小2倍
    // 因为以原始的宽高为基础计算投影比例可以使物块随窗口放大缩小
    if (aspectRatio >= (width/height)) {
        rwidth  = height / 2 * aspectRatio;
        rheight = height / 2;
    } else {
        rwidth  = width / 2;
        rheight = width / 2 / aspectRatio;
    }
    glOrtho(-rwidth, rwidth, -rheight, rheight, 1.0, -1.0);

    // (2)
    // 暂时不明 (1) 和 (2) 的作用
    // glMatrixMode 对接下来要做什么进行声明，也就是在要做下一步之前告诉计算机我要对“什么”进行操作了
    // GL_PROJECTION 投影, GL_MODELVIEW 模型视图, GL_TEXTURE 纹理;
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 检查边界，防止物体反弹时窗口变小导致物体出现在视口之外;
    if (x - xstep + rsize > rwidth) {
        x = rwidth - rsize - 1;
    } else if (x < - (rwidth + xstep)) {
        x = -rwidth - 1;
    }
    
    if (y - ystep + rsize > rheight) {
        y = rheight - rsize - 1;
    } else if (y < - (rheight + ystep)) {
        y = -rheight - 1;
    }
    
}

void TimerFunction(int value) {

    GLfloat realSize, realX, realY;
    GLfloat wrate = windowWidth * 2 / width;
    GLfloat hrate = windowHeight * 2 / height;
    // 将 x, y 及 size 从原先坐标系转化到当前坐标比例
    if ((windowWidth / windowHeight) <= (width / height)) {
        realSize = rsize * wrate;
        realX = x * wrate;
        realY = y * wrate;
    } else {
        realSize = rsize * hrate;
        realX = x * hrate;
        realY = y * hrate;
    }
    
    if (realX + realSize > windowWidth || realX < -windowWidth) {
        xstep = -xstep;
    }
    
    if (realY + realSize > windowHeight || realY < -windowHeight) {
        ystep = -ystep;
    }
    
    x += xstep;
    y += ystep;
    
    
    glutPostRedisplay(); // 重绘, 相当于 setNeedDisplay ?
    glutTimerFunc(33, TimerFunction, 1);
}

int main(int argc, char * argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_STENCIL);
    glutInitWindowSize(width, height);
    glutCreateWindow("STENCIL");
    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    glutTimerFunc(33, TimerFunction, 1);
    
    SetupRenderContext();
    
    glutMainLoop();
    
    return 0;
}
