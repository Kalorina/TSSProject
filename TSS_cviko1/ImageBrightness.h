#ifndef IMAGEBRIGHTNESS_H
#define IMAGEBRIGHTNESS_H

#pragma once

#include <vector>
#include <Windows.h> 

void AdjustBrightness(BYTE* pixels, int stride, int width, int height, float factor, bool DirectionUp, bool DirectionDown, bool DirectionLeft, bool DirectionRight)
{
	factor = max(0.0f, factor);

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			int index = y * stride + x * 4;

			// Check if the pixel is within the desired adjustment region
			if ((DirectionUp && y < height / 2) ||
				(DirectionDown && y >= height / 2) ||
				(DirectionLeft && x < width / 2) ||
				(DirectionRight && x >= width / 2)) {

				// Adjust RGB values and clamp
				BYTE b = pixels[index];
				BYTE g = pixels[index + 1];
				BYTE r = pixels[index + 2];

				b = static_cast<BYTE>(min(255, max(0, static_cast<int>(b * factor))));
				g = static_cast<BYTE>(min(255, max(0, static_cast<int>(g * factor))));
				r = static_cast<BYTE>(min(255, max(0, static_cast<int>(r * factor))));

				// Write back adjusted values
				pixels[index] = b;
				pixels[index + 1] = g;
				pixels[index + 2] = r;
			}
		}
	}
}

#endif // IMAGEBRIGHTNESS_H