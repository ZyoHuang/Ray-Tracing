#pragma once
#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H
#include <memory>
#include <vector>
#include "hittable.h"
using std::shared_ptr;
using std::make_shared;
class hittable_list :public hittable {
public:
	hittable_list(){}
	hittable_list(shared_ptr<hittable> obj) { add(obj); }
	std::vector<shared_ptr<hittable>>& objects() { return objs; }
	void clear() { objs.clear(); }
	void add(shared_ptr<hittable> obj) { objs.push_back(obj); }
	virtual bool hit(const ray& r, double t_min, double t_max, hit_recored& rec)const;
	virtual bool bounding_box(AABB& output_aabb, double t1, double t2)const;
private:
	std::vector<shared_ptr<hittable>> objs;
};

bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_recored& rec)const {
	hit_recored tmp;
	double t_closest_so_far = t_max;
	bool hit_anything = false;
	for (const auto& obj : objs)
	{
		if (obj->hit(r, t_min, t_closest_so_far, tmp)) {//¶àÌ¬
			hit_anything = true;
			t_closest_so_far = tmp.t;
			rec = tmp;
		}
	}
	return hit_anything;
}

bool hittable_list::bounding_box(AABB& output_aabb, double t1 , double t2 )const {
	if (objs.empty())return false;
	bool firstBox = true;
	AABB tmp_box;
	for (const auto& obj : objs)
	{
		if (!obj->bounding_box(tmp_box, t1, t2))return false;
		output_aabb = firstBox ? tmp_box : surrounding_box(output_aabb, tmp_box);
		firstBox = false;
	}
	return true;
}
#endif // !HITTABLE_LIST_H

