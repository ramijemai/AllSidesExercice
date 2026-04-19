#include "Headers/HdrCombiner.h"
#include "Headers/FakeCamera.h"
#include <iostream>
#include <algorithm>

int main() {
	// Test parameters
    constexpr int WIDTH = 4504, HEIGHT = 4504;
    constexpr float E1 = 5.0f, E2 = 40.0f, E3 = 320.0f;

	// Create fake camera and capture images at different exposures
    FakeCamera camera(WIDTH, HEIGHT);
	// Capture three images with different exposures
    auto frame1 = camera.capture(E1);
    auto frame2 = camera.capture(E2);
    auto frame3 = camera.capture(E3);

	// Combine the images into an HDR image
    HdrCombiner combiner(WIDTH, HEIGHT);
    auto hdr = combiner.combine(frame1, frame2, frame3, E1, E2, E3);
	// Analyze HDR result
	auto minVal = *std::min_element(hdr.begin(), hdr.end());  
    auto maxVal = *std::max_element(hdr.begin(), hdr.end());
	// Print summary
    std::cout << "HDR Result: " << hdr.size() << " pixels, range ["
        << minVal << ", " << maxVal << "]\n";

	// Print first few pixel values
    for (size_t i = 0; i < std::min<size_t>(10, hdr.size()); i++)
        std::cout << "Pixel " << i << ": " << hdr[i] << "\n";

    return 0;
}