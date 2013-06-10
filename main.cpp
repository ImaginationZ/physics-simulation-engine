#include "FreeImagePlus.h"
#include <GL/glut.h>
#include <GL/glext.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include "control.h"
#include "ArcBall.h"
//#include "FreeType.h"
#include <cmath>

const double pi = 3.141592653;
const GLfloat lightAmbient[] = { 1.0, 1.0, 1.0, 1.0 };
const GLfloat lightDiffuse[] = { 1.0, 1.0, 1.0, 1.0 };
const GLfloat lightPosition[] = { 0.0, 0.0, -20.0, 1.0 };
const GLfloat mat_ambient[] ={0.0,0.0,0.1,1.0};
const GLfloat mat_diffuse[] ={0.1,0.5,0.8,1.0};
const GLfloat mat_specular[] ={0.0,0.0,0.0,1.0};
const GLfloat mat_shininess[] ={10.0};
const GLfloat mat_ambient_o[] ={0.3,0.3,0.3,1.0};
const GLfloat mat_diffuse_o[] ={0.8,0.8,0.8,1.0};
const GLfloat mat_specular_o[] ={0.0,0.0,0.0,1.0};
const GLfloat mat_shininess_o[] ={0.0};


const GLfloat fogColor[4]= {0.2f, 0.2f, 0.2f, 0.5f};

bool light = true;
bool blend = false;
bool fog = true;
GLfloat xRot = 0;
GLfloat yRot = 0;
GLfloat zoom = -20;
GLint WindowWidth = 640;
GLint WindowHeight = 480;
std::vector<GLuint> texture;
bool keystate[256];
char FPS[1000];
//freetype::font_data our_font;
ArcBall_t* arc;
control* a;
GLUquadricObj *quadratic;
GLuint  object=0;

Matrix4fT   Transform   = {  1.0f,  0.0f,  0.0f,  0.0f,
                             0.0f,  1.0f,  0.0f,  0.0f,
                             0.0f,  0.0f,  1.0f,  0.0f,
                             0.0f,  0.0f,  0.0f,  1.0f };

Matrix3fT   LastRot     = {  1.0f,  0.0f,  0.0f,
                             0.0f,  1.0f,  0.0f,
                             0.0f,  0.0f,  1.0f };

Matrix3fT   ThisRot     = {  1.0f,  0.0f,  0.0f,
                             0.0f,  1.0f,  0.0f,
                             0.0f,  0.0f,  1.0f };
Point2fT    MousePt;

bool        isClicked  = false;
bool        isRClicked = false;
bool        isDragging = false;


void reshape(int width, int height)
{
    glViewport( 0, 0, (GLint)width, (GLint)height );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 45.0, (GLfloat)width/(GLfloat)height, 0.1, 100.0 );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    WindowWidth = width;
    WindowHeight = height;
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

}

void testdrag()
{
    if (isRClicked)													// 如果右键按下，这重置所有的变量
    {
		Matrix3fSetIdentity(&LastRot);
		Matrix3fSetIdentity(&ThisRot);
        Matrix4fSetRotationFromMatrix3f(&Transform, &ThisRot);
    }

    if (!isDragging)
    {
        if (isClicked)
        {
			isDragging = true;
			LastRot = ThisRot;
			arc->click(&MousePt);
        }
    }
    else
    {
        if (isClicked)
        {
            Quat4fT     ThisQuat;

            arc->drag(&MousePt, &ThisQuat);
            Matrix3fSetRotationFromQuat4f(&ThisRot, &ThisQuat);
            Matrix3fMulMatrix3f(&ThisRot, &LastRot);
            Matrix4fSetRotationFromMatrix3f(&Transform, &ThisRot);
        }
        else
            isDragging = false;
    }
}

void keydown(unsigned char key, int x, int y)
{
    if(keystate[key]==false)keystate[key]=true;


}

void keyup(unsigned char key, int x, int y)
{
    if(keystate[key]==true)keystate[key]=false;

}

void mouse(int button,int state,int x,int y)
{
    switch(button)
    {

        break;
    }

    if(button==GLUT_LEFT_BUTTON&&state==GLUT_UP)
    {
        isClicked = false;
    }
    else if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN)
    {
        isClicked = true;
    }
    else if(button==GLUT_RIGHT_BUTTON&&state==GLUT_UP)
    {
        isRClicked = false;
    }
    else if(button==GLUT_RIGHT_BUTTON&&state==GLUT_DOWN)
    {
        isRClicked = true;
    }

    MousePt.s.X = (GLfloat)x;
    MousePt.s.Y = (GLfloat)y;
}

void mousemove(int x,int y)
{
    MousePt.s.X = (GLfloat)x;
    MousePt.s.Y = (GLfloat)y;
}

void loadGLTexture(std::string filename)
{
    FIBITMAP* dib = NULL;
    char s[1000];
    GetCurrentDirectory(1000,s);
    std::string a;
    a.assign(s);
    a.append("\\"+filename);
    //FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
    //fif = FreeImage_GetFileType(a.c_str(),0);
    dib = FreeImage_Load(FIF_BMP, "Crate.bmp",BMP_DEFAULT);
    FIBITMAP *temp = FreeImage_ConvertTo24Bits(dib);
    FreeImage_Unload(dib);
    dib = temp;
    BYTE *bits = new BYTE[FreeImage_GetWidth(dib) * FreeImage_GetHeight(dib) * 3];
    BYTE *pixels = (BYTE*)FreeImage_GetBits(dib);
    for(int pix=0; pix<FreeImage_GetWidth(dib) * FreeImage_GetHeight(dib); pix++)
    {
        bits[pix*3+0]=pixels[pix*3+2];
        bits[pix*3+1]=pixels[pix*3+1];
        bits[pix*3+2]=pixels[pix*3+0];
    }

    GLuint i;
    glGenTextures( 1, &i );
    glBindTexture( GL_TEXTURE_2D, i );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, FreeImage_GetWidth(dib), FreeImage_GetHeight(dib), 0,GL_RGB, GL_UNSIGNED_BYTE, bits );
    texture.push_back(i);
    FreeImage_Unload(dib);
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
    testdrag();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0.0,0.0, zoom );
    glRotatef( xRot,  1.0,  0.0,  0.0 );
    glRotatef( yRot,  0.0,  1.0,  0.0 );
    glMultMatrixf(Transform.M);
//----------------------------------------------------------------//

    coordinate vec;

    for(int i=0;i<a->ask_total_number();i++)
    {
        vec = a->ask_position(i);
        glPushMatrix();

        glTranslatef(-5.0,-5.0,-5.0);
        glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
        glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
        glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
        glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);
        glTranslatef(vec.x,vec.z,vec.y  );
        glutSolidSphere(a->ask_radius(i),20,16);
        glPopMatrix();
    }

//----------------------------------------------------------------//



    glEnable( GL_BLEND );
    glDepthMask(GL_FALSE);
    glPushMatrix();

    vec = a->c->get_axis();

    if(fabs(vec.y)>1e-8)
    {
        if(fabs(vec.x)<1e-8)glRotatef((vec.y/fabs(vec.y))*90,0.0,1.0,0.0);
        else if(fabs(vec.x)>1e-8)glRotatef((vec.y/fabs(vec.y))*angle(coordinate(1,0,0),coordinate(vec.x,vec.y,0))/pi*180,0.0,1.0,0.0);
    }

    if(fabs(vec.z)>1e-8)
    {
        if(fabs(vec.x)<1e-8&&fabs(vec.y)<1e-8)glRotatef(-(vec.z/fabs(vec.z))*90,1.0,0.0,0.0);
        else glRotatef(-(vec.z/fabs(vec.z))*angle(coordinate(vec.x,vec.y,0),coordinate(vec.x,vec.y,vec.z))/pi*180,1.0,0.0,0.0);
    }

    vec = a->c->ask_position();
    glTranslatef(-5.0,-5.0,-5.0-a->c->get_height()/2);
    glTranslatef(vec.x,vec.z,vec.y);
    gluCylinder(quadratic,a->c->get_radius(),a->c->get_radius(),a->c->get_height(),32,32);
    glPopMatrix();


    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient_o);
    glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse_o);
    glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular_o);
    glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess_o);
    glEnable(GL_TEXTURE_2D);

    glScalef(5.0,5.0,5.0);
    glBegin(GL_QUADS);
      glNormal3f(  0.0,  0.0, 1.0 );
      glTexCoord2f(0.0, 0.0 ); glVertex3f( -1.0, -1.0,  1.0 );
      glTexCoord2f(1.0, 0.0 ); glVertex3f(  1.0, -1.0,  1.0 );
      glTexCoord2f( 1.0, 1.0 ); glVertex3f(  1.0,  1.0,  1.0 );
      glTexCoord2f( 0.0, 1.0 );glVertex3f( -1.0,  1.0,  1.0 );

      glNormal3f(  0.0,  0.0, -1.0 );
      glTexCoord2f(1.0, 0.0 ); glVertex3f( -1.0, -1.0, -1.0 );
      glTexCoord2f(1.0, 1.0 ); glVertex3f( -1.0,  1.0, -1.0 );
      glTexCoord2f(0.0, 1.0 ); glVertex3f(  1.0,  1.0, -1.0 );
      glTexCoord2f(0.0, 0.0 ); glVertex3f(  1.0, -1.0, -1.0 );

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

      glNormal3f(  0.0,  1.0,  0.0 );
      glTexCoord2f( 0.0, 1.0 ); glVertex3f( -1.0,  1.0, -1.0 );
      glTexCoord2f(0.0, 0.0 ); glVertex3f( -1.0,  1.0,  1.0 );
      glTexCoord2f( 1.0, 0.0 ); glVertex3f(  1.0,  1.0,  1.0 );
      glTexCoord2f( 1.0, 1.0 ); glVertex3f(  1.0,  1.0, -1.0 );

      glNormal3f(  0.0, -1.0,  0.0 );
      glTexCoord2f(1.0, 1.0 ); glVertex3f( -1.0, -1.0, -1.0 );
      glTexCoord2f(0.0, 1.0 );glVertex3f(  1.0, -1.0, -1.0 );
      glTexCoord2f(0.0, 0.0 ); glVertex3f(  1.0, -1.0,  1.0 );
      glTexCoord2f(1.0, 0.0 ); glVertex3f( -1.0, -1.0,  1.0 );
    glEnd();
    glPopMatrix();

    glDisable( GL_BLEND );
    glDisable(GL_TEXTURE_2D);
    glDepthMask(GL_TRUE);


/*/----------------------------------------------------------------------//


//-----------------------------------------------------------------/*/

    a->work();

    glutSwapBuffers();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(100,100);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("GLUT");

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keydown);
    glutKeyboardUpFunc(keyup);
    glutMouseFunc(mouse);
    glutMotionFunc(mousemove);
    glutIdleFunc(idle);

    FreeImage_Initialise();
//    our_font.init("test.TTF", 16);
    arc = new ArcBall_t(640.0f, 480.0f);

    loadGLTextures();
    glShadeModel( GL_SMOOTH );
    glClearColor(0.2f,0.2f,0.2f,0.5f);

    glLightfv( GL_LIGHT1, GL_AMBIENT, lightAmbient );
    glLightfv( GL_LIGHT1, GL_DIFFUSE, lightDiffuse );
    glLightfv( GL_LIGHT1, GL_POSITION, lightPosition );

    glEnable( GL_LIGHT1 );
    glEnable(GL_LIGHTING);

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

    glBlendFunc( GL_SRC_ALPHA, GL_ONE );
    glEnable(GL_BLEND);

    quadratic=gluNewQuadric();
	gluQuadricNormals(quadratic, GLU_SMOOTH);
	gluQuadricTexture(quadratic, GL_TRUE);

    a = new control();

    glutMainLoop();

    return EXIT_SUCCESS;
}
