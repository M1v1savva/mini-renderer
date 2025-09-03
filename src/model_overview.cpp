#include <model/model.h>
#include <iostream>

int main(int argc, char** argv) {
	if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " model.obj\n";
        return 1;
	}

	const char* filename = argv[1];
	Model model(filename);
	model.describe();
	return 0;
}