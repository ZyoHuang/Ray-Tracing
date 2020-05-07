#pragma once
#ifndef UTILITY_H
#define UTILITY_H
#include <memory>
#include <numeric>
#include <cmath>
#include <limits>
#include <cstdlib>
#include <random>
#include <functional>

using std::shared_ptr;
using std::make_shared;

const double infinity = std::numeric_limits<double>::infinity();
const double Pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees) {
	return degrees * Pi / 180;
}

inline double ffmin(double a, double b) { return a <= b ? a : b; }
inline double ffmax(double a, double b) { return a >= b ? a : b; }

inline double random_double() {//returns a random real in [0,1)
	return rand() / (RAND_MAX + 1.0);
	//static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	//static std::mt19937 generator;
	//static std::function<double()> rand_generator =
	//	std::bind(distribution, generator);
	//return rand_generator();
}

inline double random_double(double min, double max) {
	return min + (max - min) * random_double();
}

inline int random_int() {
	return rand() % 2;
}

inline int random_int(int min, int max) {
	return min + (max - min) * (rand() / (RAND_MAX + 0.0));
}

inline double clamp(double x, double min, double max) {
	if (x < min)return min;
	if (x > max)return max;
	return x;
}
#include "ray.h"
#endif // !UTILITY_H

