//
//  main.cpp
//  SOLAR
//
//  Created by Mark on 16/4/1.
//  Copyright © 2016年 Wecan Studio. All rights reserved.
//

#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>

static GLfloat lightPos[] = {0, 0, 0, 1};
static GLfloat diffusLight[] = {0.8, 0.8, 0.8, 1};
static GLfloat whiteLight[] = {0.2, 0.2, 0.2, 1};

void timerFunc(int value) {
    glutPostRedisplay();
    glutTimerFunc(100, timerFunc, 1);
}

void changeSize(GLint w, GLint h) {
    GLfloat fAspect;
    if (h == 0) {
        h = 1;
    }
    glViewport(0, 0, w, h);
    fAspect = (GLfloat)w / (GLfloat)h;
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    /*
     * 45°视野，近和远平面为 1.0 和 425*
     * GLdouble fovy,  //角度
     * GLdouble aspect,//视景体的宽高比
     * GLdouble zNear, //沿z轴方向的两裁面之间的距离的近处
     * GLdouble zFar   //沿z轴方向的两裁面之间的距离的远处
     */
    gluPerspective(45.0f, fAspect, 1.0, 425);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
}

void renderScence() {
    static float fMoonRot = 0.0f;
    static float fEarthRot = 0.0f;
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    // 把整个场景移动到视图中
    glTranslatef(0.0f, 0.0f, -300.0f);
    
    glColor3ub(255, 255, 0);
    glDisable(GL_LIGHTING);
    glutSolidSphere(15.0f, 30, 17);
    glEnable(GL_LIGHTING);
    
    // 在太阳绘制后释放光源, 设置光源位置
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    
    glRotatef(fEarthRot, 0.0f, 1.0f, 0.0f);
    // 地球
    glColor3ub(0, 0, 255);
    glTranslatef(105.0f, 0.0f, 0.0f);
    glutSolidSphere(15.0f, 30, 17);
    
    // 在地球坐标基础上旋转月球坐标
    glColor3ub(200, 200, 200);
    glRotatef(fMoonRot, 0.0f, 1.0f, 0.0f);
    glTranslatef(30.0f, 0.0f, 0.0f);
    glutSolidSphere(6, 15.0f, 15.0f);
    
    glPopMatrix();
    
    fMoonRot += 15;
    if (fMoonRot > 360) {
        fMoonRot = 0;
    }
   
    fEarthRot += 5;
    if (fEarthRot > 360) {
        fEarthRot = 0;
    }
    
    glutSwapBuffers();
}

void renderContext() {
    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    
    // 光照模型
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteLight);
    // 设置漫射光成分
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffusLight);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

int main(int argc, char * argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("SOLAR");
    
    glutReshapeFunc(changeSize);
    glutDisplayFunc(renderScence);
    
    glutTimerFunc(100, timerFunc, 1);
    
    renderContext();
    
    glutMainLoop();
    return 0;
}
