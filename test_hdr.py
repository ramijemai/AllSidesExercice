import sys
sys.path.append(r"C:\Users\Victus\Desktop\Work\Project\Allsides\AllSidesExercice\out\build\x64-debug\AllSidesExercice")

import hdr

# Create camera
camera = hdr.FakeCamera(4504, 4504)

# Capture 3 exposures
frame1 = camera.capture(5.0)
frame2 = camera.capture(40.0)
frame3 = camera.capture(320.0)

# Combine into HDR
combiner = hdr.HdrCombiner(4504, 4504)
result = combiner.combine(frame1, frame2, frame3, 5.0, 40.0, 320.0)

print(f"HDR pixels: {len(result)}")
print(f"First 5 pixels: {result[:5]}")
print("Python bindings work!")