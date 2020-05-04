#pragma once
#ifndef CAMERA_H
#define CAMERA_H
#include "ray.h"
class camera {
public:
	camera(const vec3& look_from,const vec3& look_at,const vec3& vup,
		double vert_fov,int img_width,int img_height,
		double aperture,double focus_dist
	):	origin(look_from),lens_radius(aperture/2) {
		double theta = degrees_to_radians(vert_fov);
		double half_height = tan(theta / 2) * focus_dist;//half_height = tan(theta/2)*dist
		double half_width = static_cast<double>(img_width) / img_height * half_height;
		w = unit_vector(look_from - look_at);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);
		horizontal = u * 2 * half_width;
		vertical = v * 2 * half_height;
		lower_left_corner = origin - half_width * u - half_height * v - focus_dist * w;
	}
	ray getRay(double x,double y) {
		vec3 cd = lens_radius * random_in_unit_disk();
		vec3 offset = u * cd.x() + v * cd.y();
		return ray(origin + offset, lower_left_corner + x * horizontal + y * vertical - (origin + offset));
	}
private:
	double lens_radius;
	vec3 u, v, w;
	vec3 origin;
	vec3 horizontal;
	vec3 vertical;
	vec3 lower_left_corner;
};
#endif // !CAMERA_H

