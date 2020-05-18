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

class translate :public hittable {
public:
	translate(shared_ptr<hittable> ptr,const vec3& offset):_ptr(ptr),_offset(offset){}
	virtual bool hit(const ray& r, double t_min, double t_max, hit_recored& rec)const {
		ray moved_r(r.origin() - _offset, r.direction(), r.time());
		if (!_ptr->hit(moved_r, t_min, t_max, rec))
			return false;
		rec.pt += _offset;
		rec.set_face_normal(moved_r, rec.normal);
	}
	virtual bool bounding_box(AABB& output_aabb, double t1, double t2)const {
		if (!_ptr->bounding_box(output_aabb, t1, t2))
			return false;
		output_aabb = AABB(output_aabb.min() + _offset, output_aabb.max() + _offset);
		return true;
	}
private:
	shared_ptr<hittable> _ptr;
	vec3 _offset;
};

//绕y轴转
class Rotate_y :public hittable {
public:
	Rotate_y(shared_ptr<hittable> obj, double angle);
	virtual bool hit(const ray& r, double t_min, double t_max, hit_recored& rec)const;
	virtual bool bounding_box(AABB& output_aabb, double t1, double t2)const;
private:
	shared_ptr<hittable> _ptr;
	double _sin_theta;
	double _cos_theta;
	bool _hasbox;
	AABB _bbox;
};

Rotate_y::Rotate_y(shared_ptr<hittable> obj, double angle) :_ptr(obj) {
	auto theta = degrees_to_radians(angle);
	_sin_theta = sin(theta);
	_cos_theta = cos(theta);
	_hasbox = _ptr->bounding_box(_bbox, 0, 1);
	Point3 min(infinity, infinity, infinity);
	Point3 max(-infinity, -infinity, -infinity);
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			for (int k = 0; k < 2; k++)
			{
				auto x = (1 - i) * _bbox.min().x() + i * _bbox.max().x();
				auto y = (1 - j) * _bbox.min().y() + j * _bbox.max().y();
				auto z = (1 - k) * _bbox.min().z() + k * _bbox.max().z();

				auto newx = _cos_theta * x + _sin_theta * z;
				auto newz = -_sin_theta * x + _cos_theta * z;
				//顶点旋转后的位置
				vec3 tester = vec3(newx, y, newz);
				//更新包围盒的min和max对应x，y，z三个分量，每个顶点旋转后都要更新，共8个顶点
				for (int p = 0; p < 3; p++)
				{
					min[p] = fmin(min[p], tester[p]);
					max[p] = fmax(max[p], tester[p]);
				}
			}
		}
	}
	_bbox = AABB(min, max);
}

bool Rotate_y::hit(const ray& r, double t_min, double t_max, hit_recored& rec)const {
	vec3 new_origin = r.origin();
	vec3 new_direction = r.direction();
	new_origin[0] = _cos_theta * r.origin()[0] - _sin_theta * r.origin()[2];
	new_origin[2] = _sin_theta * r.origin()[0] + _cos_theta * r.origin()[2];
	new_direction[0] = _cos_theta * r.direction()[0] - _sin_theta * r.direction()[2];
	new_direction[2] = _sin_theta * r.direction()[0] + _cos_theta * r.direction()[2];	
	ray new_ray(new_origin, new_direction, r.time());
	if (!_ptr->hit(new_ray, t_min, t_max, rec))
		return false;
	vec3 new_p = rec.pt;
	vec3 new_normal = rec.normal;
	new_p[0] = _cos_theta * rec.pt[0] + _sin_theta * rec.pt[2];
	new_p[2] = -_sin_theta * rec.pt[0] + _cos_theta * rec.pt[2];
	new_normal[0] = _cos_theta * rec.normal[0] + _sin_theta * rec.normal[2];
	new_normal[2] = -_sin_theta * rec.normal[0] + _cos_theta * rec.normal[2];
	rec.pt = new_p;
	rec.set_face_normal(new_ray,new_normal);
	return true;
}

bool Rotate_y::bounding_box(AABB& output_aabb, double t1, double t2)const {
	output_aabb = _bbox;
	return _hasbox;
}
#endif // !HITTABLE_H

