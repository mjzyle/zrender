// Mesh object class
// Created by Matthew Zyle on 29.12.18

#include "obj.h"


obj::obj()
{
}


obj::~obj()
{
}


// Determine face normals
void obj::getFaceNorms() {

	faceNorms = (Vector3f*)malloc(sizeof(Vector3f) * faceCount);
	for (int i = 0; i < faceCount; i++) {
		Vector3f x(faces[i].b(0) - faces[i].a(0), faces[i].b(1) - faces[i].a(1), faces[i].b(2) - faces[i].a(2));
		Vector3f y(faces[i].c(0) - faces[i].a(0), faces[i].c(1) - faces[i].a(1), faces[i].c(2) - faces[i].a(2));
		Vector3f temp = x.cross(y);
		temp.normalize();
		faceNorms[i] = temp;
	}

}


// Determine vertex normals
void obj::getVertNorms() {

	vertNorms = (Vector3f*)malloc(sizeof(Vector3f) * vertCount);
	for (int i = 0; i < vertCount; i++) {
		int j = 0, k = 0;
		Vector3f borderNorms[3];
		while (k < 3) {
			if (faces[j].a == verts[i] || faces[j].b == verts[i] || faces[j].c == verts[i]) {
				borderNorms[k] << faceNorms[j](0), faceNorms[j](1), faceNorms[j](2);
				k++;
			}
			j++;
		}
		Vector3f temp = borderNorms[0] + borderNorms[1] + borderNorms[2];
		temp.normalize();
		vertNorms[i] = temp;
	}

}
