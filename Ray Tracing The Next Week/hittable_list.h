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
	void clear() { objs.clear(); }
	void add(shared_ptr<hittable> obj) { objs.push_back(obj); }
	virtual bool hit(const ray& r, double t_min, double t_max, hit_recored& rec)const;
private:
	std::vector<shared_ptr<hittable>> objs;
};
bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_recored& rec)const {
	hit_recored tmp;
	double t_closest_so_far = t_max;
	bool hit_anything = false;
	for (const auto& obj : objs)
	{
		if (obj->hit(r, t_min, t_closest_so_far, tmp)) {//∂‡Ã¨
			hit_anything = true;
			t_closest_so_far = tmp.t;
			rec = tmp;
		}
	}
	return hit_anything;
}
#endif // !HITTABLE_LIST_H

