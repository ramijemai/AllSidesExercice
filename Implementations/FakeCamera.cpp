#include "../Headers/FakeCamera.h"
#include "../Headers/Constants.h"
#include <algorithm>
#include <cmath>
#include <random>



FakeCamera::FakeCamera(int width, int height):
	width(width),height(height) , scene(width*height){

	generateScene();
}

void FakeCamera::generateScene() {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float fx = static_cast<float>(x) / width;
            float fy = static_cast<float>(y) / height;

            float value = 0.5f * std::sin(fx * 3.1415f) * std::cos(fy * 3.1415f);
            value += 0.5f; // Normalize [−0.5, 0.5] → [0, 1]

            scene[y * width + x] = value;
        }
    }
}

float FakeCamera::computeGain(float exposure_ms) const {
    return exposure_ms / HdrConstants::BASE_EXPOSURE;
}


std::vector<uint16_t> FakeCamera::capture(float exposure_ms) {
    std::vector<uint16_t> image(width * height);
    float gain = computeGain(exposure_ms);

    // thread_local: safe for parallel use, unique per thread
    thread_local std::default_random_engine gen(std::random_device{}());

    float sigma = 20.0f / gain;
    std::normal_distribution<float> noise(0.0f, sigma);

    for (int i = 0; i < width * height; i++) {
        float value = scene[i] * gain * HdrConstants::MAX_VAL;
        value += noise(gen);
        value = std::clamp(value, 0.0f, HdrConstants::MAX_VAL);
        image[i] = static_cast<uint16_t>(value);
    }

    return image;
}

