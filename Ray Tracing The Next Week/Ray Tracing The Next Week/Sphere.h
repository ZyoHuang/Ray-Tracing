#pragma once
#ifndef SPHERE_H
#define SPHERE_H
#include "hittable.h"
#include "Utility.h"
#include <cmath>
class sphere:public hittable {
public:
	sphere(){}
	sphere(const vec3& ctr, double r,shared_ptr<material> m) :center(ctr),radius(r),mat_ptr(m){}
	virtual bool hit(const ray& r, double t_min, double t_max, hit_recored& rec)const;
	virtual bool bounding_box(AABB& aabb, double t1 , double t2 )const;
private:
	vec3 center;
	double radius;
	shared_ptr<material> mat_ptr;
};
bool sphere::hit(const ray& r, double t_min, double t_max, hit_recored& rec)const {
	auto a = r.direction().length_squared();
	auto half_b = dot(r.direction(), r.origin() - center);
	auto c = (r.origin() - center).length_squared() - radius * radius;
	auto discriminant = half_b * half_b - a * c;
	if (discriminant > 0) {
		auto tmp = (-half_b - sqrt(discriminant)) / a;
		if (tmp > t_min && tmp < t_max) {
			rec.t = tmp;
			rec.pt = r.at(tmp);
			auto outward_normal = (rec.pt - center)/radius;
			rec.set_face_normal(r, outward_normal);
			rec.mat_ptr = mat_ptr;
			return true;
		}
		tmp = (-half_b + sqrt(discriminant) / a);
		if (tmp > t_min && tmp < t_max) {
			rec.t = tmp;
			rec.pt = r.at(tmp);
			rec.normal = (rec.pt - center)/radius;
			auto outward_normal = (rec.pt - center) / radius;
			rec.set_face_normal(r, outward_normal);
			rec.mat_ptr = mat_ptr;
			return true;
		}
	}
	return false;
}
bool sphere::bounding_box(AABB& output_aabb, double t1 , double t2 )const {
	output_aabb = AABB(center - vec3(radius, radius, radius), center + vec3(radius, radius, radius));
	return true;
}
vec3 random_in_unit_sphere() {
	while (true)
	{
		vec3 random_p = vec3(random_double(), random_double(), random_double());
		if (random_p.length_squared() >= 1.0)
			continue;
		return random_p;
	}
}

void get_sphere_uv(const vec3& p, double& u, double& v) {
	auto phi = atan2(p.z(), p.x());
}
#endif // !SPHERE_H

