#pragma once
#ifndef MOVING_SPHERE_H
#define MOVING_SPHERE_H
#include "hittable.h"
class moving_sphere :public hittable {
public:
	moving_sphere() = default;
	moving_sphere(const vec3& ctr0,const vec3& ctr1,double tm0,double tm1, double r, shared_ptr<material> m) 
		:center0(ctr0), center1(ctr1), radius(r), mat_ptr(m),time0(tm0),time1(tm1) {}
	virtual bool hit(const ray& r, double t_min, double t_max, hit_recored& rec)const;
	virtual bool bounding_box(AABB& aabb, double t1 , double t2 )const;
	vec3 center(double time)const;
private:
	vec3 center0,center1;
	double time0, time1;
	shared_ptr<material> mat_ptr;
	double radius;

};

vec3 moving_sphere::center(double time)const {
	return center0 + (time - time0) / (time1 - time0) * (center1 - center0);
}

bool moving_sphere::hit(const ray& r, double t_min, double t_max, hit_recored& rec)const {
	auto a = r.direction().length_squared();
	auto half_b = dot(r.direction(), r.origin() - center(r.time()));
	auto c = (r.origin() - center(r.time())).length_squared() - radius * radius;
	auto discriminant = half_b * half_b - a * c;
	if (discriminant > 0) {
		auto tmp = (-half_b - sqrt(discriminant)) / a;
		if (tmp > t_min&& tmp < t_max) {
			rec.t = tmp;
			rec.pt = r.at(tmp);
			auto outward_normal = (rec.pt - center(r.time())) / radius;
			rec.set_face_normal(r, outward_normal);
			rec.mat_ptr = mat_ptr;
			return true;
		}
		tmp = (-half_b + sqrt(discriminant) / a);
		if (tmp > t_min&& tmp < t_max) {
			rec.t = tmp;
			rec.pt = r.at(tmp);
			rec.normal = (rec.pt - center(r.time())) / radius;
			auto outward_normal = (rec.pt - center(r.time())) / radius;
			rec.set_face_normal(r, outward_normal);
			rec.mat_ptr = mat_ptr;
			return true;
		}
	}
	return false;
}

bool moving_sphere::bounding_box(AABB& output_aabb, double t1 , double t2 )const {
	AABB box0(center(t1) - vec3(radius, radius, radius), center(t1) + vec3(radius, radius, radius));
	AABB box1(center(t2) - vec3(radius, radius, radius), center(t2) + vec3(radius, radius, radius));
	output_aabb = surrounding_box(box0, box1);
	return true;
}
#endif // !MOVING_SPHERE_H

