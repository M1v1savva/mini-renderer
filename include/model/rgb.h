#pragma once

struct RGB {
	unsigned char r, g, b;

	RGB() : r(0), g(0), b(0) {}
	RGB(float i) :
		r(static_cast<unsigned char>(i * 255)), 
		g(static_cast<unsigned char>(i * 255)), 
		b(static_cast<unsigned char>(i * 255)) {}
	RGB(unsigned int _r, unsigned int _g, unsigned int _b) : r(_r), g(_g), b(_b) {}

	RGB operator * (float k) const { 
		return RGB(
			static_cast<unsigned char>(k * r),
			static_cast<unsigned char>(k * g),
			static_cast<unsigned char>(k * b)
		);
	}
};	