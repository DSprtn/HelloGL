# Toy OpenGL engine 
Following LearnOpenGL.com to create a simple toy engine. Also includes usage of IMGui and the Tracy profiler. 

## How to build
Requires Conan 2 and modern CMake (3.2x+).

Create conan profiles for Release/Debug modes with the corresponding build types.
In root folder, install conan packages for release like so:

``` conan install . --build=missing -of=build/Release --profile=Release ```

and for debug:

``` conan install . --build=missing -of=build/Debug --profile=Debug ```

Running CMake should handle the rest of the setup.
