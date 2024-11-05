#ifndef HISTORAMCALCULATION_H
#define HISTORAMCALCULATION_H

#pragma once

#include <vector>
#include <Windows.h> 

void CalculateHistogramFromPixelData(const std::vector<BYTE>& pixelData, int width, int height, std::vector<int>& redChannel, std::vector<int>& greenChannel, std::vector<int>& blueChannel)
{
	redChannel.assign(256, 0);
	greenChannel.assign(256, 0);
	blueChannel.assign(256, 0);

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			int index = (y * width + x) * 4;

			BYTE b = pixelData[index];     // Blue channel
			BYTE g = pixelData[index + 1]; // Green channel
			BYTE r = pixelData[index + 2]; // Red channel

			redChannel[r]++;
			greenChannel[g]++;
			blueChannel[b]++;
		}
	}
}

#endif // HISTORAMCALCULATION_H