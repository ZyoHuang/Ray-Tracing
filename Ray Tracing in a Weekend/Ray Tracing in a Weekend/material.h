#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H
#include "ray.h"
#include "hittable.h"
class material {
public:
	virtual bool scatter(const ray& r_in, const hit_recored& rec, vec3& attenuation, ray& scattered)const = 0;
};
double schlick(double cosine, double ref_idx) {
    auto r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}
#endif // !MATERIAL_H

