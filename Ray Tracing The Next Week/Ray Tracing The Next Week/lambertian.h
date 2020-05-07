#pragma once
#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H
#include "material.h"
#include "texture.h"
class lambertian :public material {
public:
	lambertian(shared_ptr<texture> tex):abedo(tex){}
	virtual bool scatter(const ray& r_in, const hit_recored& rec, vec3& attenuation, ray& scattered)const {
		vec3 scatter_direction = rec.normal + random_unit_vec3();
		scattered = ray(rec.pt, scatter_direction,r_in.time());
		attenuation = abedo->value(rec.u,rec.v,rec.pt);
		return true;
	}
private:
	shared_ptr<texture> abedo;
};
#endif // !LAMBERTIAN

