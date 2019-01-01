// Mesh object class
// Created by Matthew Zyle on 29.12.18

#include "obj.h"
#include <iostream>


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


// Determine if two vertices are equal
bool obj::vertsEqual(Vector3f a, Vector3f b) {

	return a(0) == b(0) && a(1) == b(1) && a(2) == b(2);

}


// Determine vertex normals
void obj::getVertNorms() {

	vertNorms = (Vector3f*)malloc(sizeof(Vector3f) * vertCount);
	for (int i = 0; i < vertCount; i++) {
		int j = 0, k = 0;
		Vector3f borderNorms[3];
		while (k < 3 && j < faceCount) {
			if (vertsEqual(faces[j].a, verts[i]) || vertsEqual(faces[j].b, verts[i]) || vertsEqual(faces[j].c, verts[i])) {
				borderNorms[k] = faceNorms[j];
				k++;
			}
			j++;
		}
		Vector3f temp = borderNorms[0] + borderNorms[1] + borderNorms[2];
		temp.normalize();
		vertNorms[i] = temp;
	}

}

/*
// Determine face centroids
void obj::getFaceCenters() {

	faceCenters = (Vector3f*)malloc(sizeof(Vector3f) * faceCount);
	for (int i = 0; i < faceCount; i++) {
		// Determine the median lines from two vertices to the opposite face segments
		Vector3f midAB, midAC, ABtoC, ACtoB;                          
		midAB = faces[i].a + (faces[i].b - faces[i].a) / 2;
		midAC = faces[i].a + (faces[i].c - faces[i].a) / 2;
		ABtoC = faces[i].c - midAB;
		ACtoB = faces[i].b - midAC;

		// Determine the intersection point of the two lines by solving for the line constants (c)
		MatrixXf a(3, 3);
		VectorXf b(3), c(3);
		a << ABtoC(0), -ACtoB(0), 0.0f,
			 ABtoC(1), -ACtoB(1), 0.0f,
			 ABtoC(2), -ACtoB(2), 0.0f;
		b << midAC(0) - midAB(0),
			 midAC(1) - midAB(1),
			 midAC(2) - midAB(2);
		c = a.inverse() * b;

		// Solve for the intersection point
		faceCenters[i] = c(0, 0) * ABtoC + midAB;
	}
	
}
*/
