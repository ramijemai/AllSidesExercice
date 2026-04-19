#include "../Headers/HdrCombiner.h"
#include "../Headers/Constants.h"
#include <stdexcept>


HdrCombiner::HdrCombiner(int width, int height)
    : width(width), height(height) {
}

// Combines three images taken at different exposures into a single HDR image
std::vector<float> HdrCombiner::combine(
    const std::vector<uint16_t>& img1,
    const std::vector<uint16_t>& img2,
    const std::vector<uint16_t>& img3,
    float e1, float e2, float e3
) {
    // Input validation
    if (img1.size() != img2.size() || img2.size() != img3.size())
        throw std::invalid_argument("Images must have the same size");

    size_t N = img1.size();
    std::vector<float> hdr(N);

	// Compute gains based on exposure times
    float g1 = e1 / HdrConstants::BASE_EXPOSURE;
    float g2 = e2 / HdrConstants::BASE_EXPOSURE;
    float g3 = e3 / HdrConstants::BASE_EXPOSURE;

	// Combine pixel values using a weighted average based on the computed weights
    for (size_t i = 0; i < N; i++) {
        float p1 = static_cast<float>(img1[i]);
        float p2 = static_cast<float>(img2[i]);
        float p3 = static_cast<float>(img3[i]);

        float w1 = computeWeight(p1);
        float w2 = computeWeight(p2);
        float w3 = computeWeight(p3);

        float denominator = w1 + w2 + w3;

        if (denominator > 0.0f)
            hdr[i] = (w1 * (p1 / g1) + w2 * (p2 / g2) + w3 * (p3 / g3)) / denominator;
        else
            hdr[i] = 0.0f;
    }

    return hdr;
}

// Computes the weight for a given pixel value based on its distance from saturation
float HdrCombiner::computeWeight(float p) const {
    return p * (HdrConstants::MAX_VAL - p);
}