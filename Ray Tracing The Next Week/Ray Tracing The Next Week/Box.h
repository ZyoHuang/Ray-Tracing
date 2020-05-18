#pragma once
#ifndef BOX_H
#define BOX_H
#include "hittable.h"
#include "hittable_list.h"
#include "aarect.h"
class Box :public hittable {
public:
	Box(const Point3& min, const Point3& max,shared_ptr<material> m) {
		_min = min;
		_max = max;
		sides.add(make_shared<XZ_Rect>(_min.x(), _max.x(), _min.z(), _max.z(), _max.y(), m));//��
		sides.add(make_shared<XZ_Rect>(_min.x(), _max.x(), _min.z(), _max.z(), _min.y(), m));//��
		sides.add(make_shared<YZ_Rect>(_min.y(), _max.y(), _min.z(), _max.z(), _min.x(), m));//��
		sides.add(make_shared<YZ_Rect>(_min.y(), _max.y(), _min.z(), _max.z(), _max.x(), m));//��
		sides.add(make_shared<XY_Rect>(_min.x(), _max.x(), _min.y(), _max.y(), _max.z(), m));//ǰ
		sides.add(make_shared<XY_Rect>(_min.x(), _max.x(), _min.y(), _max.y(), _min.z(), m));//��
		
	}
	virtual bool hit(const ray& r, double t_min, double t_max, hit_recored& rec)const {
		return sides.hit(r, t_min, t_max, rec);
	}
	virtual bool bounding_box(AABB& output_aabb, double t1, double t2)const {
		output_aabb = AABB(_min, _max);
		return true;
	}
private:
	Point3 _min, _max;
	hittable_list sides;
};
#endif // !BOX_H

