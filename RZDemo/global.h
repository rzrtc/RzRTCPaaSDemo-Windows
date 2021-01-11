#pragma once
#include <stdint.h>
#include <string.h>
typedef struct VideoBuffer_ {
	uint8_t* buffer;
	int w;
	int h;
	int size;

	VideoBuffer_(uint8_t* data, int width, int height) {
		this->w = width;
		this->h = height;
		this->size = width * height * 3 / 2;
		this->buffer = new uint8_t[size];
		memcpy(buffer, data, size);
	}

	~VideoBuffer_()
	{
		delete[] buffer;
	}
}VideoBuffer;