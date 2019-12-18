#include <GL/glut.h>
#include <GL/freeglut.h>

static GLfloat spin = 0.0;
 
int init(void);
void Display(void);
void ViewPort(int xvmin, int yvmin, int vn);
void Camera(float x0, float y0, float z0, float vx, float vy, float vz, int vn);
void Spin(void);
 
 
int init(void){
 
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.5, 1.72, -1.5, 1.5);
}
 
void ViewPort(int xvmin, int yvmin, int vn){
 
    glViewport(xvmin, yvmin, 400, 400);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(vn == 4){
        gluPerspective(70, 1.0, 1.0, 50);
    } else {
        glOrtho(-3, 3, -3, 3, 1, 50); // xvmin, xvmax, xwmin, ywmax, near, far
    }
}
 
 
 
void Camera(float x0, float y0, float z0, float vx, float vy, float vz, int vn){
 
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(x0, y0, z0, 0.0, 0.0, 0.0, vx, vy, vz);
 
    float sizeTeaPoat = 1.2;
 
    if(vn == 4){
        glLoadIdentity();
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
 
        float rotacaoA = spin;
        int rotacaoB = rotacaoA;
        int angulo = rotacaoB % 365;
 
        glRotatef(spin, 0.0, 0.0, 1.0);
        glRotatef(45, 1.0, 0.0, 0.0);
 
        sizeTeaPoat = 0.4;
    }
 
    glutWireTeapot(sizeTeaPoat);
}
 
void Spin(void){
   spin = spin + 0.8;
   if (spin > 360.0)
      spin = 0;
 
   glutPostRedisplay();
}
 
void createViewPort(){
 
    ViewPort(000, 400, 1);
    Camera(0.0, 1.0, 0.0, 0.0, 0.0, -1.0, 1);
    ViewPort(400, 400, 2);
    Camera(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 2);
    ViewPort(000, 000, 3);
    Camera(0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 3);
    ViewPort(400, 000, 4);
    Camera(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 4);
}
 
void Display(void){
 
    glClear(GL_COLOR_BUFFER_BIT |  GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3f(1.0f, 0.0f, 0.0f);
    createViewPort();
    glFlush();
}
 
int main(int argc, char** argv) {
 
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE| GLUT_RGB);
    glutInitWindowSize(800, 800); // window size
    glutInitWindowPosition(200, 200);
    glutCreateWindow("Exercicio bonus 2 - 4 views tea");
    glutIdleFunc(Spin);
    glutDisplayFunc(Display);
    init();
    glutMainLoop();
    return 0;
}
