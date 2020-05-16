#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H
#include "vec3.h"
#include "perlin.h"
#include "rtw_stb_image.h"
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
	double _scale;//噪声频率
};
//图像纹理
class image_texture :public texture {
public:
	const static int byte_per_pixel = 3;
	image_texture() :data(nullptr), width(0), height(0), byte_per_scanline(0) {}
	image_texture(const char* filename) {
		auto components_per_pixel = byte_per_pixel;
		data = stbi_load(filename, &width, &height, &components_per_pixel, components_per_pixel);
		if (!data) {
			std::cerr << "ERROR:Could not load texture image file '" << filename << "'.\n";
			width = height = 0;
		}
		byte_per_scanline = byte_per_pixel * width;
	}
	~image_texture() {
		delete data;
	}
	virtual color value(double u, double v, const vec3& p)const {
		if (!data)
			return color(0, 1, 1);//纹理加载失败时用color(0,1,1)表示
		//将输入纹理坐标限制在[0,1]×[1,0]，纹理坐标的v是从下往上扫
		u = clamp(u, 0.0, 1.0);
		v = 1 - clamp(v, 0.0, 1.0);//将v翻转到图像坐标，图像坐标的v是从上往下扫
		//将uv映射到整数像素坐标上，并且实际坐标应该小于1.0
		auto i = static_cast<int>(u * width);
		auto j = static_cast<int>(v * height);
		if (i >= width) i = width - 1;
		if (j >= height)j = height - 1;

		const auto color_scale = 1.0 / 255.0;
		auto pixel = data + j * byte_per_scanline + i * byte_per_pixel;//计算出存储(i,j)坐标像素的首地址
		return color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
	}
private:
	unsigned char* data;
	int width, height;
	int byte_per_scanline;
};
#endif // !TEXTURE_H

