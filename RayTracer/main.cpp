#include <iostream>
#include <fstream>
#include "vec3.h"
#include "ray.h"
#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"
#include <limits>
#include <random>
using namespace std;

vec3 random_in_unit_sphere() {
	vec3 p;
	default_random_engine e;
	uniform_real_distribution<float> u(0, 1); //随机数分布对象 


	do {
		float ran1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float ran2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float ran3 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		if (ran1 == 1)
			ran1 = 0.999999f;
		if (ran2 == 1)
			ran2 = 0.999999f;
		if (ran3 == 1)
			ran3 = 0.999999f;
		p = 2.0*vec3(ran1,ran2,ran3) - vec3(1, 1, 1);
	} while (p.squared_length() >= 1.0);
	return p;
}

vec3 color(const ray& r,hitable *world)
{
	hit_record rec;
	if (world->hit(r, 0.0001, FLT_MAX, rec))
	{
		vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		return 0.5*color(ray(rec.p, target - rec.p), world);
		//return 0.5*vec3(rec.normal.x() + 1, rec.normal.y() + 1, rec.normal.z() + 1);
	}
	else
	{
		vec3 unit_dir = unit_vector(r.direction());
		float t = 0.5*(unit_dir.y() + 1.0f);
		return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}
}

int main()
{
	default_random_engine e;
	uniform_real_distribution<float> u(0, 1); //随机数分布对象 
	ofstream out("me1.ppm");
	int nx = 200;
	int ny = 100;
	out << "P3\n" << nx << " " << ny << "\n255\n";

	hitable *list[2];
	list[0] = new sphere(vec3(0, 0, -1), 0.5);
	list[1] = new sphere(vec3(0, -100.5, -1), 100);
	hitable *world = new hitable_list(list, 2);
	camera cam;

	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			vec3 col(0, 0, 0);
			for (int s = 0; s < 100; s++)
			{
				float ran = u(e);
				float ran1 = u(e);
				if (ran == 1)
					ran = 0.999999f;
				if (ran1 == 1)
					ran1 = 0.999999f;
				float u = float(i + ran) / float(nx);
				float v = float(j + ran1) / float(ny);
				ray r = cam.get_ray(u, v);

				col += color(r, world);
			}
			
			col /= 100.0f;

			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);
			out << ir << " " << ig << " " << ib << "\n";
		}
	}

	return 0;
}