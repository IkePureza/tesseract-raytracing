#!/bin/bash

# Build the project
cmake --build build/

# Run the raytracer and generate image
./build/raytracer > image.ppm

# Open the image with macOS default viewer
open image.ppm
