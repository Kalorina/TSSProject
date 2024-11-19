#ifndef IMAGEBRIGHTNESS_H
#define IMAGEBRIGHTNESS_H

#pragma once

#include <vector>
#include <Windows.h> 

void AdjustBrightness(const std::vector<BYTE>& pixelData, int width, int height, std::vector<int>& redChannel, std::vector<int>& greenChannel, std::vector<int>& blueChannel, float factor, bool DirectionUp, bool DirectionDown, bool DirectionLeft, bool DirectionRight)
{
    redChannel.assign(256, 0);
    greenChannel.assign(256, 0);
    blueChannel.assign(256, 0);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {

            int index = (y * width + x) * 4;
            BYTE blue = pixelData[index];     // Blue channel
            BYTE green = pixelData[index + 1]; // Green channel
            BYTE red = pixelData[index + 2]; // Red channel

            // Check if the pixel is within the desired directions
            if ((DirectionUp && y < height / 2) ||
                (DirectionDown && y >= height / 2) ||
                (DirectionLeft && x < width / 2) ||
                (DirectionRight && x >= width / 2)) {

                ModifyPixel(red, green, blue, factor);

                redChannel[red]++;
                greenChannel[blue]++;
                blueChannel[green]++;
            }
        }
    }
}

void ModifyPixel(BYTE r, BYTE g, BYTE b, float factor)
{
    // Multiply each channel by the factor
    r = min(255, max(0, static_cast<int>(r * factor)));
    g = min(255, max(0, static_cast<int>(g * factor)));
    b = min(255, max(0, static_cast<int>(b * factor)));

    /*r = min(255, int(r * factor));
    g = min(255, int(g * factor));
    b = min(255, int(b * factor));*/
}

#endif // IMAGEBRIGHTNESS_H