#include <linal/vec.h>
#include <model/model.h>
#include <model/image.h>
#include <graphics/graphics.h>
#include <nlohmann/json.hpp>

#include <iostream>
#include <fstream>
#include <cstring>
#include <filesystem>

// --- Command Line Parsing ---

struct CommandLineArgs {
    RASTERIZER_MODE rast_mode;
    std::string output_path;
    std::string config_path;
};

RASTERIZER_MODE parse_mode(const std::string& mode_str) {
    if (mode_str == "fill")  return FILL;
    if (mode_str == "bin")   return BINARY;
    if (mode_str == "bing")  return BINARY_GOURAUD;
    if (mode_str == "binp")  return BINARY_PHONG;
    if (mode_str == "tex")   return TEXTURE;
    if (mode_str == "texg")  return TEXTURE_GOURAUD;
    if (mode_str == "texp")  return TEXTURE_PHONG;
    
    std::cerr << "Error: Unknown rasterizer mode '" << mode_str << "'\n";
    exit(1);
}

void print_usage(const char* program_name) {
    std::cerr << "Usage: " << program_name << " --mode <MODE> --output <PATH> [--config <PATH>]\n\n";
    std::cerr << "Required Arguments:\n";
    std::cerr << "  --mode <MODE>      Rasterizer mode (see below)\n";
    std::cerr << "  --output <PATH>    Output TGA file path\n\n";
    std::cerr << "Optional Arguments:\n";
    std::cerr << "  --config <PATH>    Config JSON file path (default: ../config.json)\n\n";
    std::cerr << "Rasterizer Modes:\n";
    std::cerr << "  fill   Debug mode (solid fill)\n";
    std::cerr << "  bin    Black & white with lighting\n";
    std::cerr << "  bing   Black & white with Gouraud shading\n";
    std::cerr << "  binp   Black & white with Phong shading\n";
    std::cerr << "  tex    Color with lighting\n";
    std::cerr << "  texg   Color with Gouraud shading\n";
    std::cerr << "  texp   Color with Phong shading\n";
}

CommandLineArgs parse_args(int argc, char** argv) {
    CommandLineArgs args;
    args.config_path = "../config.json";  // default
    
    bool has_mode = false;
    bool has_output = false;
    
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "--mode" || arg == "-m") {
            if (i + 1 >= argc) {
                std::cerr << "Error: --mode requires an argument\n";
                print_usage(argv[0]);
                exit(1);
            }
            args.rast_mode = parse_mode(argv[++i]);
            has_mode = true;
        }
        else if (arg == "--output" || arg == "-o") {
            if (i + 1 >= argc) {
                std::cerr << "Error: --output requires an argument\n";
                print_usage(argv[0]);
                exit(1);
            }
            args.output_path = argv[++i];
            has_output = true;
        }
        else if (arg == "--config" || arg == "-c") {
            if (i + 1 >= argc) {
                std::cerr << "Error: --config requires an argument\n";
                print_usage(argv[0]);
                exit(1);
            }
            args.config_path = argv[++i];
        }
        else if (arg == "--help" || arg == "-h") {
            print_usage(argv[0]);
            exit(0);
        }
        else {
            std::cerr << "Error: Unknown argument '" << arg << "'\n";
            print_usage(argv[0]);
            exit(1);
        }
    }
    
    if (!has_mode || !has_output) {
        std::cerr << "Error: Missing required arguments\n\n";
        print_usage(argv[0]);
        exit(1);
    }
    
    return args;
}

// --- Config Loading ---

struct SceneConfig {
    std::string model_path;
    std::string texture_path;
    size_t width;
    size_t height;
    size_t depth;
    Vec3f eye;
    Vec3f center;
    Vec3f vertical;
    Vec3f light;
};

Vec3f parse_vec3(const nlohmann::json& j, const std::string& field_name) {
    if (!j.contains(field_name)) {
        throw std::runtime_error("Missing field: " + field_name);
    }
    auto array = j[field_name].get<std::vector<float>>();
    if (array.size() != 3) {
        throw std::runtime_error(field_name + " must be an array of 3 floats");
    }
    return Vec3f(array);
}

SceneConfig load_config(const std::string& config_path) {
    std::ifstream config_file(config_path);
    if (!config_file.is_open()) {
        throw std::runtime_error("Could not open config file: " + config_path);
    }

    nlohmann::json json;
    try {
        config_file >> json;
    } catch (const nlohmann::json::parse_error& e) {
        throw std::runtime_error("JSON parse error: " + std::string(e.what()));
    }

    SceneConfig config;
    
    // Required fields
    try {
        config.model_path   = json.at("model_path").get<std::string>();
        config.texture_path = json.at("texture_path").get<std::string>();
        config.width        = json.at("output_width").get<size_t>();
        config.height       = json.at("output_height").get<size_t>();
        config.depth        = json.at("output_depth").get<size_t>();
        
        config.eye      = parse_vec3(json, "eye");
        config.center   = parse_vec3(json, "center");
        config.vertical = parse_vec3(json, "vertical");
        config.light    = parse_vec3(json, "light");
        
    } catch (const std::exception& e) {
        throw std::runtime_error("Config validation error: " + std::string(e.what()));
    }
    
    return config;
}

// --- Main ---

int main(int argc, char** argv) {
    CommandLineArgs args = parse_args(argc, argv);
    
    // Load configuration
    SceneConfig config;
    try {
        config = load_config(args.config_path);
    } catch (const std::exception& e) {
        std::cerr << "Error loading config: " << e.what() << std::endl;
        return 1;
    }
    
    // Initialize scene components
    Model*      model     = new Model(config.model_path.c_str());
    Texture*    texture   = new Texture(config.texture_path.c_str());
    Canvas*     canvas    = new Canvas(config.width, config.height, config.depth);
    Geometrics* transform = new Geometrics(config.eye, config.center, config.vertical, config.light);

    Graphics g(model, texture, canvas, transform);

    // Render and output
    g.build(args.rast_mode);
    g.output(args.output_path.c_str());

    // Cleanup
    delete model;
    delete texture;
    delete transform;
    delete canvas;
    
    return 0;
}
