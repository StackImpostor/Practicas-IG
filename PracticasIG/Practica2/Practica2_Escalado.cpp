// Etapa2.cpp
// Fichero principal 
////////////////////////////////////////////////////
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
const int W_WIDTH = 500; // Tama�o incial de la ventana
const int W_HEIGHT = 500;
GLfloat fAngulo; // Variable que indica el �ngulo de rotaci�n de los ejes. 
int p_width=500, p_height=500;
void myResize(int width, int height) {
	/*glViewport(0, 0, width, height);
	glScalef(1, 1, 1);
	GLfloat tx = ((width - p_width) / (float)width);
	GLfloat ty = ((height - p_height) / (float)height);
	glTranslatef(tx,ty,0);
	p_width = width;
	p_height = height;*/
}

// Funci�n que visualiza la escena OpenGL
void Display(void)
{
	// Borramos la escena
	glClear(GL_COLOR_BUFFER_BIT);

	glPushMatrix();

	// Dibujamos los ejes
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 1.0f);
	//Eje Y
	glVertex3i(0, 1, 0);
	glVertex3i(0, -1, 0);
	//Eje X
	glVertex3i(-1, 0, 0);
	glVertex3i(1, 0, 0);
	glEnd();

	// Rotamos las proximas primitivas
	glRotatef(fAngulo, 0.0f, 0.0f, 1.0f);
	// Creamos a continuaci�n dibujamos los tres poligonos
	glBegin(GL_POLYGON);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-0.5f, 0.866f, 0.0f);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-0.5f, -0.866f, 0.0f);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glColor3f(0.0f, 1.0f, 1.0f);
	glVertex3f(-0.5f, 0.866f, 0.0f);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-0.5f, -0.866f, 0.0f);
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
	glutCreateWindow("Mi primera Ventana");

	// Indicamos cuales son las funciones de redibujado e idle
	glutDisplayFunc(Display);
	glutIdleFunc(Idle);
	glutReshapeFunc(myResize);

	// El color de fondo ser� el negro (RGBA, RGB + Alpha channel)
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glOrtho(-1.0, 1.0f, -1.0, 1.0f, -1.0, 1.0f);

	// Comienza la ejecuci�n del core de GLUT
	glutMainLoop();
	return 0;
}