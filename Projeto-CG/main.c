#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdarg.h>
#include <GL/glew.h>
#include "util/glut_wrap.h"
#include "glm.h"
#include "util/trackball.h"
#include "util/shaderutil.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"

GLdouble obsX = 0, obsY = 0, obsZ = 0;

float deathstar = 0;

float z_ce = -0.8;
float x_container = 0;

rotate_deathstar = 0;

float z_baloon = 4;

float z_slenderman = -0.8;
float x_slenderman = 0;
float angulo_slenderman = 0;

float z_cat = -5;
float x_cat = 10;
float angulo_cat = -90;

int objeto = 0;

static void updateModels(void);

static char *Model_file = NULL;		/* nome do arquivo do objeto */
static GLMmodel *Model;             /* modelo do objeto*/

/* de cada objeto */
static char *slender_file;
static GLMmodel *slenderman_model;

static char *chao_file;
static GLMmodel *chao_model;

static char *lua_file;
static GLMmodel *lua_model;

static char *baloon_file;
static GLMmodel *baloon_model;

static char *container_file;
static GLMmodel *container_model;

static char *cat_file;
static GLMmodel *cat_model;

static char *deathstar_file;
static GLMmodel *deathstar_model;

static int currentModel = 0;        /* numero do Model atual */

static GLfloat Scale = 4.0;			/* fator de escala */
static GLint WinWidth = 1024, WinHeight = 768;

typedef struct{
   // Variáveis para controles de rotação
   float rotX, rotY, rotX_ini, rotY_ini;
   int x_ini,y_ini,bot;
   float Distance;
   /* Quando o mouse está se movendo */
   GLboolean Rotating, Translating;
   GLint StartX, StartY;
   float StartDistance;
} ViewInfo;

static ViewInfo View;

static void InitViewInfo(ViewInfo *view){
   view->Rotating = GL_FALSE;
   view->Translating = GL_FALSE;
   view->Distance = 12.0;
   view->StartDistance = 0.0;
}

void criaLua(){
    //limpa o buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    //define material da superfície
    float kd[4] = {0.65f, 0.65f, 0.0f, 1.0f};
    float ks[4] = {0.9f, 0.9f, 0.9f, 1.0f};
    float ns = 5.0f;

    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, kd);
    glMaterialfv(GL_FRONT, GL_SPECULAR, ks);
    glMaterialf(GL_FRONT, GL_SHININESS, ns);

    //define que a matrix é a a model view
    glMatrixMode(GL_MODELVIEW);
    glTranslatef(-4, 8, -20);
    glutSolidSphere(1.5, 40, 40);
}

static void read_model(void){
   float objScale;
    float bg [4] = {1.0,0.0,0.0,1.0};
   // lendo o modelo



   slenderman_model = glmReadOBJ(slender_file);
   objScale = glmUnitize(slenderman_model);
   glmFacetNormals(slenderman_model);
   if (slenderman_model->numnormals == 0) {
      GLfloat smoothing_angle = 90.0;
      glmVertexNormals(slenderman_model, smoothing_angle);
   }

   glColor3f(0.0, 0.0, 0.0); //preto - slender
   glmLoadTextures(slenderman_model);
   glmReIndex(slenderman_model);
   glmMakeVBOs(slenderman_model);

     /// carregando o  balao
   baloon_model = glmReadOBJ(baloon_file);
   objScale = glmUnitize(baloon_model);
   glmFacetNormals(baloon_model);
   if (baloon_model->numnormals == 0) {
      GLfloat smoothing_angle = 90.0;
      glmVertexNormals(baloon_model, smoothing_angle);
   }
   glColor3f(0.0, 0.0, 1.0); //azul - balão
   glmLoadTextures(baloon_model);
   glmReIndex(baloon_model);
   glmMakeVBOs(baloon_model);

    /// carregando a pedra
   chao_model = glmReadOBJ(chao_file);
   objScale = glmUnitize(chao_model);
   glmFacetNormals(chao_model);
   if (chao_model->numnormals == 0) {
      GLfloat smoothing_angle = 90.0;
      glmVertexNormals(chao_model, smoothing_angle);
   }

   glmLoadTextures(chao_model);
   glmReIndex(chao_model);

   glmMakeVBOs(chao_model);


    /// carregando a deathstar
   deathstar_model = glmReadOBJ(deathstar_file);
   objScale = glmUnitize(deathstar_model);
   glmFacetNormals(deathstar_model);
   if (deathstar_model->numnormals == 0) {
      GLfloat smoothing_angle = 90.0;
      glmVertexNormals(deathstar_model, smoothing_angle);
   }
   glColor3f(0.5, 0.5, 0.5); //cinza - deathstar
   glmLoadTextures(deathstar_model);
   glmReIndex(deathstar_model);
   glmMakeVBOs(deathstar_model);

    /// carregando a container
   container_model = glmReadOBJ(container_file);
   objScale = glmUnitize(container_model);
   glmFacetNormals(container_model);
   if (container_model->numnormals == 0) {
      GLfloat smoothing_angle = 90.0;
      glmVertexNormals(container_model, smoothing_angle);
   }
   glmLoadTextures(container_model);
   glmReIndex(container_model);
   glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE, bg);
   glColor3f(1.0, 0.0, 0.0); //vermelho - container
   glmMakeVBOs(container_model);

    /// carregando a cat
   cat_model = glmReadOBJ(cat_file);
   objScale = glmUnitize(cat_model);
   glmFacetNormals(cat_model);
   if (cat_model->numnormals == 0) {
      GLfloat smoothing_angle = 90.0;
      glmVertexNormals(cat_model, smoothing_angle);
   }
   glColor3f(1.0, 1.0, 0.0); //amarelho - gato
   glmLoadTextures(cat_model);
   glmReIndex(cat_model);
   glmMakeVBOs(cat_model);
}

static void init(void){
   glClearColor(1.0, 1.0, 0.6, 0.0);
   glEnable(GL_COLOR_MATERIAL);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_CULL_FACE);
   glCullFace(GL_BACK);
   glEnable(GL_NORMALIZE);

    lighting();
}

void lighting(){
    GLfloat luzlua[4] = {0.7f, 0.7f, 0.7f, 1.0f};
    GLfloat white[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat black[4] = {0.2f, 0.2f, 0.2f, 1.0f};

    glLightfv(GL_LIGHT0,GL_POSITION,luzlua);
    glLightfv(GL_LIGHT0,GL_AMBIENT,black);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,white);
    glLightfv(GL_LIGHT0,GL_SPECULAR,white);

    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.5f);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.5f);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.1f);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

}

static void reshape(int width, int height) {
   float ar = 0.5 * (float) width / (float) height; //razão de aspecto
   WinWidth = width; //largura da janela
   WinHeight = height;  //atura da janela
   glViewport(0, 0, width, height);
   glMatrixMode(GL_PROJECTION);//
   glLoadIdentity();
   glFrustum(-ar, ar, -0.5, 0.5, 1.0, 300.0);
   gluPerspective(45.0,1.0,1.0,1.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   /*gluLookAt(0.0,0.0,1.0,
             0.0,0.0,0.0,
            1.0,1.0,1.0);
*/
   glTranslatef(0.0, 0.0, -3.0);
}

void spinDisplay(void){
   //playSound("Airplane.wav");
   rotate_deathstar = rotate_deathstar + 2.5;

    if(rotate_deathstar > 359)
        rotate_deathstar = 0;

   glutPostRedisplay();
}


static void display(void){
   GLfloat rot[4][4];
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    criaLua();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    /// carregando a chao
   glPushMatrix();
      glTranslatef(1.0, -2.5, -View.Distance+5);
      glRotatef(View.rotX,1,0,0);
//	  glRotatef(View.rotY,0,1,0);
      glScalef(Scale*4, Scale, 17*Scale);
      glmDrawVBO(chao_model);
   glPopMatrix();

    /// carregando o slender
   glPushMatrix();
      glTranslatef(x_slenderman, 0.0, -View.Distance+z_slenderman);
      //glRotatef(90, 0.0, 0.0, 1.0);       /// rotacionando em 45 graus no eixo x
      //glRotatef(-90,0.0,1.0,0.0);
	  //glRotatef(180,1.0,0.0,0.0);
      glRotatef(angulo_slenderman, 0.0, 1.0, 0.0);
      //glRotatef(View.rotX,1,0,0);
//	  glRotatef(View.rotY,0,1,0);
      glScalef(Scale/4, Scale/4, Scale/4);

      glmDrawVBO(slenderman_model);
   glPopMatrix();

   /// carregando o balao
   glPushMatrix();
      if(z_baloon > 60){
            z_baloon = 4;
      }
      glTranslatef(11.0, z_baloon, -View.Distance-90);
      glRotatef(-90,1,0,0);
      glRotatef(rotate_deathstar,0.0,0.0,1.0);

	  //elglRotatef(View.rotY,0,1,0);
      glScalef(2*Scale, 2*Scale, 2*Scale);
      glmDrawVBO(baloon_model);
   glPopMatrix();

///deathstar
   glPushMatrix();
      glTranslatef(15.0, 10, -View.Distance-25);
      glRotatef(deathstar, 0.0, 1.0, 0.0);
//    glRotatef(View.rotX,1,0,0);
//	  glRotatef(View.rotY,0,1,0);
      glScalef(Scale*3, Scale*3, Scale*3);
      glmDrawVBO(deathstar_model);
   glPopMatrix();

    /// container
   glPushMatrix();

      glTranslatef(-4.0, 0.0, -View.Distance-2);
      glRotatef(-90, 0.0, 1.0, 0.0);
      glRotatef(90,1,0,0);
	  //glRotatef(View.rotY,0,1,0);
      glScalef(Scale, Scale, Scale);

      glmDrawVBO(container_model);
   glPopMatrix();

   /// carregando o gato
   glPushMatrix();
      glTranslatef(7, -1.0, -View.Distance+z_cat);
      glRotatef(90, 0.0, 0.0, 1.0);       /// rotacionando em 45 graus no eixo x
      glRotatef(-90,0.0,1.0,0.0);
	  glRotatef(180,1.0,0.0,0.0);
      glScalef(0.5*Scale, 0.5*Scale, 0.5*Scale);
      glmDrawVBO(cat_model);
   glPopMatrix();

    glutSwapBuffers();
}



static void DoFeatureChecks(void){
   if (!GLEW_VERSION_2_0) {
      /* check for individual extensions */
      if (!GLEW_ARB_texture_cube_map) {
         printf("Sorry, GL_ARB_texture_cube_map is required.\n");
         exit(1);
      }
      if (!GLEW_ARB_vertex_shader) {
         printf("Sorry, GL_ARB_vertex_shader is required.\n");
         exit(1);
      }
      if (!GLEW_ARB_fragment_shader) {
         printf("Sorry, GL_ARB_fragment_shader is required.\n");
         exit(1);
      }
      if (!GLEW_ARB_vertex_buffer_object) {
         printf("Sorry, GL_ARB_vertex_buffer_object is required.\n");
         exit(1);
      }
   }
   if (!ShadersSupported()) {
      printf("Sorry, GLSL is required\n");
      exit(1);
   }
}

void SpecialKeys(int key, int x, int y) {
    // slenderman
    if(objeto == 0){
        switch (key) {
            case GLUT_KEY_LEFT :
                x_slenderman -= 0.5;
                angulo_slenderman = 270;
                break;
            case GLUT_KEY_RIGHT :
                x_slenderman += 0.5;
                angulo_slenderman = 90;
                break;
            case GLUT_KEY_UP :
                z_slenderman -= 0.5;
                angulo_slenderman = 180;
                break;
            case GLUT_KEY_DOWN :
                z_slenderman += 0.5;
                angulo_slenderman = 0;
                break;
		}
    } else if (objeto == 1){
        switch (key) {
            case GLUT_KEY_UP :
                z_baloon += 1.0;
                break;

		}
    }

    glutPostRedisplay();
}

void opcao(int key, int x, int y){

    switch(key){
        case 's':
            objeto = 0; // slender
            break;
        case 'b':
            objeto = 1; // balao
            break;
    }
}




int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitWindowSize(WinWidth, WinHeight);

    slender_file = "slenderman.obj";
    baloon_file = "11088_balloon_v3.obj";
    chao_file = "rock.obj";
    container_file = "12281_Container_v2_L2.obj";
    cat_file = "12221_Cat_v1_l3.obj";
    deathstar_file = "death-star-II.obj";


    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutCreateWindow("objview");
    glewInit();

    DoFeatureChecks();

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(opcao);

    glutIdleFunc(spinDisplay);
    glutSpecialFunc(SpecialKeys);

    InitViewInfo(&View);

    read_model();

    init();
    glutMainLoop();

    return 0;
}
