﻿// Etapa6.cpp
// Fichero principal 
//Bartomeu Ramis
//David Cantero
//Joan Jorquera
////////////////////////////////////////////////////
#include <Windows.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/GLU.h>
#include <iostream>
#include <vector>
#include <tuple>
#include "SOIL2/SOIL2.h"
using namespace std;
const int W_WIDTH = 1280; // Tama�o incial de la ventana
const int W_HEIGHT = 720;
const float pi = 3.141592f;
float p_width = W_WIDTH, p_height = W_HEIGHT;
float fcount = 0;

//Light 0
//Light position
GLfloat light0_position[] = { 0.0, 10.0, 0.0, 1.0 };
GLfloat ambient[] = { 0.1f, 0.1f, 0.1f, 1.00 };
GLfloat light_specular[] = { 0.0f, 0.0f, 0.0f, 1.0 };
GLfloat light_diffuse[] = {30.0f, 30.0f,30.0f, 1.0 };
GLfloat light_atenuation[] = { 1.0f, 1.0f, 1.0f };

//Light 1
GLfloat light_position1[] = { 3, 10, 3, 1 };
GLfloat light_specular1[] = { 1.5, 1, 1, 1.0 };
GLfloat light_diffuse1[] = { 10, 1, 1, 1.0 };
GLfloat light_atenuation1[] = { 0.5, 0.5, 0.5 };
//Light 2
GLfloat light_position2[] = { 0, 10, 1, 1 };
GLfloat ambient2[] = { 0.0, 1, 0.0, 1.00 };
GLfloat light_specular2[] = { 0.1, 0.5, 0.1, 1.0 };
GLfloat light_diffuse2[] = { 0.1, 0.1, 0.1, 1.0 };
GLfloat light_atenuation2[] = { 1, 1, 1 };
//Light 3
GLfloat light_position3[] = { -3, 10, 0, 1 };
GLfloat ambient3[] = { 0.0, 0.0, 0.0, 1.00 };
GLfloat light_specular3[] = { 0.5, 0.5, 1.5, 1.0 };
GLfloat light_diffuse3[] = { 0.1, 0.1, 0.5, 1.0 };
GLfloat light_atenuation3[] = { 1, 1, 1 };

bool shadeFlat = false;
bool light0 = true;
bool light1 = false;
bool light2 = false;
bool light3 = false;

//FOG
const GLfloat fogColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat fogDensity = 0.0f;

//Variables para los controles de la camara
const int POV = 1;
const int PLANTA = 2;
const int ESQUINA = 3;
const int CIRCULO = 4;
const int LIBRE = 5;
const int CINEMATICA = 6;

bool teclas[6] = { false, false, false, false, false, false }; // W A S D Q E
const float radius = 10.0f;
int modo = LIBRE;
int cx, cy; //X e Y del cursor
float zoomFactor = 1;
float anguloX = 0;
float anguloY = -pi / 8;
float camX = 0;
float camZ = 10;
float camY = 12;
float posX = 0;
float posY = 0;
float posZ = 0;
float cineX = 3;
float cineAng = -pi / 2;
float incAngulo = pi / 60;

//Texturas;
int pared, suelo;
vector<GLuint> texturas;
int numTexturas;

bool NURBS = false;


class Poligono {
public:
	float puntos[4][3];
	void setPunto(int p, float x, float y, float z) {
		puntos[p][0] = x;
		puntos[p][1] = y;
		puntos[p][2] = z;
	}
};

int initTex(string img) {
	texturas.push_back(SOIL_load_OGL_texture(img.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_MULTIPLY_ALPHA));
	numTexturas++;
	return numTexturas - 1;
}

class Material {
public:
	string name;
	int imagen;
	float Ka[3];
	float Kd[3];
	float Ks[3];
	GLfloat Ns, Ni, d;

	Material(char* name, int imagen, float Ka[3], float Kd[3], float Ks[3], float Ns, float Ni, float d) {
		this->name = string(name);
		this->imagen = imagen;
		this->Ka[0] = Ka[0]; this->Ka[1] = Ka[1]; this->Ka[2] = Ka[2];
		this->Kd[0] = Kd[0]; this->Kd[1] = Kd[1]; this->Kd[2] = Kd[2];
		this->Ks[0] = Ks[0]; this->Ks[1] = Ks[1]; this->Ks[2] = Ks[2];
		this->Ns = Ns;
		this->Ni = Ni;
		this->d = d;
	}
};

vector< Material > loadMaterial(char* path, char* fname) {
	FILE* fp;
	int read;
	GLfloat x, y, z;
	vector< Material > materiales;
	char name[2000];
	char imagen[2000] = "";
	float Ka[3];
	float Kd[3];
	float Ks[3];
	float Ns = 128.0f, Ni = 1.0f, d = 1.0f;
	char ch[100];
	fp = fopen(((string)path + fname + ".mtl").c_str(), "r");
	if (!fp)
	{
		printf("no se ha cargado el material %s\n", fname);
		return materiales;

	}
	bool primero = true;

	while (!(feof(fp)))
	{
		read = fscanf(fp, "%s ", &ch);

		if (ch[0] == 'K') {
			read = fscanf(fp, "%f %f %f", &x, &y, &z);
			switch (ch[1]) {
			case 'a':
				Ka[0] = x;
				Ka[1] = y;
				Ka[2] = z;
				break;
			case 'd':
				Kd[0] = x;
				Kd[1] = y;
				Kd[2] = z;
				break;
			case 's':
				Ks[0] = x;
				Ks[1] = y;
				Ks[2] = z;
				break;
			}
		}
		else if (ch[0] == 'N') {
			read = fscanf(fp, "%f", &x);
			if (ch[1] == 's') {
				Ns = x;
			}
			else {
				Ni = x;
			}
		}
		else if (ch[0] == 'd') {
			read = fscanf(fp, "%f", &d);
		}
		else if (ch[0] == 'n') {
			if (!primero) {
				string img = ((string)path + imagen);
				int a = initTex(img);
				materiales.push_back(Material(name, a, Ka, Kd, Ks, Ns, Ni, d));

			}
			read = fscanf(fp, "%[^\n]", &name);
			primero = false;
		}
		else if (ch[0] == 'm') {
			read = fscanf(fp, "%[^\n]", &imagen);

		}
	}
	string img = ((string)path + imagen);
	int a = initTex(img);
	materiales.push_back(Material(name, a, Ka, Kd, Ks, Ns, Ni, d));
	return materiales;
}

tuple<vector<Poligono>, vector<Poligono>, vector<Poligono>, vector<Material>, vector < int >, vector < string > >
loadObj(char* path, char* fname)
{

	GLuint id[1] = { 1 };
	float* aux;
	FILE* fp;
	int read;
	GLfloat x, y, z;

	char ch[100];
	char mat[100];
	vector <float*> vertices;
	vector <float*> normal;
	vector <float*> textura;


	fp = fopen(((string)path + fname + ".obj").c_str(), "r");
	if (!fp)
	{
		printf("can't open file %s\n", fname);
		exit(1);
	}


	int vcount = 0;
	int ncount = 0;
	int tcount = 0;
	int numPol = 0;

	vector<Poligono> vertexIndices, normalIndices, texIndices;
	vector<Material> materiales = loadMaterial(path, fname);
	vector < int > iPoligonos;
	vector < string > nMateriales;
	while (!(feof(fp)))
	{
		read = fscanf(fp, "%s ", &ch);

		if (ch[0] == 'v') {
			read = fscanf(fp, "%f %f %f", &x, &y, &z);
			switch (ch[1]) {
			case '\0':
				aux = (float*)malloc(3 * sizeof(float));
				aux[0] = x;
				aux[1] = y;
				aux[2] = z;
				vertices.push_back(aux);
				vcount++;
				break;
			case 'n':
				aux = (float*)malloc(3 * sizeof(float));
				aux[0] = x;
				aux[1] = y;
				aux[2] = z;
				normal.push_back(aux);
				ncount++;
				break;

			case 't':
				aux = (float*)malloc(2 * sizeof(float));
				aux[0] = x;
				aux[1] = y;
				textura.push_back(aux);
				tcount++;
				break;
			}
		}
		else if (ch[0] == 'f') {


			int v, vt, vn;
			Poligono pVert;
			Poligono pNorm;
			Poligono pTex;
			read = fscanf(fp, "%i/%i/%i ", &v, &vt, &vn);

			pVert.setPunto(0, vertices[v - 1][0], vertices[v - 1][1], vertices[v - 1][2]);
			if (vn <= 0) {
				pNorm.setPunto(0, NULL, NULL, NULL);
			}
			else {
				pNorm.setPunto(0, normal[vn - 1][0], normal[vn - 1][1], normal[vn - 1][2]);
			}
			if (vt <= 0) {
				pTex.setPunto(0, NULL,NULL,NULL);
			}
			else {
				pTex.setPunto(0, textura[vt - 1][0], textura[vt - 1][1], 0);
			}

			read = fscanf(fp, "%i/%i/%i ", &v, &vt, &vn);

			pVert.setPunto(1, vertices[v - 1][0], vertices[v - 1][1], vertices[v - 1][2]);
			if (vn <= 0) {
				pNorm.setPunto(1, NULL, NULL, NULL);
			}
			else {
				pNorm.setPunto(1, normal[vn - 1][0], normal[vn - 1][1], normal[vn - 1][2]);
			}
			if (vt <= 0) {
				pTex.setPunto(1, NULL, NULL, NULL);
			}
			else {
				pTex.setPunto(1, textura[vt - 1][0], textura[vt - 1][1], 0);
			}

			read = fscanf(fp, "%i/%i/%i ", &v, &vt, &vn);

			pVert.setPunto(2, vertices[v - 1][0], vertices[v - 1][1], vertices[v - 1][2]);
			if (vn <= 0) {
				pNorm.setPunto(2, NULL, NULL, NULL);
			}
			else {
				pNorm.setPunto(2, normal[vn - 1][0], normal[vn - 1][1], normal[vn - 1][2]);
			}
			if (vt <= 0) {
				pTex.setPunto(2, NULL, NULL, NULL);
			}
			else {
				pTex.setPunto(2, textura[vt - 1][0], textura[vt - 1][1], 0);
			}

			read = fscanf(fp, "%i/%i/%i ", &v, &vt, &vn);

			pVert.setPunto(3, vertices[v - 1][0], vertices[v - 1][1], vertices[v - 1][2]);
			if (vn <= 0) {
				pNorm.setPunto(3, NULL, NULL, NULL);
			}
			else {
				pNorm.setPunto(3, normal[vn - 1][0], normal[vn - 1][1], normal[vn - 1][2]);
			}

			if (vt <= 0) {
				pTex.setPunto(3, NULL, NULL, NULL);
			}
			else {
				pTex.setPunto(3, textura[vt - 1][0], textura[vt - 1][1], 0);
			}

			vertexIndices.push_back(pVert);
			normalIndices.push_back(pNorm);
			texIndices.push_back(pTex);

			numPol++;
		}
		else if (ch[0] == 'u') {
			read = fscanf(fp, "%[^\n]", &mat);
			iPoligonos.push_back(numPol);
			nMateriales.push_back(mat);
		}
	}

	fclose(fp);
	return make_tuple(vertexIndices, texIndices, normalIndices, materiales, iPoligonos, nMateriales);
}

class Modelo {
public:
	char* fname;
	vector< Poligono > vertexIndices, texIndices, normalIndices;
	vector< Material > materiales;
	vector< int > iPoligonos;
	vector< string > nMateriales;
	Modelo(char* path, char* filename) {
		glEnable(GL_TEXTURE_2D);
		fname = filename;
		tie(vertexIndices, texIndices, normalIndices, materiales, iPoligonos, nMateriales) = loadObj(path, fname);
	}
};

//Modelos
Modelo* HONK;
Modelo* botella;
Modelo* mesa;
Modelo* silla;
Modelo* tenedor;
Modelo* cuchillo;
Modelo* plato;
Modelo* limon;
Modelo* naranja;
Modelo* puerta;
Modelo* copa;
Modelo* marco;
Modelo* melocoton;

void drawModelo(Modelo modelo)
{
	glColor3f(1.0f, 1.0f, 1.0f);
	int pol = 0;
	float* Ka = NULL;
	float* Kd = NULL;
	float* Ks = NULL;
	GLfloat Ns = 0.0f;
	vector<Poligono> vI = modelo.vertexIndices;
	vector<Poligono> vN = modelo.normalIndices;
	vector<Poligono> vT = modelo.texIndices;
	vector<Poligono>::iterator itNor = vN.begin();
	vector<Poligono>::iterator itTex = vT.begin();
	vector<int>::iterator itP;
	for (vector<Poligono>::iterator itVec = vI.begin(); itVec != vI.end(); ++itVec, ++itNor, ++itTex) {
		
		itP = find(modelo.iPoligonos.begin(), modelo.iPoligonos.end(), pol);
		if (itP != modelo.iPoligonos.end()) {
			int a = distance(modelo.iPoligonos.begin(), itP);

			string s = modelo.nMateriales[a];
			for (vector<Material>::iterator itMat = modelo.materiales.begin(); itMat != modelo.materiales.end(); ++itMat) {
				Material mat = *itMat;
				if (mat.name == s) {
					glBindTexture(GL_TEXTURE_2D, texturas[mat.imagen]);
					Ns = mat.Ns;
					Ka = mat.Ka;
					Kd = mat.Kd;
					Ks = mat.Ks;
					break;
				}
			}
		}
		glBegin(GL_POLYGON);

		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Ka);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Kd);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Ks);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS,&Ns);

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
		pol++;
	}
}

void preparaCamara() {
	glLoadIdentity();
	switch (modo) {
	case LIBRE:
		posX = camX + sin(anguloX) * cos(anguloY);
		posY = camY + sin(anguloY);
		posZ = camZ + -cos(anguloX) * cos(anguloY);
		gluLookAt(camX, camY, camZ, posX, posY, posZ, 0, 1, 0);
		break;
	case POV:
		gluLookAt(-0.75, 10.5, -6.3, -0.6, 10.1, -5.4, 0, 1, 0);
		break;
	case PLANTA:
		gluLookAt(0, 23, 0, 0, 0, 0, 0, 0, 1);
		break;
	case ESQUINA:
		gluLookAt(-24, 21, 24, 0, 7.5, 0, 0, 1, 0);
		break;
	case CIRCULO:
		gluLookAt((double)sin(fcount) * radius*1.75, 13, (double)cos(fcount) * radius*1.25, 0, 7, 0, 0, 1, 0);
		fcount += 0.05f;
		break;
	case CINEMATICA:
		gluLookAt(cineX, 6.6, 3, cineX + (double)sin(cineAng), 6.6, 3 - (double)cos(cineAng), 0, 1, 0);
		break;
	}
}

void mueveCamara() {
	float speed = 0.5f;
	if (teclas[0]) {
		if (modo == LIBRE) {
			float cZ = speed * -cos(anguloX);
			float cX = speed * sin(anguloX);

			camZ += cZ;
			posZ += cZ;
			camX += cX;
			posX += cX;
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

	if (modo == CINEMATICA) {
		if (cineX > -5.8) {
			cineX += -0.2;
		}
		else if(cineAng < 0){
			cineAng += incAngulo;

			light0 = true;
			glEnable(GL_LIGHT0);
		}
	}
	
}

void InitWindow(GLfloat Width, GLfloat Height) {
	glViewport(0, 0, (GLsizei)Width, (GLsizei)Height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0 / zoomFactor, Width / Height, 0.1, 10000.0);

	glMatrixMode(GL_MODELVIEW);
}

void myResize(int width, int height) {
	if (height == 0) height = 1; //Para no dividir por 0
	if (width == 0) width = 1;
	InitWindow((GLfloat)width, (GLfloat)height);
}

void ControlesEspeciales(int key, int x, int y) {
	
	switch (key) {
	case GLUT_KEY_F6:
		light0 = false;
		glDisable(GL_LIGHT0);
		cineX = 3;
		cineAng = -pi / 2;
	case GLUT_KEY_F1:
	case GLUT_KEY_F2:
	case GLUT_KEY_F3:
	case GLUT_KEY_F4:
	case GLUT_KEY_F5:
		modo = key;
		int width = glutGet(GLUT_WINDOW_WIDTH);
		int height = glutGet(GLUT_WINDOW_HEIGHT);
		InitWindow((GLfloat)width, (GLfloat)height);
		break;
	}

}

void liberaTeclas(unsigned char key, int x, int y) {
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

	switch (key) {
	case 127: //SUPR
		NURBS = !NURBS;
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
		
		break;
	case 109: //m
		fogDensity += 0.01f;
		glFogf(GL_FOG_DENSITY, fogDensity);
		break;
	case 110: //n
		if (fogDensity >= 0) {
			fogDensity -= 0.01f;
			glFogf(GL_FOG_DENSITY, fogDensity);
		}
		break;
	case 13: //enter
		NURBS = !NURBS;
		break;
	}
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
		float incX, incY;
		incX = -0.1f * (cx - x) * incAngulo;
		incY = 0.1f * (cy - y) * incAngulo;
		cx = x;
		cy = y;
		anguloX += incX;
		if (anguloY + incY >= -pi / 2 && anguloY + incY <= pi / 2) {
			anguloY += incY;
		}
	}
}


float tenedorX = 0;
float tenedorY = 0;
float tenedorZ = 0;
float tenedorA = 0;
float tenedorB = 0;
float naranjaY = 0;

int wait = 30;

bool levantar = true;
bool avanzar = false;
bool recoger = false;
bool caida = false;
bool rebote = false;
bool final = false;
bool vuelta = false;


// Funci�n que visualiza la escena OpenGL
void Display(void)
{
	// Borramos la escena
	glClear(GL_COLOR_BUFFER_BIT);

	
	//Activamos buffer de PROFUNDIAD
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
		

	//Luces

	if (light0) {

		glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
		glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
		glLightfv(GL_LIGHT0, GL_LINEAR_ATTENUATION, light_atenuation);
		glEnable(GL_LIGHT0);
		glPushMatrix();
		glTranslatef(light0_position[0], light0_position[1], light0_position[2]);
		glColor3f(1.0f, 1.0f, 1.0f);
		glutWireSphere(0.05, 10, 10);
		glPopMatrix();
	}
	if (light1) {

		glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
		glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
		glLightfv(GL_LIGHT1, GL_LINEAR_ATTENUATION, light_atenuation1);
		glPushMatrix();
		glTranslatef(light_position1[0], light_position1[1], light_position1[2]);
		glColor3f(1.0f, 0.0f, 0.0f);
		glutWireSphere(0.05, 10, 10);
		glPopMatrix();
	}
	if (light2) {

		glLightfv(GL_LIGHT2, GL_POSITION, light_position2);
		glLightfv(GL_LIGHT2, GL_AMBIENT, ambient2);
		glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular2);
		glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse2);
		glLightfv(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, light_atenuation2);
		glPushMatrix();
		glTranslatef(light_position2[0], light_position2[1], light_position2[2]);
		glColor3f(0.0f, 1.0f, 0.0f);
		glutWireSphere(0.05, 10, 10);
		glPopMatrix();
	}
	if (light3) {

		glLightfv(GL_LIGHT3, GL_POSITION, light_position3);
		glLightfv(GL_LIGHT3, GL_AMBIENT, ambient3);
		glLightfv(GL_LIGHT3, GL_SPECULAR, light_specular3);
		glLightfv(GL_LIGHT3, GL_DIFFUSE, light_diffuse3);
		glLightfv(GL_LIGHT3, GL_CONSTANT_ATTENUATION, light_atenuation3);
		glPushMatrix();
		glTranslatef(light_position3[0], light_position3[1], light_position3[2]);
		glColor3f(0.0f, 0.0f, 1.0f);
		glutWireSphere(0.05, 10, 10);
		glPopMatrix();
	}

	//ANIMACIÓN 
	if (wait <= 0) {
		if (levantar) {
			tenedorY += 0.05f;
			tenedorA += 2;
			if (tenedorY >= 1.5) {
				levantar = false;
				avanzar = true;
			}
		}
		else if (avanzar) {
			tenedorX -= 0.004f;
			tenedorY -= 0.04;
			tenedorZ += 0.1f;
			tenedorB += 7;
			if (tenedorZ >= 2.3f) {
				avanzar = false;
				recoger = true;
				wait = 7;
			}
		}
		else if (recoger) {
			naranjaY += 0.05;
			tenedorY += 0.05;
			if (tenedorY >= 2) {
				recoger = false;
				caida = true;
			}
		}
		else if (caida) {
			naranjaY -= 0.5;
			if (naranjaY <= 0) {
				caida = false;
				rebote = true;
			}
		}
		else if (rebote) {
			naranjaY += 0.1;
			if (naranjaY >= 0.2) {
				rebote = false;
				final = true;
			}
		}
		else if (final) {
			naranjaY -= 0.05;
			if (naranjaY <= 0) {
				final = false;
				vuelta = true;
			}
		}
		else if (vuelta) {
			if (tenedorX > 0) {
				tenedorX += 0.1;
			}
			if (tenedorY > 0) {
				tenedorY -= 0.05;
			}
			if (tenedorZ > 0) {
				tenedorZ -= 0.1;
			}
			if (tenedorA > 0) {
				tenedorA -= 2;
			}
			if (tenedorB > 0) {
				tenedorB -= 7;
			}
			if (tenedorY <= 0) {
				vuelta = false;
				levantar = true;
				wait = 30;
				tenedorX = 0;
				tenedorY = 0;
				tenedorZ = 0;
				tenedorA = 0;
				tenedorB = 0;
				naranjaY = 0;
			}
		}
	}
	else {
		wait--;
	}

	//Modelos de la escena
	glPushMatrix();
	glTranslatef(-6.0f,6.3f,2.5f);
	glScalef(0.07f, 0.07f, 0.07f);
	drawModelo(*HONK);
	glPopMatrix();

	//mesa
	glPushMatrix();
	glScalef(0.1f, 0.1f, 0.1f);
	drawModelo(*mesa);
	glPopMatrix();

	//silla
	glPushMatrix();
	glScalef(0.02, 0.02, 0.02);
	glRotatef(10, 0, 1, 0);
	glTranslatef(0, 30, -250);
	drawModelo(*silla);
	glPopMatrix();

	//plato
	glPushMatrix();
	glTranslatef(0, 7.301f, -2.0f);
	glScalef(9.0f, 5.0f, 9.0f);
	drawModelo(*plato);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 7.301f, -2.0f);
	glScalef(0.6f, 0, 0.6f);
	glBegin(GL_POLYGON); //ESTO ES NECESARIO BECAUSE REASONS
	for (int i = 0; i < 20; ++i)
	{
		glNormal3f(0.0f, 1.0f, 0.0f);
		glVertex3f(cos(2 * 3.14159 * i / 20.0), 0, sin(2 * 3.14159 * i / 20.0));
	}
	glEnd();
	glPopMatrix();

	//cuchillo
	glPushMatrix();
	glScalef(0.1f, 0.1f, 0.1f);
	glTranslatef(-14, 73.31f, -24);
	glRotatef(90, -1, 0, 0);
	glRotatef(130, 0, 0, 1);
	drawModelo(*cuchillo);
	glPopMatrix();
	
	//tenedor
	glPushMatrix();
	glTranslatef(1.4 + tenedorX, 7.385f + tenedorY, -2.3 + tenedorZ);
	glScalef(0.1f, 0.1f, 0.1f);
	glRotatef(90, 0, 1, 0);
	glRotatef(86 - tenedorB, -1, 0, 0);
	glRotatef(20 + tenedorA, 0, 0, 1);
	drawModelo(*tenedor);
	glPopMatrix();

	//naranja
	glPushMatrix();
	glTranslatef(1.5f, 7.3f + naranjaY, 1);
	glScalef(0.09f, 0.09f, 0.09f);
	glRotatef(-90, 1, 0, 0);
	drawModelo(*naranja);
	glPopMatrix();

	//limon
	glColor3f(1.3f, 1.3f, 0.0f);
	glPushMatrix();
	glScalef(0.035f, 0.032f, 0.035f);
	glTranslatef(-50, 228, 40);
	glRotatef(-90, 1, 0, 0);
	glRotatef(40, 0, 0, 1);
	drawModelo(*limon);
	glPopMatrix(); 
		
	glPushMatrix();
	glScalef(0.035f, 0.032f, 0.035f);
	glTranslatef(-70, 228, 60);
	glRotatef(-90, 1, 0, 0);
	glRotatef(-80, 0, 0, 1);
	drawModelo(*limon);
	glPopMatrix();


	//melocoton
	glPushMatrix();
	glTranslatef(3.5f, 7.3f, -0.45);
	glScalef(0.15f, 0.15f, 0.15f);
	glRotatef(-90, 1, 0, 0);
	drawModelo(*melocoton);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(4.0f, 7.5f, 0);
	glScalef(0.15f, 0.15f, 0.15f);
	glRotatef(-40, 1, 0, 0);
	glRotatef(90, 0, 0, 1);
	drawModelo(*melocoton);
	glPopMatrix();

	//puerta
	glPushMatrix();
	glTranslatef(10, 0, -25);
	glRotatef(90, -1, 0, 0);
	glScalef(0.1f, 0.1f, 0.1f);
	drawModelo(*puerta);
	glPopMatrix();

	//marco
	glPushMatrix();
	glTranslatef(4.0f, 7.3f, 2.5f);
	glRotatef(135, 0, -1, 0);
	glScalef(0.015f, 0.015f, 0.015f);
	drawModelo(*marco);
	glPopMatrix();
	
	//bottle
	glPushMatrix();
	glTranslatef(-3, 7.301, 0);
	glRotatef(90, -1, 0,0 );
	glRotatef(30, 0, 0, 1);
	glScalef(0.12f, 0.12f, 0.12f);
	drawModelo(*botella);
	glPopMatrix();

	//puerta
	glPushMatrix();
	glTranslatef(10, 0, -25);
	glRotatef(90, -1, 0, 0);
	glScalef(0.1f, 0.1f, 0.1f);
	drawModelo(*puerta);
	glPopMatrix();

	//suelo
	glBindTexture(GL_TEXTURE_2D, texturas[suelo]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBegin(GL_POLYGON); 
	glNormal3d(0, 1, 0);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-0, -0.01, -0);
	glTexCoord2f(0.0f, 2.0f);
	glVertex3f(25, -0.01, -0);
	glTexCoord2f(2.0f, 2.0f);
	glVertex3f(25, -0.01, 25);
	glTexCoord2f(2.0f, 0.0f);
	glVertex3f(-0, -0.01, 25);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, -0.01, 0);
	glTexCoord2f(0.0f, 2.0f);
	glVertex3f(-25, -0.01, 0);
	glTexCoord2f(2.0f, 2.0f);
	glVertex3f(-25, -0.01, 25);
	glTexCoord2f(2.0f, 0.0f);
	glVertex3f(0, -0.01, 25);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-0, -0.01, -0);
	glTexCoord2f(0.0f, 2.0f);
	glVertex3f(25, -0.01, -0);
	glTexCoord2f(2.0f, 2.0f);
	glVertex3f(25, -0.01, -25);
	glTexCoord2f(2.0f, 0.0f);
	glVertex3f(-0, -0.01, -25);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-0, -0.01, -0);
	glTexCoord2f(0.0f, 2.0f);
	glVertex3f(-25, -0.01, -0);
	glTexCoord2f(2.0f, 2.0f);
	glVertex3f(-25, -0.01, -25);
	glTexCoord2f(2.0f, 0.0f);
	glVertex3f(-0, -0.01, -25);
	glEnd();

	//wall
	glBindTexture(GL_TEXTURE_2D, texturas[pared]);
	glBegin(GL_POLYGON);
	glNormal3f(0, 0, 1);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-25, 0, -25);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(25, 0, -25);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(25, 25, -25);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-25, 25, -25);
	glEnd();
	glBegin(GL_POLYGON);
	glNormal3f(0, 0, -1);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-25, 0, 25);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(25, 0, 25);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(25, 25, 25);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-25, 25, 25);
	glEnd();
	glBegin(GL_POLYGON);
	glNormal3f(1, 0, 0);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-25, 0, -25);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-25, 0, 25);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-25, 25, 25);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-25, 25, -25);
	glEnd();
	glBegin(GL_POLYGON);
	glNormal3f(-1, 0, 0);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(25, 0, -25);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(25, 0, 25);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(25, 25, 25);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(25, 25, -25);
	glEnd();

	//NURBS (opcional y funciona y todo)
	if (NURBS) {
		float i, j;
		glLineWidth(5);
		glPushMatrix();
		glTranslatef(0.0f, 7.0f, -2.0f);
		glScalef(0.4f, 0.4f, 0.4f);
		for (j = 0; j <= 8; j += 1) {
			glBegin(GL_LINE_STRIP);
			for (i = 0; i <= 30; i += 1)
				glEvalCoord2f((GLfloat)i / 30.0, (GLfloat)j / 8.0);
			glEnd();
			glBegin(GL_LINE_STRIP);
			for (i = 0; i <= 30; i += 1)
				glEvalCoord2f((GLfloat)j / 8.0, (GLfloat)i / 30.0);
			glEnd();
		}
		glPopMatrix();
		glLineWidth(1);
	}
	glutSwapBuffers();
}

// Funci�n que se ejecuta cuando el sistema no esta ocupado
void Idle(void)
{
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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA_SATURATE, GL_ONE);

	// El color de fondo ser� el negro (RGBA, RGB + Alpha channel)
	glClearColor(0, 0, 0, 1);
	
	//MATERIALES
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	glEnable(GL_LIGHTING);
	InitWindow(W_WIDTH, W_HEIGHT);
	preparaCamara();

	//Cargamos suelo y paredes
	pared = initTex("models/wall/wall.jpg");
	suelo = initTex("models/floor/floor.png");

	HONK = new Modelo((char*)"models\\Honk\\", (char*)"honk");
	botella = new Modelo((char*)"models/wine/", (char*)"rum");
	mesa = new Modelo((char*)"models/old_wooden_table/", (char*)"old_wooden_table");
	silla = new Modelo((char*)"models/chair/", (char*)"Chair");
	tenedor = new Modelo((char*)"models/fork/", (char*)"fork");
	cuchillo = new Modelo((char*)"models/Knife/", (char*)"knife");
	plato = new Modelo((char*)"models/plate/", (char*)"plate");
	limon = new Modelo((char*)"models/lemon/", (char*)"lemon");
	naranja = new Modelo((char*)"models/orange/", (char*)"orange");
	puerta = new Modelo((char*)"models/wooden_door/", (char*)"wooden_door");
	marco = new Modelo((char*)"models/frame/", (char*)"album");
	melocoton = new Modelo((char*)"models/peach/", (char*)"peach");

	//NURBS
	GLfloat ctrlpoints[4][4][3] = {
		 {{-1.5, 1.0, -1.5}, {-0.5, 1.0,-1.5 }, {0.5, 1.0, -1.5 }, {1.5, 1.0,-1.5}},
		 {{-1.5, 1.0, -0.5}, {-0.5, 2.0,-0.5 }, {0.5, 2.0, -0.5 }, {1.5, 1.0,-0.5}},
		 {{-1.5, 1.0,  0.5}, {-0.5, 2.0, 0.5 }, {0.5, 2.0,  0.5 }, {1.5, 1.0, 0.5}},
		 {{-1.5, 1.0,  1.5}, {-0.5, 1.0, 1.5 }, {0.5, 1.0,  1.5 }, {1.5, 1.0, 1.5}}
	};
	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4,
		0, 1, 12, 4, &ctrlpoints[0][0][0]);
	glEnable(GL_MAP2_VERTEX_3);
	glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);

	//Fog
	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_EXP);
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_DENSITY, fogDensity);
	glHint(GL_FOG_HINT, GL_DONT_CARE);
	glFogf(GL_FOG_START, 1.0f);
	glFogf(GL_FOG_END, 5.0f);
	

	// Comienza la ejecuci�n del core de GLUT
	glutMainLoop();
	return 0;
}

