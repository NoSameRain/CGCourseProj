#pragma once
#define _USE_MATH_DEFINES
#include<cmath>                 
#include "memory.h"
#define SQ(x) (x) * (x)
#define max(a,b)(a>b?a:b)
#define max_tri(a, b, c) ((a > b ? a : b) > c ? (a > b ? a : b) : c)
#define min(a,b)(a<b?a:b)
#define min_tri(a, b, c) ((a < b ? a : b) < c ? (a < b ? a : b) : c)
#define M_PI       3.14159265358979323846

const float _near = 0.1;
const float _far = 300;

template<typename T>
static T lerp(const T a, const T b, float t)
{
	return a * (1.0f - t) + (b * t);
}

template <typename T>
T clamp(T value, T min, T max) {
	return (value < min) ? min : (value > max) ? max : value;
}

//-------------------------------Vec3--------------------------------
class Vec3
{
public:
	union
	{
		float v[3];
		struct { float x, y, z; };
	};
	Vec3() {
		for (int i = 0; i < 3; i++) v[i] = 0.f;
	}
	Vec3(float _x, float _y, float _z) {
		v[0] = _x;
		v[1] = _y;
		v[2] = _z;
	}
	Vec3 operator*(const float val) const
	{
		return Vec3(v[0] * val, v[1] * val, v[2] * val);
	}
	Vec3 operator-() const //?
	{
		return Vec3(-v[0], -v[1], -v[2]);
	}
	Vec3 operator+(const Vec3& v)const { return Vec3(x + v.x, y + v.y, z + v.z); }
	Vec3 operator-(const Vec3& v)const { return Vec3(x - v.x, y - v.y, z - v.z); }
	Vec3 operator*(const Vec3& v)const { return Vec3(x * v.x, y * v.y, z * v.z); }
	Vec3 operator/(const Vec3& v)const { return Vec3(x / v.x, y / v.y, z / v.z); }
	Vec3& operator+=(const Vec3& v) { x += v.x, y += v.y, z += v.z; return *this; }
	Vec3& operator-=(const Vec3& v) { x -= v.x, y -= v.y, z -= v.z; return *this; }
	Vec3& operator*=(const Vec3& v) { x *= v.x, y *= v.y, z *= v.z; return *this; }
	Vec3& operator/=(const Vec3& v) { x /= v.x, y /= v.y, z /= v.z; return *this; }
	Vec3& operator=(const Vec3& v) { x = v.x, y = v.y, z = v.z; return *this; }
	//bool operator<(const Vec3& v) { float len1 = this->Length(); float len2 = v.Length(); return len1<len2; }
	//bool operator>(const Vec3& v) { float len1 = this->Length(); float len2 = v.Length(); return len1>len2; }

	float distance(const Vec3& v) const {
		Vec3 delta = Vec3(v.x - x, v.y - y, v.z - z);
		return sqrtf(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
	}
	float Length() const {
		return SQ(x) + SQ(y) + SQ(z);
	}
	float squareLength() const {
		return sqrtf(SQ(x) + SQ(y) + SQ(z));
	}
	Vec3 normalize()
	{
		float len = 1.0f / sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
		return Vec3(x * len, y * len, z * len);
	}

	float normalize_GetLength()
	{
		float length = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
		float len = 1.0f / length;
		v[0] *= len; v[1] *= len; v[2] *= len;
		return length;
	}
	float dot(const Vec3& pVec) const
	{
		return v[0] * pVec.v[0] + v[1] * pVec.v[1] + v[2] * pVec.v[2];
	}
	Vec3 cross(const Vec3& v1)
	{
		return Vec3(v1.v[1] * v[2] - v1.v[2] * v[1],
			v1.v[2] * v[0] - v1.v[0] * v[2],
			v1.v[0] * v[1] - v1.v[1] * v[0]);
	}
	Vec3 Max(const Vec3& v1, const Vec3& v2)
	{
		return Vec3(max(v1.v[0], v2.v[0]),
			max(v1.v[1], v2.v[1]),
			max(v1.v[2], v2.v[2]));
	}
	Vec3 Min(const Vec3& v1, const Vec3& v2)
	{
		return Vec3(min(v1.v[0], v2.v[0]),
			min(v1.v[1], v2.v[1]),
			min(v1.v[2], v2.v[2]));
	}

};
static Vec3 cross(const Vec3& v1, const Vec3& v2)
{
	return Vec3(v1.v[1] * v2.v[2] - v1.v[2] * v2.v[1],
		v1.v[2] * v2.v[0] - v1.v[0] * v2.v[2],
		v1.v[0] * v2.v[1] - v1.v[1] * v2.v[0]);
}

//-------------------------------Vec4--------------------------------
class Vec4
{
public:
	union
	{
		float v[4];
		struct { float x, y, z, w; };
	};
	Vec4() {
		for (int i = 0; i < 4; i++) v[i] = 0.f;
	}
	Vec4(float _x, float _y, float _z, float _w) {
		v[0] = _x;
		v[1] = _y;
		v[2] = _z;
		v[3] = _w;
	}
	Vec4 operator-() const //?
	{
		return Vec4(-v[0], -v[1], -v[2], -v[3]);
	}
	Vec4 operator+(const Vec4& v)const { return Vec4(x + v.x, y + v.y, z + v.z, w + v.w); }
	Vec4 operator-(const Vec4& v)const { return Vec4(x - v.x, y - v.y, z - v.z, w - v.w); }
	Vec4 operator*(const Vec4& v)const { return Vec4(x * v.x, y * v.y, z * v.z, w * v.w); }
	Vec4 operator/(const Vec4& v)const { return Vec4(x / v.x, y / v.y, z / v.z, w / v.w); }
	Vec4 operator/(const float val)const { return Vec4(x / val, y / val, z / val, w / val); }
	Vec4& operator+=(const Vec4& v) { x += v.x, y += v.y, z += v.z, w += v.w; return *this; }
	Vec4& operator-=(const Vec4& v) { x -= v.x, y -= v.y, z -= v.z, w -= v.w; return *this; }
	Vec4& operator*=(const Vec4& v) { x *= v.x, y *= v.y, z *= v.z, w *= v.w; return *this; }
	Vec4& operator/=(const Vec4& v) { x /= v.x, y /= v.y, z /= v.z, w /= v.w; return *this; }
	Vec4& operator/=(const float val) { x /= val, y /= val, z /= val, w /= val; return *this; }

	float Length() const {
		return SQ(x) + SQ(y) + SQ(z) + SQ(w);
	}
	float squareLength() const {
		return sqrtf(SQ(x) + SQ(y) + SQ(z) + SQ(w));
	}
	Vec4 normalize(void)
	{
		float len = 1.0f / sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3]);
		return Vec4(x * len, y * len, z * len, w * len);
	}

};

class Matrix // row major
{
public:
	union
	{
		float a[4][4];
		float m[16];
	};
	Matrix(float* otherM) {
		memcpy(&m, otherM, 16 * sizeof(float));
	}
	Matrix() {
		a[0][0] = 0.f;
		a[0][1] = 0.f;
		a[0][2] = 0.f;
		a[0][3] = 0.f;
		a[1][0] = 0.f;
		a[1][1] = 0.f;
		a[1][2] = 0.f;
		a[1][3] = 0.f;
		a[2][0] = 0.f;
		a[2][1] = 0.f;
		a[2][2] = 0.f;
		a[2][3] = 0.f;
		a[3][0] = 0.f;
		a[3][1] = 0.f;
		a[3][2] = 0.f;
		a[3][3] = 0.f;
	}

	Matrix(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33) {
		a[0][0] = m00;
		a[0][1] = m01;
		a[0][2] = m02;
		a[0][3] = m03;
		a[1][0] = m10;
		a[1][1] = m11;
		a[1][2] = m12;
		a[1][3] = m13;
		a[2][0] = m20;
		a[2][1] = m21;
		a[2][2] = m22;
		a[2][3] = m23;
		a[3][0] = m30;
		a[3][1] = m31;
		a[3][2] = m32;
		a[3][3] = m33;
	}
	void identity() {
		memset(m, 0, 16 * sizeof(float));
		m[0] = 1.0f;
		m[5] = 1.0f;
		m[10] = 1.0f;
		m[15] = 1.0f;
	}
	float& operator[](int index) {
		return m[index];
	}
	Matrix operator*(const Matrix& matrix)
	{
		return mul(matrix);
	}

	static Matrix worldMatrix(const Vec3& v_t, const Vec3& v_s, const Vec3& v_r) {
		Matrix translation = translation.translation(v_t);
		Matrix scaling = scaling.scaling(v_s);
		Matrix rotateX = rotateX.rotateX(v_r.x);
		Matrix rotateY = rotateY.rotateY(v_r.y);
		Matrix rotateZ = rotateZ.rotateZ(v_r.z);
		Matrix rotation = rotateX * rotateY * rotateZ;
		return(scaling * rotation * translation);
		//return (translation* rotation* scaling);
	}

	//---------------trans scale rotate----------------------------------------------
	static Matrix translation(const Vec3& v) {
		Matrix mat;
		mat.identity();  // Set up an identity matrix
		mat.a[0][3] = v.x;
		mat.a[1][3] = v.y;
		mat.a[2][3] = v.z;
		return mat;
	}
	static Matrix scaling(const Vec3& v) {
		Matrix mat;
		mat.identity();  // Set up an identity matrix
		mat.m[0] = v.x;
		mat.m[5] = v.y;
		mat.m[10] = v.z;
		return mat;
	}
	static Matrix rotateX(float theta) {
		Matrix mat;
		mat.identity();  

		float ct = cosf(theta);
		float st = sinf(theta);

		mat.a[1][1] = ct;
		mat.a[1][2] = -st;
		mat.a[2][1] = st;
		mat.a[2][2] = ct;

		return mat;
	}

	static Matrix rotateY(float theta) {
		Matrix mat;
		mat.identity();  // Set up an identity matrix

		float ct = cosf(theta);
		float st = sinf(theta);

		// Fill in rotation matrix for Y axis
		mat.a[0][0] = ct;
		mat.a[0][2] = st;
		mat.a[2][0] = -st;
		mat.a[2][2] = ct;

		return mat;
	}

	static Matrix rotateZ(float theta) {
		Matrix mat;
		mat.identity();  // Set up an identity matrix

		float ct = cosf(theta);
		float st = sinf(theta);

		// Fill in rotation matrix for Z axis
		mat.a[0][0] = ct;
		mat.a[0][1] = -st;
		mat.a[1][0] = st;
		mat.a[1][1] = ct;

		return mat;
	}
	//----------------------------mul---------------------------------------------
	Vec3 mulPoint(const Vec3& v)
	{
		return Vec3(
			(v.x * m[0] + v.y * m[1] + v.z * m[2]) + m[3],
			(v.x * m[4] + v.y * m[5] + v.z * m[6]) + m[7],
			(v.x * m[8] + v.y * m[9] + v.z * m[10]) + m[11]);
	}

	Vec3 mulVec(const Vec3& v)
	{
		return Vec3(
			(v.x * m[0] + v.y * m[1] + v.z * m[2]),
			(v.x * m[4] + v.y * m[5] + v.z * m[6]),
			(v.x * m[8] + v.y * m[9] + v.z * m[10]));
	}
	Vec4 mulVec(const Vec4& v)
	{
		return Vec4(
			(v.x * m[0] + v.y * m[1] + v.z * m[2] + v.w * m[3]),
			(v.x * m[4] + v.y * m[5] + v.z * m[6] + v.w * m[7]),
			(v.x * m[8] + v.y * m[9] + v.z * m[10] + v.w * m[11]),
			(v.x * m[12] + v.y * m[13] + v.z * m[14] + v.w * m[15]));
	}
	Matrix mul(const Matrix& matrix) const
	{
		Matrix ret;
		ret.m[0] = m[0] * matrix.m[0] + m[4] * matrix.m[1] + m[8] * matrix.m[2] + m[12] * matrix.m[3];
		ret.m[1] = m[1] * matrix.m[0] + m[5] * matrix.m[1] + m[9] * matrix.m[2] + m[13] * matrix.m[3];
		ret.m[2] = m[2] * matrix.m[0] + m[6] * matrix.m[1] + m[10] * matrix.m[2] + m[14] * matrix.m[3];
		ret.m[3] = m[3] * matrix.m[0] + m[7] * matrix.m[1] + m[11] * matrix.m[2] + m[15] * matrix.m[3];
		ret.m[4] = m[0] * matrix.m[4] + m[4] * matrix.m[5] + m[8] * matrix.m[6] + m[12] * matrix.m[7];
		ret.m[5] = m[1] * matrix.m[4] + m[5] * matrix.m[5] + m[9] * matrix.m[6] + m[13] * matrix.m[7];
		ret.m[6] = m[2] * matrix.m[4] + m[6] * matrix.m[5] + m[10] * matrix.m[6] + m[14] * matrix.m[7];
		ret.m[7] = m[3] * matrix.m[4] + m[7] * matrix.m[5] + m[11] * matrix.m[6] + m[15] * matrix.m[7];
		ret.m[8] = m[0] * matrix.m[8] + m[4] * matrix.m[9] + m[8] * matrix.m[10] + m[12] * matrix.m[11];
		ret.m[9] = m[1] * matrix.m[8] + m[5] * matrix.m[9] + m[9] * matrix.m[10] + m[13] * matrix.m[11];
		ret.m[10] = m[2] * matrix.m[8] + m[6] * matrix.m[9] + m[10] * matrix.m[10] + m[14] * matrix.m[11];
		ret.m[11] = m[3] * matrix.m[8] + m[7] * matrix.m[9] + m[11] * matrix.m[10] + m[15] * matrix.m[11];
		ret.m[12] = m[0] * matrix.m[12] + m[4] * matrix.m[13] + m[8] * matrix.m[14] + m[12] * matrix.m[15];
		ret.m[13] = m[1] * matrix.m[12] + m[5] * matrix.m[13] + m[9] * matrix.m[14] + m[13] * matrix.m[15];
		ret.m[14] = m[2] * matrix.m[12] + m[6] * matrix.m[13] + m[10] * matrix.m[14] + m[14] * matrix.m[15];
		ret.m[15] = m[3] * matrix.m[12] + m[7] * matrix.m[13] + m[11] * matrix.m[14] + m[15] * matrix.m[15];
		return ret;
	}

	Matrix invert()
	{
		Matrix inv;
		inv[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
		inv[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
		inv[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
		inv[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
		inv[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
		inv[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
		inv[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
		inv[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
		inv[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
		inv[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
		inv[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
		inv[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];
		inv[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];
		inv[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];
		inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];
		inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];
		float det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
		if (det == 0) {
			// Handle this case
			//throw std::runtime_error("Matrix is singular and cannot be inverted.");
		}
		det = 1.0 / det;
		for (int i = 0; i < 16; i++) {
			inv[i] = inv[i] * det;
		}
		return inv;
	}

	void perspectiveProjection(float aspect, float tan) {
		m[0] = 1 / (aspect * tan);
		m[5] = 1 / tan;
		m[10] = -_far / (_far - _near);
		m[11] = -_far * _near / (_far - _near);
		m[14] = -1;
		m[15] = 0;
	}

	Matrix lookAtMat(const Vec3& from, const Vec3& to, const Vec3& up) {
		Matrix lookat;
		Vec3 dir, right, up_;
		dir = from - to;
		dir = dir.normalize();
		right = cross(up, dir).normalize(); //normalize????????????????
		up_ = cross(dir, right).normalize(); //????????????????
		// rotation
		lookat.m[0] = right.x;
		lookat.m[1] = right.y;
		lookat.m[2] = right.z;

		lookat.m[4] = up_.x;
		lookat.m[5] = up_.y;
		lookat.m[6] = up_.z;

		lookat.m[8] = dir.x;
		lookat.m[9] = dir.y;
		lookat.m[10] = dir.z;
		lookat.m[15] = 1.0f;

		// transltion
		lookat.m[3] = -from.dot(right);
		lookat.m[7] = -from.dot(up_);
		lookat.m[11] = -from.dot(dir);
		//std::cout << "from " << from.x << " " << from.y << " " << from.z << std::endl;
		//std::cout<< "dir " << dir.x << " " << dir.y << " " << dir.z << std::endl;
		//std::cout << from.dot(dir) << std::endl;

		return lookat;
	}

};

class Quaternion {
public:
	union {
		struct {
			//q=a+bi+cj+dk
			float a, b, c, d;
		};
		float q[4];
	};
	//constructor
	Quaternion(float _a, float _b, float _c, float _d) : a(_a), b(_b), c(_c), d(_d) {}
	//no rotation - identify
	Quaternion() : a(1.0f), b(0.0f), c(0.0f), d(0.0f) {}

	//magnitude of the quaternion
	float magnitude() const {
		return sqrt(SQ(a) + SQ(b) + SQ(c) + SQ(d));
	}

	//inverse of the quaternion (only valid for unit quaternions)
	Quaternion inverse() const {
		float mag = magnitude();
		if (mag > 0) {
			return Quaternion(a / mag, -b / mag, -c / mag, -d / mag);
		}
	}

	Quaternion normalized() const {
		float mag = magnitude();
		if (mag > 0) {
			return Quaternion(a / mag, b / mag, c / mag, d / mag);
		}
		return Quaternion(1.0f, 0.0f, 0.0f, 0.0f);
	}

	// Slerp (Spherical Linear Interpolation)
	static Quaternion slerp(const Quaternion& q1, const Quaternion& q2, float t) {
		float cos;
		Quaternion q1_ = Quaternion(q1.a, q1.b, q1.c, q1.d);

		cos = q1.a * q2.a + q1.b * q2.b + q1.c * q2.c + q1.d * q2.d;
		if (cos < 0) {
			Quaternion q1_ = Quaternion(-q1.a, -q1.b, -q1.c, -q1.d);
			cos = fabs(cos);
		}

		if (cos > 0.95) {
			return Quaternion(
				(1 - t) * q1_.a + t * q2.a,
				(1 - t) * q1_.b + t * q2.b,
				(1 - t) * q1_.c + t * q2.c,
				(1 - t) * q1_.d + t * q2.d
			).normalized();
		}

		float theta = acos(cos);
		float sinTheta = sinf(theta);
		// sin((1-t)theta) * q1_ / sin theta  +  sin(t*theta) * q2 / sin(theta)
		float a_ = sinf((1 - t) * theta) * q1_.a / sinTheta + sinf(t * theta) * q2.a / sinTheta;
		float b_ = sinf((1 - t) * theta) * q1_.b / sinTheta + sinf(t * theta) * q2.b / sinTheta;
		float c_ = sinf((1 - t) * theta) * q1_.c / sinTheta + sinf(t * theta) * q2.c / sinTheta;
		float d_ = sinf((1 - t) * theta) * q1_.d / sinTheta + sinf(t * theta) * q2.d / sinTheta;

		return Quaternion(a_,b_,c_,d_).normalized();
	}

	// convert quaternion to a 4*4 rotation matrix 
	Matrix toMatrix() const {
		float xx = q[0] * q[0];
		float xy = q[0] * q[1];
		float xz = q[0] * q[2];
		float yy = q[1] * q[1];
		float zz = q[2] * q[2];
		float yz = q[1] * q[2];
		float wx = q[3] * q[0];
		float wy = q[3] * q[1];
		float wz = q[3] * q[2];
		Matrix matrix;
		matrix[0] = 1.0f - 2.0f * (yy + zz);
		matrix[1] = 2.0f * (xy - wz);
		matrix[2] = 2.0f * (xz + wy);
		matrix[3] = 0.0;
		matrix[4] = 2.0f * (xy + wz);
		matrix[5] = 1.0f - 2.0f * (xx + zz);
		matrix[6] = 2.0f * (yz - wx);
		matrix[7] = 0.0;
		matrix[8] = 2.0f * (xz - wy);
		matrix[9] = 2.0f * (yz + wx);
		matrix[10] = 1.0f - 2.0f * (xx + yy);
		matrix[11] = 0.0;
		matrix[12] = 0;
		matrix[13] = 0;
		matrix[14] = 0;
		matrix[15] = 1;

		return matrix;
	}

	// use rotation axis and angle to create Quaternion 
	Quaternion fromAxisAngle(Vec3 _axis, const float angle) const  {
		Vec3 axis = _axis.normalize();
		float halfAngle = angle / 2.f;
		float cosHalfAngle = cosf(halfAngle);
		float sinHalfAngle = sinf(halfAngle);
		return Quaternion(
			cosHalfAngle,
			axis.x * sinHalfAngle,
			axis.y * sinHalfAngle,
			axis.z * sinHalfAngle
		);
	}

	Quaternion operator*(const Quaternion& q) const {
		return Quaternion(
			a * q.a - b * q.b - c * q.c - d * q.d,
			a * q.b + b * q.a + c * q.d - d * q.c,
			a * q.c - b * q.d + c * q.a + d * q.b,
			a * q.d + b * q.c - c * q.b + d * q.a
		);
	}

	Vec3 toEulerAngle() const {
		float pitch;
		if (abs(-2.f * (a * c - d * b)) >= 1) pitch = copysign(M_PI / 2, -2.f * (a * c - d * b));
		else pitch = asin(-2.f * (a * c - d * b));
		float yaw = atan2(2.f * (a * b + c * d), 1.f - 2.f * (SQ(b) + SQ(c)));
		float roll = atan2(2.f * (b * c + d * a), 1.f - 2.f * (SQ(a) + SQ(b)));

		return Vec3(pitch, yaw, roll);
	}

	Quaternion fromEulerAngle(const Vec3 euler) const {
		float sin_pitch = sinf(euler.x / 2.f);
		float cos_pitch = cosf(euler.x / 2.f);
		float sin_yaw = sinf(euler.y / 2.f);
		float cos_yaw = cosf(euler.y / 2.f);
		float sin_roll = sinf(euler.z / 2.f);
		float cos_roll = cosf(euler.z / 2.f);

		return Quaternion(
			cos_yaw* cos_pitch* cos_roll+ sin_pitch* sin_yaw* sin_roll,
			sin_roll * cos_pitch * cos_yaw - cos_roll * sin_pitch * sin_yaw, 
			cos_roll * sin_pitch * cos_yaw + sin_roll * cos_pitch * sin_yaw, 
			cos_roll * cos_pitch * sin_yaw - sin_roll * sin_pitch * cos_yaw  
		);
	}
};





