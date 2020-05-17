#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H
#include "ray.h"
#include "hittable.h"
#include "texture.h"
class material {
public:
	virtual bool scatter(const ray& r_in, const hit_recored& rec, vec3& attenuation, ray& scattered)const = 0;
    //不发光材质设置返回黑色
    virtual Color emitted(double u, double v, const Point3& p)const {
        return Color(0, 0, 0);
    };
};
double schlick(double cosine, double ref_idx) {
    auto r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}
//漫射光
class diffuse_light :public material {
public:
    diffuse_light() = default;
    diffuse_light(shared_ptr<texture> emit):_emit(emit){}
    //不反射
    virtual bool scatter(const ray& r_in, const hit_recored& rec, Color& attenuation, ray& scattered)const {
        return false;
    }
    //发射函数，发光
    virtual Color emitted(double u, double v, const Point3& p)const {
        return _emit->value(u, v, p);
    }
private:
    shared_ptr<texture> _emit;
};
#endif // !MATERIAL_H

