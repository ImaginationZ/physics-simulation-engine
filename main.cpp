#include <windows.h>
#include "GL/glut.h"
#include "GL/glext.h"
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include "control.h"
#include "ArcBall.h"
#include <cmath>

const double pi = 3.141592653;
const GLfloat lightAmbient[] = { 1.0, 1.0, 1.0, 1.0 };
const GLfloat lightDiffuse[] = { 1.0, 1.0, 1.0, 1.0 };
const GLfloat lightPosition[] = { 0.0, 0.0, 0.0, 1.0 };
const GLfloat mat_ambient[] ={0.0,0.0,0.1,1.0};
const GLfloat mat_diffuse[] ={0.1,0.5,0.8,1.0};
const GLfloat mat_specular[] ={0.0,0.0,0.0,1.0};
const GLfloat mat_shininess[] ={10.0};
const GLfloat mat_ambient_o[] ={0.4,0.4,0.0,1.0};
const GLfloat mat_diffuse_o[] ={1.0,1.0,1.0,1.0};
const GLfloat mat_specular_o[] ={0.0,0.0,0.0,1.0};
const GLfloat mat_shininess_o[] ={0.0};
const GLfloat mat_ambient_c[] ={1.0,1.0,1.0,1.0};
const GLfloat mat_diffuse_c[] ={1.0,1.0,1.0,0.2};
const GLfloat mat_specular_c[] ={0.0,0.0,0.0,1.0};
const GLfloat mat_shininess_c[] ={0.0};


const GLfloat fogColor[4]= {0.2f, 0.2f, 0.2f, 0.5f};

bool light = true;
bool blend = false;
bool fog = true;
GLfloat xRot = 0;
GLfloat yRot = 0;
GLfloat zoom = -30;
GLint WindowWidth = 640;
GLint WindowHeight = 480;
std::vector<GLuint> texture;
bool keystate[256];
char FPS[1000];
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
    if(keystate['Q']==true||keystate['q']==true||keystate[27]==true)exit(0);
    if(keystate['M']==true||keystate['m']==true){
        flag = true;
    }

}

void testdrag()
{
    if (isRClicked)
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
    if(button==GLUT_WHEEL_UP)
    {
        zoom++;
    }
    else if(button==GLUT_WHEEL_DOWN)
    {
        zoom--;
    }

    MousePt.s.X = (GLfloat)x;
    MousePt.s.Y = (GLfloat)y;
}

void mousemove(int x,int y)
{
    MousePt.s.X = (GLfloat)x;
    MousePt.s.Y = (GLfloat)y;
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

    glTranslatef(0,0,zoom);
    glMultMatrixf(Transform.M);
    glTranslatef(-a->c->ask_position().y+5,-a->c->ask_position().z+5,-a->c->ask_position().x+5);
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
    glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient_c);
    glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse_c);
    glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular_c);
    glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess_c);

    vec = a->c->ask_position();
    glTranslatef(-5.0+vec.x,-5.0+vec.z,-5.0+vec.y);
    vec = a->c->get_axis();

    if(fabs(vec.x)>1e-8)
    {
        if(fabs(vec.y)<1e-8)glRotatef((vec.x/fabs(vec.x))*90,0.0,1.0,0.0);
        else glRotatef((vec.x/fabs(vec.x))*angle(coordinate(0,1,0),coordinate(vec.x,vec.y,0))/pi*180,0.0,1.0,0.0);
    }
    if(fabs(vec.x)<1e-8&&vec.y<-1e-8)glRotatef(180,0.0,1.0,0.0);
    if(fabs(vec.z)>1e-8)
    {
        if(fabs(vec.x)<1e-8&&fabs(vec.y)<1e-8)glRotatef(-(vec.z/fabs(vec.z))*90,1.0,0.0,0.0);
        else glRotatef(-(vec.z/fabs(vec.z))*(angle(coordinate(sqrt(pow(vec.x,2)+pow(vec.y,2)),0,0),coordinate(sqrt(pow(vec.x,2)+pow(vec.y,2)),0,vec.z))/pi*180),1.0,0.0,0.0);
    }



    glTranslatef(0,0,-0.5-a->c->get_height()/2);
    gluCylinder(quadratic,a->c->get_radius()+0.5,a->c->get_radius()+0.5,a->c->get_height(),32,32);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0,0.0,0.0);
    for(int i=0; i<=32; ++i)glVertex3f((a->c->get_radius()+0.5)*cos(2*pi/32*i), (a->c->get_radius()+0.5)*sin(2*pi/32*i),0.0);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(-5.0,-5.0,-5.0);
    glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient_o);
    glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse_o);
    glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular_o);
    glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess_o);
    glBegin(GL_LINES);
    for(int i=-50;i<=50;i++)
    {
        glVertex3f(-50,0,i);
        glVertex3f(50,0,i);
        glVertex3f(i,0,-50);
        glVertex3f(i,0,50);
    }
    glEnd();
    glPopMatrix();

    glDisable( GL_BLEND );
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

    arc = new ArcBall_t(640.0f, 480.0f);

    glShadeModel( GL_SMOOTH );
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
    glLineWidth(0.1);
    glClearColor(0.2f,0.2f,0.2f,0.5f);

    glLightfv( GL_LIGHT1, GL_AMBIENT, lightAmbient );
    glLightfv( GL_LIGHT1, GL_DIFFUSE, lightDiffuse );
    glLightfv( GL_LIGHT1, GL_POSITION, lightPosition );

    glEnable( GL_LIGHT1 );
    glEnable(GL_LIGHTING);

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
