//
//  main.c
//  Bounce
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
    glClear(GL_COLOR_BUFFER_BIT);
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
    GLfloat horizontal = width / 2;
    GLfloat vertical   = height / 2;
    // glOrtho 相当于将图元绘制在投影面积上，后缩放到当前视口上
    // 例如视口大小为 (100,100) , 投影为 (-50, 50, -50, 50, -1.0, 1.0) 物体为正常大小
    // 若为 -100, 100, 则将缩小2倍
    // 因为以原始的宽高为基础计算投影比例可以使物块随窗口放大缩小
    if (aspectRatio >= (horizontal/vertical)) {
        glOrtho(-vertical * aspectRatio, vertical * aspectRatio, -vertical, vertical, 1.0, -1.0);
    } else {
        glOrtho(-horizontal, horizontal, -horizontal / aspectRatio, horizontal / aspectRatio, 1.0, -1.0);
    }
    
    // (2)
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // 暂时不明 (1) 和 (2) 的作用
    // glMatrixMode 对接下来要做什么进行声明，也就是在要做下一步之前告诉计算机我要对“什么”进行操作了
    // GL_PROJECTION 投影, GL_MODELVIEW 模型视图, GL_TEXTURE 纹理;
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
    printf("realSize: %f----windowWidth: %f-----x: %f\n", realSize, windowWidth, x);
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
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(width, height);
    glutCreateWindow("Bounce");
    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    glutTimerFunc(33, TimerFunction, 1);
    
    SetupRenderContext();
    
    glutMainLoop();
    
    return 0;
}
