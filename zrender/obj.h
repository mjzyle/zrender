#include <Eigen/Dense>

using namespace Eigen;

#define ON  1
#define OFF 0

// Face subclass
typedef struct _face {
	Vector4f a, b, c;
} face;

#pragma once
class obj
{
public:
	Vector4f *verts;
	face *faces;
	Vector3f *faceNorms, *vertNorms;
	int vertCount, faceCount;
	Vector3f translate;

	obj();
	~obj();

	void getFaceNorms();
	void getVertNorms();
};
