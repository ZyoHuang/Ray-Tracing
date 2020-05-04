#pragma once
#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H
#include "material.h"
class lambertian :public material {
public:
	lambertian(const vec3& a):abedo(a){}
	virtual bool scatter(const ray& r_in, const hit_recored& rec, vec3& attenuation, ray& scattered)const {
		vec3 scatter_direction = rec.normal + random_unit_vec3();
		scattered = ray(rec.pt, scatter_direction,r_in.time());
		attenuation = abedo;
		return true;
	}
private:
	vec3 abedo;
};
#endif // !LAMBERTIAN

