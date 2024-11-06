#include "pch.h"
#include "CppUnitTest.h"
#include <vector>
#include <iostream>
#include <windows.h>
#include "../TSS_cviko1/HistogramCalculation.h"


using namespace std;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{

	TEST_CLASS(UnitTest)
	{
	public:

		// kombinatoricke unit test
		// velke bitmapy/ obrazky -> viac ako 2x2 pixels

		TEST_METHOD(TestMethod_SingleRedPixel)
		{
			// 1x1 image with a single red pixel
			int width = 1, height = 1;
			std::vector<BYTE> pixelData = { 0, 0, 255, 255 }; // Pixel: Red (B=0, G=0, R=255, A=255) 
			// tuto 256x256x256 pixels obr -> kombinatorika farieb napr. csv subory v matlabe (?)

			std::vector<int> redChannel, greenChannel, blueChannel;

			CalculateHistogramFromPixelData(pixelData, width, height, redChannel, greenChannel, blueChannel);

			Assert::AreEqual(1, redChannel[255]);   // Single red pixel
			Assert::AreEqual(0, greenChannel[255]); // No green pixels
			Assert::AreEqual(0, blueChannel[255]);  // No blue pixels
		}

		TEST_METHOD(TestMethod_GrayScaleImage)
		{
			// Grayscale image (R=G=B for all pixels)
			int width = 2, height = 2;
			std::vector<BYTE> pixelData = { 128, 128, 128, 255,   
											128, 128, 128, 255,
											128, 128, 128, 255,   
											128, 128, 128, 255 };

			std::vector<int> redChannel, greenChannel, blueChannel;
			
			CalculateHistogramFromPixelData(pixelData, width, height, redChannel, greenChannel, blueChannel);

			Assert::AreEqual(4, redChannel[128]);   // All pixels grayscale value 128
			Assert::AreEqual(4, greenChannel[128]);
			Assert::AreEqual(4, blueChannel[128]);
		}

		TEST_METHOD(TestMethod_2x2Pixels)
		{
			// 2x2 image with Red, Green, Blue, and White pixels
			int width = 2, height = 2;
			std::vector<BYTE> pixelData = {
				255, 0, 0, 255,    // Pixel 1: Red
				0, 255, 0, 255,    // Pixel 2: Green
				0, 0, 255, 255,    // Pixel 3: Blue
				255, 255, 255, 255 // Pixel 4: White
			};

			std::vector<int> redChannel, greenChannel, blueChannel;

			CalculateHistogramFromPixelData(pixelData, width, height, redChannel, greenChannel, blueChannel);


			Assert::AreEqual(2, redChannel[255]);   // Red appears in Red and White 
			Assert::AreEqual(2, greenChannel[255]); // Green appears in Green and White 
			Assert::AreEqual(2, blueChannel[255]);  // Blue appears in Blue and White

		}

	};

}
