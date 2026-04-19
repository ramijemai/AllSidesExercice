#include "HdrCombiner.h"
#include "FakeCamera.h"
#include <iostream>
#include <algorithm>
#include <chrono>
#include <filesystem>
// stb_image_write.h - single-header PNG writer
// Author: Sean Barrett
// License: Public Domain
// Source: https://github.com/nothings/stb/blob/master/stb_image_write.h
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"




#define BENCHMARK(label, code) \
    { \
        auto _start = std::chrono::high_resolution_clock::now(); \
        code; \
        auto _end = std::chrono::high_resolution_clock::now(); \
        auto _ms = std::chrono::duration_cast<std::chrono::milliseconds>(_end - _start).count(); \
        std::cout << label << ": " << _ms << " ms\n"; \
    }

// Reinhard tone mapping
float reinhard(float x, float maxVal) {
    float normalized = x / maxVal;         // normalize to [0, 1]
    return normalized / (1.0f + normalized); // Reinhard formula
}

// Gamma correction
float gammaCorrect(float x, float gamma = 2.2f) {
    return std::pow(x, 1.0f / gamma);
}

int main() {
    constexpr int   WIDTH = 4504, HEIGHT = 4504;
    constexpr float E1 = 5.0f, E2 = 40.0f, E3 = 320.0f;

    FakeCamera camera(WIDTH, HEIGHT);

    std::vector<uint16_t> frame1, frame2, frame3;
    BENCHMARK("Capture 5ms   ", frame1 = camera.capture(E1));
    BENCHMARK("Capture 40ms  ", frame2 = camera.capture(E2));
    BENCHMARK("Capture 320ms ", frame3 = camera.capture(E3));

    HdrCombiner combiner(WIDTH, HEIGHT);
    std::vector<float> hdr;
    BENCHMARK("HDR merge     ", hdr = combiner.combine(frame1, frame2, frame3, E1, E2, E3));

    // Results
    auto minVal = *std::min_element(hdr.begin(), hdr.end());
    auto maxVal = *std::max_element(hdr.begin(), hdr.end());
    std::cout << "\nHDR Result: " << hdr.size() << " pixels, range ["
        << minVal << ", " << maxVal << "]\n";

    // Tone mapping → 8-bit
    std::vector<uint8_t> ldr(WIDTH * HEIGHT);
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        float mapped = reinhard(hdr[i], maxVal);   // Reinhard
        float gamma = gammaCorrect(mapped);        // gamma correction
        ldr[i] = static_cast<uint8_t>(gamma * 255.0f);
    }

    // Save as PNG
    stbi_write_png("output.png", WIDTH, HEIGHT, 1, ldr.data(), WIDTH);
    std::cout << "Saved output.png \n";

    std::cout << "Saved to: " << std::filesystem::current_path() << "\n";

    return 0;
}