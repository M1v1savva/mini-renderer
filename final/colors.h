#ifndef __COLORS_H__
#define __COLORS_H__

struct RGB {
	unsigned int r, g, b;
	RGB() : r(0), g(0), b(0) {}
	RGB(float i) : r(i * 255), g(i * 255), b(i * 255) {}
	RGB(unsigned int _r, unsigned int _g, unsigned int _b) : r(_r), g(_g), b(_b) {}

	RGB operator * (const float &k) const { return RGB(r * k, g * k, b * k); }
};	

#endif //__COLORS_H__