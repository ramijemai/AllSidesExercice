# HDR Image Combiner

## What it does

This project simulates a camera sensor (`FakeCamera`) and combines three images taken at different exposure times into a single High Dynamic Range (HDR) image (`HdrCombiner`).

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

## Assumptions

- The sensor response is **linear** (pixel = scene × gain)
- The scene is **static** — only brightness changes across exposures, not content
- Pixel format is **12-bit unsigned** — values in [0, 4095]
- Noise is **Gaussian**, stronger at low exposure

---

## Known Limitations

- **No tone mapping** — output is a raw HDR float image, not directly displayable
- **Linear sensor assumed** — real cameras may have a non-linear response curve
- **Single-threaded** — for 4504×4504 images (~20M pixels), performance could be improved with parallelism (e.g. OpenMP)
- **No color support** — grayscale only

---

## Reference

> Debevec, P. E., & Malik, J. (1997). *Recovering High Dynamic Range Radiance Maps from Photographs*. SIGGRAPH 1997.  
> https://www.pauldebevec.com/Research/HDR/