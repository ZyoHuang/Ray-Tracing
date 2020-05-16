// Ray Tracing The Next Week.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include "Utility.h"
#include "hittable_list.h"
#include "Sphere.h"
#include "camera.h"
#include "metal.h"
#include "lambertian.h"
#include "dielectric.h"
#include "moving_sphere.h"
#include "BVH.h"
const int image_width = 600;
const int image_height = 400;
const int samples_per_pixel = 200;
const int max_depth = 50;

double hit_sphere(const ray& r, const vec3& center, double radius) {
    auto a = dot(r.direction(), r.direction());
    auto b = 2 * dot(r.direction(), r.origin() - center);
    auto c = dot(r.origin() - center, r.origin() - center) - radius * radius;
    auto discriminant = b * b - 4 * a * c;
    if (discriminant < 0.0) {
        return -1.0;
    }
    else {
        return (-b - sqrt(discriminant) / (2.0 * a));
    }
}
hittable_list random_scene() {
    hittable_list world;
    auto perlin_texture = make_shared<noise_texture>(5.0);
    auto earth_texture = make_shared<image_texture>("img-bed/earthmap.jpg");
    world.add(make_shared<sphere>(Point3(0, -1000, 0), 1000, make_shared<lambertian>(perlin_texture)));
    world.add(make_shared<sphere>(Point3(0, 2, 0), 2, make_shared<lambertian>(earth_texture)));
    return static_cast<hittable_list>(make_shared<BVH_Node>(world,0,1));
}
vec3 ray_color(const ray& r, const hittable& world, int depth) {
    hit_recored rec;
    if (depth <= 0)
        return vec3(0.0, 0.0, 0.0);
    if (world.hit(r, 0.001, infinity, rec)) {
        ray scattered;
        vec3 attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * (ray_color(scattered, world, --depth));
    }
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return t * vec3(0.5, 0.7, 1.0) + (1 - t) * vec3(1.0, 1.0, 1.0);
}
int main()
{
    std::ofstream file("work1_1.ppm");
    file << "P3\n" << image_width << " " << image_height << "\n255\n";
    if (file.is_open()) {
        vec3 origin(0.0, 0.0, 0.0);
        vec3 horizontal(4.0, 0.0, 0.0);
        vec3 vertical(0.0, 2.0, 0.0);
        vec3 lower_left_corner(-2.0, -1.0, -1.0);
        hittable_list world = random_scene();
        for (int i = image_height - 1; i >= 0; --i)
        {
            for (int j = 0; j < image_width; j++)
            {
                vec3 color = vec3(0.0, 0.0, 0.0);
                for (int x = 0; x < samples_per_pixel; x++)
                {
                    auto u = (j + random_double()) / image_width;
                    auto v = (i + random_double()) / image_height;
                    vec3 lookfrom = vec3(13, 2, 3);
                    vec3 lookat = Point3(0, 2, 0);
                    vec3 vup = vec3(0.0, 1.0, 0.0);
                    double fov = 20;
                    double aperture = 0.0;
                    double focusDist = 10.0;
                    camera cam(lookfrom, lookat, vup,
                        fov, image_width, image_height,
                        aperture, focusDist,0.0,1.0);
                    ray r = cam.getRay(u, v);
                    color += ray_color(r, world, max_depth);
                }
                color *= (1.0 / samples_per_pixel);
                file << color;
            }
        }
    }
}

