#include <GL/glut.h>

int rotacao = 0, translacao = 1;
float tx = 0, ty = 0;
int angulo = 0;

int init(void){
    glClearColor(1.0, 1.0, 1.0, 1.0);     
    glMatrixMode(GL_PROJECTION);          
    gluOrtho2D(0.0, 100.0, 0.0, 100.0);      

}

void Tela(){
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex2i(0, 0);
    glVertex2i(10, 0);
    glVertex2i(10, 10);
    glVertex2i(0, 10);
    glEnd();
}

void Gerenciador(unsigned char key, int x, int y)
{
    switch (key) {
            case 't':
                translacao = 1;
                rotacao = 0;
                break;
            case 'r':
                rotacao = 1;
                translacao = 0;
                break;
    }
    glutPostRedisplay();
}

void TeclasEspeciais(int key, int x, int y)
{
    while (translacao == 1 || rotacao == 1){
        if(key == GLUT_KEY_UP) {
            if(translacao == 1){
                ty++;
                break;
            }
            if(rotacao == 1){
                break;
            }
        }
        if(key == GLUT_KEY_DOWN) {
            if(translacao == 1){
                ty--;
                break;
            }
            if(rotacao == 1){
                break;
            }
        }
        if(key == GLUT_KEY_LEFT) {
            if(translacao == 1){
                tx--;
                break;
            }
            if(rotacao == 1){
                angulo++;
                break;
            }
        }
        if(key == GLUT_KEY_RIGHT) {
        }
            if(translacao == 1){
                tx++;
                break;
            }
            if(rotacao == 1){
                angulo--;
                break;
            }
    }
    glutPostRedisplay();
}

void display(void){
    glClear(GL_COLOR_BUFFER_BIT);         
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef((tx+5),(ty+5),0);
	glRotatef(angulo,0,0,1);
    glTranslatef(-(tx+5),-(ty+5),0);
    glTranslatef(tx,ty,0);
    Tela();
    glFlush();                            
}

int main(int argc, char** argv) {

    glutInit(&argc,argv);                                     
    glutInitDisplayMode(GLUT_SINGLE| GLUT_RGB);               
    glutInitWindowSize(500,500);                              
    glutInitWindowPosition(200,0);
    glutCreateWindow("Um programa OpenGL Exemplo");
    init();
    glutKeyboardFunc(Gerenciador);
    glutSpecialFunc(TeclasEspeciais);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
