#include <graphics/graphics.h>
#include <graphics/fragment.h>

#include <iostream>
#include <limits>
#include <cmath>

void Graphics::build(RASTERIZER_MODE rasterizer_mode) {
    transform-> build(canvas-> get_width(), canvas-> get_height(), canvas-> get_depth());

    std::unique_ptr<IRasterizer> rasterizer = RasterizerFactory::create(rasterizer_mode);
    rasterizer-> set_canvas(canvas);

    for (int i = 0; i < model-> nfaces(); i++) {
        if (i % 5 == 0)
            std::cout << "\rProgress: " << int(ceil((i * 100.f) / model-> nfaces())) << "%. " << std::flush;
        Vec3i face = model-> get_face(i);
        Vec3i screen[3];
        Vec3f world[3];

        Vec3f norm[3];
        Vec3i faceNorm = model-> get_face_norm(i);
        Vec3f tex[3];
        Vec3i faceTex = model-> get_face_tex(i);
        float vertIntensity[3];

        for (int j = 0; j < 3; j++) {
            Vec3f local = model-> get_vert(face.val[j]);
            Vec3f localNorm = model-> get_norm(faceNorm.val[j]);
            Vec3f localTex = model-> get_tex(faceTex.val[j]);

            world[j] = local;
            norm[j] = localNorm;
            tex[j] = localTex;

            norm[j].normalize();
            norm[j] = norm[j] * (-1);
            vertIntensity[j] = norm[j] * (transform-> get_light());

            screen[j] = transform-> pass(world[j]);
        }
        Vec3f normal = ((world[1] - world[0]) ^ (world[2] - world[0]));
        normal.normalize();
        normal = normal * (-1);
        float intensity = normal * (transform-> get_light());
        
        Vec3f light = transform-> get_light();
        Fragment job { screen, tex, &intensity, vertIntensity, norm, &light, texture };
        rasterizer-> set_job(&job);
        rasterizer-> rasterize();
    }
}

void Graphics::output(const char* filename) {
    Image dr;
    dr.width = canvas-> get_width();
    dr.height = canvas-> get_height();
    RGB** old_canvas = canvas-> get_pixelmap();
    RGB** rotated_canvas = new RGB*[dr.height];
    for (size_t i = 0; i < dr.height; ++i)
        rotated_canvas[i] = new RGB[dr.width];

    for (size_t i = 0; i < dr.height; ++i)
        for (size_t j = 0; j < dr.width; ++j)
            rotated_canvas[i][j] = old_canvas[j][i];
    
    dr.pixelmap = rotated_canvas;
    dr.save_bmp(filename);
    std::cout << "Saved to " << filename << std::endl;
}