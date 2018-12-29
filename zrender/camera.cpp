// Camera class for storing and maintaining location-based info for a single camera
// Created by Matthew Zyle on 29.12.18

#include "camera.h"
#include <stdio.h>
#include <stdlib.h>
#include <gl/glut.h>
#include <math.h>
#include <Eigen/Dense>

using namespace Eigen;

#define ON  1
#define OFF 0
#define PI 3.14159265


camera::camera()
{
	initCamera();
}


camera::~camera()
{
}


// Initialize camera settings
void camera::initCamera() {

	radius = 10.0f;
	theta = 0.0f;
	psi = PI / 2;
	viewCenter << 0, 0, 0;
	up << 0, 0.01, 10;
	location << 0, 0, 10;

}


// Print the current camera location info
void camera::printCameraLoc() {

	printf("Location: %f %f %f; Viewing: %f %f %f; Up: %f %f %f; Radius = %f\n", 
		location(0), location(1), location(2), 
		viewCenter(0), viewCenter(1), viewCenter(2), 
		up(0), up(1), up(2), 
		radius);

}


// Move camera forward
void camera::moveCamForward() {

	Vector3f moveDirection = viewCenter - location;
	moveDirection.normalize();
	location += camSensitivity * moveDirection;
	viewCenter += camSensitivity * moveDirection;
	up += camSensitivity * moveDirection;
	printCameraLoc();

}


// Move camera backward
void camera::moveCamBackward() {

	Vector3f moveDirection = location - viewCenter;
	moveDirection.normalize();
	location += camSensitivity * moveDirection;
	viewCenter += camSensitivity * moveDirection;
	up += camSensitivity * moveDirection;
	printCameraLoc();

}


// Move camera left
void camera::moveCamLeft() {

	Vector3f viewDirection = viewCenter - location;
	Vector3f upDirection = up - location;
	Vector3f moveDirection = upDirection.cross(viewDirection);
	moveDirection.normalize();
	location += camSensitivity * moveDirection;
	viewCenter += camSensitivity * moveDirection;
	up += camSensitivity * moveDirection;
	printCameraLoc();

}


// Move camera right
void camera::moveCamRight() {

	Vector3f viewDirection = viewCenter - location;
	Vector3f upDirection = up - location;
	Vector3f moveDirection = viewDirection.cross(upDirection);
	moveDirection.normalize();
	location += camSensitivity * moveDirection;
	viewCenter += camSensitivity * moveDirection;
	up += camSensitivity * moveDirection;
	printCameraLoc();

}


// Move camera up
void camera::moveCamUp() {

	Vector3f upDirection = up - location;
	upDirection.normalize();
	location += camSensitivity * upDirection;
	viewCenter += camSensitivity * upDirection;
	up += camSensitivity * upDirection;
	printCameraLoc();

}


// Move camera down
void camera::moveCamDown() {

	Vector3f downDirection = location - up;
	downDirection.normalize();
	location += camSensitivity * downDirection;
	viewCenter += camSensitivity * downDirection;
	up += camSensitivity * downDirection;
	printCameraLoc();

}


// Rotate camera left
void camera::rotCamLeft() {

	theta -= angleSensitivity;
	location -= viewCenter;
	location << radius * sin(theta) * sin(psi), radius * cos(psi), radius * cos(theta) * sin(psi);
	location += viewCenter;
	up -= viewCenter;
	up << radius * sin(theta) * sin(psi), 1 + radius * cos(psi), radius * cos(theta) * sin(psi);
	up += viewCenter;
	printCameraLoc();

}


// Rotate camera right
void camera::rotCamRight() {

	theta += angleSensitivity;
	location -= viewCenter;
	location << radius * sin(theta) * sin(psi), radius * cos(psi), radius * cos(theta) * sin(psi);
	location += viewCenter;
	up -= viewCenter;
	up << radius * sin(theta) * sin(psi), 1 + radius * cos(psi), radius * cos(theta) * sin(psi);
	up += viewCenter;
	printCameraLoc();

}


// Rotate camera up
void camera::rotCamUp() {

	psi -= angleSensitivity;
	if (psi <= 0.1) psi = 0.1;
	location -= viewCenter;
	location << radius * sin(theta) * sin(psi), radius * cos(psi), radius * cos(theta) * sin(psi);
	location += viewCenter;

	float upRad = sqrt(0.01 + pow(radius, 2));
	float upPsi = psi - tan(0.01 / radius);
	up -= viewCenter;
	up << upRad * sin(theta) * sin(upPsi), upRad * cos(upPsi), upRad * cos(theta) * sin(upPsi);
	up += viewCenter;
	printCameraLoc();

}


// Rotate camera down
void camera::rotCamDown() {

	psi += angleSensitivity;
	if (psi >= PI - 0.1) psi = PI - 0.1;
	location -= viewCenter;
	location << radius * sin(theta) * sin(psi), radius * cos(psi), radius * cos(theta) * sin(psi);
	location += viewCenter;

	float upRad = sqrt(0.01 + pow(radius, 2));
	float upPsi = psi - tan(0.01 / radius);
	up -= viewCenter;
	up << upRad * sin(theta) * sin(upPsi), upRad * cos(upPsi), upRad * cos(theta) * sin(upPsi);
	up += viewCenter;
	printCameraLoc();

}


// Zoom in
void camera::zoomIn() {

	radius -= zoomSensitivity;
	if (radius > 0.01) {
		Vector3f zoomDirection = viewCenter - location;
		zoomDirection.normalize();
		location += zoomSensitivity * zoomDirection;
		up += zoomSensitivity * zoomDirection;
	}
	else {
		radius = 0.01;
	}
	printCameraLoc();

}


// Zoom out
void camera::zoomOut() {

	radius += zoomSensitivity;
	Vector3f zoomDirection = viewCenter - location;
	zoomDirection.normalize();
	location -= zoomSensitivity * zoomDirection;
	up -= zoomSensitivity * zoomDirection;
	printCameraLoc();

}
