#include "bvh.h"
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "quad.h"
#include "rtweekend.h"
#include "sphere.h"

color ray_color(const ray &r, const hittable &world) {
  hit_record rec;
  if (world.hit(r, interval(0, infinity), rec)) {
    return 0.5 * (rec.normal + color(1, 1, 1));
  }
  vec3 unit_direction = unit_vector(r.direction());
  auto a = 0.5 * (unit_direction.y() + 1.0);
  return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}

void quads() {
  hittable_list world;

  // Materials
  auto left_red = make_shared<lambertian>(color(1.0, 0.2, 0.2));
  auto back_green = make_shared<lambertian>(color(0.2, 1.0, 0.2));
  auto right_blue = make_shared<lambertian>(color(0.2, 0.2, 1.0));
  auto upper_orange = make_shared<lambertian>(color(1.0, 0.5, 0.0));
  auto lower_teal = make_shared<lambertian>(color(0.2, 0.8, 0.8));
  auto difflight = make_shared<diffuse_light>(color(4, 4, 4));

  // Quads
  world.add(make_shared<quad>(point3(-3, -2, 5), vec3(0, 0, -4), vec3(0, 4, 0),
                              left_red));
  world.add(make_shared<quad>(point3(-2, -2, 0), vec3(4, 0, 0), vec3(0, 4, 0),
                              back_green));
  world.add(make_shared<quad>(point3(3, -2, 1), vec3(0, 0, 4), vec3(0, 4, 0),
                              right_blue));
  world.add(make_shared<quad>(point3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4),
                              upper_orange));
  world.add(make_shared<quad>(point3(-2, -3, 5), vec3(4, 0, 0), vec3(0, 0, -4),
                              lower_teal));

  camera cam;

  cam.aspect_ratio = 1.0;
  cam.image_width = 400;
  cam.samples_per_pixel = 100;
  cam.max_depth = 50;

  cam.background = color(0.70, 0.80, 1.00);

  cam.vfov = 80;
  cam.lookfrom = point3(0, 0, 9);
  cam.lookat = point3(0, 0, 0);
  cam.vup = vec3(0, 1, 0);

  cam.defocus_angle = 0;

  cam.render(world);
}

void mirrors() {
  hittable_list world;

  auto albedo = color(0.8, 0.8, 0.8);
  /*auto fuzz = 0;*/
  auto fuzz = random_double(0, 0.5);

  // Materials
  auto left = make_shared<metal>(albedo, fuzz);
  auto back = make_shared<metal>(albedo, fuzz);
  auto right = make_shared<metal>(albedo, fuzz);
  auto upper = make_shared<metal>(albedo, fuzz);
  auto lower = make_shared<metal>(albedo, fuzz);
  auto difflight = make_shared<diffuse_light>(color(4, 4, 4));

  // Quads
  world.add(make_shared<quad>(point3(-3, -2, 5), vec3(0, 0, -4), vec3(0, 4, 0),
                              left));
  world.add(
      make_shared<quad>(point3(-2, -2, 0), vec3(4, 0, 0), vec3(0, 4, 0), back));
  world.add(
      make_shared<quad>(point3(3, -2, 1), vec3(0, 0, 4), vec3(0, 4, 0), right));
  world.add(
      make_shared<quad>(point3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4), upper));
  world.add(make_shared<quad>(point3(-2, -3, 5), vec3(4, 0, 0), vec3(0, 0, -4),
                              lower));

  auto lambertian_material =
      make_shared<lambertian>(color(0.7, 0.3, 0.3)); // Reddish color

  // Inner cube - 3 times smaller and centered
  // Assuming the outer cube has dimensions roughly 6x6x6,
  // the inner cube will be 2x2x2 (1/3 the size)
  // Inner cube - centered in the mirror box
  double inner_size = 2.0 / 3.0;

  // Calculate center of the outer cube based on its coordinates
  // The outer cube seems to be roughly centered around (0, 0, 2.5)
  // So let's place the inner cube there, but moved slightly forward (closer to
  // camera)
  point3 inner_center(0, 0, 3); // Moved forward in z-direction

  // Front face (facing the camera)
  world.add(make_shared<quad>(point3(inner_center.x() - inner_size,
                                     inner_center.y() - inner_size,
                                     inner_center.z() + inner_size),
                              vec3(2 * inner_size, 0, 0),
                              vec3(0, 2 * inner_size, 0), lambertian_material));

  // Back face
  world.add(make_shared<quad>(point3(inner_center.x() - inner_size,
                                     inner_center.y() - inner_size,
                                     inner_center.z() - inner_size),
                              vec3(2 * inner_size, 0, 0),
                              vec3(0, 2 * inner_size, 0), lambertian_material));

  // Left face
  world.add(make_shared<quad>(point3(inner_center.x() - inner_size,
                                     inner_center.y() - inner_size,
                                     inner_center.z() - inner_size),
                              vec3(0, 0, 2 * inner_size),
                              vec3(0, 2 * inner_size, 0), lambertian_material));

  // Right face
  world.add(make_shared<quad>(point3(inner_center.x() + inner_size,
                                     inner_center.y() - inner_size,
                                     inner_center.z() - inner_size),
                              vec3(0, 0, 2 * inner_size),
                              vec3(0, 2 * inner_size, 0), lambertian_material));

  // Top face
  world.add(make_shared<quad>(
      point3(inner_center.x() - inner_size, inner_center.y() + inner_size,
             inner_center.z() - inner_size),
      vec3(2 * inner_size, 0, 0), vec3(0, 0, 2 * inner_size), difflight));

  // Bottom face
  world.add(make_shared<quad>(point3(inner_center.x() - inner_size,
                                     inner_center.y() - inner_size,
                                     inner_center.z() - inner_size),
                              vec3(2 * inner_size, 0, 0),
                              vec3(0, 0, 2 * inner_size), lambertian_material));
  camera cam;

  cam.aspect_ratio = 1.0;
  cam.image_width = 400;
  cam.samples_per_pixel = 100;
  cam.max_depth = 50;

  cam.background = color(0.70, 0.80, 1.00);

  cam.vfov = 80;
  cam.vup = vec3(0, 1, 0);
  cam.lookfrom = point3(5, 5, 5); // Position diagonally from the cube
  cam.lookat = point3(inner_size, 0, inner_size);

  cam.defocus_angle = 0;

  cam.render(world);
}

void spheres() {
  hittable_list world;

  auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
  world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      auto choose_mat = random_double();
      point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

      if ((center - point3(4, 0.2, 0)).length() > 0.9) {
        shared_ptr<material> sphere_material;

        if (choose_mat < 0.8) {
          // diffuse
          auto albedo = color::random() * color::random();
          sphere_material = make_shared<lambertian>(albedo);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        } else if (choose_mat < 0.95) {
          // metal
          auto albedo = color::random(0.5, 1);
          auto fuzz = random_double(0, 0.5);
          sphere_material = make_shared<metal>(albedo, fuzz);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        } else {
          // glass
          sphere_material = make_shared<dielectric>(1.5);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        }
      }
    }
  }

  auto material1 = make_shared<dielectric>(1.5);
  world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

  auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
  world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

  auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
  world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

  world = hittable_list(make_shared<bvh_node>(world));

  camera cam;

  cam.aspect_ratio = 16.0 / 9.0;
  cam.image_width = 1200;
  cam.samples_per_pixel = 10;
  cam.max_depth = 50;

  cam.vfov = 20;
  cam.lookfrom = point3(13, 2, 3);
  cam.lookat = point3(0, 0, 0);
  cam.vup = vec3(0, 1, 0);

  cam.defocus_angle = 0.6;
  cam.focus_dist = 10.0;
  cam.render(world);
}

int main() {
  switch (3) {
  case 1:
    spheres();
    break;
  case 2:
    quads();
    break;
  case 3:
    mirrors();
    break;
  }
}
