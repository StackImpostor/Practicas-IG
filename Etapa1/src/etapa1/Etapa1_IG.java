package etapa1;

import com.jogamp.opengl.GL2;
import com.jogamp.opengl.GLAutoDrawable;
import com.jogamp.opengl.GLCapabilities;
import com.jogamp.opengl.GLEventListener;
import com.jogamp.opengl.GLProfile;
import com.jogamp.opengl.awt.GLCanvas;
import com.jogamp.opengl.glu.GLU;
import com.jogamp.opengl.util.FPSAnimator;
import java.awt.BorderLayout;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import javax.swing.JFrame;

public class Etapa1_IG implements GLEventListener {

    public static final int DIMENSION = 500;
    private float fAngulo = 0;

    /**
     * Etapa uno para JOGL 2.4.0
     * @param args the command line arguments
     */
    public static void main(String[] args) {

        //Se indica que se trabajará con GL2
        //Es decir OpenGL 3
        GLProfile profile = GLProfile.get(GLProfile.GL2);
        System.setProperty("sun.awt.noerasebackground", "true");
        GLCapabilities capabilities = new GLCapabilities(profile);
        capabilities.setDoubleBuffered(true);
        //Se genera un render para OpenGL
        GLCanvas canvas = new GLCanvas(capabilities);
        canvas.addGLEventListener(new Etapa1_IG());
        JFrame frame = new JFrame();
        frame.getContentPane().add(canvas, BorderLayout.CENTER);
        frame.setSize(DIMENSION, DIMENSION);
        FPSAnimator animator = new FPSAnimator(canvas,60);

        frame.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                /**
                 * Se ejecuta en otro thread para que AWT pueda estar seguro que
                 * se cierra todo antes de salir
                 */
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        animator.stop();
                        frame.dispose();
                        System.exit(0);
                    }
                }).start();
            }
        });

        frame.setLocationRelativeTo(null);
        frame.setVisible(true);
        animator.start();

    }

    @Override
    public void init(GLAutoDrawable glad) {
        GL2 gl = glad.getGL().getGL2();
        // Enable VSync
        gl.setSwapInterval(1);
        // Setup the drawing area and shading mode
        gl.glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        gl.glShadeModel(GL2.GL_SMOOTH); // try setting this to GL_FLAT and see what happens.
    }



    @Override
    public void display(GLAutoDrawable glad) {
        GL2 gl = glad.getGL().getGL2();

        //Limpia la área de dibujo
        gl.glClear(GL2.GL_COLOR_BUFFER_BIT | GL2.GL_DEPTH_BUFFER_BIT);

        //Reiniciar la matriz a la identidad
        gl.glLoadIdentity();

        gl.glPushMatrix();
        // Rotamos las próximas primitivas
        gl.glRotatef(fAngulo, 0.0f, 0.0f, 1.0f);
        // Dibujamos los tres polígonos
        gl.glBegin(GL2.GL_POLYGON);
            gl.glColor3f(1.0f, 1.0f, 1.0f);
            gl.glVertex3f(0.0f, 0.0f, 0.0f);
            gl.glColor3f(0.0f, 1.0f, 0.0f);
            gl.glVertex3f(1.0f, 0.0f, 0.0f);
            gl.glColor3f(0.0f, 1.0f, 0.0f);
            gl.glVertex3f(-0.5f, 0.866f, 0.0f);
        gl.glEnd();

        gl.glBegin(GL2.GL_POLYGON);
            gl.glColor3f(1.0f, 1.0f, 1.0f);
            gl.glVertex3f(0.0f, 0.0f, 0.0f);
            gl.glColor3f(1.0f, 0.0f, 0.0f);
            gl.glVertex3f(1.0f, 0.0f, 0.0f);
            gl.glColor3f(0.0f, 0.0f, 1.0f);
            gl.glVertex3f(-0.5f, -0.866f, 0.0f);
        gl.glEnd();

        gl.glBegin(GL2.GL_POLYGON);
            gl.glColor3f(1.0f, 1.0f, 1.0f);
            gl.glVertex3f(0.0f, 0.0f, 0.0f);
            gl.glColor3f(0.0f, 1.0f, 1.0f);
            gl.glVertex3f(-0.5f, 0.866f, 0.0f);
            gl.glColor3f(0.0f, 0.0f, 1.0f);
            gl.glVertex3f(-0.5f, -0.866f, 0.0f);
        gl.glEnd();

        gl.glPopMatrix();

        fAngulo += 1f;
        // Si es mayor que dos pi decrementamos el ángulo
        if (fAngulo > 360) {
            fAngulo -= 360;
        }
        //Repintamos
        gl.glFlush();

    }

    @Override
    public void reshape(GLAutoDrawable glad, int x, int y, int width, int height) {
        GL2 gl = glad.getGL().getGL2();
        GLU glu = new GLU();
        if (height <= 0) { // avoid a divide by zero error!
            height = 1;
        }
        //Realizar operaciones
    }

    
    @Override
    public void dispose(GLAutoDrawable glad) {
    }
}
