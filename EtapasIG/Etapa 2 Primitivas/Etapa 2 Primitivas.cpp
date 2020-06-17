// Etapa2.cpp
// Fichero principal 
////////////////////////////////////////////////////
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
const int W_WIDTH = 700; // Tamaño incial de la ventana
const int W_HEIGHT = 700;
GLfloat fAngulo; // Variable que indica el �ngulo de rotaci�n de los ejes. 
GLfloat fTraslacionX;
GLfloat fEscala = 1;
GLfloat fCizalla = 0;
float incTraslacion = -0.01f;
float incEscalado = 0.01f;
float incCizalla = 0.01f;

void myResize(int width, int height) {
	glScalef(1, 1, 1);
}

void cizallamientoX(GLfloat c) {
	GLfloat m[16] = {
	1.0f, 0.0f, 0.0f, 0.0f,
	c, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
	};
	glMultMatrixf(m);
}

// Función que visualiza la escena OpenGL
void Display(void)
{
	// Borramos la escena
	glClear(GL_COLOR_BUFFER_BIT);

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


	//Cuadrante arriba izquierda
	glPushMatrix();
	glTranslatef(fTraslacionX, 0.0f, 0.0f);

	glBegin(GL_QUADS);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.2f, 0.2f, 0.0f);
	glVertex3f(-0.2f, 0.4f, 0.0f);
	glVertex3f(-0.4f, 0.4f, 0.0f);
	glVertex3f(-0.4f, 0.2f, 0.0f);
	glEnd();
	glPopMatrix();

	//Cuadrante arriba derecha
	glPushMatrix();

	glTranslatef(0.3f, 0.3f, 0.0f);
	glRotatef(fAngulo, 0.0f, 0.0f, 1.0f);
	glTranslatef(-0.3f, -0.3f, 0.0f);

	glBegin(GL_QUADS);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.2f, 0.2f, 0.0f);
	glVertex3f(0.2f, 0.4f, 0.0f);
	glVertex3f(0.4f, 0.4f, 0.0f);
	glVertex3f(0.4f, 0.2f, 0.0f);
	glEnd();
	glPopMatrix();

	//Cuadrante abajo izquierda
	glPushMatrix();

	glTranslatef(-0.3f, -0.3f, 0.0f);
	glScalef(fEscala, fEscala, 0.0f);
	glTranslatef(0.3f, 0.3f, 0.0f);

	glBegin(GL_QUADS);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.2f, -0.2f, 0.0f);
	glVertex3f(-0.2f, -0.4f, 0.0f);
	glVertex3f(-0.4f, -0.4f, 0.0f);
	glVertex3f(-0.4f, -0.2f, 0.0f);
	glEnd();
	glPopMatrix();

	//Cuadrante abajo derecha
	glPushMatrix();

	glTranslatef(0.2f, -0.2f, 0.0f);
	cizallamientoX(fCizalla);
	glTranslatef(-0.2f, 0.2f, 0.0f);

	glBegin(GL_QUADS);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.2f, -0.2f, 0.0f);
	glVertex3f(0.4f, -0.2f, 0.0f);
	glVertex3f(0.4f, -0.4f, 0.0f);
	glVertex3f(0.2f, -0.4f, 0.0f);
	glEnd();
	glPopMatrix();

	glutSwapBuffers();
}

// Función que se ejecuta cuando el sistema no esta ocupado
void Idle(void)
{
	// Incrementamos el ángulo
	fAngulo += 0.865384615f;
	if (fAngulo > 360)
		fAngulo -= 360;

	if (fTraslacionX < -0.5f || fTraslacionX > 0.0f) {
		incTraslacion *= -1;
	}
	fTraslacionX += incTraslacion;

	if (fEscala > 2.02f || fEscala < 1.0f) {
		incEscalado *= -1;
	}
	fEscala += incEscalado;

	if (fCizalla > 0.0f || fCizalla < -1.02f) {
		incCizalla *= -1;
	}
	fCizalla += incCizalla;

	// Indicamos que es necesario repintar la pantalla
	glutPostRedisplay();
}

// Funci�n principal
int main(int argc, char** argv)
{
	// Inicializamos la librería GLUT
	glutInit(&argc, argv);

	// Indicamos como ha de ser la nueva ventana
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(W_WIDTH, W_HEIGHT);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

	// Creamos la nueva ventana
	glutCreateWindow("Etapa 2 primitivas");

	// Indicamos cuales son las funciones de redibujado e idle
	glutDisplayFunc(Display);
	glutIdleFunc(Idle);
	glutReshapeFunc(myResize);

	// El color de fondo será el negro (RGBA, RGB + Alpha channel)
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glOrtho(-1.0, 1.0f, -1.0, 1.0f, -1.0, 1.0f);

	// Comienza la ejecución del core de GLUT
	glutMainLoop();
	return 0;
}