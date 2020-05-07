#pragma once
#ifndef BVH_H
#define BVH_H
#include "hittable.h"
#include "hittable_list.h"
bool box_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axis);
bool box_x_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b);
bool box_y_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b);
bool box_z_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b);
class BVH_Node :public hittable{
public:
	BVH_Node() = default;
	BVH_Node(hittable_list& world, double time0, double time1):
		BVH_Node(world.objects(),0,world.objects().size(),time0,time1){}
	BVH_Node(std::vector<shared_ptr<hittable>>& objs, size_t begin, size_t end, double time0, double time1);
	virtual bool hit(const ray& r, double t_min, double t_max, hit_recored& rec)const;
	virtual bool bounding_box(AABB& output_aabb, double t1 = 0.0, double t2 = 0.0)const;
private:
	shared_ptr<hittable> left;
	shared_ptr<hittable> right;
	AABB box;
};

bool BVH_Node::bounding_box(AABB& output_aabb, double t1 = 0.0, double t2 = 0.0)const {
	output_aabb = box;
	return true;
}

bool BVH_Node::hit(const ray& r, double t_min, double t_max, hit_recored& rec)const {
	if (!box.hit(r, t_min, t_max))
		return false;
	bool hit_left = left->hit(r, t_min, t_max, rec);
	bool hit_right = right->hit(r, t_min, hit_left ? rec.t : t_max, rec);
	return hit_left || hit_right;
}

BVH_Node::BVH_Node(std::vector<shared_ptr<hittable>>& objs,
	size_t begin, size_t end, double time0, double time1) {
	int axis = random_int(0, 2);
	auto comparator = (axis == 0) ? box_x_compare :
		(axis == 1) ? box_y_compare :
		box_z_compare;
	size_t objs_span = end - begin;
	if (objs_span == 1) {
		left = right = objs[begin];
	}
	else if (objs_span == 2) {
		left = objs[begin];
		right = objs[end];
	}
	else {
		std::sort(objs.begin() + begin, objs.begin() + end, comparator);
		size_t mid = begin + objs_span / 2;
		left = make_shared<hittable>(objs, begin, mid, time0, time1);
		right = make_shared<hittable>(objs, mid, end, time0, time1);
	}
	AABB box_left, box_right;
	if (!left->bounding_box(box_left, time0, time1) || !right->bounding_box(box_right, time0, time1))
		std::cerr << "No bounding box in bvh_node constructor.\n";
	box = surrounding_box(box_left, box_right);
}

bool box_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axis) {
	AABB box1;
	AABB box2;
	if (!a->bounding_box(box1) || !b->bounding_box(box2))
		std::cerr << "No bounding box in bvh_node constructor.\n";
	return box1.min()[axis] < box2.min()[axis];
}

bool box_x_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
	return box_compare(a, b, 0);
}

bool box_y_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
	return box_compare(a, b, 1);
}

bool box_z_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
	return box_compare(a, b, 2);
}

#endif // !BVH_NODE_H

