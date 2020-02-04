#ifndef __DRAWER_H__
#define __DRAWER_H__

#include "colors.h"

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>

struct Drawer {
	int w, h;
	RGB** map; 

	Drawer() {}	

	void open(const char* filename) {
		FILE* f = fopen(filename, "rb");
	    unsigned char info[54];
	    fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

	    // extract image height and width from header
	    w = *(int*)&info[18];
	    h = *(int*)&info[22];

	    int size = 3 * w * h;
	    unsigned char* data = new unsigned char[size]; // allocate 3 bytes per pixel
	    fread(data, sizeof(unsigned char), size, f); // read the rest of the data at once
	    fclose(f);

	    map = new RGB*[w];
	    for (int i = 0; i < w; i++)
	    	map[i] = new RGB[h];
	    
		for (int i = 0; i < w; i++)
	    {
	    	for (int j = 0; j < h; j++) {
	    		unsigned int r = data[(i * w + j) * 3];
	    		unsigned int g = data[(i * w + j) * 3 + 1];
	    		unsigned int b = data[(i * w + j) * 3 + 2];
	    		swap(r, b);
	    		map[j][w - 1 - i] = RGB(r, g, b);
	    	}
	    }

	    Drawer dr;
	    dr.w = w;
	    dr.h = h;
	    dr.map = map;
	}

	void build(const char* filename) {
		FILE *f;
		unsigned char *img = NULL;
		int filesize = 54 + 3 * w * h;  //w is your image width, h is image height, both int

		img = (unsigned char *)malloc(3*w*h);
		memset(img, 0, 3 * w * h);

		for (int i = 0; i < w; i++) {
		    for (int j = 0; j < h; j++) {
		        int x = i; 
		        int y = (h - 1) - j;
		        int r = map[i][h - 1 - j].r;
		        int g = map[i][h - 1 - j].g;
		        int b = map[i][h - 1 - j].b;
		        if (r > 255) r=255;
		        if (g > 255) g=255;
		        if (b > 255) b=255;
		        img[(x+y*w)*3+2] = (unsigned char)(r);
		        img[(x+y*w)*3+1] = (unsigned char)(g);
		        img[(x+y*w)*3+0] = (unsigned char)(b);
		    }
		}

		unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
		unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};
		unsigned char bmppad[3] = {0,0,0};

		bmpfileheader[ 2] = (unsigned char)(filesize    );
		bmpfileheader[ 3] = (unsigned char)(filesize>> 8);
		bmpfileheader[ 4] = (unsigned char)(filesize>>16);
		bmpfileheader[ 5] = (unsigned char)(filesize>>24);

		bmpinfoheader[ 4] = (unsigned char)(       w    );
		bmpinfoheader[ 5] = (unsigned char)(       w>> 8);
		bmpinfoheader[ 6] = (unsigned char)(       w>>16);
		bmpinfoheader[ 7] = (unsigned char)(       w>>24);
		bmpinfoheader[ 8] = (unsigned char)(       h    );
		bmpinfoheader[ 9] = (unsigned char)(       h>> 8);
		bmpinfoheader[10] = (unsigned char)(       h>>16);
		bmpinfoheader[11] = (unsigned char)(       h>>24);

		f = fopen(filename,"wb");
		fwrite(bmpfileheader,1,14,f);
		fwrite(bmpinfoheader,1,40,f);
		for(int i=0; i<h; i++)
		{
		    fwrite(img+(w*(h-i-1)*3),3,w,f);
		    fwrite(bmppad,1,(4-(w*3)%4)%4,f);
		}

		free(img);
		fclose(f);
	}
};

#endif //__DRAWER_H__