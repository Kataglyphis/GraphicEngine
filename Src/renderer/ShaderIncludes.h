#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

// https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_shading_language_include.txt
class ShaderIncludes {
 public:
  ShaderIncludes();

  ~ShaderIncludes();

 private:
  std::vector<const char*> includeNames = {
      "host_device_shared.h", "Matlib.glsl",     "microfacet.glsl",
      "ShadingLibrary.glsl",  "disney.glsl",     "frostbite.glsl",
      "pbrBook.glsl",         "phong.glsl",      "unreal4.glsl",
      "clouds.glsl",          "grad_noise.glsl", "worley_noise.glsl",
      "bindings.h",           "GlobalValues.h",  "directional_light.glsl",
      "light.glsl",           "material.glsl",   "point_light.glsl"};

  std::vector<const char*> file_locations_relative = {
      "Shaders/hostDevice/host_device_shared.h",
      "Shaders/common/Matlib.glsl",
      "Shaders/pbr/microfacet.glsl",
      "Shaders/common/ShadingLibrary.glsl",
      "Shaders/pbr/brdf/disney.glsl",
      "Shaders/pbr/brdf/frostbite.glsl",
      "Shaders/pbr/brdf/pbrBook.glsl",
      "Shaders/pbr/brdf/phong.glsl",
      "Shaders/pbr/brdf/unreal4.glsl",
      "Shaders/clouds/clouds.glsl",
      "Shaders/common/grad_noise.glsl",
      "Shaders/common/worley_noise.glsl",
      "Shaders/hostDevice/bindings.h",
      "Shaders/hostDevice/GlobalValues.h",
      "Shaders/common/directional_light.glsl",
      "Shaders/common/light.glsl",
      "Shaders/common/material.glsl",
      "Shaders/common/point_light.glsl"};
};
