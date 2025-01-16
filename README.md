# (Abandoned, for now) Toy OpenGL engine 
Following LearnOpenGL.com to create a simple toy engine. 

Features include:
* ImGUI control of scene
* Performance tracing with Tracy
* Forward rendering with Phong/Blinn light shader supporting point/spot/directional light
* Normal map support
* Custom transform/scene hierarchy
* Lots of jank (this probably should not be public)

## How to build
Requires Conan 2 and modern CMake (3.2x+).

Create conan profiles for Release/Debug modes with the corresponding build types.
In root folder, install conan packages for release like so:

``` conan install . --build=missing -of=build --profile=Release ```

and for debug:

``` conan install . --build=missing -of=build--profile=Debug ```

Running CMake should handle the rest of the setup with automatically configured vars.


![image](https://github.com/user-attachments/assets/ced1e59c-4e77-4cf5-9dc5-33b4d61075e4)
