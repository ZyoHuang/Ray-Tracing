#pragma once
#ifndef AABB_H
#define AABB_H
#include "vec3.h"
class AABB {
public:
	AABB() = default;
	AABB(const vec3& min,const vec3& max):_min(min),_max(max){}
	vec3 min() const{ return _min; }
	vec3 max() const{ return _max; }
	bool hit(const ray& r, double t_min, double t_max) const{
		for (int i = 0; i < 3; i++)
		{
			auto invD = 1.0 / r.direction()[i];
			auto t0 = ffmin((_min[i] - r.origin[i]) * invD, (_max[i] - r.origin()[i]) * invD);
			auto t1 = ffmax((_min[i] - r.origin[i]) * invD, (_max[i] - r.origin()[i]) * invD);
			if (invD < 0.0)
				std::swap(t0, t1);
			t_min = t0 > t_min ? t0 : t_max;
			t_max = t1 < t_max ? t1 : t_max;
			if (t_min >= t_max)
				return false;
		}
		return true;
	}
private:
	vec3 _min;
	vec3 _max;
};
AABB surrounding_box(const AABB& box0, const AABB& box1) {
	vec3 min(ffmin(box0.min()[0], box1.min()[0]),
			 ffmin(box0.min()[1], box1.min()[1]),
			 ffmin(box0.min()[2], box1.min()[2]));
	vec3 max(ffmax(box0.max()[0], box1.max()[0]),
			 ffmax(box0.max()[1], box1.max()[1]),
			 ffmax(box0.max()[2], box1.max()[2]));
	return AABB(min, max);
}
#endif // !AABB_H

