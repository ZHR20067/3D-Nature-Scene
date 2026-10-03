#include <GL/glut.h>
#include <stdlib.h>
#include <math.h> 

float azimuth = 90.0f;
float elevation = 50.0f;
float radius = 15.0f;
int   lastX, lastY;
int   mouseDown = 0;
float cx = 0.0f, cy = 1.0f, cz = 0.0f;

void onSpec(int key, int x, int y);
void onMouse(int button, int state, int x, int y);
void onMotion(int x, int y);

GLfloat camX = 0.0, camY = 3.0, camZ = 15.0;

void init() {
    glClearColor(0.5, 0.8, 0.9, 1.0);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat lightPos[] = { 10.0, 15.0, 10.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

void drawGroundAndRiver() {
    glPushMatrix();
    glColor3f(0.3, 0.8, 0.3);
    glBegin(GL_QUADS);
    glVertex3f(-20.0, 0.0, -20.0);
    glVertex3f(-20.0, 0.0, 20.0);
    glVertex3f(10.0, 0.0, 20.0);
    glVertex3f(10.0, 0.0, -20.0);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.2, 0.5, 0.9);
    glBegin(GL_QUADS);
    glVertex3f(10.0, 0.01, -20.0);
    glVertex3f(10.0, 0.01, 20.0);
    glVertex3f(20.0, 0.01, 20.0);
    glVertex3f(20.0, 0.01, -20.0);
    glEnd();
    glPopMatrix();
}

void drawTree(float x, float z) {
    glPushMatrix();
    glTranslatef(x, 0.0, z);

    glPushMatrix();
    glColor3f(0.5, 0.3, 0.1);
    glTranslatef(0.0, 1.5, 0.0);
    glScalef(0.5, 3.0, 0.5);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.1, 0.6, 0.1);
    glTranslatef(0.0, 3.5, 0.0);
    glutSolidSphere(1.8, 20, 20);
    glPopMatrix();

    glPopMatrix();
}

void drawVaseAndFlower(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);

    glPushMatrix();
    glColor3f(1.0, 0.4, 0.7);
    glRotatef(-90, 1, 0, 0);
    glutSolidCone(0.15, 0.4, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glTranslatef(0.0, 0.4, 0.0);
    glutSolidSphere(0.15, 15, 15);
    glPopMatrix();

    glPopMatrix();
}

void drawTable(float x, float z) {
    glPushMatrix();
    glTranslatef(x, 0.0, z);

    glPushMatrix();
    glColor3f(0.5, 0.3, 0.1);
    glTranslatef(0.0, 0.5, 0.0);
    glScalef(0.4, 1.0, 0.4);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glTranslatef(0.0, 1.05, 0.0);
    glScalef(2.5, 0.1, 2.5);
    glutSolidCube(1.0);
    glPopMatrix();

    drawVaseAndFlower(0.4, 1.1, 0.0);

    glPushMatrix();
    glColor3f(0.8, 0.2, 0.2);
    glTranslatef(-0.4, 1.3, 0.0);
    glutSolidTeapot(0.25);
    glPopMatrix();

    glPopMatrix();
}

void drawChair(float x, float z, float rotation) {
    glPushMatrix();
    glTranslatef(x, 0.0, z);
    glRotatef(rotation, 0, 1, 0);

    glColor3f(1.0, 0.5, 0.0);

    glPushMatrix();
    glTranslatef(0.0, 0.5, 0.0);
    glScalef(1.0, 0.1, 1.0);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 1.0, -0.45);
    glScalef(1.0, 1.0, 0.1);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 0.25, 0.0);
    glScalef(0.8, 0.5, 0.8);
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();
}

void drawFootballGoalAndBall(float x, float z) {
    glPushMatrix();
    glTranslatef(x, 0.0, z);

    glColor3f(0.9, 0.9, 0.9);

    glPushMatrix();
    glTranslatef(2.0, 1.5, 0.0);
    glScalef(0.1, 3.0, 0.1);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-2.0, 1.5, 0.0);
    glScalef(0.1, 3.0, 0.1);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 3.0, 0.0);
    glScalef(4.1, 0.1, 0.1);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glTranslatef(0.0, 0.4, 2.0);
    glutSolidSphere(0.4, 20, 20);
    glPopMatrix();

    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    float rad = radius;
    float az = azimuth * 3.14159f / 180.0f;
    float el = elevation * 3.14159f / 180.0f;

    camX = cx + rad * cos(el) * cos(az);
    camY = cy + rad * sin(el);
    camZ = cz + rad * cos(el) * sin(az);

    gluLookAt(camX, camY, camZ, cx, cy, cz, 0.0, 1.0, 0.0);

    drawGroundAndRiver();

    drawTree(-5.0, -8.0);
    drawTree(4.0, -10.0);
    drawTree(-8.0, 2.0);
    drawTree(6.0, -3.0);
    drawTree(-15.0, 12.0);
    drawTree(2.0, 14.0);
    drawTree(-16.0, -12.0);

    drawTable(0.0, 0.0);
    drawChair(-2.0, 0.0, 90.0);
    drawChair(2.0, 0.0, -90.0);

    drawFootballGoalAndBall(-14.0, -15.0);

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)w / h, 0.1, 200.0);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'w': case 'W':
        radius -= 0.5f;
        if (radius < 1.0f) radius = 1.0f;
        break;
    case 's': case 'S':
        radius += 0.5f;
        break;
    case 'a': case 'A':
        azimuth -= 3.0f;
        break;
    case 'd': case 'D':
        azimuth += 3.0f;
        break;
    case '+': case '=':
        radius -= 0.5f;
        if (radius < 1.0f) radius = 1.0f;
        break;
    case '-': case '_':
        radius += 0.5f;
        break;
    case 'r': case 'R':
        azimuth = 90.0f;
        elevation = 50.0f;
        radius = 15.0f;
        break;
    case 27:
        exit(0);
    }
   glutPostRedisplay();
}

void onSpec(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_LEFT: azimuth -= 3.0f; break;
    case GLUT_KEY_RIGHT: azimuth += 3.0f; break;
    case GLUT_KEY_UP: elevation += 3.0f; if (elevation > 89.9f) elevation = 89.9f; break;
    case GLUT_KEY_DOWN: elevation -= 3.0f; if (elevation < -89.9f) elevation = -89.9f; break;
    }
    glutPostRedisplay();
}

void onMouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        mouseDown = (state == GLUT_DOWN);
        lastX = x; lastY = y;
    }
}

void onMotion(int x, int y)
{
    if (!mouseDown) return;
    azimuth += (x - lastX) * 0.5f;
    elevation += (y - lastY) * 0.5f;
    if (elevation > 89.9f) elevation = 89.9f;
    if (elevation < -89.9f) elevation = -89.9f;
    lastX = x; lastY = y;
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000, 700);
    glutCreateWindow("Park - Final Project");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(onSpec);
    glutMouseFunc(onMouse);
    glutMotionFunc(onMotion);

    glutMainLoop();
    return 0;
}