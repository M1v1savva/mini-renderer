#pragma once

struct RGB {
	unsigned char r, g, b;

	RGB() : r(0), g(0), b(0) {}
	
	RGB(float intensity) 
		: r(static_cast<unsigned char>(intensity * 255)) 
		, g(static_cast<unsigned char>(intensity * 255)) 
		, b(static_cast<unsigned char>(intensity * 255)) {}

	RGB(unsigned char _r, unsigned char _g, unsigned char _b) 
		: r(_r), g(_g), b(_b) {}

	inline RGB operator * (float k) const { 
		auto cast = [](float x) {
			return static_cast<unsigned char>(x);
		};
		
		return RGB(cast(k * r), cast(k * g), cast(k * b));
	}
};	