#include "vec.h"

vec2d::vec2d() {

	x = .0f;
	y = .0f;

}

vec2d::vec2d(float x, float y) {

	this->x = x;
	this->y = y;

}

float vec2d::get_x() {

	return x;

}

void vec2d::set_x(float x) {

	this->x = x;

}

float vec2d::get_y() {

	return y;

}

void vec2d::set_y(float y) {

	this->y = y;

}

vec2d vec2d::operator+(vec2d b) {

	return vec2d(x + b.get_x(), y + b.get_y());

}

vec2d vec2d::operator*(float a) {

	return vec2d(x * a, y * a);

}

vec2d vec2d::operator-(vec2d b) {

	return *this + b * -1.f;

}

vec2d vec2d::operator/(float a) {

	return *this * (1.f / a);

}

float vec2d::get_len() {

	if (x == .0f && y == .0f)
		return .0f;

	return std::sqrtf(x * x + y * y);

}

vec2d vec2d::get_norm() {

	if (get_len() == 0)
		return vec2d(0, 0);

	return *this / get_len();

}

vec2d vec2d::get_nrm2d() {

	return vec2d(y, -x);

}

float vec2d::dot_pr(vec2d b) {

	return x * b.get_x() + y * b.get_y();

}

float vec2d::crs_pr(vec2d b) {

	return x * b.get_y() - y * b.get_x();

}

vec2d::~vec2d() {

	

}

vec3d::vec3d() {

	x = .0f;
	y = .0f;
	z = .0f;

}

vec3d::vec3d(float x, float y, float z) {

	this->x = x;
	this->y = y;
	this->z = z;

}

// Getters and setters
float vec3d::get_x() {

	return x;

}

void vec3d::set_x(float x) {

	this->x = x;

}

float vec3d::get_y() {

	return y;

}

void vec3d::set_y(float y) {
	
	this->y = y;

}

float vec3d::get_z() {

	return z;

}

void vec3d::set_z(float z) {

	this->z = z;

}

vec3d vec3d::operator+(vec3d b) {

	return vec3d(x + b.get_x(), y + b.get_y(), z + b.get_z());

}

vec3d vec3d::operator*(float a) {

	return vec3d(x * a, y * a, z * a);

}

vec3d vec3d::operator-(vec3d b) {

	return *this + b * -1.f;

}

vec3d vec3d::operator/(float a) {

	return *this * (1.f / a);

}

float vec3d::get_len() {

	if (x == .0f && y == .0f && z == .0f)
		return .0f;

	return std::sqrtf(x * x + y * y + z * z);

}

vec3d vec3d::get_norm() {

	if (get_len() == 0)
		return vec3d(0, 0, 0);

	return *this / get_len();

}

float vec3d::dot_pr(vec3d b) {

	return x * b.get_x() + y * b.get_y() + z * b.get_z();

}

vec3d::~vec3d() {

	

}