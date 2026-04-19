# HDR Image Combiner

## What it does

This project simulates a camera sensor (`FakeCamera`) and combines three images taken at different exposure times into a single High Dynamic Range (HDR) image (`HdrCombiner`). The result is tone mapped and saved as a viewable 8-bit PNG.

---

## How to Build

### Requirements
- CMake 3.12+
- C++20 compatible compiler (GCC, MSVC, Clang)
- No external dependencies — `stb_image_write.h` is included in `Headers/`

### Steps (Windows - MinGW / Linux / Mac)
```bash
mkdir build       
cd build
cmake ..
cmake --build .
./AllSidesExercice.exe    #to run the project
```

## Project Structure

```
AllSidesExercice/
├── Headers/
│   ├── Constants.h          # Shared constants (MAX_VAL, BASE_EXPOSURE)
│   ├── FakeCamera.h
│   ├── HdrCombiner.h
│   └── stb_image_write.h    # Single-header PNG writer  
├── Implementations/
│   ├── FakeCamera.cpp
│   └── HdrCombiner.cpp
├── CMakeLists.txt
├── main.cpp
└── README.md
```

---

## Algorithm: Weighted Exposure Merging

We use the **exposure merging** method introduced by Debevec & Malik (1997).

The idea is simple:
- Take multiple photos of the same scene at different exposures
- Each photo captures some parts well (and others poorly)
- Combine them by **trusting each pixel based on how well-exposed it is**

### How we weight each pixel

```
weight(p) = p × (4095 - p)
```

| Pixel value | Weight | Reason |
|-------------|--------|--------|
| 0 (black) | 0 | Underexposed — no information |
| 4095 (white) | 0 | Saturated — clipped, no information |
| ~2047 (middle) | MAX | Perfectly exposed — most trusted |

### Final HDR pixel value

```
HDR = (w1 × p1/gain1 + w2 × p2/gain2 + w3 × p3/gain3) / (w1 + w2 + w3)
```

---

## Exposures used

| Frame | Exposure | Captures best |
|-------|----------|---------------|
| Frame 1 | 5 ms | Bright areas |
| Frame 2 | 40 ms | Mid-tones |
| Frame 3 | 320 ms | Dark areas |

---

## Tone Mapping

The raw HDR output (float values) is converted to a viewable 8-bit image using:

**1. Reinhard tone mapping:**
```
mapped = x / (1 + x)
```

**2. Gamma correction:**
```
output = mapped ^ (1/2.2)
```

The result is saved as `output.png` in the build directory.

---

## Benchmarking

Timings measured on a 4504×4504 image (single-threaded):

| Operation | Time |
|-----------|------|
| Capture 5ms | ~2400 ms |
| Capture 40ms | ~2400 ms |
| Capture 320ms | ~2400 ms |
| HDR merge | ~450 ms |

**Main bottleneck:** Gaussian noise generation (~20M random numbers per frame).

**Suggested optimization:** Parallelizing the pixel loop with OpenMP would distribute work across all CPU cores, reducing capture time by 4-8x on a modern processor.

---

## Assumptions

- The sensor response is **linear** (pixel = scene × gain)
- The scene is **static** — only brightness changes across exposures, not content
- Pixel format is **12-bit unsigned** — values in [0, 4095]
- Noise is **Gaussian**, stronger at low exposure

---

## Known Limitations

- **Linear sensor assumed** — real cameras may have a non-linear response curve
- **Single-threaded** — performance could be improved with OpenMP for 4504×4504 images
- **Grayscale only** — no color/Bayer support
- **No color tone mapping** — grayscale Reinhard only

---

## Third Party Libraries

| Library | Purpose | License | Source |
|---------|---------|---------|--------|
| stb_image_write.h | Save PNG images | Public Domain | https://github.com/nothings/stb |


## Reference

> Debevec, P. E., & Malik, J. (1997). *Recovering High Dynamic Range Radiance Maps from Photographs*. SIGGRAPH 1997.  
> https://www.pauldebevec.com/Research/HDR/