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
#include <GL/GLU.h>
#include <iostream>
#include <vector>
#include <tuple>
#include "SOIL2/SOIL2.h"
using namespace std;
const int W_WIDTH = 700; // Tama�o incial de la ventana
const int W_HEIGHT = 700;
const float pi = 3.141592f;
GLfloat fAngulo; // Variable que indica el �ngulo de rotaci�n de los ejes. 
float p_width = W_WIDTH, p_height = W_HEIGHT;
float fcount = 0;
bool muestraReferencias = true;

//Light 0
//Light position
GLfloat light0_position[] = { 5.0, 10.0, 5.0, 1.0 };
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

bool normal = false;

//FOG
const GLfloat fogColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat fogDensity = 0.0f;

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
float anguloX = 0;
float anguloY = -pi / 8;
float anguloX2 = 0;
float anguloY2 = 0;
float camX = 0;
float camZ = 10;
float camY = 12;
float posX = 0;
float posY = 0;
float posZ = 0;

//Carcar modelos
bool modelos = false;
int mod = 1;

//Texturas;
int pared, suelo;
vector<GLuint> texturas;
int numTexturas;
//GLuint elephant;
//float elephantrot;
char ch = '1';
bool modelo_normal = false;




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
	glEnable(GL_TEXTURE_2D);
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

tuple<int, vector<Poligono>, vector<Poligono>, vector<Poligono>, vector<Material>, vector < int >, vector < string > >
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
	return make_tuple(numPol, vertexIndices, texIndices, normalIndices, materiales, iPoligonos, nMateriales);
}

class Modelo {
public:
	int numPol = 0;
	char* fname;
	vector< Poligono > vertexIndices, texIndices, normalIndices;
	vector< Material > materiales;
	vector< int > iPoligonos;
	vector< string > nMateriales;
	Modelo(char* path, char* filename) {
		glEnable(GL_TEXTURE_2D);
		fname = filename;
		tie(numPol, vertexIndices, texIndices, normalIndices, materiales, iPoligonos, nMateriales) = loadObj(path, fname);
	}
};

Modelo* HONK;

Modelo* botella;
Modelo* mesa;
Modelo* silla;
Modelo* tenedor;
Modelo* cuchillo;
Modelo* plato;
Modelo* manzana;
Modelo* limon;
Modelo* naranja;
Modelo* tasso;
Modelo* puerta;

//Modelo* marco;



void foto() {
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texturas[5]);

	glBegin(GL_POLYGON);

	glTexCoord2f(0, 0);
	glVertex3f(0, 0, 0);

	glTexCoord2f(0, 1);
	glVertex3f(0, 1, 0);

	glTexCoord2f(1, 1);
	glVertex3f(1, 1, 0);

	glTexCoord2f(1, 0);
	glVertex3f(1, 0, 0);

	glEnd();



	glBindTexture(GL_TEXTURE_2D, texturas[1]);

	glBegin(GL_POLYGON);

	glTexCoord2f(0, 0);
	glVertex3f(0, 0, 0);

	glTexCoord2f(0, 1);
	glVertex3f(0, 1, 0);

	glTexCoord2f(1, 1);
	glVertex3f(-1, 1, 0);

	glTexCoord2f(1, 0);
	glVertex3f(-1, 0, 0);



	glEnd();

	glBindTexture(GL_TEXTURE_2D, texturas[0]);

	glBegin(GL_POLYGON);

	glTexCoord2f(0, 0);
	glVertex3f(0, 0, 0);

	glTexCoord2f(0, 1);
	glVertex3f(0, 0, 1);

	glTexCoord2f(1, 1);
	glVertex3f(1, 0, 1);

	glTexCoord2f(1, 0);
	glVertex3f(1, 0, 0);



	glEnd();

	glPopMatrix();
}


void drawModelo(Modelo modelo)
{

	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
	int pol = 0;
	float KKs[] = { 0.0f, 0.0f, 0.0f };
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

		//modelo.iPoligonos.find
		itP = find(modelo.iPoligonos.begin(), modelo.iPoligonos.end(), pol);
		if (itP != modelo.iPoligonos.end()) {
			int a = distance(modelo.iPoligonos.begin(), itP);

			string s = modelo.nMateriales[a];
			for (vector<Material>::iterator itMat = modelo.materiales.begin(); itMat != modelo.materiales.end(); ++itMat) {
				Material mat = *itMat;
				if (mat.name == s) {
					//initTex(mat.imagen);
					glBindTexture(GL_TEXTURE_2D, texturas[mat.imagen]);
					glEnable(GL_TEXTURE_2D);
					Ns = mat.Ns;
					Ka = mat.Ka;
					Kd = mat.Kd;
					Ks = mat.Ks;
					break;
				}
			}
		}
		glBegin(GL_POLYGON);


		//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Ka);
		//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Kd);
		//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Ks);
		//glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS,&Ns);

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
		gluLookAt(0, 10, -10, 0, 0, 0, 0, 1, 0);
		break;
	case PLANTA:
		gluLookAt(0, 10, 0, 0, 0, 0, 1, 0, 0);
		break;
	case PERFIL:
		gluLookAt(-10, 10, 0, 0, 0, 0, 0, 1, 0);
		break;
	case CIRCULO:
		gluLookAt((double)sin(fcount) * radius, 10, (double)cos(fcount) * radius, 0, 5, 0, 0, 1, 0);
		fcount += 0.005f;
		break;
	case ESFERICO:
		float x = radio2 * sin(anguloX2) * cos(anguloY2);
		float y = radio2 * sin(anguloY2);
		float z = radio2 * -cos(anguloX2) * cos(anguloY2);
		gluLookAt(x, (y + 10), z, 0, 5, 0, 0, 1, 0);
		break;
	}
}

void mueveCamara() {
	float incAngulo = pi / 60;
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
	glViewport(0, 0, (GLsizei)Width, (GLsizei)Height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	switch (modo) {
	case 4:
	case LIBRE:
	case 6:
		gluPerspective(45.0 / zoomFactor, Width / Height, 0.1, 10000.0);
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
	InitWindow((GLfloat)width, (GLfloat)height);
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
		InitWindow((GLfloat)width, (GLfloat)height);
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
		InitWindow((GLfloat)width, (GLfloat)height);
		//std::cout << zoomFactor << "\n";
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
		//std::cout << zoomFactor << "\n";
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
		fogDensity += 0.02f;
		break;
	case 110: //n
		if (fogDensity > 0) {
			fogDensity -= 0.02f;
		}
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

	std::cout << "X:"<<camX<<" Y: "<<camY<<" Z: "<< camZ;
	//std::cout << "key: " << (int)key << "\n";
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
		float incAngulo = (float)(pi / 60);
		int dx, dy;
		float incX, incY;
		dx = cx - x;
		dy = cy - y;
		cx = x;
		cy = y;
		incX = -0.1f * dx * incAngulo;
		incY = 0.1f * dy * incAngulo;
		anguloX += incX;
		if (anguloY + incY >= -pi / 2 && anguloY + incY <= pi / 2) {
			anguloY += incY;
		}
		//std::cout << "anguloY: " << anguloY << "\n";
	}
}


float tenedorX = 0;
float tenedorY = 0;
float tenedorZ = 0;
float tenedorA = 0;
float tenedorB = 0;
float manzanaY = 0;

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

		glPushMatrix();

		glLineWidth(1);
		//Objetos
		//Activamos buffer de PROFUNDIAD
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		
		//Actualizamos la fog
		glFogf(GL_FOG_DENSITY, fogDensity);

		//Colores soras y cosas divertidas
		/*He intentado hacer cosas con las luces*/
		glEnable(GL_LIGHTING);


		//Light 0
		glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
		glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
		glLightfv(GL_LIGHT0, GL_LINEAR_ATTENUATION, light_atenuation);

		//light 1
		glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
		glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
		glLightfv(GL_LIGHT1, GL_LINEAR_ATTENUATION, light_atenuation1);

		//light 2
		glLightfv(GL_LIGHT2, GL_POSITION, light_position2);
		glLightfv(GL_LIGHT2, GL_AMBIENT, ambient2);
		glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular2);
		glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse2);
		glLightfv(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, light_atenuation2);
		//light3
		glLightfv(GL_LIGHT3, GL_POSITION, light_position3);
		glLightfv(GL_LIGHT3, GL_AMBIENT, ambient3);
		glLightfv(GL_LIGHT3, GL_SPECULAR, light_specular3);
		glLightfv(GL_LIGHT3, GL_DIFFUSE, light_diffuse3);
		glLightfv(GL_LIGHT3, GL_CONSTANT_ATTENUATION, light_atenuation3);

		if (light0) {
			glEnable(GL_LIGHT0);
			glPushMatrix();
			glTranslatef(light0_position[0], light0_position[1], light0_position[2]);
			glColor3f(1.0f, 1.0f, 1.0f);
			glutWireSphere(0.05, 10, 10);
			glPopMatrix();
		}
		if (light1) {
			glPushMatrix();
			glTranslatef(light_position1[0], light_position1[1], light_position1[2]);
			glColor3f(1.0f, 0.0f, 0.0f);
			glutWireSphere(0.05, 10, 10);
			glPopMatrix();
		}
		if (light2) {
			glPushMatrix();
			glTranslatef(light_position2[0], light_position2[1], light_position2[2]);
			glColor3f(0.0f, 1.0f, 0.0f);
			glutWireSphere(0.05, 10, 10);
			glPopMatrix();
		}
		if (light3) {
			glPushMatrix();
			glTranslatef(light_position3[0], light_position3[1], light_position3[2]);
			glColor3f(0.0f, 0.0f, 1.0f);
			glutWireSphere(0.05, 10, 10);
			glPopMatrix();
		}

		//Ejes de cordenadas
		if (false) {
			glLineWidth(3);

			glBegin(GL_LINES);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(-100.0f, 0.0f, 0.0f);
			glVertex3f(100.0f, 0.0f, 0.0f);
			glEnd();

			glBegin(GL_LINES);
			glColor3f(0.0f, 1.0f, 0.0f);
			glVertex3f(0.0f, -100.0f, 0.0f);
			glVertex3f(0.0f, 100.0f, 0.0f);
			glEnd();

			glBegin(GL_LINES);
			glColor3f(0.0f, 0.0f, 1.0f);
			glVertex3f(0.0f, 0.0f, -100.0f);
			glVertex3f(0.0f, 0.0f, 100.0f);
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
					glColor3f(0.0f, 0.0f, 0.0f);
					glVertex3f(i, 0.0f, -100.0f);
					glVertex3f(i, 0.0f, 100.0f);
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
		GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat mat_shininess[] = { 128.0f };

		//glClearColor(0.0, 0.0, 0.0, 0.0);

		/*glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MatAmbient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MatDiffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);*/


		//Dibujamos el "suelo" que es trans
		if (false) {
			/*glPushMatrix();
			glTranslatef(0, -5, 0);
			//glRectf();
			glColor4f(0.5f, 0.5f, 1.0f, 0.5f);
			glutSolidCube(1);*/
			glBegin(GL_POLYGON);
			glColor4f(0.5f, 0.5f, 2.0f, 0.5f);
			//glColor3f(0.5f, 0.5f, 1.0f);
			glVertex3f(-100.0f, -0.01f, -100.0f);
			glVertex3f(100.0f, -0.01f, -100.0f);
			glVertex3f(100.0f, -0.01f, 100.0f);
			glVertex3f(-100.0f, -0.01f, 100.0f);
			glNormal3d(0, 1, 0);
			glEnd();
		}

		if (false) {
			float MatAmbient[] = { 1.1f, 1.1f, 1.1f, 0.0f };
			float MatDiffuse[] = { 0.0f, 0.0f, 0.0f, 0.0f };
			GLfloat mat_specular[] = { 0.0f, 0.0f, 0.0f, 0.0f };
			GLfloat mat_shininess[] = { 1.0f };

			glMaterialfv(GL_FRONT, GL_AMBIENT, MatAmbient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, MatDiffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
			glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
			//dibuja el modelo cargado
		}
		//ains();
		//foto();

		//Modelos de la escena

		//Mesa

		drawModelo(*HONK);


		//silla
		/*glPushMatrix();
		glTranslatef(0, -5, 0);
		//glRectf();
		glColor4f(0.5f, 0.5f, 1.0f, 0.5f);
		glutSolidCube(1);*/
		glBegin(GL_POLYGON);
		glColor4f(0.5f, 0.5f, 2.0f, 0.5f);
		//glColor3f(0.5f, 0.5f, 1.0f);
		glVertex3f(-100.0f, -0.01f, -100.0f);
		glVertex3f(100.0f, -0.01f, -100.0f);
		glVertex3f(100.0f, -0.01f, 100.0f);
		glVertex3f(-100.0f, -0.01f, 100.0f);
		glNormal3d(0, 1, 0);
		glEnd();
	}

	if (false) {
		float MatAmbient[] = { 1.1f, 1.1f, 1.1f, 0.0f };
		float MatDiffuse[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		GLfloat mat_specular[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		GLfloat mat_shininess[] = { 1.0f };

		glMaterialfv(GL_FRONT, GL_AMBIENT, MatAmbient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, MatDiffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
		//dibuja el modelo cargado
	}
	//ains();
	//foto();

	//Modelos de la escena

	//Mesa

	drawModelo(*HONK);


	//silla
	/*glPushMatrix();
	glScalef( 0.17, 0.17, 0.17);
	glRotatef(90, 0, 0, 1);
	glTranslatef(-10.1, -150, 0);
	glPopMatrix();*/
	//plato (nope, este modelo es gigante o algo, laggea que flipas)
	//loadObj((char*)"models/Plate/plate.obj");
	glColor3f(1.0, 0.3f, 0.3f);
	glPushMatrix();
	glScalef(0.1f, 0.1f, 0.1f);
	drawModelo(*mesa);
	glPopMatrix();
	//silla
	glColor3f(0.3, 1.0f, 0.3f);
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
			glVertex3f(cos(2 * 3.14159 * i / 20.0), 0, sin(2 * 3.14159 * i / 20.0));
		}
		glEnd();
		glPopMatrix();

	//cuchillo
	glColor3f(0.3f, 0.3f, 1.0f);
	glPushMatrix();
	glScalef(0.1f, 0.1f, 0.1f);
	glTranslatef(-14, 73.31f, -24);
	glRotatef(90, -1, 0, 0);
	glRotatef(130, 0, 0, 1);
	drawModelo(*cuchillo);
	glPopMatrix();

		//INCREIBLE ANIMACIÓN ESPECTACULAR Y APABULLANTE (la he hehco con 3fps asi que alome se vera regular)
		if (wait <= 0) {
			if (levantar) {
				//tenedorX += 0;
				tenedorY += 0.05f;
				//tenedorZ += 0;
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
				manzanaY += 0.05;
				tenedorY += 0.05;
				if (tenedorY >= 2) {
					recoger = false;
					caida = true;
				}
			}
			else if (caida) {
				manzanaY -= 0.5;
				if (manzanaY <= 0) {
					caida = false;
					rebote = true;
				}
			}
			else if (rebote) {
				manzanaY += 0.1;
				if (manzanaY >= 0.2) {
					rebote = false;
					final = true;
				}
			}
			else if (final) {
				manzanaY -= 0.05;
				if (manzanaY <= 0) {
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
			}
		}
	}
	else {
		wait--;
	}

	//tenedor
	glColor3f(0.3f, 0.3f, 0.3f);
	glPushMatrix();
	glTranslatef(1.4 + tenedorX, 7.385f + tenedorY, -2.3 + tenedorZ);
	glScalef(0.1f, 0.1f, 0.1f);
	glRotatef(90, 0, 1, 0);
	glRotatef(86 - tenedorB, -1, 0, 0);
	glRotatef(20 + tenedorA, 0, 0, 1);
	drawModelo(*tenedor);
	glPopMatrix();

		//tenedor
		glColor3f(0.3f, 0.3f, 0.3f);
		glPushMatrix();
		glTranslatef(1.5f, 7.28f + manzanaY, 1);
		glRotatef(90, -1, 0, 0);
		glScalef(0.005f, 0.005f, 0.005f);
		drawModelo(*manzana);
		glPopMatrix();

	//limon
	{
		glColor3f(1.3f, 1.3f, 0.0f);
		glPushMatrix();
		glScalef(0.035f, 0.032f, 0.035f);
		glTranslatef(-70, 228, 30);
		glRotatef(-90, 1, 0, 0);
		drawModelo(*limon);
		glPopMatrix();

		glColor3f(1.3f, 1.3f, 0.0f);
		glPushMatrix();
		glScalef(0.035f, 0.032f, 0.035f);
		glTranslatef(-50, 228, 40);
		glRotatef(-90, 1, 0, 0);
		glRotatef(40, 0, 0, 1);
		drawModelo(*limon);
		glPopMatrix();

		//naranja
		{
			glColor3f(1.3f, 0.7f, 0.0f);
			glPushMatrix();
			glTranslatef(5, 7.3f, -2);
			glScalef(0.1f, 0.1f, 0.1f);
			glRotatef(-90, 1, 0, 0);
			drawModelo(*naranja);
			glPopMatrix();
			glColor3f(1.3f, 0.7f, 0.0f);
			glPushMatrix();
			glTranslatef(3.5f, 7.3f, -0.45);
			glScalef(0.1f, 0.1f, 0.1f);
			glRotatef(-90, 1, 0, 0);
			drawModelo(*naranja);
			glPopMatrix();
			glPopMatrix();
			glColor3f(1.3f, 0.7f, 0.0f);
			glPushMatrix();
			glTranslatef(4.0f, 7.5f, 0);
			glScalef(0.1f, 0.1f, 0.1f);
			glRotatef(-40, 1, 0, 0);
			glRotatef(90, 0, 0, 1);
			drawModelo(*naranja);
			glPopMatrix();
		}

		//bottle
		glPushMatrix();
		glScalef(0.035f, 0.032f, 0.035f);
		glTranslatef(-70, 228, 60);
		glRotatef(-90, 1, 0, 0);
		glRotatef(-80, 0, 0, 1);
		drawModelo(*limon);
		glPopMatrix();

		//glass
		glPushMatrix();
		glTranslatef(-2, 7.25f, -1);
		glScalef(0.2f, 0.2f, 0.2f);
		//drawModelo(*tasso);
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
		//drawModelo(*marco);
		glPopMatrix();
	}

	//bottle
	glPushMatrix();
	glTranslatef(-3, 8.35, 0);
	glScalef(0.01f, 0.01f, 0.01f);
	drawModelo(*botella);
	glPopMatrix();

	//glass
	glPushMatrix();
	glTranslatef(-2, 7.25f, -1);
	glScalef(0.2f, 0.2f, 0.2f);
	drawModelo(*tasso);
	glPopMatrix();

	//puerta
	glPushMatrix();
	glTranslatef(10, 0, -25);
	glRotatef(90, -1, 0, 0);
	glScalef(0.1f, 0.1f, 0.1f);
	drawModelo(*puerta);
	glPopMatrix();

	//suelo
	glColor3f(1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, texturas[suelo]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBegin(GL_POLYGON); 
	glNormal3d(0, 1, 0);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-0, -0.01, -0);
	glNormal3d(0, 1, 0);
	glTexCoord2f(0.0f, 5.0f);
	glVertex3f(25, -0.01, -0);
	glNormal3d(0, 1, 0);
	glTexCoord2f(5.0f, 5.0f);
	glVertex3f(25, -0.01, 25);
	glNormal3d(0, 1, 0);
	glTexCoord2f(5.0f, 0.0f);
	glVertex3f(-0, -0.01, 25);
	glEnd();
	glBegin(GL_POLYGON);
	glNormal3d(0, 1, 0);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, -0.01, 0);
	glNormal3d(0, 1, 0);
	glTexCoord2f(0.0f, 5.0f);
	glVertex3f(-25, -0.01, 0);
	glNormal3d(0, 1, 0);
	glTexCoord2f(5.0f, 5.0f);
	glVertex3f(-25, -0.01, 25);
	glNormal3d(0, 1, 0);
	glTexCoord2f(5.0f, 0.0f);
	glVertex3f(0, -0.01, 25);
	glEnd();
	glBegin(GL_POLYGON);
	glNormal3d(0, 1, 0);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-0, -0.01, -0);
	glNormal3d(0, 1, 0);
	glTexCoord2f(0.0f, 5.0f);
	glVertex3f(25, -0.01, -0);
	glNormal3d(0, 1, 0);
	glTexCoord2f(5.0f, 5.0f);
	glVertex3f(25, -0.01, -25);
	glNormal3d(0, 1, 0);
	glTexCoord2f(5.0f, 0.0f);
	glVertex3f(-0, -0.01, -25);
	glEnd();
	glBegin(GL_POLYGON);
	glNormal3d(0, 1, 0);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-0, -0.01, -0);
	glNormal3d(0, 1, 0);
	glTexCoord2f(0.0f, 5.0f);
	glVertex3f(-25, -0.01, -0);
	glNormal3d(0, 1, 0);
	glTexCoord2f(5.0f, 5.0f);
	glVertex3f(-25, -0.01, -25);
	glNormal3d(0, 1, 0);
	glTexCoord2f(5.0f, 0.0f);
	glVertex3f(-0, -0.01, -25);
	glEnd();

	//wall
	glBindTexture(GL_TEXTURE_2D, texturas[pared]);
	glBegin(GL_POLYGON);
	glNormal3f(0, 0, 1);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-25, 0, -25);
	glNormal3f(0, 0, 1);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(25, 0, -25);
	glNormal3f(0, 0, 1);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(25, 25, -25);
	glNormal3f(0, 0, 1);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-25, 25, -25);
	glEnd();
	glBegin(GL_POLYGON);
	glNormal3f(0, 0, -1);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-25, 0, 25);
	glNormal3f(0, 0, -1);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(25, 0, 25);
	glNormal3f(0, 0, -1);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(25, 25, 25);
	glNormal3f(0, 0, -1);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-25, 25, 25);
	glEnd();
	glBegin(GL_POLYGON);
	glNormal3f(1, 0, 0);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-25, 0, -25);
	glNormal3f(1, 0, 0);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-25, 0, 25);
	glNormal3f(1, 0, 0);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-25, 25, 25);
	glNormal3f(1, 0, 0);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-25, 25, -25);
	glEnd();
	glBegin(GL_POLYGON);
	glNormal3f(-1, 0, 0);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(25, 0, -25);
	glNormal3f(-1, 0, 0);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(25, 0, 25);
	glNormal3f(-1, 0, 0);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(25, 25, 25);
	glNormal3f(-1, 0, 0);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(25, 25, -25);
	glEnd();

		//wall
		glBegin(GL_POLYGON);
		glColor4f(0.0f, 0.2f, 0.7f, 1.0f);
		glNormal3f(0, 0, 1);
		glVertex3f(-25, 0, -25);
		glNormal3f(0, 0, 1);
		glVertex3f(25, 0, -25);
		glNormal3f(0, 0, 1);
		glVertex3f(25, 25, -25);
		glNormal3f(0, 0, 1);
		glVertex3f(-25, 25, -25);
		glEnd();
		glBegin(GL_POLYGON);
		glColor4f(0.0f, 0.2f, 0.7f, 1.0f);
		glNormal3f(0, 0, -1);
		glVertex3f(-25, 0, 25);
		glNormal3f(0, 0, -1);
		glVertex3f(25, 0, 25);
		glNormal3f(0, 0, -1);
		glVertex3f(25, 25, 25);
		glNormal3f(0, 0, -1);
		glVertex3f(-25, 25, 25);
		glEnd();
		glBegin(GL_POLYGON);
		glColor4f(0.0f, 0.2f, 0.7f, 1.0f);
		glNormal3f(1, 0, 0);
		glVertex3f(-25, 0, -25);
		glNormal3f(1, 0, 0);
		glVertex3f(-25, 0, 25);
		glNormal3f(1, 0, 0);
		glVertex3f(-25, 25, 25);
		glNormal3f(1, 0, 0);
		glVertex3f(-25, 25, -25);
		glEnd();
		glBegin(GL_POLYGON);
		glColor4f(0.0f, 0.2f, 0.7f, 1.0f);
		glNormal3f(-1, 0, 0);
		glVertex3f(25, 0, -25);
		glNormal3f(-1, 0, 0);
		glVertex3f(25, 0, 25);
		glNormal3f(-1, 0, 0);
		glVertex3f(25, 25, 25);
		glNormal3f(-1, 0, 0);
		glVertex3f(25, 25, -25);
		glEnd();

		//NURBS (opcional y funciona y todo)
		/*float i, j;
		glPushMatrix();
		glRotatef(25.0, 1.0, 1.0, 1.0);
		for (j = 0; j <= 8; j+=0.01) {
			glBegin(GL_LINE_STRIP);
			for (i = 0; i <= 30; i+=0.01)
				glEvalCoord2f((GLfloat)i / 30.0, (GLfloat)j / 8.0);
			glEnd();
			glBegin(GL_LINE_STRIP);
			for (i = 0; i <= 30; i+=0.01)
				glEvalCoord2f((GLfloat)j / 8.0, (GLfloat)i / 30.0);
			glEnd();
		}
		glPopMatrix();*/


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
	//glOrtho(-1.0, 1.0f, -1.0, 1.0f, -100.0, 100.0f);

	InitWindow(W_WIDTH, W_HEIGHT);
	preparaCamara();

	//SUBSTITUIR EL FICHERO PARA CARGAR DIFERENTES MODELOS

	//loadObj((char*)"modelos/pato.obj");

	//Cargamos suelo y paredes
	pared = initTex("models/wall/wall.jpg");
	suelo = initTex("models/floor/floor.jpg");


	HONK = new Modelo((char*)"models\\Honk\\", (char*)"honk");
	botella = new Modelo((char*)"models/bottle/", (char*)"bottle");
	mesa = new Modelo((char*)"models/old_wooden_table/", (char*)"old_wooden_table");
	silla = new Modelo((char*)"models/chair/", (char*)"Chair");
	tenedor = new Modelo((char*)"models/fork/", (char*)"fork");
	cuchillo = new Modelo((char*)"models/Knife/", (char*)"knife");
	plato = new Modelo((char*)"models/plate/", (char*)"plate");
	manzana = new Modelo((char*)"models/apples/", (char*)"apple");
	limon = new Modelo((char*)"models/lemon/", (char*)"lemon");
	naranja = new Modelo((char*)"models/orange/", (char*)"orange");
	tasso = new Modelo((char*)"models/glass/", (char*)"glass");
	puerta = new Modelo((char*)"models/wooden_door/", (char*)"wooden_door");

	//marco = &amarco;

	//NURBS
	/*GLfloat ctrlpoints[4][4][3] = {
		 {{-1.5, 1.0, -1.5}, {-0.5, 1.0,-1.5 }, {0.5, 1.0, -1.5 }, {1.5, 1.0,-1.5}},
		 {{-1.5, 1.0, -0.5}, {-0.5, 2.0,-0.5 }, {0.5, 2.0, -0.5 }, {1.5, 1.0,-0.5}},
		 {{-1.5, 1.0,  0.5}, {-0.5, 2.0, 0.5 }, {0.5, 2.0,  0.5 }, {1.5, 1.0, 0.5}},
		 {{-1.5, 1.0,  1.5}, {-0.5, 1.0, 1.5 }, {0.5, 1.0,  1.5 }, {1.5, 1.0, 1.5}}
	};
	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4,
		0, 1, 12, 4, &ctrlpoints[0][0][0]);
	glEnable(GL_MAP2_VERTEX_3);
	glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);*/

	//Fog
	glEnable(GL_FOG);
	{
		glFogi(GL_FOG_MODE, GL_EXP);
		glFogfv(GL_FOG_COLOR, fogColor);
		glFogf(GL_FOG_DENSITY, fogDensity);
		glHint(GL_FOG_HINT, GL_DONT_CARE);
		glFogf(GL_FOG_START, 1.0f);
		glFogf(GL_FOG_END, 5.0f);
	}

	// Comienza la ejecuci�n del core de GLUT
	glutMainLoop();
	return 0;
}

