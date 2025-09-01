#include <graphics/graphics.h>
#include <graphics/iviewport.h>

#include <iostream>
#include <limits>

void Graphics::build(RASTERIZER_MODE rasterizer_mode) {
    transform.build(canvas-> get_width(), canvas-> get_height(), canvas-> get_depth());

    IRasterizer rasterizer = RasterizerFactory.create(rasterizer_mode);
    rasterizer.set_canvas(canvas);

    for (int i = 0; i < model-> nfaces(); i++) {
        Vec3i face = model-> getFace(i);
        Vec3i screen[3];
        Vec3f world[3];

        Vec3f norm[3];
        Vec3i faceNorm = model-> getFaceNorm(i);
        Vec3f tex[3];
        Vec3i faceTex = model-> getFaceTex(i);
        float vertIntensity[3];

        for (int j = 0; j < 3; j++) {
            Vec3f local = model-> getVert(face.val[j]);
            Vec3f localNorm = model-> getNorm(faceNorm.val[j]);
            Vec3f localTex = model-> getTex(faceTex.val[j]);

            world[j] = local;
            norm[j] = localNorm;
            tex[j] = localTex;

            norm[j].normalize();
            norm[j] = norm[j] * (-1);
            vertIntensity[j] = norm[j] * (transform-> get_light());

            screen[j] = transform.pass(world[j]);
        }

        Vec3f normal = ((world[2] - world[0]) ^ (world[1] - world[0]));
        normal.normalize();
        float intensity = normal * (transform-> get_light());
        
        IViewport* view = new IViewport(
            screen,
            tex,
            intensity,
            vertIntensity,
            norm
        );

        rasterizer.set_view(view);
        rasterizer.rasterize();
    }
}

void Graphics::output() {
    Image dr;
    dr.w = width;
    dr.h = height;
    dr.map = map;
    dr.build(outputFilename);
}
