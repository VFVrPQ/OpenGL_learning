
//author:VFVrPQ
//time:
#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// angle of rotation for the camera direction
float angle = 0.0f;

// actual vector representing the camera's direction
float lx = 0.0f, lz = -1.0f;

// XZ position of the camera
float x = 0.0f, z = 5.0f;

#include "Arrow.h"//箭头的头文件
#include "Jitter.h"//抖动效果
#include "Bowstring.h"//弓箭弦的头文件
AABB aabb;//用于碰撞检测
Arrow mArrow;//箭头的结构体
Bowstring bowstring;

// the key states. These variables will be zero
// when no key is being pressesed
float deltaAngle = 0.0f;
float deltaMove = 0;
int xOrigin = -1;

// color for the snowman's nose
float red = 1.0f, blue = 0.5f, green = 0.5f;

// scale of snowman
float scale = 1.0f;

// default font
void *font = GLUT_STROKE_ROMAN;

// width and height of the window
int h, w;

// variables to compute frames per second
int frame;
long time, timebase;
char s[60];
char currentMode[80];

// this string keeps the last good setting
// for the game mode
char gameModeString[40] = "640x480";

void init();

void changeSize(int ww, int hh) {

	h = hh;
	w = ww;
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	float ratio = w * 1.0 / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}
#pragma warning(disable : 4996)
void renderBitmapString(
	float x,
	float y,
	float z,
	void *font,
	char *string) {

	char *c;
	glRasterPos3f(x, y, z);
	for (c = string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}

void renderStrokeFontString(
	float x,
	float y,
	float z,
	void *font,
	char *string) {

	char *c;
	glPushMatrix();
	glTranslatef(x, y, z);
	glScalef(0.002f, 0.002f, 0.002f);
	for (c = string; *c != '\0'; c++) {
		glutStrokeCharacter(font, *c);
	}
	glPopMatrix();
}

void restorePerspectiveProjection() {

	glMatrixMode(GL_PROJECTION);
	// restore previous projection matrix
	glPopMatrix();

	// get back to modelview mode
	glMatrixMode(GL_MODELVIEW);
}

void setOrthographicProjection() {

	// switch to projection mode
	glMatrixMode(GL_PROJECTION);

	// save previous matrix which contains the
	//settings for the perspective projection
	glPushMatrix();

	// reset matrix
	glLoadIdentity();

	// set a 2D orthographic projection
	gluOrtho2D(0, w, h, 0);

	// switch back to modelview mode
	glMatrixMode(GL_MODELVIEW);
}

void computePos(float deltaMove) {

	x += deltaMove * lx * 0.1f;
	z += deltaMove * lz * 0.1f;
}
void getCorners(Vec3*dst, Vec3 _min, Vec3 _max) {
	// 朝着Z轴正方向的面

	// 左上顶点坐标
	dst[0].set(_min.x, _max.y, _max.z);
	// 左下顶点坐标
	dst[1].set(_min.x, _min.y, _max.z);
	// 右下顶点坐标
	dst[2].set(_max.x, _min.y, _max.z);
	// 右上顶点坐标
	dst[3].set(_max.x, _max.y, _max.z);

	// 朝着Z轴负方向的面

	// 右上顶点坐标
	dst[4].set(_max.x, _max.y, _min.z);
	// 右下顶点坐标.
	dst[5].set(_max.x, _min.y, _min.z);
	// 左下顶点坐标.
	dst[6].set(_min.x, _min.y, _min.z);
	// 左上顶点坐标.
	dst[7].set(_min.x, _max.y, _min.z);
}
void drawCuboid(Vec3 _min,Vec3 _max) {
	aabb.add(_min, _max);//增加物体
	glColor3f(1.0f, 0.0f, 0.0f);

	Vec3 dst[8];
	getCorners(dst,_min,_max);
	for (int i = 0; i < 8; i++) {
		glBegin(GL_LINES);
		glVertex3f(dst[i].x, dst[i].y, dst[i].z);
		int j = (i + 1) % 8;
		glVertex3f(dst[j].x, dst[j].y, dst[j].z);
		glEnd();
	}

	glBegin(GL_LINES);
	glVertex3f(dst[0].x, dst[0].y, dst[0].z);
	glVertex3f(dst[3].x, dst[3].y, dst[3].z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(dst[2].x, dst[2].y, dst[2].z);
	glVertex3f(dst[5].x, dst[5].y, dst[5].z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(dst[4].x, dst[4].y, dst[4].z);
	glVertex3f(dst[7].x, dst[7].y, dst[7].z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(dst[1].x, dst[1].y, dst[1].z);
	glVertex3f(dst[6].x, dst[6].y, dst[6].z);
	glEnd();
}
int lineState = 0, lineNum = 0;
void renderScene(void) {

	if (deltaMove)
		computePos(deltaMove);

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();
	// Set the camera
	gluLookAt(x, 1.0f, z,
		x + lx, 1.0f, z + lz,
		0.0f, 1.0f, 0.0f);

	mArrow.move(Vec3(x + lx, 1.0f, z + lz),aabb);
	mArrow.drawStick(aabb);
	// Draw ground
	
	glColor3f(0.9f, 0.9f, 0.9f);
	glBegin(GL_QUADS);
	glVertex3f(-100.0f, 0.0f, -100.0f);
	glVertex3f(-100.0f, 0.0f, 100.0f);
	glVertex3f(100.0f, 0.0f, 100.0f);
	glVertex3f(100.0f, 0.0f, -100.0f);
	glEnd();

	aabb.clear();
	glColor3f(0.0f, 1.0f, 1.0f);
	for (int i = -3; i < 3; i++) {
		for (int j = -3; j < 3; j++) {
			drawCuboid(Vec3(i*10.0f, 0.0f, j*10.0f), Vec3(i*10.0f+1.0f, 1.0f, j*10.0f+1.0f));
		}
	}

	//抖动效果（fps有点高，抖动一下子就没了），Jitter
	//在（0，0.5，50)可以按d看到抖动
	const int lineNumMax = 66;
	Vec2 line1 = Vec2(0.0f, 0.5f);float lineZ1 = 50.0f;
	Vec2 line2 = Vec2(0.0f, 0.5f);float lineZ2 = -50.0f;
	if (lineState && lineNum < lineNumMax) {
		line1 = Jitter::next(line1,lineNum);
		lineNum++;
	}
	glColor3f(0, 1.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex3f(line1.x, line1.y, lineZ1);
	glVertex3f(line2.x, line2.y, lineZ2);
	glEnd();


	//弓箭的弦，bowstring
	bowstring.clear();
	bowstring.add(Vec3(0.0f, 0.5f, 5.0f));
	bowstring.add(Vec3(10.0f, 0.5f, 0.0f));
	bowstring.add(Vec3(0.0f, 0.5f, -5.0f));
	bowstring.getCurve();
	bowstring.draw();


	// Code to compute frames per second
	frame++;

	time = glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		sprintf_s(s, "Lighthouse3D - FPS:%4.2f",
			frame*1000.0 / (time - timebase));
		timebase = time;
		frame = 0;
	}

	setOrthographicProjection();
	void *font = GLUT_BITMAP_8_BY_13;
	glPushMatrix();
	glLoadIdentity();
	renderBitmapString(30, 15, 0, font, (char *)"GLUT Tutorial @ Lighthouse3D");
	renderBitmapString(30, 30, 0, font, s);
	renderBitmapString(30, 45, 0, font, (char *)"F1 - Game Mode  640x480 32 bits");
	renderBitmapString(30, 60, 0, font, (char *)"F2 - Game Mode  800x600 32 bits");
	renderBitmapString(30, 75, 0, font, (char *)"F3 - Game Mode 1024x768 32 bits");
	renderBitmapString(30, 90, 0, font, (char *)"F4 - Game Mode 1280x1024 32 bits");
	renderBitmapString(30, 105, 0, font, (char *)"F5 - Game Mode 1920x1200 32 bits");
	renderBitmapString(30, 120, 0, font, (char *)"F6 - Window Mode");
	renderBitmapString(30, 135, 0, font, (char *)"Esc - Quit");
	renderBitmapString(30, 150, 0, font, currentMode);
	char tmp[100],tmp2[100],tmp3[100];
	sprintf_s(tmp, "position:%f %f %f\n", mArrow.position.x, mArrow.position.y, mArrow.position.z);
	sprintf_s(tmp2, "speed:%f %f %f\n",    mArrow.speed.x,	 mArrow.speed.y,    mArrow.speed.z);
	sprintf_s(tmp3, "state:%d\n", mArrow.state);
	renderBitmapString(30, 175, 0, font, (char *)tmp);
	renderBitmapString(30, 190, 0, font, (char *)tmp2);
	renderBitmapString(30, 205, 0, font, (char *)tmp3);
	glPopMatrix();

	restorePerspectiveProjection();

	glutSwapBuffers();
}

// -----------------------------------
//             KEYBOARD
// -----------------------------------

void processNormalKeys(unsigned char key, int xx, int yy) {
	switch (key) {
	case 27:
		if (glutGameModeGet(GLUT_GAME_MODE_ACTIVE) != 0)
			glutLeaveGameMode();
		exit(0);
		break;
	case 's'://shoot
		mArrow.setSpeed(Vec3((lx + 0.1)*10.0f, 5.0f, (lz + 0.1)*10.0f));
		mArrow.shoot(Vec3((lx+0.1)*10.0f, 5.0f, (lz+0.1)*10.0f));
		break;
	case 'c'://create
		mArrow.create(Vec3(x + lx, 1.0f, z + lz));
		break;
	case 'd'://Jitter
		lineState = 1,lineNum = 0;
		break;
	}
}

void pressKey(int key, int xx, int yy) {

	switch (key) {
	case GLUT_KEY_UP: deltaMove = 0.5f; break;
	case GLUT_KEY_DOWN: deltaMove = -0.5f; break;
	case GLUT_KEY_F1:
		// define resolution, color depth
		glutGameModeString("640x480:32");
		// enter full screen
		if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) {
			glutEnterGameMode();
			sprintf_s(gameModeString, "640x480:32");
			// register callbacks again
			// and init OpenGL context
			init();
		}
		else
			glutGameModeString(gameModeString);
		break;
	case GLUT_KEY_F2:
		// define resolution, color depth
		glutGameModeString("800x600:32");
		// enter full screen
		if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) {
			glutEnterGameMode();
			sprintf_s(gameModeString, "800x600:32");
			// register callbacks again
			// and init OpenGL context
			init();
		}
		else
			glutGameModeString(gameModeString);
		break;
	case GLUT_KEY_F3:
		// define resolution, color depth
		glutGameModeString("1024x768:32");
		// enter full screen
		if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) {
			glutEnterGameMode();
			sprintf_s(gameModeString, "1024x768:32");
			// register callbacks again
			// and init OpenGL context
			init();
		}
		else
			glutGameModeString(gameModeString);
		break;
	case GLUT_KEY_F4:
		// define resolution, color depth
		glutGameModeString("1280x1024:32");
		// enter full screen
		if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) {
			glutEnterGameMode();
			sprintf_s(gameModeString, "1280x1024:32");
			// register callbacks again
			// and init OpenGL context
			init();
		}
		else
			glutGameModeString(gameModeString);
		break;
	case GLUT_KEY_F5:
		// define resolution, color depth
		glutGameModeString("1920x1200");
		// enter full screen
		if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) {
			glutEnterGameMode();
			sprintf_s(gameModeString, "1920x1200");
			// register callbacks again
			// and init OpenGL context
			init();
		}
		else
			glutGameModeString(gameModeString);
		break;
	case GLUT_KEY_F6:
		// return to default window
		w = 800; h = 600;
		if (glutGameModeGet(GLUT_GAME_MODE_ACTIVE) != 0) {
			glutLeaveGameMode();
			//init();
		}
		break;
	}
	if (glutGameModeGet(GLUT_GAME_MODE_ACTIVE) == 0)
		sprintf_s(currentMode, "Current Mode: Window");
	else
		sprintf_s(currentMode,
			"Current Mode: Game Mode %dx%d at %d hertz, %d bpp",
			glutGameModeGet(GLUT_GAME_MODE_WIDTH),
			glutGameModeGet(GLUT_GAME_MODE_HEIGHT),
			glutGameModeGet(GLUT_GAME_MODE_REFRESH_RATE),
			glutGameModeGet(GLUT_GAME_MODE_PIXEL_DEPTH));
}

void releaseKey(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
	case GLUT_KEY_DOWN: deltaMove = 0; break;
	}
}

// -----------------------------------
//             MOUSE
// -----------------------------------

void mouseMove(int x, int y) {

	// this will only be true when the left button is down
	if (xOrigin >= 0) {

		// update deltaAngle
		deltaAngle = (x - xOrigin) * 0.001f;

		// update camera's direction
		lx = sin(angle + deltaAngle);
		lz = -cos(angle + deltaAngle);
	}
}

void mouseButton(int button, int state, int x, int y) {
	// only start motion if the left button is pressed
	if (button == GLUT_LEFT_BUTTON) {

		// when the button is released
		if (state == GLUT_UP) {
			angle += deltaAngle;
			xOrigin = -1;
		}
		else {// state = GLUT_DOWN
			xOrigin = x;
		}
	}
}

void init() {

	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);

	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);

	// OpenGL init
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

// -----------------------------------
//             MAIN
// -----------------------------------

//在（0，0.5，50)可以按d看到抖动
//按c呼出小球，按s射出小球
//上下走路，按住鼠标左键转方向
int main(int argc, char **argv) {

	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Lighthouse3D - GLUT Tutorial");

	// register callbacks
	init();

	// enter GLUT event processing cycle
	glutMainLoop();

	return 1;
}