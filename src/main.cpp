#include <linal/vec.h>
#include <model/model.h>
#include <model/image.h>
#include <graphics/graphics.h>
#include <nlohmann/json.hpp>

#include <iostream>
#include <fstream>
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

void usage(int argc, char** argv) {
	if (argc != 5 || strcmp(argv[1], "--mode") != 0 || strcmp(argv[3], "--path") != 0) {
        std::cerr << "Usage: " << argv[0] << " --mode <RASTERIZER_MODE> --path <OUTPUT_PATH>\n";
		std::cerr << "Modify config.json to adjust the scene or input model\n";
		std::cerr << "RASTERIZER_MODES:\n";
		std::cerr << "  fill - for debug\n";
		std::cerr << "  bin  - black & white, lighting \n";
		std::cerr << "  bing - black & white, Gouraud shading\n";
		std::cerr << "  binp - black & white, Phong shading\n";
		std::cerr << "  tex  - color, lighting \n";
		std::cerr << "  texg - color, Gouraud shading\n";
		std::cerr << "  texp - color, Phong shading\n";
		exit(1);
    }
}

int main(int argc, char** argv) {
	usage(argc, argv);

	const RASTERIZER_MODE rast_mode = parse_mode(argv[2]);
	const char* OUTPUT_BMP = argv[4];

	// --- read json config ---
	std::ifstream config("config.json");
	if (!config.is_open()) {
		std::cerr << "Could not open config.json file." << std::endl;
		return 1;
	}

	nlohmann::json jsonConfig;
	try {
		config >> jsonConfig;
	} catch (const nlohmann::json::parse_error& e) {
		std::cerr << "Error parsing JSON: " << e.what() << std::endl;
		return 1;
	}

	if (!jsonConfig.contains("model_path") || !jsonConfig.contains("texture_path") ||
		!jsonConfig.contains("output_width") || !jsonConfig.contains("output_height") ||
		!jsonConfig.contains("output_depth") || !jsonConfig.contains("eye") ||
		!jsonConfig.contains("center") || !jsonConfig.contains("vertical") ||
		!jsonConfig.contains("light")) {
		std::cerr << "Missing required fields in config.json." << std::endl;
		return 1;
	}

	std::string INPUT_OBJ = jsonConfig["model_path"].get<std::string>();
	std::string INPUT_TEX = jsonConfig["texture_path"].get<std::string>();
	
	const size_t WIDTH  = jsonConfig["output_width"].get<size_t>();
	const size_t HEIGHT = jsonConfig["output_height"].get<size_t>();
	const size_t DEPTH  = jsonConfig["output_depth"].get<size_t>();

	auto eye_array      = jsonConfig["eye"].get<std::vector<float>>();
	auto center_array   = jsonConfig["center"].get<std::vector<float>>();
	auto vertical_array = jsonConfig["vertical"].get<std::vector<float>>();
	auto light_array    = jsonConfig["light"].get<std::vector<float>>();

	if (eye_array.size() != 3 || center_array.size() != 3 || 
		vertical_array.size() != 3 || light_array.size() != 3) {
		std::cerr << "Eye, center, vertical, and light must be arrays of three floats." << std::endl;
		return 1;
	}
	// --- done reading config ---

	const Vec3f eye(eye_array);
	const Vec3f center(center_array);
	const Vec3f vertical(vertical_array);
	const Vec3f light(light_array);

	Model*          model = new Model(INPUT_OBJ.c_str());
	Texture*      texture = new Texture(INPUT_TEX.c_str());
	Canvas*        canvas = new Canvas(WIDTH, HEIGHT, DEPTH);
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
