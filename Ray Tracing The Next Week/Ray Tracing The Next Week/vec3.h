#pragma once
#ifndef VEC3_H
#define VEC3_H

#include <iostream>
#include "Utility.h"
class vec3 {
public:
	vec3() :e{ 0,0,0 } {}
	vec3(double e1, double e2, double e3) :e{ e1,e2,e3 } {}
	double x() const { return e[0]; }
	double y() const { return e[1]; }
	double z() const { return e[2]; }
	vec3 operator-() const { return vec3{ -e[0],-e[1],-e[2] }; }
	double operator[](int i) const {
		switch (i)
		{
		case 0:
		case 1:
		case 2:return e[i]; break;
		default:std::cerr << "vector index error!" << std::flush;
			break;
		}
	}
	double& operator[](int i) {
		switch (i)
		{
		case 0:
		case 1:
		case 2:return e[i]; break;
		default:std::cerr << "vector index error!" << std::flush;
			break;
		}
	}
	vec3& operator+=(const vec3& src) {
		e[0] += src.e[0];
		e[1] += src.e[1];
		e[2] += src.e[2];
		return *this;
	}
	vec3& operator*=(const double src) {
		e[0] *= src;
		e[1] *= src;
		e[2] *= src;
		return *this;
	}
	vec3& operator/=(const double src) {
		return *this *= (1 / src);
	}
	double length()const {
		return sqrt(length_squared());
	}
	double length_squared()const {
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}
	void write_color(std::ostream& os) {
		os << static_cast<int>(255.999f * e[0]) << " "
			<< static_cast<int>(255.999f * e[1]) << " "
			<< static_cast<int>(255.999f * e[2]) << "\n";
	}
private:
	double e[3];
};

using Color = vec3;
using Point3 = vec3;

inline std::ostream& operator<<(std::ostream& os, const vec3& v) {
	auto r = sqrt(v[0]);//gamma correct
	auto g = sqrt(v[1]);
	auto b = sqrt(v[2]);
	//std::cout << v[0] << " " << v[1] << " " << v[2] << " \n";
	//std::cout << r << " " << g << " " << b << " \n";
	return os << static_cast<int>(256 * clamp(r, 0.0, 0.999)) <<
		' ' << static_cast<int>(256 * clamp(g, 0.0, 0.999)) <<
		' ' << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << "\n";
}

inline vec3 operator+(const vec3& v1, const vec3& v2) {
	return vec3(v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2]);
}

inline vec3 operator-(const vec3& v1, const vec3& v2) {
	return vec3(v1[0] - v2[0], v1[1] - v2[1], v1[2] - v2[2]);
}

inline vec3 operator*(const vec3& v1, const vec3& v2) {//数乘，而非点乘
	return vec3(v1[0] * v2[0], v1[1] * v2[1], v1[2] * v2[2]);
}

inline vec3 operator*(double t, const vec3& v) {
	return vec3(t * v[0], t * v[1], t * v[2]);
}

inline vec3 operator*(const vec3& v, double t) {
	return t * v;
}

inline vec3 operator/(const vec3& v, double t) {
	return (1 / t) * v;
}

inline double dot(const vec3& u, const vec3& v) {
	return u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
}

inline vec3 cross(const vec3& u, const vec3& v) {
	return vec3(u[1] * v[2] - u[2] * v[1],
		u[2] * v[0] - u[0] * v[2],
		u[0] * v[1] - u[1] * v[0]);
}

inline vec3 unit_vector(const vec3& v) {
	return 1 / v.length() * v;
}

inline static vec3 random_vec3() {
	return vec3(random_double(), random_double(), random_double());
}

inline static vec3 random_vec3(double min, double max) {
	return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
}

inline vec3 random_unit_vec3() {
	auto a = random_double(0, 2 * Pi);
	auto z = random_double(-1, 1);
	auto r = sqrt(1 - z * z);
	return vec3(r * cos(a), r * sin(a), z);
}

vec3 random_in_unit_disk() {
	while (true)
	{
		vec3 p = vec3(random_double(), random_double(), 0);
		if (p.length_squared() >= 1)continue;
		return p;
	}
}

inline vec3 reflect(const vec3& v_in, const vec3& nor) {
	return v_in - 2 * dot(v_in, nor) * nor;
}

inline vec3 refract(const vec3& v_in, const vec3& nor,double etaI_over_etaO) {
	auto cos_theta = dot(-v_in, nor);
	auto r_out_parallel = etaI_over_etaO * (v_in + nor * cos_theta);
	auto r_out_perp = -sqrt(abs(1.0 - r_out_parallel.length_squared())) * nor;
	return r_out_parallel + r_out_perp;
}



#endif // !VEC3_H
