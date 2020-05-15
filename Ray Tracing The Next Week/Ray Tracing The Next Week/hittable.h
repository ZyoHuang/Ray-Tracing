#pragma once
#ifndef HITTABLE_H
#define HITTABLE_H
#include "ray.h"
#include "AABB.h"
class material;
struct hit_recored
{
	vec3 pt;							//hit中的点
	vec3 normal;						//法线向量
	std::shared_ptr<material> mat_ptr;	//一点处的材质
	double u, v;						//纹理uv
	double t;							//射线步长，p(x)=a+t*b;
	bool front_face;					//图元的正反面
	inline void set_face_normal(const ray& r, const vec3& outward_normal) {
		front_face = dot(r.direction(), outward_normal) < 0.0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};
class hittable {
public:
	virtual bool hit(const ray& r, double t_min, double t_max, hit_recored& rec)const = 0;
	virtual bool bounding_box(AABB& output_aabb, double t1, double t2)const = 0;
};
#endif // !HITTABLE_H

