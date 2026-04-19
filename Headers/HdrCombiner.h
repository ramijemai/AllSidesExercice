#pragma once

#include <vector>
#include <cstdint>

class HdrCombiner {
public:
	HdrCombiner(int width, int height);

	std::vector<float> combine(
		const std::vector<uint16_t>& img1,
		const std::vector<uint16_t>& img2,
		const std::vector<uint16_t>& img3,
		float e1, float e2, float e3
	);

private:
	int width;
	int height;

	float computeWeight(float pixel) const;
};