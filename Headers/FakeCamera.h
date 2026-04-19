#pragma once

#include <vector>
#include <cstdint>

class FakeCamera {
public:
    // Constructor
    FakeCamera(int width, int height);

    // Capture an image at a given exposure time (in milliseconds)
    std::vector<uint16_t> capture(float exposure_ms);

private:
    int width;
    int height;

    // Underlying scene radiance (values in [0.0, 1.0])
    std::vector<float> scene;

    // Compute exposure gain
    float computeGain(float exposure_ms) const;

    // Initialize the synthetic scene
    void generateScene();
};