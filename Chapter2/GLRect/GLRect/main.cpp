//
//  main.cpp
//  GLRect
//
//  Created by Mark on 16/3/24.
//  Copyright © 2016年 Wecan Studio. All rights reserved.
//

#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>

void RenderScene() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 0.0f, 0.0f);
    glRectf(-25.0, 25.0, 25.0, -25.0);
    
    glFlush();
}

void SetupRenderContext() {
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
        glOrtho(-100.0f, 100.0f, -100.0f / aspectRatio, 100.0f / aspectRatio, 1.0, -1.0);
    } else {
        glOrtho(-100.0f * aspectRatio, 100.0f * aspectRatio, -100, 100, 1.0, -1.0);
    }
    
    // (2)
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // 暂时不明 (1) 和 (2) 的作用
    // glMatrixMode 对接下来要做什么进行声明，也就是在要做下一步之前告诉计算机我要对“什么”进行操作了
    // GL_PROJECTION 投影, GL_MODELVIEW 模型视图, GL_TEXTURE 纹理;
}

int main(int argc, char * argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutCreateWindow("GLRect");
    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    
    SetupRenderContext();
    
    glutMainLoop();
    return 0;
}
