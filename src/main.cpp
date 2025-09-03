#include <linal/vec.h>
#include <model/model.h>
#include <model/image.h>
#include <graphics/graphics.h>

#include <iostream>
#include <cstring>

RASTERIZER_MODE parse_mode(const char* mode_str) {
	if (strcmp(mode_str, "fill") == 0)
		return FILL;
	else if (strcmp(mode_str, "bin") == 0)
		return BINARY;
	else if (strcmp(mode_str, "bing") == 0)
		return BINARY_GOURAUD;
	else if (strcmp(mode_str, "binp") == 0)
		return BINARY_PHONG;
	else if (strcmp(mode_str, "tex") == 0)
		return TEXTURE;
	else if (strcmp(mode_str, "texg") == 0)
		return TEXTURE_GOURAUD;
	else if (strcmp(mode_str, "texp") == 0)
		return TEXTURE_PHONG;
	else {
		std::cerr << "Unknown RASTERIZER_MODE: " << mode_str << "\n";
		exit(1);
	}
}

int main(int argc, char** argv) {
	if (argc != 5 || strcmp(argv[1], "--mode") != 0 || strcmp(argv[3], "--path") != 0) {
        std::cerr << "Usage: " << argv[0] << " --mode <RASTERIZER_MODE> --path <OUTPUT_PATH>\n";
		std::cerr << "RASTERIZER_MODES:\n";
		std::cerr << "  fill - for debug\n";
		std::cerr << "  bin  - black & white, lighting \n";
		std::cerr << "  bing - black & white, Gouraud shading\n";
		std::cerr << "  binp - black & white, Phong shading\n";
		std::cerr << "  tex  - color, lighting \n";
		std::cerr << "  texg - color, Gouraud shading\n";
		std::cerr << "  texp - color, Phong shading\n";
		return 1;
    }

	const RASTERIZER_MODE rast_mode = parse_mode(argv[2]);
	const char* OUTPUT_BMP = argv[4];

	const char* INPUT_OBJ = "obj/african_head.obj";
	const char* INPUT_TEX = "obj/african_head_diffuse.bmp";
	const size_t WIDTH = 800, HEIGHT = 800, DEPTH = 255;

	const Vec3f      eye( 1,  1,  3);
	const Vec3f   center( 0,  0,  0);
	const Vec3f vertical( 0,  1,  0);
	const Vec3f    light( 1,  1,  1);

	Model* model = new Model(INPUT_OBJ);
	Texture* texture = new Texture(INPUT_TEX);
	Canvas* canvas = new Canvas(WIDTH, HEIGHT, DEPTH);
	Geometrics* transform = new Geometrics(eye, center, vertical, light);

	Graphics g(
		model,
		texture,
		canvas,
		transform
	);

	g.build(rast_mode);
	g.output(OUTPUT_BMP);

	delete model;
	delete texture;
	delete transform;
	delete canvas;
	return 0;
}
