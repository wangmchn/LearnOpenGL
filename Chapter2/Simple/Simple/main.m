//
//  main.m
//  Simple
//
//  Created by Mark on 16/3/24.
//  Copyright © 2016年 Wecan Studio. All rights reserved.
//

#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>

// 绘制场景
void RenderScene(void) {
    // 用当前清除颜色清除窗口
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, 400, 400);
    glOrtho(-200, 200, -200, 200, -1.0, 1.0);
    glColor3f(0.0, 1.0, 0.0);
    glRectf(0, 0, 200, 200);
    // 刷新绘图命令
    glFlush();
}

// 设置渲染状态
void SetupRenderContext(void) {
    glClearColor(0.45f, 0.242f, 0.125f, 1.0f);
}

int main(int argc, char * argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowSize(400, 400);
    glutCreateWindow("Simple"); // 创建窗口
    glutDisplayFunc(RenderScene); // 设置回调函数(绘制)
    SetupRenderContext(); // 设置渲染的状态
    glutMainLoop(); // 初步理解为在Loop中不断调用绘制函数绘制场景
    return 0;
}
