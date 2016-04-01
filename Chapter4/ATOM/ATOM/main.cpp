//
//  main.cpp
//  ATOM
//
//  Created by Mark on 16/3/24.
//  Copyright © 2016年 Wecan Studio. All rights reserved.
//

#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>

void RenderScene() {
    static GLfloat fElect = 0.0f;
    
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    // 重置模型视图矩阵
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glTranslatef(0.0f, 0.0f, -100.0f);
    glColor3f(1.0f, 0.0f, 0.0f);
    // 类似经线纬线
    glutSolidSphere(10.0f, 15, 15);
    glColor3f(1.0f, 1.0f, 0.0f);
    
    glPushMatrix();
    glRotatef(fElect, 0.0f, 1.0f, 0.0f);
    glTranslatef(90.0f, 0.0f, 0.0f);
    glutSolidSphere(6.0, 15, 15);
    
    glPopMatrix();
    
    glPushMatrix();
    glColor3ub(0, 255, 0);
    glRotatef(-45, 0, 0, 1);
    glRotatef(fElect, 0, 1, 0);
    glTranslatef(0, 0, 60);
    glutSolidSphere(6, 15, 15);
    glPopMatrix();
   
    
    fElect += 10.0f;
    if (fElect > 360.0) {
        fElect = 0.0f;
    }
    
    glutSwapBuffers();
}

void SetupRenderContext() {
    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
}

// 在窗口大小发生改变时调用, 以宽高为参数调用
// 关于 glViewport 和 glOrtho 详见：http://www.cnblogs.com/yxnchinahlj/archive/2010/10/30/1865298.html
void ChangeSize(GLsizei w, GLsizei h) {
    
    GLfloat aspectRatio;
    
    if (h == 0) { h = 1; }
    
    // 把视口设置为窗口大小
    glViewport(0, 0, w, h);
    
    // 重置坐标系统 (1)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // 裁剪超出正方形的部分
    aspectRatio = (GLfloat)w / (GLfloat)h;
    if (w <= h) {
        glOrtho(-100.0f, 100.0f, -100.0f / aspectRatio, 100.0f / aspectRatio, 100.0 * 2, -100.0 * 2);
    } else {
        glOrtho(-100.0f * aspectRatio, 100.0f * aspectRatio, -100, 100, 100.0 * 2, -100.0 * 2);
    }
    
    // (2)
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // 暂时不明 (1) 和 (2) 的作用
    // glMatrixMode 对接下来要做什么进行声明，也就是在要做下一步之前告诉计算机我要对“什么”进行操作了
    // GL_PROJECTION 投影, GL_MODELVIEW 模型视图, GL_TEXTURE 纹理;
}

void timerFunc(int value) {
    glutPostRedisplay();
    glutTimerFunc(100, timerFunc, 1);
}

int main(int argc, char * argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB |GLUT_DEPTH);
    glutCreateWindow("ATOM");
    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    glutTimerFunc(100, timerFunc, 1);
    
    SetupRenderContext();
    
    glutMainLoop();
    return 0;
}
