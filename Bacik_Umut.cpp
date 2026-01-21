/**********************************
 CTIS164 - HOMEWORK #1
-----------------------------------
NAME SURNAME : UMUT BACIK
STUDENT ID   : 22402642
SECTION      : 1
-----------------------------------
**********************************/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#define WINDOW_WIDTH   1000
#define WINDOW_HEIGHT  800

// ---------------- Template Globals ----------------
#define TIMER_PERIOD 50
#define TIMER_ON 1
#define D2R 0.01745329252
#define PI  3.14159265358

#define BUTTON_RADIUS 80

#define STATE_INITIAL 0
#define STATE_TITLE   1
#define STATE_SCENE   2

int appState = STATE_INITIAL;
int winWidth, winHeight;
int glMouseX, glMouseY;

float cloudX = -600;
float vehicleX = -600;

bool up = false, down = false, left = false, right = false;

// ---------- Template primitives ----------
void circle(int x, int y, int r) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i += 3)
        glVertex2f(x + r * cos(i * D2R), y + r * sin(i * D2R));
    glEnd();
}
void circle_wire(int x, int y, int r) {
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 360; i += 3)
        glVertex2f(x + r * cos(i * D2R), y + r * sin(i * D2R));
    glEnd();
}
void vprint(int x, int y, void* font, const char* string, ...) {
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);
    glRasterPos2f((float)x, (float)y);
    for (int i = 0; str[i]; i++)
        glutBitmapCharacter(font, str[i]);
}
void vprint2(int x, int y, float size, const char* string, ...) {
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);
    glPushMatrix();
    glTranslatef((float)x, (float)y, 0);
    glScalef(size, size, 1);
    for (int i = 0; str[i]; i++)
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    glPopMatrix();
}

bool checkCircle(float px, float py, float cx, float cy, float R) {
    float dx = px - cx;
    float dy = py - cy;
    float d2 = dx * dx + dy * dy;
    return d2 <= R * R;
}

void drawButtons() {
    int titleBtnX = -250, btnY = 300;
    int sceneBtnX = 0;
    int exitBtnX = 250;

    bool inTitle = checkCircle(glMouseX, glMouseY, (float)titleBtnX, (float)btnY, BUTTON_RADIUS) || appState == STATE_TITLE;
    bool inScene = checkCircle(glMouseX, glMouseY, (float)sceneBtnX, (float)btnY, BUTTON_RADIUS) || appState == STATE_SCENE;
    bool inExit = checkCircle(glMouseX, glMouseY, (float)exitBtnX, (float)btnY, BUTTON_RADIUS);

    glLineWidth(2);

    // --- TITLE BUTTON ---
    // dark border
    glColor3f(0.3f, 0.0f, 0.0f);
    circle(titleBtnX, btnY, BUTTON_RADIUS + 4);

    // fill
    if (inTitle) glColor3f(1.0f, 0.3f, 0.3f);
    else         glColor3f(0.8f, 0.0f, 0.0f);
    circle(titleBtnX, btnY, BUTTON_RADIUS);

    // inner ring
    glColor3f(0.2f, 0.0f, 0.0f);
    circle_wire(titleBtnX, btnY, BUTTON_RADIUS);

    // label
    if (inTitle) glColor3f(0.1f, 0.1f, 0.1f);
    else         glColor3f(1.0f, 1.0f, 1.0f);
    vprint2(titleBtnX - 55, btnY - 15, 0.3f, "TITLE");


    // --- SCENE BUTTON ---
    glColor3f(0.0f, 0.3f, 0.0f);
    circle(sceneBtnX, btnY, BUTTON_RADIUS + 4);

    if (inScene) glColor3f(0.3f, 1.0f, 0.3f);
    else         glColor3f(0.0f, 0.8f, 0.0f);
    circle(sceneBtnX, btnY, BUTTON_RADIUS);

    glColor3f(0.0f, 0.3f, 0.0f);
    circle_wire(sceneBtnX, btnY, BUTTON_RADIUS);

    if (inScene) glColor3f(0.1f, 0.1f, 0.1f);
    else         glColor3f(1.0f, 1.0f, 1.0f);
    vprint2(sceneBtnX - 60, btnY - 15, 0.3f, "SCENE");


    // --- EXIT BUTTON ---
    glColor3f(0.0f, 0.3f, 0.3f);
    circle(exitBtnX, btnY, BUTTON_RADIUS + 4);

    if (inExit) glColor3f(0.3f, 1.0f, 1.0f);
    else        glColor3f(0.0f, 0.7f, 0.7f);
    circle(exitBtnX, btnY, BUTTON_RADIUS);

    glColor3f(0.0f, 0.3f, 0.3f);
    circle_wire(exitBtnX, btnY, BUTTON_RADIUS);

    if (inExit) glColor3f(0.1f, 0.1f, 0.1f);
    else        glColor3f(1.0f, 1.0f, 1.0f);
    vprint2(exitBtnX - 45, btnY - 15, 0.3f, "EXIT");

    glLineWidth(1);
}


// ---------- Title Screen ----------
void drawTitleScreen() {
    glColor3f(1, 1, 1);
    glRectf(-450, 200, 450, -250);

    glColor3f(0.1, 0.1, 0.1);
    vprint(-160, 150, GLUT_BITMAP_TIMES_ROMAN_24, "BILKENT UNIVERSITY");
    vprint(-320, 100, GLUT_BITMAP_TIMES_ROMAN_24, "        CTIS164-Technical Mathematics with Programming");
    vprint(-120, 50, GLUT_BITMAP_TIMES_ROMAN_24, "    HOMEWORK #1");
    vprint(-140, 0, GLUT_BITMAP_TIMES_ROMAN_24, "      2025-2026 FALL");

    glColor3f(0.8, 0.1, 0.1);
    vprint(-100, -150, GLUT_BITMAP_TIMES_ROMAN_24, "  UMUT BACIK");
    vprint(-90, -200, GLUT_BITMAP_TIMES_ROMAN_24, "  22402642");
}

// ---------- Scene Primitives ----------
void drawSky() {
    glColor3f(0.5, 0.8, 1.0);
    glRectf(-WINDOW_WIDTH / 2, 200, WINDOW_WIDTH / 2, -100);
}
void drawGround() {
    glColor3f(0.2, 0.8, 0.2);
    glRectf(-WINDOW_WIDTH / 2, -100, WINDOW_WIDTH / 2, -WINDOW_HEIGHT / 2);
}
void drawRoad() {
    glColor3f(0.3, 0.3, 0.3);
    glRectf(-WINDOW_WIDTH / 2, -120, WINDOW_WIDTH / 2, -220);
    glColor3f(1, 1, 0);
    for (int x = -480; x < 500; x += 80) glRectf((float)x, -168, (float)x + 40, -172);
}
void drawMountain() {
    glColor3f(0.5, 0.35, 0.05);
    glBegin(GL_TRIANGLES);
    glVertex2f(-400, -100);
    glVertex2f(-150, 180);
    glVertex2f(100, -100);
    glEnd();
    glColor3f(1, 1, 1);
    glBegin(GL_TRIANGLES);
    glVertex2f(-150, 180);
    glVertex2f(-200, 130);
    glVertex2f(-100, 130);
    glEnd();

    glColor3f(0.55, 0.4, 0.1);
    glBegin(GL_TRIANGLES);
    glVertex2f(0, -100);
    glVertex2f(250, 200);
    glVertex2f(500, -100);
    glEnd();
    glColor3f(1, 1, 1);
    glBegin(GL_TRIANGLES);
    glVertex2f(250, 200);
    glVertex2f(200, 150);
    glVertex2f(300, 150);
    glEnd();
}
void drawHouse() {
    glColor3f(0.7, 0.9, 1.0);
    glRectf(200, -100, 380, 50);

    glColor3f(0.8f, 0.2f, 0.1f);
    glBegin(GL_TRIANGLES);
    glVertex2f(180, 50);
    glVertex2f(400, 50);
    glVertex2f(290, 120);
    glEnd();
    glColor3f(0, 0, 0);
    glLineWidth(3);
    glBegin(GL_LINE_LOOP);
    glVertex2f(180, 50);
    glVertex2f(400, 50);
    glVertex2f(290, 120);
    glEnd();
    glLineWidth(1);

    glColor3f(0.6, 0.4, 0.1);
    glRectf(270, -100, 310, -30);
    glColor3f(0, 0, 0);
    circle(300, -65, 3);

    glColor3f(1, 1, 1);
    glRectf(215, -10, 255, 30);
    glRectf(325, -10, 365, 30);
    glColor3f(0.1, 0.1, 0.5);
    glRectf(218, -7, 252, 27);
    glRectf(328, -7, 362, 27);
    glColor3f(1, 1, 1);
    glRectf(233, -10, 237, 30);
    glRectf(215, 8, 255, 12);
    glRectf(343, -10, 347, 30);
    glRectf(325, 8, 365, 12);

    glColor3f(0.4, 0.4, 0.4);
    glBegin(GL_QUADS);
    glVertex2f(280, -120);
    glVertex2f(300, -120);
    glVertex2f(310, -100);
    glVertex2f(270, -100);
    glEnd();
}
void drawCloud(float x, float y) {
    glColor3f(1, 1, 1);
    circle((int)x, (int)y, 25);
    circle((int)(x + 25), (int)y, 30);
    circle((int)(x - 20), (int)y, 20);
    circle((int)(x + 10), (int)(y + 15), 25);
    circle((int)(x + 10), (int)(y - 10), 25);
}
void drawVehicle(float x, float y) {
    // 1) Body
    glColor3f(0.85f, 0.1f, 0.1f);
    glRectf(x, y, x + 120, y + 35);

    // 2) Roof
    glColor3f(0.85f, 0.1f, 0.1f);
    glBegin(GL_POLYGON);
    glVertex2f(x + 20, y + 35);
    glVertex2f(x + 45, y + 55);
    glVertex2f(x + 90, y + 55);
    glVertex2f(x + 105, y + 35);
    glVertex2f(x + 95, y + 35);
    glEnd();

    // 3) Front window
    glColor3f(0.5f, 0.8f, 1.0f);
    glBegin(GL_POLYGON);
    glVertex2f(x + 40, y + 35);
    glVertex2f(x + 50, y + 52);
    glVertex2f(x + 72, y + 52);
    glVertex2f(x + 72, y + 35);
    glEnd();

    // 4) Rear window
    glColor3f(0.5f, 0.8f, 1.0f);
    glBegin(GL_POLYGON);
    glVertex2f(x + 78, y + 35);
    glVertex2f(x + 78, y + 52);
    glVertex2f(x + 97, y + 35);
    glVertex2f(x + 90, y + 35);
    glEnd();

    // 5) Hood
    glColor3f(0.85f, 0.1f, 0.1f);
    glRectf(x + 110, y + 20, x + 130, y + 32);

    // 6) Trunk top
    glColor3f(0.85f, 0.1f, 0.1f);
    glRectf(x - 10, y + 22, x + 8, y + 32);

    // 7) Front bumper
    glColor3f(0.15f, 0.15f, 0.15f);
    glRectf(x + 120, y, x + 132, y + 10);

    // 8) Rear bumper
    glColor3f(0.15f, 0.15f, 0.15f);
    glRectf(x - 12, y, x, y + 10);

    // 9) Front wheel
    glColor3f(0.1f, 0.1f, 0.1f);
    circle((int)(x + 95), (int)(y - 3), 13);

    // 10) Rear wheel
    glColor3f(0.1f, 0.1f, 0.1f);
    circle((int)(x + 30), (int)(y - 3), 13);
}

void drawSceneScreen() {
    drawSky();
    drawGround();
    drawMountain();
    drawRoad();
    drawHouse();
    drawCloud(cloudX, 160);
    drawVehicle(vehicleX, -150);
}

// ---------- Display ----------
void display() {
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    if (appState == STATE_TITLE)
        drawTitleScreen();
    else if (appState == STATE_SCENE)
        drawSceneScreen();

    drawButtons();

    glutSwapBuffers();
}

// ---------- Input ----------
void onKeyDown(unsigned char key, int x, int y) {
    if (key == 27) exit(0);
    glutPostRedisplay();
}
void onKeyUp(unsigned char key, int x, int y) {
    if (key == 27) exit(0);
    glutPostRedisplay();
}
void onSpecialKeyDown(int key, int x, int y) {
    if (key == GLUT_KEY_UP)    up = true;
    if (key == GLUT_KEY_DOWN)  down = true;
    if (key == GLUT_KEY_LEFT)  left = true;
    if (key == GLUT_KEY_RIGHT) right = true;
    glutPostRedisplay();
}
void onSpecialKeyUp(int key, int x, int y) {
    if (key == GLUT_KEY_UP)    up = false;
    if (key == GLUT_KEY_DOWN)  down = false;
    if (key == GLUT_KEY_LEFT)  left = false;
    if (key == GLUT_KEY_RIGHT) right = false;
    glutPostRedisplay();
}
void onClick(int button, int stat, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN) {
        int clickX = x - winWidth / 2;
        int clickY = winHeight / 2 - y;

        int titleBtnX = -250, btnY = 300;
        int sceneBtnX = 0;
        int exitBtnX = 250;

        if (checkCircle(clickX, clickY, (float)titleBtnX, (float)btnY, BUTTON_RADIUS))
            appState = STATE_TITLE;
        else if (checkCircle(clickX, clickY, (float)sceneBtnX, (float)btnY, BUTTON_RADIUS))
            appState = STATE_SCENE;
        else if (checkCircle(clickX, clickY, (float)exitBtnX, (float)btnY, BUTTON_RADIUS))
            exit(0);
    }
    glutPostRedisplay();
}
void onResize(int w, int h) {
    winWidth = w; winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display();
}
void onMoveDown(int x, int y) {
    glutPostRedisplay();
}
void onMove(int x, int y) {
    glMouseX = x - winWidth / 2;
    glMouseY = winHeight / 2 - y;
    glutPostRedisplay();
}

// ---------- Timer ----------
#if TIMER_ON == 1
void onTimer(int v) {
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);

    if (appState == STATE_SCENE) {
        cloudX += 1.0f;
        if (cloudX - BUTTON_RADIUS > winWidth / 2)
            cloudX = -winWidth / 2 - BUTTON_RADIUS;

        vehicleX += 3.0f;
        if (vehicleX > winWidth / 2)
            vehicleX = -winWidth / 2 - 175;
    }
    glutPostRedisplay();
}
#endif

void Init() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

// ---------- Main ----------
int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("CTIS164 HW1 - 2025-2026 FALL - UMUT BACIK");

    glutDisplayFunc(display);
    glutReshapeFunc(onResize);

    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);
    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    glutMouseFunc(onClick);
    glutMotionFunc(onMoveDown);
    glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

    Init();
    glutMainLoop();
    return 0;
}