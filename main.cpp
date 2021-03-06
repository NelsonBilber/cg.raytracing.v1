#include <iostream>
#include <random>
#include "camera.h"

#include "sphere.h"
#include "hitable_list.h"
#include "float.h"
#include "material.h"

using namespace std;

vec3 randon_in_unit_sphere(){
    vec3 p;
    do {
        p = 2.0*vec3(drand48(), drand48(), drand48()) - vec3 (1,1,1);
    } while(dot(p,p) >= 1.0);
    return p;
}

vec3 color(const ray& r, hitable *world) {

    hit_record rec;
    if(world->hit(r, 0.0, MAXFLOAT, rec)) {
        vec3 target = rec.p + rec.normal + randon_in_unit_sphere();
        return 0.5*color(ray (rec.p, target - rec.p), world);
    }
    else {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5*(unit_direction.y() + 1.0);
        return (1.0 - t )*vec3(1.0, 1.0, 1.0) + t*vec3(0.5,0.7,1.0);
     }
}

int main(int argc, const char * argv[]) {

    int nx = 200;
    int ny = 100;
    int ns = 100;

    cout << "P3\n" << nx << " " << ny << "\n255\n";

    hitable *list[2];
    list[0] = new sphere (vec3(0,0,-1), 0.5, new lambertian(vec3(0.8,0.3,0.3)));
    list[1] = new sphere (vec3(0,-100.5,-1), 100, new lambertian(vec3(0.8,0.8,0.0)));
    //list[2] = new sphere (vec3(1,0,-1), 0.5, new metal(vec3(0.8,0.6,0.2)));
    //list[3] = new sphere (vec3(-1,0,-1), 0.5, new metal(vec3(0.8,0.8,0.8)));

    hitable *world = new hitable_list(list, 2);
    vec3 lookfrom(3,3,2);
    vec3 lookat(0,0,-1);
    float dist_to_focus = (lookfrom-lookat).length();
    float aperture = 2.0;
    camera cam(lookfrom,
               lookat,
               vec3(0,1,0),
               20,
               float(nx)/float(ny),
               aperture,
               dist_to_focus
     );

    for(int j = ny-1; j >= 0; j--){
        for(int i = 0; i < nx; i++){
            vec3 col(0,0,0);
            for( int s = 0; s < ns; s++){
                float u = float(i + drand48()) / float (nx);
                float v = float(j + drand48()) / float (ny);
                ray r = cam.get_ray(u,v);
                vec3 p = r.point_at_parameter(2.0);
                col += color(r, world);
            }
            col /= float(ns);
            col = vec3 (sqrt(col[0]), sqrt(col[1]), sqrt(col[2]) );
            int ir = int (255.99*col[0]);
            int ig = int (255.99*col[1]);
            int ib = int (255.99*col[2]);

            cout << ir << " " << ig << " " << ib << endl;
        }
    }

    return 0;
}
