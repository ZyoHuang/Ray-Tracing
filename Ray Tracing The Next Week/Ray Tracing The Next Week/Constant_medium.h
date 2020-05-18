#pragma once
#ifndef CONSTANT_MEDIUM_H
#define CONSTANT_MEDIUM_H
#include "hittable.h"
#include "texture.h"
#include "material.h"
class Constant_medium :public hittable {
public:
	Constant_medium(shared_ptr<hittable> boundary, double density, shared_ptr<texture> a) :
		_boundary(boundary), _neg_inv_density(-1 / density) 
	{
		_phase_function = make_shared<isotropic>(a);
	}
	virtual bool hit(const ray& r, double t_min, double t_max, hit_recored& rec)const;
	virtual bool bounding_box(AABB& output_aabb, double t1, double t2)const {
		return _boundary->bounding_box(output_aabb, t1, t2);
	}
private:
	double _neg_inv_density;//浓度
	shared_ptr<hittable> _boundary;//体积雾边界，形状
	shared_ptr<material> _phase_function;
};

bool Constant_medium::hit(const ray& r, double t_min, double t_max, hit_recored& rec)const {
	const bool enableDebug = false;
	const bool debugging = enableDebug && random_double() < 0.00001;
	hit_recored	rec1, rec2;
	if (!_boundary->hit(r, -infinity, infinity, rec1))
		return false;
	if (!_boundary->hit(r, rec1.t + 0.001, infinity, rec2))
		return false;
	if (debugging)std::cerr << "\nt0=" << rec1.t << ",t1=" << rec2.t << "\n";

	if (rec1.t < t_min)rec1.t = t_min;
	if (rec2.t > t_max)rec2.t = t_max;

	if (rec1.t >= rec2.t)
		return false;
	if (rec1.t < 0)
		rec1.t = 0;
	
	const auto ray_length = r.direction().length();
	const auto distance_inside_boundary = (rec2.t - rec1.t) * ray_length;
	const auto hit_distance = _neg_inv_density * log(random_double());

	if (hit_distance > distance_inside_boundary)
		return false;
	rec.t = rec1.t + hit_distance / ray_length;
	rec.pt = r.at(rec.t);
	
	if (debugging) {
		std::cerr << "hit_distance = " << hit_distance << "\n"
			<< "rec.t = " << rec.t << "\n"
			<< "rec.pt = " << rec.pt << "\n";
	}
	rec.normal = vec3(1, 0, 0);
	rec.front_face = true;
	rec.mat_ptr = _phase_function;
	return true;
}
#endif // !CONSTANT_MEDIUM_H

