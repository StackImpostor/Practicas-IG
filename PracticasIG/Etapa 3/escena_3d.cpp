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
const double pi = 3.1415926535897;
GLfloat fAngulo; // Variable que indica el �ngulo de rotaci�n de los ejes. 
float p_width = W_WIDTH, p_height = W_HEIGHT;
float fcount = 0;
bool muestraReferencias = true;

//Light position
GLfloat light0_position[] = { 0.0, 1.0, 0.0, 1.0 };
bool shadeFlat = false;
bool light0 = true;
bool light1 = false;
bool light2 = false;
bool light3 = false;

bool normal = false;

//Variables para los controles de la camara
const int ALZADO = 1;
const int PLANTA = 2;
const int PERFIL = 3;
const int CIRCULO = 4;
const int LIBRE = 5;
const int ESFERICO = 6;

const float radius = 10.0f;
float radio2 = 5.0f;
int modo = LIBRE;
float zoomFactor = 1;
float anguloX = -pi/2;
float anguloY = -pi/4;
float anguloX2 = 0;
float anguloY2 = 0;
float camX = 4;
float camZ = 0;
float camY = 3;
float posX = 0;
float posY = 0;
float posZ = 0;

//Carcar modelos
bool modelos = false;
int mod = 1;

GLuint elephant;
float elephantrot;
char ch = '1';

void loadObj(char* fname)
{
	FILE* fp;
	int read;
	GLfloat x, y, z;
	char ch;

	elephant = glGenLists(1);
	fp = fopen(fname, "r");
	if (!fp)
	{
		printf("can't open file %s\n", fname);
		exit(1);
	}
	glPointSize(2.0);
	glNewList(elephant, GL_COMPILE);
	{
		glPushMatrix();
		glBegin(GL_POINTS);
		while (!(feof(fp)))
		{
			read = fscanf(fp, "%c %f %f %f", &ch, &x, &y, &z);
			
			if (read == 4 && ch == 'v')
			{
				glVertex3f( x,  y,  z);
			}
		
		}
		glEnd();
	}
	
	glPopMatrix();
	glEndList();
	fclose(fp);
}

void drawModelo()
{
	glPushMatrix();
	glTranslatef(0, 0.00, 0);
	if ( mod != 4 && mod != 3) {
		glRotatef(-90, 1, 0, 0);
		glScalef(0.01, 0.01, 0.01);
	}
	if (mod == 3) {
		//glRotatef(90, 1, 0, 0);
		glScalef(0.1, 0.1, 0.1);
	}
	glColor3f(1.0, 0.23, 0.27);
	
	glRotatef(elephantrot, 0, 1, 0);
	glCallList(elephant);
	glPopMatrix();
}

//END of cargar modelos

void preparaCamara() {
	glLoadIdentity();
	switch (modo) {
	case LIBRE:
		posX = camX + sin(anguloX) * cos(anguloY);
		posY = camY + sin(anguloY);
		posZ = camZ + -cos(anguloX) * cos(anguloY);

		gluLookAt(camX, camY, camZ, posX, posY, posZ, 0, 1, 0);
		break;
	case ALZADO:
		gluLookAt(0, 0, -4, 0, 0, 0, 0, 1, 0);
		break;
	case PLANTA:
		gluLookAt(0, 4, 0, 0, 0, 0, 1, 0, 0);
		break;
	case PERFIL:
		gluLookAt(-4, 0, 0, 0, 0, 0, 0, 1, 0);
		break;
	case CIRCULO:
		gluLookAt(sin(fcount) * radius, 3, cos(fcount) * radius, 0, 0, 0, 0, 1, 0);
		fcount += 0.005f;
		break;
	case ESFERICO:
		float x = radio2 * sin(anguloX2) * cos(anguloY2);
		float y = radio2 * sin(anguloY2);
		float z = radio2 * -cos(anguloX2) * cos(anguloY2);
		gluLookAt(x, y, z, 0, 0, 0, 0, 1, 0);
		break;
	}
}

void InitWindow(GLfloat Width, GLfloat Height) {
	glViewport(0, 0, Width, Height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	switch (modo) {
	case 4:
	case LIBRE:
	case 6:
		gluPerspective(45.0 / zoomFactor, Width / Height, 0.1, 100.0);
		break;
	case 1:
	case 2:
	case 3:
		glOrtho(-5, 5, -5, 5, -5, 5);
	}
	glMatrixMode(GL_MODELVIEW);
}

//Movimiento objetos
int cubo = 1;
bool incremento = true;
int i = 1;

float cone = 1;
bool loong = true;

float torus = 0;
bool trans = true;
float distancia = 0.7;
bool gusanete = true;

//Transformaciones de los objetos
void rotacionCubo() {
	
	if (incremento) {
		if (cubo == 90) {
			if (i == 5) {
				incremento = false;
			}
			else {
				i++;
				cubo = 0;
			}
		}
		else {
			cubo += 1;
		}
	}
	else {
		if (cubo  == 0) {
			if (i == 1) {
				incremento = true;
			}
			else {
				i--;
				cubo = 90;
			}
		}
		else {
			cubo += -1;
		}
	}
	glTranslatef(-0.25 - (i * 0.5), 0, 0.5);
	glRotatef(cubo, 0, 0, 1);
	glTranslatef(0.25, 0.25, 0);
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
		if (torus >= 0.0) {
			trans = false;
		}
		if (gusanete) {
			distancia += 0.005;
			if (distancia >= 2.9) {
				gusanete = false;
			}
		}
		else {
			distancia -= 0.005;
			if (distancia <= 0.7) {
				gusanete = true;
			}
		}
		
	}
	else {
		torus -= 0.008;
		if (torus <= -0.75) {
			trans = true;
		}
	}
	
	glTranslatef(distancia , 0.15,  -distancia);
	glRotatef(90, 1.0f, 0, 0);
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
	float incAngulo = pi / 60;
	int width, height;

	switch (key) {
	case GLUT_KEY_LEFT:
		if (modo == LIBRE) {
			float cZ = 0.1 * -cos(anguloX - pi/2);
			float cX = 0.1 * sin(anguloX - pi/2);

			camZ += cZ;
			posZ += cZ;
			camX += cX;
			posX += cX;
		}
		if (modo == ESFERICO) {
			anguloX2 += incAngulo;
		}
		break;
	case GLUT_KEY_RIGHT:
		if (modo == LIBRE) {
			float cZ = -0.1 * -cos(anguloX - pi/2);
			float cX = -0.1 * sin(anguloX - pi/2);

			camZ += cZ;
			posZ += cZ;
			camX += cX;
			posX += cX;
		}
		if (modo == ESFERICO) {
			anguloX2 -= incAngulo;
		}
		break;
	case GLUT_KEY_UP:
		if (modo == LIBRE) {
			float cZ = 0.1 * -cos(anguloX);
			float cX = 0.1 * sin(anguloX);

			camZ += cZ;
			posZ += cZ;
			camX += cX;
			posX += cX;
		}
		if (modo == ESFERICO && anguloY2 < pi/2 - incAngulo) {
			anguloY2 += incAngulo;
		}
		break;
	case GLUT_KEY_DOWN:
		if (modo == LIBRE) {
			float cZ = -0.1 * -cos(anguloX);
			float cX = -0.1 * sin(anguloX);

			camZ += cZ;
			posZ += cZ;
			camX += cX;
			posX += cX;
		}
		if (modo == ESFERICO && anguloY2 > -pi / 2 + incAngulo) {
			anguloY2 -= incAngulo;
		}
		break;
	case GLUT_KEY_F1:
	case GLUT_KEY_F2:
	case GLUT_KEY_F3:
	case GLUT_KEY_F4:
	case GLUT_KEY_F5:
	case GLUT_KEY_F6:
		modo = key;
		width = glutGet(GLUT_WINDOW_WIDTH);
		height = glutGet(GLUT_WINDOW_HEIGHT);
		InitWindow(width, height);
		break;
	case GLUT_KEY_INSERT:
		if (modo == LIBRE) {
			anguloX += -incAngulo;
		}
		break;
	case GLUT_KEY_PAGE_UP:
		if (modo == LIBRE) {
			anguloX += incAngulo;
		}
		break;
	case GLUT_KEY_HOME:
		if (modo == LIBRE) {
			if (anguloY + incAngulo <= pi / 2) 
				anguloY += incAngulo;
		}
		if (modo == ESFERICO && radio2 > 0) {
			radio2 -= 0.3;
		}
		break;
	case GLUT_KEY_END:
		if (modo == LIBRE) {
			if (anguloY - incAngulo >= -pi / 2)
				anguloY -= incAngulo;
		}
		if (modo == ESFERICO) {
			radio2 += 0.3;
		}
		break;
	case GLUT_KEY_PAGE_DOWN:
		zoomFactor += 0.1;
		width = glutGet(GLUT_WINDOW_WIDTH);
		height = glutGet(GLUT_WINDOW_HEIGHT);
		InitWindow(width, height);
		std::cout << zoomFactor << "\n";
		break;
	}

}

void ControlesTeclado(unsigned char key, int x, int y) {
	int width;
	int height;
	switch (key) {
	case 127:
		if(zoomFactor > 0.35)
			zoomFactor -= 0.1;
		width = glutGet(GLUT_WINDOW_WIDTH);
		height = glutGet(GLUT_WINDOW_HEIGHT);
		InitWindow(width, height);
		std::cout << zoomFactor << "\n";
		break;
	case 27:
		muestraReferencias = !muestraReferencias;
		break;
	case 97: //A +x
		light0_position[0] += 0.25;
		std::cout << "Posicion de la luz blanca: "<<light0_position[0] <<", "<< light0_position[1]<<", "<< light0_position[2] << "\n";
		break;
	case 122: //Z -x
		light0_position[0] += - 0.25;
		std::cout << "Posicion de la luz blanca: " << light0_position[0] << ", " << light0_position[1] << ", " << light0_position[2] << "\n";
		break;
	case 115: //S +y
		light0_position[1] += 0.25;
		std::cout << "Posicion de la luz blanca: " << light0_position[0] << ", " << light0_position[1] << ", " << light0_position[2] << "\n";
		break;
	case 120: //X -y
		light0_position[1] += -0.25;
		std::cout << "Posicion de la luz blanca: " << light0_position[0] << ", " << light0_position[1] << ", " << light0_position[2] << "\n";
		break;
	case 100: //D +z
		light0_position[2] += 0.25;
		std::cout << "Posicion de la luz blanca: " << light0_position[0] << ", " << light0_position[1] << ", " << light0_position[2] << "\n";
		break;
	case 99: //C -z
		light0_position[2] += -0.25;
		std::cout << "Posicion de la luz blanca: " << light0_position[0] << ", " << light0_position[1] << ", " << light0_position[2] << "\n";
		break;
	case 32: //spacebar
		if (!shadeFlat) {
			glShadeModel(GL_FLAT);
			shadeFlat = true;
		}
		else {
			glShadeModel(GL_SMOOTH);
			shadeFlat = false;
		}
		break;
	case 48:
		light0 = !light0;
		if (!light0) {
			glDisable(GL_LIGHT0);
		}
		else {
			glEnable(GL_LIGHT0);
		}
		break;
	case 49:
		light1 = !light1;
		if (!light1) {
			glDisable(GL_LIGHT1);
		}
		else {
			glEnable(GL_LIGHT1);
		}
		break;
	case 50:
		light2 = !light2;
		if (!light2) {
			glDisable(GL_LIGHT2);
		}
		else {
			glEnable(GL_LIGHT2);
		}
		break;

	case 51: 
		light3 = !light3;
		if (!light3) {
			glDisable(GL_LIGHT3);
		}
		else {
			glEnable(GL_LIGHT3);
		}
		//normal = !normal;
		break;
	case 109: //m
		modelos = !modelos;
		break;
	case 13: //enter
		mod += 1;
		switch (mod) {
		case 1:
			loadObj((char*)"modelos/pato.obj");
			break;
		case 2:
			loadObj((char*)"modelos/cat.obj");
			break;
		case 3:
			loadObj((char*)"modelos/honk.obj");
			break;
		case 4:
			loadObj((char*)"modelos/bugatti.obj");
			break;
		case 5:
			mod = 1;
			break;
		}
		break;
	}
	

	std::cout << "key: " << (int)key<<"\n";
}

// Funci�n que visualiza la escena OpenGL
void Display(void)
{
	// Borramos la escena
	glClear(GL_COLOR_BUFFER_BIT);

	glPushMatrix();

	//Activamos buffer de PROFUNDIAD
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	// Rotamos las proximas primitivas
	//glRotatef(-45, 1.0f, 0.0f, 0.0f);
	//glRotatef(fAngulo, 0.0f, 1.0f, 0.0f);
	//glRotatef(45, 0.0f, 1.0f, 0.0f);

	//Colores soras y cosas divertidas
	/*He intentado hacer cosas con las luces*/
	glEnable(GL_LIGHTING);

	//Light 0
	//GLfloat light_direction[] = {3.0, 3 ,3 };
	GLfloat ambient[] = { 0.5, 0.5, 0.5, 1.00};
	GLfloat light_specular[] = { 10, 10, 10, 1.0 };
	GLfloat light_diffuse[] = { 1.9, 1.9, 1.9, 1.0 };
	GLfloat light_atenuation[] = { 1, 1, 1 };
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	//glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_direction);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_LINEAR_ATTENUATION, light_atenuation);

	//Light 1
	GLfloat light_position1[] = { 3, 3, 3, 1 };
	GLfloat light_specular1[] = { 1.5, 1, 1, 1.0 };
	GLfloat light_diffuse1[] = { 10, 1, 1, 1.0 };
	GLfloat light_atenuation1[] = { 0.5, 0.5, 0.5 };
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
	glLightfv(GL_LIGHT1, GL_LINEAR_ATTENUATION, light_atenuation1);

	//Light 2
	GLfloat light_position2[] = { 0, 0.5, 1, 1 };
	GLfloat ambient2[] = { 0.0, 1, 0.0, 1.00 };
	GLfloat light_specular2[] = { 0.1, 0.5, 0.1, 1.0 };
	GLfloat light_diffuse2[] = { 0.1, 0.1, 0.1, 1.0 };
	GLfloat light_atenuation2[] = { 1, 1, 1 };
	glLightfv(GL_LIGHT2, GL_POSITION, light_position2);
	glLightfv(GL_LIGHT2, GL_AMBIENT, ambient2);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse2);
	glLightfv(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, light_atenuation2);

	//Light 3
	GLfloat light_position3[] = { -3, 0.5, 0, 1 };
	GLfloat ambient3[] = { 0.0, 0.0, 0.0, 1.00 };
	GLfloat light_specular3[] = { 0.5, 0.5, 1.5, 1.0 };
	GLfloat light_diffuse3[] = { 0.1, 0.1, 0.5, 1.0 };
	GLfloat light_atenuation3[] = { 1, 1, 1 };
	glLightfv(GL_LIGHT3, GL_POSITION, light_position3);
	glLightfv(GL_LIGHT3, GL_AMBIENT, ambient3);
	glLightfv(GL_LIGHT3, GL_SPECULAR, light_specular3);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, light_diffuse3);
	glLightfv(GL_LIGHT3, GL_CONSTANT_ATTENUATION, light_atenuation3);

	if (light0) {
		glEnable(GL_LIGHT0);
		glPushMatrix();
		glTranslatef(light0_position[0], light0_position[1], light0_position[2]);
		glColor3f(1, 1, 1.0f);
		glutWireSphere(0.05, 10, 10);
		glPopMatrix();
	}
	if (light1) {
		glPushMatrix();
		glTranslatef(light_position1[0], light_position1[1], light_position1[2]);
		glColor3f(1, 0, 0);
		glutWireSphere(0.05, 10, 10);
		glPopMatrix();
	}
	if (light2) {
		glPushMatrix();
		glTranslatef(light_position2[0], light_position2[1], light_position2[2]);
		glColor3f(0, 1, 0);
		glutWireSphere(0.05, 10, 10);
		glPopMatrix();
	}
	if (light3) {
		glPushMatrix();
		glTranslatef(light_position3[0], light_position3[1], light_position3[2]);
		glColor3f(0, 0, 1);
		glutWireSphere(0.05, 10, 10);
		glPopMatrix();
	}
	/*glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);*/


	//Ejes de cordenadas
	if (muestraReferencias) {
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
			if(i != 0){
				if (i - (int)i == 0) {
					glLineWidth(2);
				} else {
					glLineWidth(1);
				}
			
				glBegin(GL_LINES);
				glColor3f(0, 0, 0);
				glVertex3f(i, 0.0, -20);
				glVertex3f(i, 0.0, 20);
				glEnd();
				glBegin(GL_LINES);
				glColor3f(0, 0, 0);
				glVertex3f(-20, 0.0, i);
				glVertex3f(20, 0.0, i);
				glEnd();
			}
		}
	}
	
	glLineWidth(1);
	//Objetos
	
	//Materiales
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	

	float MatAmbient[] = { 2.1f, 2.1f, 2.1f, 1.0f };
	float MatDiffuse[] = { 5.0f, 5.0f, 5.0f, 1.0f };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 128.0 };

	//glClearColor(0.0, 0.0, 0.0, 0.0);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MatAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MatDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

	
	
	//Teapot
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

	rotacionCubo();

	glColor3f(0.5, 0.5, 0.5);

	glutSolidCube(0.5);

	//glColor3f(0, 0, 0);
	//glutWireCube(0.5);

	glPopMatrix();
	
	//Torus
	glPushMatrix();
	
	destruirDonut();
	
	//glTranslatef(0.7, 0.15, -0.7);
	

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
	/*
	glBegin(GL_POLYGON);
	glColor3f(0.5f, 0.5f, 1.0f);
	if (normal) {
		glNormal3f(0, 1, 0);
	}
	glVertex3f(-2, 1.01, -2);
	if (normal) {
		glNormal3f(0, 1, 0);
	}
	glVertex3f(2, 1.01, -2);
	if (normal) {
		glNormal3f(0, 1, 0);
	}
	glVertex3f(2, 1.01, 2);
	if (normal) {
		glNormal3f(0, 1, 0);
		std::cout << "normal: "<< normal <<"\n";
	}
	glVertex3f(-2, 1.01, 2);
	glEnd();*/

	//Dibujamos el "suelo" que es trans
	if(muestraReferencias){
		/*glPushMatrix();
		glTranslatef(0, -5, 0);
		//glRectf();
		glColor4f(0.5f, 0.5f, 1.0f, 0.5f);
		glutSolidCube(1);*/
		glBegin(GL_POLYGON);
		glColor4f(0.5f, 0.5f, 2.0f, 0.5f);
		//glColor3f(0.5f, 0.5f, 1.0f);
		glVertex3f(-20, -0.01, -20);
		glVertex3f(20, -0.01, -20);
		glVertex3f(20, -0.01, 20);
		glVertex3f(-20, -0.01, 20);
		glNormal3d(0, 1, 0);
		glEnd();
	}

	if (modelos) {
		//dibuja el modelo cargado
		drawModelo();
	}
	

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

	//camX = sin(fcount) * radius;
	//camY = 3 * sin(3 * fcount);
	//camZ = cos(fcount) * radius;
	
	preparaCamara();

	// Indicamos que es necesario repintar la pantalla
	glutPostRedisplay();
}

// Funci�n principal
int main(int argc, char** argv)
{
	// Inicializamos la librer�a GLUT
	glutInit(&argc, argv);

	glShadeModel(GL_SMOOTH);
	
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
	glutKeyboardFunc(ControlesTeclado);

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
	preparaCamara();
	
	//SUBSTITUIR EL FICHERO PARA CARGAR DIFERENTES MODELOS
	loadObj((char*)"modelos/pato.obj");

	// Comienza la ejecuci�n del core de GLUT
	glutMainLoop();
	return 0;
}

