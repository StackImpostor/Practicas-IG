// Etapa6.cpp
// Fichero principal 
//Bartomeu Ramis
//David Cantero
//Joan Jorquera
////////////////////////////////////////////////////
#include <Windows.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <vector>
#include <tuple>
#include "SOIL2/SOIL2.h"

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

bool teclas[6] = { false, false, false, false, false, false }; // W A S D Q E
const float radius = 10.0f;
float radio2 = 5.0f;
int modo = LIBRE;
int cx, cy; //X e Y del cursor
float zoomFactor = 1;
float anguloX = -pi / 2;
float anguloY = -pi / 4;
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

GLuint img;
GLuint elephant;
float elephantrot;
char ch = '1';
bool modelo_normal = false;

using namespace std;


class Poligono {
public:
	float puntos[4][3];
	void setPunto(int p, float x, float y, float z) {
		puntos[p][0] = x;
		puntos[p][1] = y;
		puntos[p][2] = z;
	}
};



tuple<int, vector<Poligono>, vector<Poligono>, vector<Poligono> > loadObj(char* fname)
{

	GLuint id[1] = { 1 };

	FILE* fp;
	int read;
	GLfloat x, y, z;

	char ch[100];
	float vertices[25000][3];
	float normal[25000][3];
	float textura[25000][2];

	elephant = glGenLists(1);
	fp = fopen(fname, "r");
	if (!fp)
	{
		printf("can't open file %s\n", fname);
		exit(1);
	}
	glPointSize(2.0);
	glNewList(elephant, GL_COMPILE);
	
	glPushMatrix();



	glBindTexture(GL_TEXTURE_2D, img);

	//glBegin(GL_POINTS);
	int vcount = 0;
	int ncount = 0;
	int tcount = 0;
	int numVert = 0;
	vector<Poligono> vertexIndices, normalIndices, texIndices;
	while (!(feof(fp)))
	{
		//read = fscanf(fp, "%c %f %f %f", &ch, &x, &y, &z);
		read = fscanf(fp, "%s ", &ch);

		if (ch[0] == 'v') {
			read = fscanf(fp, "%f %f %f", &x, &y, &z);
			switch (ch[1]) {
			case '\0':
					
				vertices[vcount][0] = x;
				vertices[vcount][1] = y;
				vertices[vcount][2] = z;
				vcount++;
				break;
			case 'n':
				normal[ncount][0] = x;
				normal[ncount][1] = y;
				normal[ncount][2] = z;
				ncount++;
				break;

			case 't':
				textura[tcount][0] = x;
				textura[tcount][1] = y;
				tcount++;
				break;
			}
		}

			else if (ch[0] == 'f') {
				//glTexCoord2f(textura[vt - 1][0], textura[vt - 1][1]);
				
				int v, vt, vn;
				//glBegin(GL_POLYGON);
				Poligono pVert, pNorm, pTex;
				read = fscanf(fp, "%i/%i/%i ", &v, &vt, &vn);

				pVert.setPunto(0, vertices[v - 1][0], vertices[v - 1][1], vertices[v - 1][2]);
				pNorm.setPunto(0, normal[vn - 1][0], normal[vn - 1][1], normal[vn - 1][2]);
				pTex.setPunto(0, textura[vt - 1][0], textura[vt - 1][1],0);

				read = fscanf(fp, "%i/%i/%i ", &v, &vt, &vn);

				pVert.setPunto(1, vertices[v - 1][0], vertices[v - 1][1], vertices[v - 1][2]);
				pNorm.setPunto(1, normal[vn - 1][0], normal[vn - 1][1], normal[vn - 1][2]);
				pTex.setPunto(1, textura[vt - 1][0], textura[vt - 1][1],0);

				read = fscanf(fp, "%i/%i/%i ", &v, &vt, &vn);

				pVert.setPunto(2, vertices[v - 1][0], vertices[v - 1][1], vertices[v - 1][2]);
				pNorm.setPunto(2, normal[vn - 1][0], normal[vn - 1][1], normal[vn - 1][2]);
				pTex.setPunto(2, textura[vt - 1][0], textura[vt - 1][1],0);

				read = fscanf(fp, "%i/%i/%i ", &v, &vt, &vn);

				pVert.setPunto(3, vertices[v - 1][0], vertices[v - 1][1], vertices[v - 1][2]);
				pNorm.setPunto(3, normal[vn - 1][0], normal[vn - 1][1], normal[vn - 1][2]);
				pTex.setPunto(3, textura[vt - 1][0], textura[vt - 1][1],0);

				vertexIndices.push_back(pVert);
				normalIndices.push_back(pNorm);
				texIndices.push_back(pTex);

				numVert++;
			}

			
		//glEnd();
	}
	
	glPopMatrix();
	glEndList();
	fclose(fp);
	return make_tuple(numVert, vertexIndices, texIndices, normalIndices );
}

class Modelo {
public:
	int numVert = 0;
	char* fname;
	vector< Poligono > vertexIndices, texIndices, normalIndices;
	Modelo(char* filename) {
		fname = filename;
		tie(numVert, vertexIndices, texIndices, normalIndices) = loadObj(fname);
	}
};

Modelo HONK((char*)"modelos/honk.obj");

void foto() {
	glPushMatrix();

	GLuint img = SOIL_load_OGL_texture
	(
		"modelos\\Honk\\Face.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_MULTIPLY_ALPHA
	);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, img);

	glBegin(GL_POLYGON);

	glTexCoord2f(0, 0);
	glVertex3f(0,0,0);

	glTexCoord2f(0, 1);
	glVertex3f(0, 1, 0);

	glTexCoord2f(1, 1);
	glVertex3f(1, 1, 0);

	glTexCoord2f(1, 0);
	glVertex3f(1, 0, 0);



	glEnd();

	glPopMatrix();
}

void drawModelo(Modelo modelo)
{
	glPushMatrix();
	glColor3f(1.0f,1.0f,1.0f);
	glBindTexture(GL_TEXTURE_2D, img);

	vector<Poligono> vI = modelo.vertexIndices;
	vector<Poligono> vN = modelo.normalIndices;
	vector<Poligono> vT = modelo.texIndices;
	vector<Poligono>::iterator itNor = vN.begin();
	std::vector<Poligono>::iterator itTex = vT.begin();
	for (std::vector<Poligono>::iterator itVec = vI.begin(); itVec != vI.end(); ++itVec, ++itNor, ++itTex) {
		
		glBegin(GL_POLYGON);
		Poligono pV = *itVec;
		Poligono pN = *itNor;
		Poligono pT = *itTex;


		glTexCoord2f(pT.puntos[0][0], pT.puntos[0][1]);
 		glNormal3f(pN.puntos[0][0], pN.puntos[0][1], pN.puntos[0][2]);
		glVertex3f(pV.puntos[0][0], pV.puntos[0][1], pV.puntos[0][2]);

		glTexCoord2f(pT.puntos[1][0], pT.puntos[1][1]);
		glNormal3f(pN.puntos[1][0], pN.puntos[1][1], pN.puntos[1][2]);
		glVertex3f(pV.puntos[1][0], pV.puntos[1][1], pV.puntos[1][2]);

		glTexCoord2f(pT.puntos[2][0], pT.puntos[2][1]);
		glNormal3f(pN.puntos[2][0], pN.puntos[2][1], pN.puntos[2][2]);
		glVertex3f(pV.puntos[2][0], pV.puntos[2][1], pV.puntos[2][2]);

		glTexCoord2f(pT.puntos[3][0], pT.puntos[3][1]);
		glNormal3f(pN.puntos[3][0], pN.puntos[3][1], pN.puntos[3][2]);
		glVertex3f(pV.puntos[3][0], pV.puntos[3][1], pV.puntos[3][2]);

		glEnd();
	}

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

void mueveCamara() {
	float incAngulo = pi / 60;
	float speed = 0.2;
	if (teclas[0]) {
		if (modo == LIBRE) {
			float cZ = speed * -cos(anguloX);
			float cX = speed * sin(anguloX);

			camZ += cZ;
			posZ += cZ;
			camX += cX;
			posX += cX;
		}
		if (modo == ESFERICO && anguloY2 < pi / 2 - incAngulo) {
			anguloY2 += incAngulo;
		}
	}
	if (teclas[1]) {
		if (modo == LIBRE) {
			float cZ = speed * -cos(anguloX - pi / 2);
			float cX = speed * sin(anguloX - pi / 2);

			camZ += cZ;
			posZ += cZ;
			camX += cX;
			posX += cX;
		}
		if (modo == ESFERICO) {
			anguloX2 += incAngulo;
		}
	}
	if (teclas[2]) {
		if (modo == LIBRE) {
			float cZ = -speed * -cos(anguloX);
			float cX = -speed * sin(anguloX);

			camZ += cZ;
			posZ += cZ;
			camX += cX;
			posX += cX;
		}
		if (modo == ESFERICO && anguloY2 > -pi / 2 + incAngulo) {
			anguloY2 -= incAngulo;
		}
	}
	if (teclas[3]) {
		if (modo == LIBRE) {
			float cZ = -speed * -cos(anguloX - pi / 2);
			float cX = -speed * sin(anguloX - pi / 2);

			camZ += cZ;
			posZ += cZ;
			camX += cX;
			posX += cX;
		}
		if (modo == ESFERICO) {
			anguloX2 -= incAngulo;
		}
	}
	if (teclas[4]) {
		if (modo == LIBRE) {
			float cY = -speed;

			camY += cY;
			posY += cY;
		}
	}
	if (teclas[5]) {
		if (modo == LIBRE) {
			float cY = speed;
			camY += cY;
			posY += cY;
		}
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

void myResize(int width, int height) {
	if (height == 0) height = 1; //Para no dividir por 0
	if (width == 0) width = 1;
	InitWindow(width, height);
}

void ControlesEspeciales(int key, int x, int y) {
	float incAngulo = pi / 60;
	int width, height;

	switch (key) {
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

void liberaTeclas(unsigned char key, int x, int y) {
	//std::cout << "Liberada key: " << (int)key << "\n";
	switch (key) {
	case 119: // W
		teclas[0] = false;
		break;
	case 97:  // A
		teclas[1] = false;
		break;
	case 115: // S
		teclas[2] = false;
		break;
	case 100: // D
		teclas[3] = false;
		break;
	case 113: // Q
		teclas[4] = false;
		break;
	case 101: // E
		teclas[5] = false;
		break;
	}
}

void ControlesTeclado(unsigned char key, int x, int y) {
	float incAngulo = pi / 60;
	int width;
	int height;
	switch (key) {
	case 127:
		if (zoomFactor > 0.35)
			zoomFactor -= 0.1;
		width = glutGet(GLUT_WINDOW_WIDTH);
		height = glutGet(GLUT_WINDOW_HEIGHT);
		InitWindow(width, height);
		std::cout << zoomFactor << "\n";
		break;
	case 27:
		muestraReferencias = !muestraReferencias;
		break;
	case 119: // W
		teclas[0] = true;
		break;
	case 97:  // A
		teclas[1] = true;
		break;
	case 115: // S
		teclas[2] = true;
		break;
	case 100: // D
		teclas[3] = true;
		break;
	case 113: // Q
		teclas[4] = true;
		break;
	case 101: // E
		teclas[5] = true;
		break;
	case 122: //Z -x
		light0_position[0] += -0.25;
		std::cout << "Posicion de la luz blanca: " << light0_position[0] << ", " << light0_position[1] << ", " << light0_position[2] << "\n";
		break;
	case 120: //X -y
		light0_position[1] += -0.25;
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
	case 110: //n
		modelo_normal = !modelo_normal;
		break;
	case 13: //enter
		mod += 1;
		if (mod > 3) {
			mod = 1;
		}
		switch (mod) {
		case 1:
			//loadObj((char*)"modelos/pato.obj");
			break;
		case 2:
			//loadObj((char*)"modelos/cat.obj");
			break;
		case 3:
			//loadObj((char*)"modelos/honk.obj");
			break;
		}
		break;
	}


	std::cout << "key: " << (int)key << "\n";
}

void controlesRueda(int button, int state, int x, int y) {
	int width, height;
	if (button == 0 && state == 0) {
		cx = x;
		cy = y;
	}
	if (button == 3 && state == 0) {
		zoomFactor += 0.1;
		width = glutGet(GLUT_WINDOW_WIDTH);
		height = glutGet(GLUT_WINDOW_HEIGHT);
		InitWindow(width, height);
	}
	if (button == 4 && state == 0) {
		if (zoomFactor > 0.35)
			zoomFactor -= 0.1;
		width = glutGet(GLUT_WINDOW_WIDTH);
		height = glutGet(GLUT_WINDOW_HEIGHT);
		InitWindow(width, height);
	}
}

void controlesRaton(int x, int y) {
	if (modo == LIBRE) {
		float incAngulo = pi / 60;
		int dx, dy;
		float incX, incY;
		dx = cx - x;
		dy = cy - y;
		cx = x;
		cy = y;
		incX = -0.1 * dx * pi / 60;
		incY = 0.1 * dy * pi / 60;
		anguloX += incX;
		if (anguloY + incY >= -pi / 2 && anguloY + incY <= pi / 2) {
			anguloY += incY;
		}
		std::cout << "anguloY: " << anguloY << "\n";
	}
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
	GLfloat ambient[] = { 0.5, 0.5, 0.5, 1.00 };
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

	//Ejes de cordenadas
	if (muestraReferencias) {
		glLineWidth(3);

		glBegin(GL_LINES);
		glColor3f(1.0f, 0, 0);
		glVertex3f(-100, 0, 0);
		glVertex3f(100, 0, 0);
		glEnd();

		glBegin(GL_LINES);
		glColor3f(0, 1.0f, 0);
		glVertex3f(0, -100, 0);
		glVertex3f(0, 100, 0);
		glEnd();

		glBegin(GL_LINES);
		glColor3f(0, 0, 1.0f);
		glVertex3f(0, 0, -100);
		glVertex3f(0, 0, 100);
		glEnd();

		for (float i = -100; i <= 100; i++) {
			if (i != 0) {
				if (i - (int)i == 0) {
					glLineWidth(2);
				}
				else {
					glLineWidth(1);
				}

				glBegin(GL_LINES);
				glColor3f(0, 0, 0);
				glVertex3f(i, 0.0, -100);
				glVertex3f(i, 0.0, 100);
				glEnd();
				glBegin(GL_LINES);
				glColor3f(0, 0, 0);
				glVertex3f(-100, 0.0, i);
				glVertex3f(100, 0.0, i);
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



	

	//Dibujamos el "suelo" que es trans
	if (muestraReferencias) {
		/*glPushMatrix();
		glTranslatef(0, -5, 0);
		//glRectf();
		glColor4f(0.5f, 0.5f, 1.0f, 0.5f);
		glutSolidCube(1);*/
		glBegin(GL_POLYGON);
		glColor4f(0.5f, 0.5f, 2.0f, 0.5f);
		//glColor3f(0.5f, 0.5f, 1.0f);
		glVertex3f(-100, -0.01, -100);
		glVertex3f(100, -0.01, -100);
		glVertex3f(100, -0.01, 100);
		glVertex3f(-100, -0.01, 100);
		glNormal3d(0, 1, 0);
		glEnd();
	}

	if (false) {
		float MatAmbient[] = { 1.1f, 1.1f, 1.1f, 0.0f };
		float MatDiffuse[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 0.0 };
		GLfloat mat_shininess[] = { 1.0 };

		glMaterialfv(GL_FRONT, GL_AMBIENT, MatAmbient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, MatDiffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
		//dibuja el modelo cargado
	}

	foto();

	//Modelos de la escena
	//Mesa

	drawModelo(HONK);


	//silla
	/*glPushMatrix();
	glScalef( 0.17, 0.17, 0.17);
	glRotatef(90, 0, 0, 1); 
	glTranslatef(-10.1, -150, 0);
	glPopMatrix();*/
	//plato (nope, este modelo es gigante o algo, laggea que flipas)
	/*loadObj((char*)"models/Plate/plate.obj");
	glPushMatrix();
	glScalef(0.005, 0.005, 0.005);
	glRotatef(90, 0, 0, 1);
	glTranslatef(0, 0, 0);
	drawModelo();
	glPopMatrix();*/
	//botella (tambien da problemas a la hora de cargar)
	/*loadObj((char*)"models/bottle/bottle_mid.obj");
	glPushMatrix();
	glScalef(0.17, 0.17, 0.17);
	glRotatef(90, 0, 0.1, 1); //"levantada y un poco torcida, para que no quede paralela a la mesa
	glTranslatef(-10.1, -150, 0);
	drawModelo();
	glPopMatrix();*/

	//fork
	/*loadObj((char*)"models/Fork/Fork.obj");
	glPushMatrix();
	glScalef(1,1,1);
	glRotatef(90, 0, 0.1, 1);
	glTranslatef(-10.1, -150, 0);
	drawModelo();
	glPopMatrix();*/

	//tassó (no funcona?)
	/*glPushMatrix();
	glScalef(10, 10, 10);
	glRotatef(0, 0, 0, 0);
	glTranslatef(0, 0, 0);
	glPopMatrix();*/

	//cuchillo
	/*glPushMatrix();
	glScalef(0.01, 0.01, 0.01);
	glRotatef(0, 0, 0, 0);
	glTranslatef(0, 0, 0);
	glPopMatrix();*/

	glPopMatrix();
	glutSwapBuffers();
}


void initTex() {
	img = SOIL_load_OGL_texture
	(
		"modelos/Honk/Face.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_MULTIPLY_ALPHA
	);
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

	mueveCamara();
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
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE);

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
	glutCreateWindow("Etapa 6 - Texturas! :O");

	// Indicamos cuales son las funciones de redibujado e idle
	glutDisplayFunc(Display);
	glutIdleFunc(Idle);
	glutReshapeFunc(myResize);
	glutSpecialFunc(ControlesEspeciales);
	glutKeyboardFunc(ControlesTeclado);
	glutKeyboardUpFunc(liberaTeclas);
	glutMouseFunc(controlesRueda);
	glutMotionFunc(controlesRaton);


	// El color de fondo ser� el negro (RGBA, RGB + Alpha channel)
	glClearColor(0, 0, 0, 0);
	//glOrtho(-1.0, 1.0f, -1.0, 1.0f, -100.0, 100.0f);

	InitWindow(W_WIDTH, W_HEIGHT);
	preparaCamara();

	//SUBSTITUIR EL FICHERO PARA CARGAR DIFERENTES MODELOS
	initTex();
	//loadObj((char*)"modelos/pato.obj");

	
	// Comienza la ejecuci�n del core de GLUT
	glutMainLoop();
	return 0;
}

