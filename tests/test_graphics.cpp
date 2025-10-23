#include <graphics/graphics.h>
#include <graphics/rasterizer.h>
#include <gtest/gtest.h>

TEST(GraphicsTest, Initialize) {
    const Vec3f      eye(1.0, 0.5, 6.0);
	const Vec3f   center(0.0, 0.0, 0.0);
	const Vec3f vertical(0.0, 1.0, 0.0);
	const Vec3f    light(0.0, 0.0, 1.0);
    
    Model*          model = new Model("obj/african_head.obj");
    Texture*      texture = new Texture("obj/african_head_diffuse.tga");
    Canvas*        canvas = new Canvas(800, 800, 255);
	Geometrics* transform = new Geometrics(eye, center, vertical, light);
    Graphics g(
		model,
		texture,
		canvas,
		transform
	);

	g.build(BINARY_GOURAUD);
    g.output("output/test_graphics_output.tga");
    EXPECT_EQ(true, true); // see the output image
}