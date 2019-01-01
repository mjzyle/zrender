#include <Eigen/Dense>

using namespace Eigen;

#define ON  1
#define OFF 0

// Face subclass
typedef struct _face {
	Vector3f a, b, c;
} face;

#pragma once
class obj
{
public:
	Vector3f *verts;
	face *faces;
	Vector3f *faceNorms, *vertNorms, *faceCenters;
	int vertCount, faceCount;
	Vector3f translate;

	obj();
	~obj();

	void getFaceNorms();
	bool vertsEqual(Vector3f a, Vector3f b);
	void getVertNorms();
	void getFaceCenters();
};
