#include <Eigen/Dense>

using namespace Eigen;

#define ON  1
#define OFF 0

#pragma once
class camera
{
public:
	float camSensitivity = 0.05f;
	float angleSensitivity = 0.05f;
	float zoomSensitivity = 0.05f;
	float radius, theta, psi;
	int camRotate = ON;
	int lastX, lastY;
	Vector3f viewCenter, location, up;

	camera();
	~camera();
	
	void initCamera();
	void printCameraLoc();
	void moveCamForward();
	void moveCamBackward();
	void moveCamLeft();
	void moveCamRight();
	void moveCamUp();
	void moveCamDown();
	void rotCamLeft();
	void rotCamRight();
	void rotCamUp();
	void rotCamDown();
	void zoomIn();
	void zoomOut();
};

