#pragma once
#ifndef METAL_H
#define METAL_H
#include "material.h"
class metal :public material {
public:
	metal(const vec3& a, double f) :abedo(a), fuzz(f < 1 ? f : 1){}
	virtual bool scatter(const ray& r_in, const hit_recored& rec, vec3& attenuation, ray& scattered)const {
		vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);//反射方向
		scattered = ray(rec.pt, reflected+fuzz*random_in_unit_sphere());
		attenuation = abedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}
private:
	vec3 abedo;
	double fuzz;//模糊系数
};
#endif // !METAL_H
