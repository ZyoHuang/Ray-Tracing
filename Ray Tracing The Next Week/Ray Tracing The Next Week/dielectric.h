#pragma once
#ifndef DIELECTRIC
#define DIELECTRIC
#include "material.h"

class dielectric :public material {
public:
	dielectric(double r_factor):ref_factor(r_factor){}
	virtual bool scatter(const ray& r_in, const hit_recored& rec, vec3& attenuation, ray& scattered)const {
		attenuation = vec3(1.0, 1.0, 1.0);
		double etaI_over_etaO = (rec.front_face) ? (1.0 / ref_factor) : (ref_factor);
		vec3 unit_direction = unit_vector(r_in.direction());
		double cos_theta = ffmin(dot(-unit_direction, rec.normal), 1.0);
		double sin_theta = sqrt(abs(1.0 - cos_theta * cos_theta));
		
		if (etaI_over_etaO * sin_theta > 1.0) {//reflection
			vec3 reflected = reflect(unit_direction, rec.normal);
			scattered = ray(rec.pt, reflected);
			return true;
		}
		double reflect_prob = schlick(cos_theta, etaI_over_etaO);
		if (random_double() < reflect_prob)
		{
			vec3 reflected = reflect(unit_direction, rec.normal);
			scattered = ray(rec.pt, reflected);
			return true;
		}
		//refraction
		vec3 refracted = refract(unit_direction, rec.normal, etaI_over_etaO);
		scattered = ray(rec.pt, refracted);
		return true;
	}
private:
	double ref_factor;
};

#endif // !DIELECTRIC

