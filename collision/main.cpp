#include <windows.h>
#include <GL/glut.h>
#include <GL/glext.h>
#include <GL/glaux.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include "collision.h"

const GLfloat lightAmbient[4] = { 1.0, 1.0, 1.0, 1.0 };
const GLfloat lightDiffuse[4] = { 1.0, 1.0, 1.0, 1.0 };
const GLfloat lightPosition[4] = { 0.0, 0.0, -20.0, 1.0 };

const GLfloat fogColor[4]= {0.2f, 0.2f, 0.2f, 0.5f};

//PFNGLMULTITEXCOORD1FARBPROC        glMultiTexCoord1fARB    = NULL;
//PFNGLMULTITEXCOORD2FARBPROC        glMultiTexCoord2fARB    = NULL;
//PFNGLACTIVETEXTUREARBPROC          glActiveTextureARB      = NULL;
//PFNGLCLIENTACTIVETEXTUREARBPROC    glClientActiveTextureARB= NULL;

bool light = true;
bool blend = false;
bool fog = true;
GLfloat xRot = 0;
GLfloat yRot = 0;
GLfloat zRot = 0;
GLfloat zoom = -20;
std::vector<GLuint> texture;
bool keystate[256];

collision *a;
double delta_time=0.1;

ball Ball_one(coordinate(1,2,1),coordinate(1,1,0),1,2);
ball Ball_two(coordinate(9,6,3),coordinate(-1,-1,0),1,2);


static void reshape(int width, int height)
{
    glViewport( 0, 0, (GLint)width, (GLint)height );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 45.0, (GLfloat)width/(GLfloat)height, 0.1, 100.0 );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}

void testkey()
{
    if(keystate['Q']==true)exit(0);
    if(keystate['D']==true)
    {
        yRot+=2;
    }
    if(keystate['A']==true)
    {
        yRot-=2;
    }
    if(keystate['W']==true)
    {
        xRot-=2;
    }
    if(keystate['S']==true)
    {
        xRot+=2;
    }

    //glutPostRedisplay();
}

static void keydown(unsigned char key, int x, int y)
{
    if(keystate[key]==false)keystate[key]=true;


}

static void keyup(unsigned char key, int x, int y)
{
    if(keystate[key]==true)keystate[key]=false;

}

static void mouse(int button,int state,int x,int y)
{
    switch(button)
    {
        case GLUT_WHEEL_UP:
        zoom++;
        break;
        case GLUT_WHEEL_DOWN:
        zoom--;
        break;
    }
    glutPostRedisplay();
}

void loadGLTexture(std::string filename)
{
    AUX_RGBImageRec* TEXTURE;
    char s[1000];
    GetCurrentDirectory(1000,s);
    std::string a;
    a.assign(s);
    a.append("\\"+filename);
    TEXTURE = auxDIBImageLoad(a.c_str());
    GLuint i;
    glGenTextures( 1, &i );
    glBindTexture( GL_TEXTURE_2D, i );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, TEXTURE->sizeX, TEXTURE->sizeY, 0,GL_RGB, GL_UNSIGNED_BYTE, TEXTURE->data );
    texture.push_back(i);
}
static void loadGLTextures()
{
    loadGLTexture("Crate.bmp");
}

static void idle(void)
{
    glutPostRedisplay();
}

static void display(void)
{
    testkey();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0.0,0.0, zoom );
    glRotatef( xRot,  1.0,  0.0,  0.0 );
    glRotatef( yRot,  0.0,  1.0,  0.0 );

//----------------------------------------------------------------//
    coordinate vec = a->get_ball_1_position();

    glPushMatrix();

    glTranslatef(-5.0,-5.0,-5.0);
    glColor3f(1.0,0.0,0.0);
    glTranslatef(vec.x,vec.y,vec.z  );
    glutSolidSphere(a->get_ball_1_radius(),20,16);
    glPopMatrix();

    vec = a->get_ball_2_position();

    glPushMatrix();
    glTranslatef(-5.0,-5.0,-5.0);
    glColor3f(0.0,0.0,1.0);
    glTranslatef(vec.x,vec.y,vec.z  );
    glutSolidSphere(a->get_ball_2_radius(),20,16);
    glPopMatrix();
//----------------------------------------------------------------//

    glDepthMask(GL_FALSE);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glEnable(GL_TEXTURE_2D);

    glPushMatrix();
    glColor3f(0.5,0.5,0.5);
    glScalef(5.0,5.0,5.0);
    glBegin( GL_QUADS );
      glNormal3f(  0.0,  0.0, 1.0 );
      glTexCoord2f(0.0, 0.0 ); glVertex3f( -1.0, -1.0,  1.0 );
      glTexCoord2f(1.0, 0.0 ); glVertex3f(  1.0, -1.0,  1.0 );
      glTexCoord2f( 1.0, 1.0 ); glVertex3f(  1.0,  1.0,  1.0 );
      glTexCoord2f(0.0, 1.0 );glVertex3f( -1.0,  1.0,  1.0 );

      glNormal3f(  0.0,  0.0, -1.0 );
      glTexCoord2f(1.0, 0.0 ); glVertex3f( -1.0, -1.0, -1.0 );
      glTexCoord2f(1.0, 1.0 ); glVertex3f( -1.0,  1.0, -1.0 );
      glTexCoord2f(0.0, 1.0 ); glVertex3f(  1.0,  1.0, -1.0 );
      glTexCoord2f(0.0, 0.0 ); glVertex3f(  1.0, -1.0, -1.0 );

      glNormal3f(  0.0,  1.0,  0.0 );
      glTexCoord2f(0.0, 1.0 ); glVertex3f( -1.0,  1.0, -1.0 );
      glTexCoord2f(0.0, 0.0 ); glVertex3f( -1.0,  1.0,  1.0 );
      glTexCoord2f(1.0, 0.0 ); glVertex3f(  1.0,  1.0,  1.0 );
      glTexCoord2f(1.0, 1.0 ); glVertex3f(  1.0,  1.0, -1.0 );

      glNormal3f(  0.0, -1.0,  0.0 );
      glTexCoord2f(1.0, 1.0 ); glVertex3f( -1.0, -1.0, -1.0 );
      glTexCoord2f(0.0, 1.0 );glVertex3f(  1.0, -1.0, -1.0 );
      glTexCoord2f(0.0, 0.0 ); glVertex3f(  1.0, -1.0,  1.0 );
      glTexCoord2f(1.0, 0.0 ); glVertex3f( -1.0, -1.0,  1.0 );

      glNormal3f(  1.0,  0.0,  0.0 );
      glTexCoord2f(1.0, 0.0 ); glVertex3f(  1.0, -1.0, -1.0 );
      glTexCoord2f(1.0, 1.0 ); glVertex3f(  1.0,  1.0, -1.0 );
      glTexCoord2f(0.0, 1.0 ); glVertex3f(  1.0,  1.0,  1.0 );
      glTexCoord2f(0.0, 0.0 ); glVertex3f(  1.0, -1.0,  1.0 );

      glNormal3f( -1.0,  0.0,  0.0 );
      glTexCoord2f(0.0, 0.0 ); glVertex3f( -1.0, -1.0, -1.0 );
      glTexCoord2f(1.0, 0.0 ); glVertex3f( -1.0, -1.0,  1.0 );
      glTexCoord2f(1.0, 1.0 ); glVertex3f( -1.0,  1.0,  1.0 );
      glTexCoord2f(0.0, 1.0 ); glVertex3f( -1.0,  1.0, -1.0 );
    glEnd();
    glPopMatrix();

    glDepthMask(GL_TRUE);


    a->move();

    glutSwapBuffers();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(100,100);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("GLUT");

    //glMultiTexCoord1fARB    = (PFNGLMULTITEXCOORD1FARBPROC)        wglGetProcAddress("glMultiTexCoord1fARB");
    //glMultiTexCoord2fARB    = (PFNGLMULTITEXCOORD2FARBPROC)        wglGetProcAddress("glMultiTexCoord2fARB");
    //glActiveTextureARB        = (PFNGLACTIVETEXTUREARBPROC)        wglGetProcAddress("glActiveTextureARB");
    //glClientActiveTextureARB= (PFNGLCLIENTACTIVETEXTUREARBPROC)    wglGetProcAddress("glClientActiveTextureARB");

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keydown);
    glutKeyboardUpFunc(keyup);
    glutMouseFunc(mouse);
    glutIdleFunc(idle);

    loadGLTextures();
    glShadeModel( GL_SMOOTH );
    glClearColor(0.0f,0.0f,0.0f,0.5f);

    glLightfv( GL_LIGHT1, GL_AMBIENT, lightAmbient );
    glLightfv( GL_LIGHT1, GL_DIFFUSE, lightDiffuse );
    glLightfv( GL_LIGHT1, GL_POSITION, lightPosition );

    glEnable( GL_LIGHT1 );
    glEnable(GL_LIGHTING);
    //glEnable(GL_COLOR_MATERIAL);

    /*glFogi(GL_FOG_MODE, GL_EXP);
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogf(GL_FOG_DENSITY, 0.35f);
    glHint(GL_FOG_HINT, GL_DONT_CARE);
    glFogf(GL_FOG_START, 1.0f);
    glFogf(GL_FOG_END, 5.0f);
    glEnable(GL_FOG);*/

    glClearDepth( 1.0 );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );



    glColor4f( 1.0, 1.0, 1.0, 0.5 );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE );
    glEnable( GL_BLEND );

    a=new collision(delta_time,Ball_one,Ball_two,coordinate(10,10,10));

    glutMainLoop();

    return EXIT_SUCCESS;
}
