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
#include "aarect.h"
#include "Box.h"
const int image_width = 600;
const int image_height = 400;
const int samples_per_pixel = 400;
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
    auto red = make_shared<lambertian>(make_shared<solid_color>(.65, .05, .05));
    auto white = make_shared<lambertian>(make_shared<solid_color>(.73, .73, .73));
    auto green = make_shared<lambertian>(make_shared<solid_color>(.12, .45, .15));
    auto light = make_shared<diffuse_light>(make_shared<solid_color>(15, 15, 15));
    //Cornell box
    world.add(make_shared<YZ_Rect>(0, 555, 0, 555, 555, green));
    world.add(make_shared<YZ_Rect>(0, 555, 0, 555, 0, red));
    world.add(make_shared<XZ_Rect>(213, 343, 227, 332, 554, light));
    world.add(make_shared<XZ_Rect>(0, 555, 0, 555, 0, white));
    world.add(make_shared<XZ_Rect>(0, 555, 0, 555, 555, white));
    world.add(make_shared<XY_Rect>(0, 555, 0, 555, 555, white));
    
    //world.add(make_shared<Box>(Point3(130, 0, 65), Point3(295, 165, 230), white));
    //world.add(make_shared<Box>(Point3(265, 0, 295), Point3(430, 330, 460), white));

    shared_ptr<hittable> box1 = make_shared<Box>(Point3(0, 0, 0), Point3(165, 330, 165), white);
    box1 = make_shared<Rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265, 0, 295));
    world.add(box1);

    shared_ptr<hittable> box2 = make_shared<Box>(Point3(0, 0, 0), Point3(165, 165, 165), white);
    box2 = make_shared<Rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vec3(130, 0, 65));
    world.add(box2);
    return static_cast<hittable_list>(make_shared<BVH_Node>(world,0,1));
}
vec3 ray_color(const ray& r,const Color& background, const hittable& world, int depth) {
    hit_recored rec;
    if (depth <= 0)
        return Color(0.0, 0.0, 0.0);
    if (!world.hit(r, 0.001, infinity, rec))
        return background;
    ray scattered;
    vec3 attenuation;
    Color emit_color = rec.mat_ptr->emitted(rec.u, rec.v, rec.pt);
    if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        return emit_color;
    return emit_color + attenuation * (ray_color(scattered, background, world, --depth));
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
                Color color = Color(0.0, 0.0, 0.0);
                for (int x = 0; x < samples_per_pixel; x++)
                {
                    auto u = (j + random_double()) / image_width;
                    auto v = (i + random_double()) / image_height;
                    vec3 lookfrom = vec3(278, 278, -800);
                    vec3 lookat = Point3(278, 278, 0);
                    vec3 vup = vec3(0.0, 1.0, 0.0);
                    Color background = Color(0, 0, 0);
                    double fov = 45;
                    double aperture = 0.0;
                    double focusDist = 10.0;
                    camera cam(lookfrom, lookat, vup,
                        fov, image_width, image_height,
                        aperture, focusDist,0.0,1.0);
                    ray r = cam.getRay(u, v);
                    color += ray_color(r, background, world, max_depth);
                }
                color *= (1.0 / samples_per_pixel);
                file << color;
            }
        }
    }
}

