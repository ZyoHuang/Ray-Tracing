#pragma once
#ifndef AARECT_H
#define AARECT_H
#include "hittable.h"
#include "material.h"
class XY_Rect :public hittable {
public:
	XY_Rect() = default;
	XY_Rect(double x0,double x1,double y0,double y1,double z,shared_ptr<material> m):
		_x0(x0),_x1(x1),_y0(y0),_y1(y1),_z(z),_mat(m){}
	virtual bool hit(const ray& r, double t_min, double t_max, hit_recored& rec)const {
		auto t = (_z - r.origin().z()) / r.direction().z();
		if (t<t_min || t>t_max)
			return false;
		auto x = r.origin().x() + t * r.direction().x();
		auto y = r.origin().y() + t * r.direction().y();
		if (x<_x0 || x>_x1 || y<_y0 || y>_y1)
			return false;
		rec.u = (x - _x0) / (_x1 - _x0);
		rec.v = (y - _y0) / (_y1 - _y0);
		rec.t = t;
		auto outward_normal = vec3(0, 0, 1);//矩形朝向z轴正方向
		rec.set_face_normal(r, outward_normal);
		rec.mat_ptr = _mat;
		rec.pt = r.at(t);
		return true;
	}
	virtual bool bounding_box(AABB& output_aabb, double t1, double t2)const {
		output_aabb = AABB(vec3(_x0, _y0, _z - 0.0001), vec3(_x1, _y1, _z + 0.0001));//-0.0001和+0.0001保证XY面矩形有厚度
		return true;
	}
private:
	double _x0, _x1, _y0, _y1, _z;
	shared_ptr<material> _mat;
};

class XZ_Rect :public hittable {
public:
	XZ_Rect(double x0,double x1,double z0,double z1,double y,shared_ptr<material> m):
		_x0(x0),_x1(x1),_z0(z0),_z1(z1),_y(y),_mat(m){}
	virtual bool hit(const ray& r, double t_min, double t_max, hit_recored& rec)const {
		auto t = (_y - r.origin().y()) / r.direction().y();
		if (t<t_min || t>t_max)
			return false;
		auto x = r.origin().x() + t * r.direction().x();
		auto z = r.origin().z() + t * r.direction().z();
		if (x<_x0 || x>_x1 || z<_z0 || z>_z1)
			return false;
		rec.u = (x - _x0) / (_x1 - _x0);
		rec.v = (z - _z0) / (_z1 - _z0);
		rec.t = t;
		rec.pt = r.at(t);
		auto outward_normal = vec3(0, 1, 0);//朝向y轴正方向
		rec.set_face_normal(r, outward_normal);
		rec.mat_ptr = _mat;
		return true;
	}
	virtual bool bounding_box(AABB& output_aabb, double t1, double t2)const {
		output_aabb = AABB(vec3(_x0, _y - 0.0001, _z0), vec3(_x1, _y + 0.0001, _z1));
		return true;
	}
private:
	double _x0, _x1, _z0, _z1, _y;
	shared_ptr<material> _mat;
};

class YZ_Rect :public hittable {
public:
	YZ_Rect(double y0,double y1,double z0,double z1,double x,shared_ptr<material> m):
		_y0(y0),_y1(y1),_z0(z0),_z1(z1),_x(x),_mat(m){}
	virtual bool hit(const ray& r, double t_min, double t_max, hit_recored& rec)const {
		auto t = (_x - r.origin().x()) / r.direction().x();
		if (t<t_min || t>t_max)
			return false;
		auto y = r.origin().y() + t * r.direction().y();
		auto z = r.origin().z() + t * r.direction().z();
		if (y<_y0 || y>_y1 || z<_z0 || z>_z1)
			return false;
		rec.u = (z - _z0) / (_z1 - _z0);
		rec.v = (y - _y0) / (_y1 - _y0);
		rec.t = t;
		rec.pt = r.at(t);
		auto outward_normal = vec3(1, 0, 0);
		rec.set_face_normal(r, outward_normal);
		rec.mat_ptr = _mat;
		return  true;
	}
	virtual bool bounding_box(AABB& output_aabb, double t1, double t2)const {
		output_aabb = AABB(vec3(_x - 0.0001, _y0, _z0), vec3(_x + 0.0001, _y1, _z1));
		return true;
	}
private:
	double _y0, _y1, _z0, _z1, _x;
	shared_ptr<material> _mat;
};
#endif // !AARECT_H

