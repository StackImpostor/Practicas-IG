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
float fcount = 0;

//Variables para los controles de la camara
const int LIBRE = 0;
const int ALZADO = 1;
const int PLANTA = 2;
const int PERFIL = 3;
const int CIRCULO = 4;
const float radius = 10.0f;
float camX = 3;
float camZ = 3;
float camY = 3;
float posX = 0;
float posY = 0;
float posZ = 0;

void InitWindow(GLfloat Width, GLfloat Height) {
	glViewport(0, 0, Width, Height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, Width / Height, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
}

//Movimiento objetos
float cubo = 1;
bool incremento = false;

float cone = 1;
bool loong = true;

float torus = 0;
bool trans = true;

//Transformaciones de los objetos
void rotacionCubo() {
	if (incremento) {
		cubo += 0.02;
		if (cubo >= 0.2) {
			incremento = false;
		}
	}
	else {
		cubo += -0.02;
		if (cubo <= -1.9) {
			incremento = true;
		}
	}
	glTranslatef(0.5 * cubo, 0, 0);
	glRotatef(90, 0, 1, 0);
	glRotatef(100 * cubo, 1, 0, 0);
}

void looongCone() {
	if (loong) {
		cone += 0.01;
		if (cone >= 3) {
			loong = false;
		}
	}
	else {
		cone += -0.01;
		if (cone <= 0.5) {
			loong = true;
		}
	}
	glScalef(1, 1, cone);
}

void destruirDonut() {
	if (trans) {
		torus += 0.008;
		if (torus >= 0.75) {
			trans = false;
		}
	}
	else {
		torus -= 0.008;
		if (torus <= -0.5) {
			trans = true;
		}
	}
	float shear[] = {
   1, torus, 0, 0,
   torus, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1 };
	glMultMatrixf(shear);
}



void myResize(int width, int height) {
	if (height == 0) height = 1; //Para no dividir por 0
	if (width == 0) width = 1;
	InitWindow(width, height);
}

void ControlesEspeciales(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		break;
	case GLUT_KEY_RIGHT:
		break;
	case GLUT_KEY_UP:
		break;
	case GLUT_KEY_DOWN:
		break;
	}
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
	/*He intentado hacer cosas con las luces*/
	glEnable(GL_LIGHTING);
	
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 5.0 };
	GLfloat light_position[] = { 2.0, 0.0, 0.0, 1.0 };
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glShadeModel(GL_SMOOTH);

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
	
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	
	GLfloat ambient[] = { 0.5, 0.5, 0.5, 1.00};
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	//GLfloat direction[] = { 0, 0, 1 };
	//glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, direction);
	GLfloat light_specular[] = { 10, 10, 10, 1.0 };
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	GLfloat light_diffuse[] = { 1, 1, 1, 1.0 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);


	glEnable(GL_LIGHT0);
	
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

	for (float i = -20; i <= 20; i += 0.25) {
		if (i - (int)i == 0) {
			glLineWidth(2);
		} else {
			glLineWidth(1);
		}
		
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
	glLineWidth(1);
	//Objetos
	
	//Teapod
	glPushMatrix();

	glTranslatef(-0.5, 0.0, -0.8);

	glRotatef(fAngulo * 5, 0.0f, 1.0f, 0.0f);
	glTranslatef(-0.40, 0.7, 0);
	glRotatef(30, 0.0f, 0.0f, 1.0f);
	//glTranslatef(1, 0, 1);

	glColor3f(0.5, 0.5, 0.5);
	glutSolidTeapot(0.4);
	//glColor3f(0, 0, 0);
	//glutWireTeapot(0.4);

	
	glPopMatrix();

	//theCube
	glPushMatrix();
	glTranslatef(-0.5, 0.25, 0.5);

	rotacionCubo();

	glColor3f(0.5, 0.5, 0.5);
	glutSolidCube(0.5);

	//glColor3f(0, 0, 0);
	//glutWireCube(0.5);

	glPopMatrix();

	//Torus
	glPushMatrix();

	glTranslatef(0.7, 0.15, -0.7);
	glRotatef(90, 1.0f, 0, 0);

	destruirDonut();

	glColor3f(0.5, 0.5, 0.5);
	glutSolidTorus(0.15, 0.3, 50, 50);

	//glColor3f(0, 0, 0);
	//glutWireTorus(0.15, 0.3, 50, 50);

	glPopMatrix();

	//Cone
	glPushMatrix();
	glTranslatef(0.5, 0, 0.5);
	glRotatef(-90, 1, 0, 0);

	looongCone();

	glColor3f(0.5, 0.5, 0.5);
	glutSolidCone(0.2, 1, 50, 50);

	//glColor3f(0, 0, 0);
	//glutWireCone(0.2, 1, 50, 50);

	glPopMatrix();


	//Dibujamos el "suelo" que es trans
	glBegin(GL_POLYGON);
	//glColor4f(0.5f, 0.5f, 1.0f, 0.5f);
	glColor3f(0.5f, 0.5f, 1.0f);
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

	camX = sin(fcount) * radius;
	camY = 3 * sin(3 * fcount);
	camZ = cos(fcount) * radius;
	
	glLoadIdentity();
	gluLookAt(camX, camY, camZ, posX, posY, posZ, 0, 1, 0);
	//std::cout << camX << ", " << camZ << "\n";
	fcount += 0.005f;
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

	//Codigo copiado para las luces
	/*GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat light_position[] = { 100.0, 100.0, 1.0, 0.0 };
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);*/

	// Creamos la nueva ventana
	glutCreateWindow("Mis cosas 3D");

	// Indicamos cuales son las funciones de redibujado e idle
	glutDisplayFunc(Display);
	glutIdleFunc(Idle);
	glutReshapeFunc(myResize);
	glutSpecialFunc(ControlesEspeciales);

	// El color de fondo ser� el negro (RGBA, RGB + Alpha channel)
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	//glOrtho(-1.0, 1.0f, -1.0, 1.0f, -100.0, 100.0f);
	
	
	/*glMatrixMode(GL_PROJECTION);
	//gluLookAt(0.5, 0.5, 0, 0, 0, 0, 0, 1, 0);
	glLoadIdentity();
	gluPerspective(30, p_width/p_height, 0.1, 100);
	//glFrustum(-0.05, 0.05, -0.05, 0.05, 0.2, 100);
	
	glMatrixMode(GL_MODELVIEW);*/
	InitWindow(W_WIDTH, W_HEIGHT);
	glLoadIdentity();
	gluLookAt(3, 3, 3, 0, 0, 0, 0, 1, 0);
	// Comienza la ejecuci�n del core de GLUT
	glutMainLoop();
	return 0;
}

