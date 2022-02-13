// Vector class (2D -> x, y | 3D -> x, y, z)
#pragma once
#include "global.h"

class vec2d {
public:
	vec2d();
	vec2d(float x, float y);

	// Getters and setters
	float get_x();
	void set_x(float x);
	float get_y();
	void set_y(float y);

	vec2d operator+(vec2d b);
	vec2d operator*(float a);

	vec2d operator-(vec2d b);
	vec2d operator/(float a);

	float get_len();
	vec2d get_norm();
	vec2d get_nrm2d();

	float dot_pr(vec2d b); // Dot product
	float crs_pr(vec2d b); // Cross product (magnitude)

	~vec2d();
protected:
	float x, y;
};

class vec3d {
public:
	vec3d();
	vec3d(float x, float y, float z);
	
	// Getters and setters
	float get_x();
	void set_x(float x);
	float get_y();
	void set_y(float y);
	float get_z();
	void set_z(float z);

	vec3d operator+(vec3d b);
	vec3d operator*(float a);

	vec3d operator-(vec3d b);
	vec3d operator/(float a);

	float get_len();
	vec3d get_norm();

	float dot_pr(vec3d b); // Dot product

	~vec3d();
protected:
	float x, y, z;
};