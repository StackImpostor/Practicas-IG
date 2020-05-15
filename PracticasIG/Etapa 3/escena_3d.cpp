// Etapa2.cpp
// Fichero principal 
//Bartomeu Ramis
//David Cantero
//Joan Jorquera
////////////////////////////////////////////////////
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
const int W_WIDTH = 700; // Tama�o incial de la ventana
const int W_HEIGHT = 700;
GLfloat fAngulo; // Variable que indica el �ngulo de rotaci�n de los ejes. 
float p_width = W_WIDTH, p_height = W_HEIGHT;

void myResize(int width, int height) {

	//glViewport(0, 0, width, height);

	float nr_w = width / p_width;
	float nr_h = height / p_height;
	//glOrtho(-nr_w, nr_w, -nr_h, nr_h, 1, -1);
	//glFrustum(nr_w, nr_w, nr_h, nr_h, 1, 1);
	p_width = width;
	p_height = height;
}

// Funci�n que visualiza la escena OpenGL
void Display(void)
{

	// Borramos la escena
	glClear(GL_COLOR_BUFFER_BIT);

	glPushMatrix();

	//Activamos buffer de PROFUNDIAD (modo sexo: on)
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	//Colores sobras y cosas divertidas
	/*glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	glEnable(GL_LIGHT0);

	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 1);*/

	// Rotamos las proximas primitivas
	//glRotatef(-45, 1.0f, 0.0f, 0.0f);
	//glRotatef(fAngulo, 0.0f, 1.0f, 0.0f);
	//glRotatef(45, 0.0f, 1.0f, 0.0f);

	//Ejes de cordenadas
	glLineWidth(3);

	glBegin(GL_LINES);
	glColor3f(1.0f, 0, 0);
	glVertex3f(-20, 0, 0);
	glVertex3f(20, 0, 0);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0, 1.0f, 0);
	glVertex3f(0, -20, 0);
	glVertex3f(0, 20, 0);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0, 0, 1.0f);
	glVertex3f(0, 0, -20);
	glVertex3f(0, 0, 20);
	glEnd();

	glLineWidth(1);

	for (float i = -20; i <= 20; i += 0.25) {
		glBegin(GL_LINES);
		glColor3f(0, 0, 0);
		glVertex3f(i, 0, -20);
		glVertex3f(i, 0, 20);
		glEnd();
		glBegin(GL_LINES);
		glColor3f(0, 0, 0);
		glVertex3f(-20, 0, i);
		glVertex3f(20, 0, i);
		glEnd();
	}

	//Objetos

	
	
	//Teapod
	glPushMatrix();
	glTranslatef(-0.7, 0.25, -0.7);

	glColor3f(0.5, 0.5, 0.5);
	glutSolidTeapot(0.4);
	glColor3f(0, 0, 0);
	glutWireTeapot(0.4);

	glPopMatrix();

	//theCube
	glPushMatrix();
	glTranslatef(-0.5, 0.25, 0.5);

	

	glColor3f(0.5, 0.5, 0.5);
	glutSolidCube(0.5);

	glColor3f(0, 0, 0);
	glutWireCube(0.5);

	//glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	//Torus
	glPushMatrix();

	glTranslatef(0.7, 0.15, -0.7);
	glRotatef(90, 1.0f, 0, 0);

	glColor3f(0.5, 0.5, 0.5);
	glutSolidTorus(0.15, 0.3, 50, 50);

	glColor3f(0, 0, 0);
	glutWireTorus(0.15, 0.3, 50, 50);

	glPopMatrix();

	//Cone
	glPushMatrix();
	glTranslatef(0.5, 0, 0.5);
	glRotatef(-90, 1, 0, 0);

	glColor3f(0.5, 0.5, 0.5);
	glutSolidCone(0.2, 1, 50, 50);

	glColor3f(0, 0, 0);
	glutWireCone(0.2, 1, 50, 50);

	glPopMatrix();

	


	//Dibujamos el "suelo" que es trans
	glBegin(GL_POLYGON);
	glColor4f(0.5f, 0.5f, 1.0f, 0.5f);
	glVertex3f(-20, 0, -20);
	glVertex3f(20, 0, -20);
	glVertex3f(20, 0, 20);
	glVertex3f(-20, 0, 20);
	glEnd();

	

	glPopMatrix();

	glutSwapBuffers();
}

// Funci�n que se ejecuta cuando el sistema no esta ocupado
void Idle(void)
{
	// Incrementamos el �ngulo
	fAngulo += 0.3f;
	// Si es mayor que dos pi la decrementamos
	if (fAngulo > 360)
		fAngulo -= 360;
	// Indicamos que es necesario repintar la pantalla
	glutPostRedisplay();
}

// Funci�n principal
int main(int argc, char** argv)
{
	// Inicializamos la librer�a GLUT
	glutInit(&argc, argv);
	
	// Indicamos como ha de ser la nueva ventana
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(W_WIDTH, W_HEIGHT);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

	

	// Creamos la nueva ventana
	glutCreateWindow("Mis cosas 3D");

	// Indicamos cuales son las funciones de redibujado e idle
	glutDisplayFunc(Display);
	glutIdleFunc(Idle);
	glutReshapeFunc(myResize);

	// El color de fondo ser� el negro (RGBA, RGB + Alpha channel)
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	//glOrtho(-1.0, 1.0f, -1.0, 1.0f, -100.0, 100.0f);
	
	
	
	//gluLookAt(0.5, 0.5, 0, 0, 0, 0, 0, 1, 0);
	//gluPerspective(30, p_width/p_height, 0.1, 100);
	glFrustum(-0.05, 0.05, -0.05, 0.05, 0.2, 100);
	gluLookAt(3, 3, 3, 0, 0, 0, 0, 1, 0);
	//glMatrixMode(GL_MODELVIEW);

	// Comienza la ejecuci�n del core de GLUT
	glutMainLoop();
	return 0;
}