[Implementation of `Ray Tracing in One Weekend`](https://raytracing.github.io/books/RayTracingInOneWeekend.html)

Configure with cmake `prefix` (Debug here)

```bash
cmake --preset "Default Debug"
```

And build:

```bash
cmake --build build_debug/
```

Launch with a specified render option, either: `checkered-spheres` `bouncing-spheres`, or `earth-texture`

```bash
./build_debug/bin/RayTracingInOneWeekend  --render checkered-spheres --image-width 1200
```

`--image-width` is an optional argument.

Output images are put in an `images` directory, relative to the location where the executable was run.
