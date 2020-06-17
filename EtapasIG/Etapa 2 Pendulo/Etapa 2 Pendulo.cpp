// Etapa2.cpp
// Fichero principal
////////////////////////////////////////////////////
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
const int W_WIDTH = 700; // Tamaño incial de la ventana
const int W_HEIGHT = 700;
GLfloat fAngulo1; // Variable que indica el �ngulo de rotaci�n de los ejes.
GLfloat fAngulo2;
GLfloat incA1 = 0.6f, incA2 = 1.2f;

void myResize(int width, int height) {
    glScalef(1, 1, 1);
}

// Función que visualiza la escena OpenGL
void Display(void)
{
    // Borramos la escena
    glClear(GL_COLOR_BUFFER_BIT);

    glPushMatrix();
    glRotatef(fAngulo1, 0, 0, 1);

    glBegin(GL_QUADS);
    glColor3f(0, 0, 0);
    glVertex2f(-0.005f, 0.0f);
    glVertex2f(0.005f, 0.0f);
    glVertex2f(0.005f, -0.5f);
    glVertex2f(-0.005f, -0.5f);
    glVertex2f(1, 1);
    glEnd();

    glPushMatrix();

    glTranslatef(0, -0.5, 0);
    glRotatef(fAngulo2, 0, 0, 1);
    glTranslatef(0, 0.5, 0);

    glBegin(GL_QUADS);
    glColor3f(0, 1, 0);
    glVertex2f(-0.005f, -0.5f);
    glVertex2f(0.005f, -0.5f);
    glVertex2f(0.005f, -1.0f);
    glVertex2f(-0.005f, -1.0f);
    glVertex2f(1, 1);
    glEnd();

    glPopMatrix();

    glPopMatrix();

    glutSwapBuffers();
}

// Función que se ejecuta cuando el sistema no esta ocupado
void Idle(void)
{
    // Incrementamos el ángulo
    fAngulo1 += incA1;
    if (fAngulo1 > 360)
        fAngulo1 -= 360;

    if (fAngulo1 > 70 || fAngulo1 < -70)
        incA1 *= -1;

    fAngulo2 += incA2;
    if (fAngulo2 > 360)
        fAngulo2 -= 360;

    // Indicamos que es necesario repintar la pantalla
    glutPostRedisplay();
}

// Función principal
int main(int argc, char** argv)
{
    // Inicializamos la librería GLUT
    glutInit(&argc, argv);

    // Indicamos como ha de ser la nueva ventana
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(W_WIDTH, W_HEIGHT);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

    // Creamos la nueva ventana
    glutCreateWindow("Etapa 2 pendulo");

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