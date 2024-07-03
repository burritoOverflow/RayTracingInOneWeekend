[Implementation of `Ray Tracing in One Weekend`](https://raytracing.github.io/books/RayTracingInOneWeekend.html)

Configure with cmake `prefix` (Debug here)

```bash
cmake --preset "Default Debug"
```

And build:

```bash
cmake --build build_debug/
```

Launch with a specified render option, either: `checkered-spheres` or `bouncing-spheres`

```bash
./build_debug/bin/ray_tracing_in_one_weekend  --render checkered-spheres
```

Output images are put in an `images` directory, relative to the location where the executable was run.
