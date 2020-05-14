#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H
#include "vec3.h"
#include "perlin.h"
class texture {
public:
	virtual color value(double u, double v, const vec3& p)const = 0;
};

class solid_color :public texture {
public:
	solid_color(){}
	solid_color(color c):color_value(c){}
	solid_color(double r,double g,double b):solid_color(color(r,g,b)){}
	virtual color value(double u, double v, const vec3& p)const {
		return color_value;
	}
private:
	color color_value;
};

class checker_texture :public texture {
public:
	checker_texture(){}
	checker_texture(shared_ptr<texture> odd,shared_ptr<texture> even):_odd_tex(odd),_even_tex(even){}
	virtual color value(double u, double v, const vec3& p)const {
		auto sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
		if (sines < 0.0)
			return _odd_tex->value(u, v, p);
		else
			return _even_tex->value(u, v, p);
	}
private:
	shared_ptr<texture> _odd_tex;
	shared_ptr<texture> _even_tex;
};

class noise_texture :public texture {
public:
	noise_texture() = default;
	noise_texture(double scale):_scale(scale){}
	virtual color value(double u, double v, const vec3& p)const {
		return color(1, 1, 1) * 0.5 * (sin(_scale*p.y()+10*noise.turb(p)) + 1.0);
	}
private:
	perlin noise;
	double _scale;//‘Î…˘∆µ¬ 
};
#endif // !TEXTURE_H

