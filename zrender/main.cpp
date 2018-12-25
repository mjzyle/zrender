// Main method responsible for processing inputs and generating window views
// Created by Matthew Zyle on 24.12.18 (ho ho ho!)
// With credit to Professor Cenk Cavusoglu, CWRU, as certain functions have been adapted from his EECS 366 homework code from Fall 2017

#pragma warning(disable:4996)          // Want to use fopen and fscanf

#include <stdio.h>
#include <stdlib.h>
#include <gl/glut.h>
#include <math.h>
#include <Eigen/Dense>

using namespace Eigen;

#define ON  1
#define OFF 0
#define PI 3.14159265

// Face subclass
typedef struct _face {
	Vector4f a, b, c;
} face;

// Object subclass
typedef struct _obj {
	Vector4f *verts;
	face *faces;
	int vertCount, faceCount;
	Vector3f translate;
} obj;

// Window/view variables
int width, height;
int PERSPECTIVE = ON;

// Camera variables
float camSensitivity = 0.1;
float zoomSensitivity = 0.5;
float radius, theta, psi;
int camRotate = ON;
int lastX, lastY;
Vector3f viewCenter;
Vector3f location;

// Scene variables
int objCount;
obj *objList;
int showAxes = ON;


// Load a single mesh object and store corresponding vertex, face, and texture data
void loadMesh(char *filename, int objIndex) {

	FILE *f;
	f = fopen(filename, "r");
	
	if (f == NULL) {
		printf("Error opening %s\n", filename);
	}
	else {
		// Count vertices and faces
		float x, y, z;
		int a, b, c;
		char letter;
		objList[objIndex].vertCount = 0;
		objList[objIndex].faceCount = 0;
		while (!feof(f)) {
			fscanf(f, "%c %f %f %f\n", &letter, &x, &y, &z);
			if (letter == 'v') objList[objIndex].vertCount++;
			if (letter == 'f') objList[objIndex].faceCount++;
		}
		fclose(f);
		printf("Object %d: verts = %d, faces = %d\n", objIndex, objList[objIndex].vertCount, objList[objIndex].faceCount);

		// Prepare mesh data lists
		objList[objIndex].verts = (Vector4f*)malloc(sizeof(Vector4f) * objList[objIndex].vertCount);
		objList[objIndex].faces = (face*)malloc(sizeof(face) * objList[objIndex].faceCount);
		f = fopen(filename, "r");

		// Read vertices
		for (int i = 0; i < objList[objIndex].vertCount; i++) {
			fscanf(f, "%c %f %f %f\n", &letter, &x, &y, &z);
			objList[objIndex].verts[i] = Vector4f(x, y, z, 1);
		}

		// Read faces
		for (int i = 0; i < objList[objIndex].faceCount; i++) {
			fscanf(f, "%c %d %d %d\n", &letter, &a, &b, &c);
			objList[objIndex].faces[i].a = objList[objIndex].verts[a - 1];
			objList[objIndex].faces[i].b = objList[objIndex].verts[b - 1];
			objList[objIndex].faces[i].c = objList[objIndex].verts[c - 1];
		}

		fclose(f);
		printf("Successfully read file %s\n", filename);
	}
}


// Load an entire scene of objects from a scene file
void loadScene(char *filename) {

	FILE *f;
	f = fopen(filename, "r");

	if (f == NULL) {
		printf("Error opening %s\n", filename);
	}
	else {
		// Count objects
		float x, y, z;
		char letter;
		char objName[256];
		while (!feof(f)) {
			fscanf(f, "%c %s %f %f %f\n", &letter, objName, &x, &y, &z);
			if (letter == 'o') objCount++;
		}
		fclose(f);
		printf("Total objects: %d\n", objCount);

		// Prepare object data list
		objList = (obj*)malloc(sizeof(obj) * objCount);
		f = fopen(filename, "r");

		// Read individual object data
		for (int i = 0; i < objCount; i++) {
			fscanf(f, "%c %s %f %f %f\n", &letter, objName, &x, &y, &z);
			loadMesh(objName, i);
			objList[i].translate << x, y, z;
		}

		fclose(f);
	}
}


// Primary rendering method
void render() {

	// Clear BG
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Determine projection mode
	if (PERSPECTIVE) {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60, (GLdouble)width / height, 0.01, 10000);      // Set view frustum
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	else {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-2.5, 2.5, -2.5, 2.5, -10000, 10000);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	// Set camera position
	gluLookAt(location(0), location(1), location(2), viewCenter(0), viewCenter(1), viewCenter(2), 0, 1, 0);

	// Draw scene
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	for (int i = 0; i < objCount; i++) {
		glColor3f(0, 1, 0);
		glTranslatef(objList[i].translate(0), objList[i].translate(1), objList[i].translate(2));
		for (int j = 1; j < objList[i].faceCount; j++) {
			glBegin(GL_TRIANGLES);
			glVertex3f(objList[i].faces[j].a(0), objList[i].faces[j].a(1), objList[i].faces[j].a(2));
			glVertex3f(objList[i].faces[j].b(0), objList[i].faces[j].b(1), objList[i].faces[j].b(2));
			glVertex3f(objList[i].faces[j].c(0), objList[i].faces[j].c(1), objList[i].faces[j].c(2));
			glEnd();
		}
		glTranslatef(-objList[i].translate(0), -objList[i].translate(1), -objList[i].translate(2));
	}

	// Draw coordinate axes
	if (showAxes) {
		// Draw the coordinate axes
		// x-axis in green
		glColor3f(0, 1, 0);
		glBegin(GL_LINES);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(1.0, 0.0, 0.0);
		glEnd();
		// y-axis in red
		glColor3f(1, 0, 0);
		glBegin(GL_LINES);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 1.0, 0.0);
		glEnd();
		// z-axis in blue
		glColor3f(0, 0, 1);
		glBegin(GL_LINES);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 1.0);
		glEnd();
	}

	glutSwapBuffers();

}


// Process screen resizing
void resize(int x, int y) {

	glViewport(0, 0, x, y);
	width = x;
	height = y;
	if (PERSPECTIVE) {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60, (GLdouble)width / height, 0.01, 10000);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	printf("Screen resized to dimensions %d x %d\n", x, y);

}


// Update the camera location coordinates after a rotational change occurs
void updateCameraLocation() {

	location << radius * sin(theta) * sin(psi), radius * cos(psi), radius * cos(theta) * sin(psi);

}


// Process mouse button input
void mouseButton(int button, int state, int x, int y) {

	// Left click rotates camera
	if (button == 0 && state == 0) {
		camRotate = ON;
	}

	// Right click zooms camera
	if (button == 2 && state == 0) {
		camRotate = OFF;
	}

}


// Process mouse movement input
void mouseMotion(int x, int y) {

	// Left click rotates camera
	if (camRotate) {
		if (lastX > x) {
			theta += camSensitivity;
		}
		if (lastY > y && psi < PI - 0.1) {
			psi += camSensitivity;
		}
		if (lastX < x) {
			theta -= camSensitivity;
		}
		if (lastY < y && psi > 0.1) {
			psi -= camSensitivity;
		}
	}
	
	// Right click zooms camera
	else {
		if (lastY > y && radius > 0.1) {
			radius -= zoomSensitivity;
		}
		if (lastY < y) {
			radius += zoomSensitivity;
		}
	}

	lastX = x; lastY = y;
	updateCameraLocation();
	glutPostRedisplay();

}


// Initialize camera
void initCamera() {

	radius = 10;
	theta = 0;
	psi = PI / 2;
	viewCenter << 0, 0, 0;
	updateCameraLocation();

}


// Move camera forward
void moveCamForward() {

	Vector3f moveDirection(viewCenter(0) - location(0), viewCenter(1) - location(1), viewCenter(2) - location(2));
	moveDirection.normalize();
	location += camSensitivity * moveDirection;
	viewCenter += camSensitivity * moveDirection;

}


// Move camera backward
void moveCamBackward() {

	Vector3f moveDirection(location(0) - viewCenter(0),location(1) - viewCenter(1), location(2) - viewCenter(2));
	moveDirection.normalize();
	location += camSensitivity * moveDirection;
	viewCenter += camSensitivity * moveDirection;

}


// Move camera left
void moveCamLeft() {

	Vector3f viewDirection(viewCenter(0) - location(0), viewCenter(1) - location(1), viewCenter(2) - location(2));
	Vector3f moveDirection(0, 0, 1);
	moveDirection.normalize();
	location += camSensitivity * moveDirection;
	viewCenter += camSensitivity * moveDirection;

}


// Move camera right
void moveCamRight() {



}


// Move camera up
void moveCamUp() {



}


// Move camera down
void moveCamDown() {



}


// Process keyboard input
void keyboard(unsigned char key, int x, int y) {

	switch (key) {
		case '`': case '~': exit(1); break;                                                        // Close window
		case 'p': case 'P': if (PERSPECTIVE) PERSPECTIVE = OFF; else PERSPECTIVE = ON;      break; // Toggle projection/perspective view
		case 'o': case 'O': if (showAxes)       showAxes = OFF; else    showAxes = ON;      break; // Toggle coordinate axes
		case 'w': case 'W': moveCamForward();  break; // Move forward
		case 's': case 'S': moveCamBackward(); break; // Move back
		case 'a': case 'A': moveCamLeft();     break; // Move left
		case 'd': case 'D': moveCamRight();    break; // Move right
		case 'r': case 'R': moveCamUp();       break; // Move up
		case 'f': case 'F': moveCamDown();     break; // Move down
		case '1':           initCamera();      break; // Reset camera
	}

	glutPostRedisplay();

}


// Window setup using GLUT
void initWindow(int argc, char *argv[]) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("ZRender");
	glutDisplayFunc(render);
	glutReshapeFunc(resize);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMotion);
	glutKeyboardFunc(keyboard);

}


// Initialize OpenGL settings
void initGL() {

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-2.5, 2.5, -2.5, 2.5, -10000, 10000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);

}


// Main setup method
int main(int argc, char *argv[]) {

	initWindow(argc, argv);
	initGL();
	initCamera();
	loadScene("scene.txt");

	glutMainLoop();
	return 0;

}